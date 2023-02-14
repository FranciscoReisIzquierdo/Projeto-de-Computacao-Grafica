#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <IL/il.h>
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "tinyxml/tinyxml.h"
#include "catmullRom.h"


typedef struct point{
	float x = 0;
	float y = 0;
	float z = 0;
} *POINT;

typedef struct material{
	float diffuse[4] = {0.1f, 0.1f, 0.1f, 1};
	float ambient[4] = {0.2f, 0.2f, 0.2f, 1};
	float specular[4] = {0, 0, 0, 1};
	float emissive[4] = {0, 0, 0, 1};
	float shininess = 128;
} *MATERIAL;

typedef struct light{
	float pos[4] = {0.0, 0.0, 0.0, 1.0};
	float dir[4] = {0.0, 0.0, 0.0, 0.0};
	unsigned int type = 0;
	float cutoff = 20;
} *LIGHT;

typedef struct transform{
	POINT p = new point();
	float (*points)[3];
	int point_count = -1;
	float time;
	bool align;
	int type = -1;
} *TRANSFORM;

typedef struct object{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> tex_vert;
	std::vector<float> normals;
	unsigned int texID = 0;
	unsigned int texCoords = 0;
	GLuint index_count = -1;
	GLuint vertex_count = -1;
	GLuint norm_count = -1;
	GLuint vbo;
	GLuint idx;
	GLuint norm;
	std::vector<TRANSFORM> transforms;
	POINT position = new point();
	POINT scale = new point();
	bool centerable = false;
	float modelview[16];
	float curve_modelview[16];
	MATERIAL material = new struct material();
} *OBJECT;

// All objects loaded
std::vector<OBJECT> objs;

// All lights loaded
std::vector<LIGHT> lights;

// Object on screen
int current_obj = -1;

// Up vector
int up_x = 0;
int up_y = 0;
int up_z = 1;

// Look At
float lAt_x = 0;
float lAt_y = 0;
float lAt_z = 0;

// Last Look At
float cur_lAt_x;
float cur_lAt_y;
float cur_lAt_z;

// Initial positions
float pos_x = 3;
float pos_y = 3;
float pos_z = 2;

// Last camera position
float last_pos_x;
float last_pos_y;
float last_pos_z;

// Camera projection
float proj_fov = 60;
float proj_near = 1;
float proj_far = 1000;
float ratio = 1;
float camera_div = 10;

// Camera position
int last_x = -1;
int last_y = -1;
float m_lat = 0.5;
float m_lon = 0.0;
float center_dist = 3.0;

// Control variables
float mouse_speed = 0.007;
float zoom_speed = 0.07;
float orbit_correction = 0;
bool fixed = false;
bool draw_all = true;
bool auto_orbit = false;
bool draw_axis = false;
bool draw_orbits = true;
bool show_help = true;

// Screen size
int width = 1300;
int height = 800;


void change_size(int w, int h)
{
	if (h == 0) h = 1;

	ratio = w * 1.0f / h;
	width = w;
	height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, w, h);
}

void setObjMaterial(OBJECT obj){
	glMaterialfv(GL_FRONT, GL_DIFFUSE, obj->material->diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, obj->material->ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, obj->material->specular);
	glMaterialf(GL_FRONT, GL_SHININESS, obj->material->shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, obj->material->emissive);
}

void draw_in_gpu(OBJECT obj){
	
	// Material properties
	glPushAttrib(GL_LIGHTING_BIT);
	setObjMaterial(obj);

	// Draw in GPU
	glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
	glVertexPointer(3,GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->idx);

	glBindBuffer(GL_ARRAY_BUFFER, obj->texCoords);
	glTexCoordPointer(2,GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, obj->norm);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindTexture(GL_TEXTURE_2D, obj->texID);

	glDrawElements(
		GL_TRIANGLES,
		obj->index_count,
		GL_UNSIGNED_INT,
		NULL
	);

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopAttrib();
}


void apply_transforms(OBJECT obj){
	
	obj->scale->x = 1;
	obj->scale->y = 1;
	obj->scale->z = 1;
	for(TRANSFORM trans : obj->transforms){
		switch (trans->type){
			case 0:
				// Normal translate
				glTranslatef(trans->p->x, trans->p->y, trans->p->z);
				break;
			case 1:
				// Rotation
				glRotatef(trans->p->x, 1, 0, 0);
				glRotatef(trans->p->y, 0, 1, 0);
				glRotatef(trans->p->z, 0, 0, 1);
				break;
			case 2:
				// Scale
				glScalef(trans->p->x, trans->p->y, trans->p->z);
				obj->scale->x *= trans->p->x;
				obj->scale->y *= trans->p->y;
				obj->scale->z *= trans->p->z;
				break;
			case 3:
				// Animated translate
				glGetFloatv(GL_MODELVIEW_MATRIX, obj->curve_modelview);
				catmullRomAnimation(trans->time, trans->align, trans->point_count, trans->points);
				break;
			case 4:
				// Animated rotate
				glRotatef(
					glutGet(GLUT_ELAPSED_TIME) / 2.78f / trans->time,
					trans->p->x,
					trans->p->y,
					trans->p->z);
				break;
			default:
				break;
		}
	}
	//float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, obj->modelview);
	obj->position->x = obj->modelview[12];
	obj->position->y = obj->modelview[13];
	obj->position->z = obj->modelview[14];
}

void render_object(OBJECT obj){

	if (draw_orbits)
		for (TRANSFORM trans : obj->transforms){
			if (trans->type == 3){
				glPushMatrix();
				glLoadMatrixf(obj->curve_modelview);
				renderCatmullRomCurve(trans->points, trans->point_count);
				glPushMatrix();
			}

		}

	glPushMatrix();
	
	glLoadMatrixf(obj->modelview);

	// Draw vertices
	draw_in_gpu(obj);

	glPopMatrix();
}


void render_all_objects(){
	// Render all objects in scene
	for(OBJECT obj : objs){
		render_object(obj);
	}
}

void render_axis() {

	float length = 1000.0;

	glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-length, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -length, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -length);
    glVertex3f(0.0f, 0.0f, length);

	glColor3f(0.8f, 0.8f, 0.8f);

    glEnd();
	glEnable(GL_LIGHTING);
}

void draw_string(float x, float y, float z, char *string) {

	glColor3f(1, 1, 1);
	glRasterPos3d(x, y, z);

	for (char* c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
}

void draw_ui(){

	float x2 = 0.99 - 377.0 / (float)width;
	float y2 = 0.99 - 592.0 / (float)height;

	char str0[100] = "HELP";
	draw_string(-0.98, 0.99 - (0.99 - y2)/15.0, 0, str0);
	char str1[100] = "Change Obj => ARROWS";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 2.5, 0, str1);
	char str2[100] = "Zoom   => ScrollWheel";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 4, 0, str2);
	char str3[100] = "Reset Position  => R";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 5.5, 0, str3);
	char str4[100] = "Lock drag       => F";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 7, 0, str4);
	char str5[100] = "Draw Axis       => E";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 8.5, 0, str5);
	char str6[100] = "Object mode     => L";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 10, 0, str6);
	char str7[100] = "Draw Orbits     => H";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 11.5, 0, str7);
	char str8[100] = "Auto orbit      => O";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 13, 0, str8);



	glBegin(GL_POLYGON);
	glColor3f(0.1, 0.1, 0.1);
    glVertex2f(-0.99, 0.99 - (0.99 - y2)/10);
    glVertex2f(-x2, 0.99 - (0.99 - y2)/10);
    glVertex2f(-x2, 0.99);
    glVertex2f(-0.99, 0.99);
    glEnd();

    glBegin(GL_POLYGON);
	glColor3f(0.15, 0.15, 0.15);
    glVertex2f(-0.99, y2);
    glVertex2f(-x2, y2);
    glVertex2f(-x2, 0.99);
    glVertex2f(-0.99, 0.99);
    glEnd();
}

void set_camera_position(){
	OBJECT cur_obj = objs[current_obj];
	float delta_x, delta_y, delta_z, lAt_x_delta, lAt_y_delta, lAt_z_delta;
	if(current_obj != -1){

		delta_x = ((((cur_obj->scale->x + cur_obj->scale->y) / 2) * center_dist * cos(m_lon) * sin(m_lat) + cur_obj->position->x) - last_pos_x) / camera_div;

		if(up_y == 1){
			delta_y = ((((cur_obj->scale->x + cur_obj->scale->z) / 2) * center_dist * cos(m_lat) + cur_obj->position->y) - last_pos_y) / camera_div +
				(0.1f * sqrt(pow(cur_obj->position->x - last_pos_x, 2) + pow(cur_obj->position->z - last_pos_z, 2)));
			delta_z = ((((cur_obj->scale->x + cur_obj->scale->z) / 2) * center_dist * sin(m_lon) * sin(m_lat) + cur_obj->position->z) - last_pos_z) / camera_div;
		}
		else if(up_z == 1){
			delta_y = ((((cur_obj->scale->x + cur_obj->scale->y) / 2) * center_dist * sin(m_lon) * sin(m_lat) + cur_obj->position->y) - last_pos_y) / camera_div;
			delta_z = ((((cur_obj->scale->x + cur_obj->scale->y) / 2) * center_dist * cos(m_lat) + cur_obj->position->z) - last_pos_z) / camera_div +
				(0.1f * sqrt(pow(cur_obj->position->x - last_pos_x, 2) + pow(cur_obj->position->y - last_pos_y, 2)));;
		}

		lAt_x_delta = (cur_obj->position->x - cur_lAt_x) / camera_div;
		lAt_y_delta = (cur_obj->position->y - cur_lAt_y) / camera_div;
		lAt_z_delta = (cur_obj->position->z - cur_lAt_z) / camera_div;

		camera_div = camera_div <= 1 ? 1 : camera_div - 0.3f;
		if(current_obj != 0 && !auto_orbit) m_lon += orbit_correction;
	}
	else{
		
		delta_x = ((center_dist * cos(m_lon) * sin(m_lat)) - last_pos_x) / 10;

		if(up_y == 1){
			delta_y = ((center_dist * cos(m_lat)) - last_pos_y) / 10;
			delta_z = ((center_dist * sin(m_lon) * sin(m_lat)) - last_pos_z) / 10;
		}
		else if(up_z == 1){
			delta_y = ((center_dist * sin(m_lon) * sin(m_lat)) - last_pos_y) / 10;
			delta_z = ((center_dist * cos(m_lat)) - last_pos_z) / 10;
		}

		lAt_x_delta = (lAt_x - cur_lAt_x) / 10;
		lAt_y_delta = (lAt_y - cur_lAt_y) / 10;
		lAt_z_delta = (lAt_z - cur_lAt_z) / 10;

	}

	last_pos_x += delta_x;
	last_pos_y += delta_y;
	last_pos_z += delta_z;
	
	cur_lAt_x += lAt_x_delta;
	cur_lAt_y += lAt_y_delta;
	cur_lAt_z += lAt_z_delta;
}

void render_lights(){
	
	int idx = GL_LIGHT0;

	for(int i = 0; i < lights.size(); i++){

		LIGHT l = lights[i];

		switch (l->type)
		{
			case 0: // point light
				glLightfv(idx, GL_POSITION, l->pos);
				break;
			case 1: // directional light
				glLightfv(idx, GL_POSITION, l->dir);
				break;
			case 2: // spot ligth
				glLightfv(idx, GL_POSITION, l->pos);
				glLightfv(idx, GL_SPOT_DIRECTION, l->dir);
				glLightf (idx, GL_SPOT_CUTOFF, l->cutoff);
				glLightf (idx, GL_SPOT_EXPONENT, 0.0);
				break;
			default:
				break;
		}
		idx++;
	}
}

void render_scene(void)
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glDisable(GL_LIGHTING);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Draw UI
	if(show_help) draw_ui();
	glEnable(GL_LIGHTING);
	
	for(OBJECT obj : objs){
		glPushMatrix();
		apply_transforms(obj);
		glPopMatrix();
	}

	// Calculate camera position
	set_camera_position();

	// Set camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(proj_fov, ratio, proj_near, proj_far);
	gluLookAt(last_pos_x, last_pos_y, last_pos_z,
		cur_lAt_x, cur_lAt_y, cur_lAt_z,
		up_x, up_y, up_z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Render lights
	render_lights();

	// Draw
	if(current_obj == -1) current_obj = 0;
	if (draw_axis) render_axis();
	if (draw_all) render_all_objects();
	else {
		render_object(objs[current_obj]);
	}

	// End of frame
	glutSwapBuffers();
}

void read_object(TiXmlElement* model, std::string path, bool centerable, std::vector<TRANSFORM> orders){
	std::ifstream file(path);
	std::string text;
	getline(file, text);
	int num = atoi(text.c_str());
	getline(file, text);
	int idx_count = atoi(text.c_str());
	getline(file, text);
	int tex_count = atoi(text.c_str());
	getline(file, text);
	int norm_count = atoi(text.c_str());
	OBJECT obj = new object();

	// Read vertices
	std::vector<float> vertices;
	obj->vertex_count = num;
	for(int i = 0; i < num; i++){
		getline(file, text);
		vertices.push_back(atof(text.c_str()));
		getline(file, text);
		vertices.push_back(atof(text.c_str()));
		getline(file, text);
		vertices.push_back(atof(text.c_str()));
	}
	obj->vertices = vertices;

	// Read indices
	std::vector<unsigned int> indices;
	obj->index_count = idx_count;
	for(int i = 0; i < idx_count; i++){
		getline(file, text);
		indices.push_back(atoi(text.c_str()));
	}
	obj->indices = indices;

	// Read Textures vertices
	std::vector<float> tex_vert;
	for(int i = 0; i < tex_count; i++){
		getline(file, text);
		tex_vert.push_back(atof(text.c_str()));
		getline(file, text);
		tex_vert.push_back(atof(text.c_str()));
	}
	obj->tex_vert = tex_vert;

	// Read normals
	std::vector<float> normals;
	obj->norm_count = norm_count;
	for(int i = 0; i < norm_count; i++){
		getline(file, text);
		normals.push_back(atof(text.c_str()));
		getline(file, text);
		normals.push_back(atof(text.c_str()));
		getline(file, text);
		normals.push_back(atof(text.c_str()));
	}
	obj->normals = normals;

	// Centering
	obj->centerable = centerable;

	// Get World Position and Scale
	POINT scale = new point();
	scale->x = 1;
	scale->y = 1;
	scale->z = 1;
	obj->scale = scale;

	// Transformation order
	obj->transforms = orders;

	// Copy vertices to GPU
	glGenBuffers(1, &obj->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * obj->vertices.size(),
		obj->vertices.data(),
		GL_STATIC_DRAW);

	// Copy indices to GPU
	glGenBuffers(1, &obj->idx);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->idx);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * obj->indices.size(),
		obj->indices.data(),
		GL_STATIC_DRAW);

	// Copy normals to GPU
	glGenBuffers(1, &obj->norm);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->norm);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * obj->normals.size(),
		obj->normals.data(),
		GL_STATIC_DRAW);

	objs.push_back(obj);
	file.close();

	TiXmlElement* elem = model->FirstChildElement();
	for (; elem!=nullptr; elem = elem->NextSiblingElement()){
		if(strcmp(elem->Value(), "texture") == 0){

			// Load texture
			unsigned int t,tw,th;
			unsigned char *texData;
			unsigned int texID, texCoords;
			
			// Colocar a origem da textura no canto inferior esquerdo
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

			// Carregar a textura para memória
			ilGenImages(1, &t);
			ilBindImage(t);
			const char *tex_name = model->FirstChildElement()->Attribute("file");
			ilLoadImage((ILstring) tex_name);
			tw = ilGetInteger(IL_IMAGE_WIDTH);
			th = ilGetInteger(IL_IMAGE_HEIGHT);

			// Assegurar que a textura se encontra em RGBA (Red, Green, Blue, Alpha) com um byte (0 - 255) por componente
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
			texData = ilGetData();

			// Gerar a textura para a placa gráfica
			glGenTextures(1, &texID);
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_ATTENUATION);

			// Upload dos dados de imagem
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			// Store texture in GPU
			glGenBuffers(1, &texCoords);
			glBindBuffer(GL_ARRAY_BUFFER, texCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * idx_count, tex_vert.data(), GL_STATIC_DRAW);

			obj->texID = texID;
			obj->texCoords = texCoords;
		}
		else if(strcmp(elem->Value(), "color") == 0){
			MATERIAL mat = new struct material();
			TiXmlElement* l = elem->FirstChildElement();
			for (; l!=nullptr; l = l->NextSiblingElement()){
				if(strcmp(l->Value(), "diffuse") == 0){

					mat->diffuse[0] = atof(l->Attribute("R")) / 255.0;
					mat->diffuse[1] = atof(l->Attribute("G")) / 255.0;
					mat->diffuse[2] = atof(l->Attribute("B")) / 255.0;
				}
				else if(strcmp(l->Value(), "ambient") == 0){

					mat->ambient[0] = atof(l->Attribute("R")) / 255.0;
					mat->ambient[1] = atof(l->Attribute("G")) / 255.0;
					mat->ambient[2] = atof(l->Attribute("B")) / 255.0;
				}
				else if(strcmp(l->Value(), "specular") == 0){

					mat->specular[0] = atof(l->Attribute("R")) / 255.0;
					mat->specular[1] = atof(l->Attribute("G")) / 255.0;
					mat->specular[2] = atof(l->Attribute("B")) / 255.0;
				}
				else if(strcmp(l->Value(), "emissive") == 0){

					mat->emissive[0] = atof(l->Attribute("R")) / 255.0;
					mat->emissive[1] = atof(l->Attribute("G")) / 255.0;
					mat->emissive[2] = atof(l->Attribute("B")) / 255.0;
				}
				else if(strcmp(l->Value(), "shininess") == 0){

					mat->shininess = atof(l->Attribute("value"));
				}
			}
			obj->material = mat;
		}
	}
}

void reset_position(){
	// Calculate m_lat, m_lon and center_dist
	center_dist = sqrt(pow(lAt_x - pos_x, 2) + pow(lAt_y - pos_y, 2) + pow(lAt_z - pos_z, 2));
	m_lat = up_y == 1 ? acos(pos_y / center_dist) : acos(pos_z / center_dist);
	m_lon = acos(pos_x / (center_dist * sin(m_lat)));
	m_lon = up_y == 1 ?
		(round(sin(m_lon) * (center_dist * sin(m_lat))) == round(pos_z) ? m_lon : M_PI + (M_PI - m_lon)) :
		(round(sin(m_lon) * (center_dist * sin(m_lat))) == round(pos_y) ? m_lon : M_PI + (M_PI - m_lon));
	current_obj = -1;
	
}

void obj_down(){
	camera_div = 10;
	if(!draw_all){
		if(current_obj - 1 < 0)
			current_obj = objs.size() - 1;
		else current_obj--;
	}else{
		int first = -1;
		for(int i = objs.size() - 1; i >= 0; i--)
			if(objs[i]->centerable){
				first = i;
				break;
			}
		for(int i = current_obj - 1; i >= 0; i--)
			if(objs[i]->centerable){
				first = i;
				break;
			}
		if(first != -1) current_obj = first;
	}
	
}

void obj_up(){
	camera_div = 10;
	if(!draw_all){
		if(current_obj + 1 >= objs.size())
			current_obj = 0;
		else current_obj++;
	}else{
		int first = -1;
		for(int i = 0; i < objs.size(); i++)
			if(objs[i]->centerable){
				first = i;
				break;
			}
		for(int i = current_obj + 1; i < objs.size(); i++)
			if(objs[i]->centerable){
				first = i;
				break;
			}
		if(first != -1) current_obj = first;
	}
	
}

void normal_key_func(unsigned char c, int x, int y){
	if (c == 'a' || c == 's'){
		obj_down();
	}
	else if (c == 'd' || c == 'w'){
		obj_up();
	}
	else if (c == 'f'){ // fixed position
		fixed = !fixed;
	}
	else if (c == 'r'){ // reset position
		reset_position();
	}
	else if (c == 'l'){ // toggle draw all objects
		draw_all = !draw_all;
	}
	else if (c == 'e'){ // toggle draw axis
		draw_axis = !draw_axis;
	}
	else if (c == 'h'){ // toggle draw orbits
		draw_orbits = !draw_orbits;
	}
	
	else if (c == 'q'){ // toggle show help menu
		show_help = !show_help;
	}
	else if(current_obj != 0 && c == 'o'){ // Automatic orbit
		auto_orbit = !auto_orbit;
	}
	
}

void special_key_func(int c, int x, int y){
	switch (c)
	{
	case 100: // Left
		obj_down();
		break;
	case 102: // Right
		obj_up();
		break;
	case 103: // Down
		obj_down();
		break;
	case 101: // Up
		obj_up();
		break;
	default:
		break;
	}
	
}

void drag_func(int x, int y){

	if(fixed) return;

	if(last_x == -1) last_x = x;
	if(last_y == -1) last_y = y;

	int x_delta = last_x - x;
	int y_delta = last_y - y;

	if(y_delta < 0) y_delta = std::max(-30, y_delta);
	else y_delta = std::min(30, y_delta);

	if (cos(m_lat) > 0.99 && y_delta < 0)
		y_delta = 0;
	else if (cos(m_lat) < -0.99 && y_delta > 0)
		y_delta = 0;

	m_lon += (up_y == 1 ? -x_delta : x_delta) * mouse_speed;
	m_lat += y_delta * mouse_speed;

	last_x = x;
	last_y = y;

	
}

void mouse_func(int button, int state, int x, int y){

	if(fixed) return;

	if(state == 0){
		last_x = x;
		last_y = y;
	}
	float delta = 0;
	switch (button)
	{
	case 3:
		//center_dist -= zoom_speed;
		center_dist -= (center_dist - 1) * zoom_speed;
		if(center_dist < 1.01f) center_dist = 1.01f;
		break;
	case 4:
		//center_dist += zoom_speed;
		center_dist += (center_dist - 1) * zoom_speed;
		if (center_dist > 200) center_dist = 200;
		break;
	default:
		break;
	}

	
}

void read_group(TiXmlElement* group, std::vector<TRANSFORM> transforms){//point pos, point rot, point scale){

	TiXmlElement* son = group->FirstChildElement();

	for (; son!=nullptr; son = son->NextSiblingElement()){
		if (strcmp(son->Value(), "transform") == 0){
			TiXmlElement* trans1 = son->FirstChildElement();
			for(; trans1!=nullptr; trans1 = trans1->NextSiblingElement()){
				
				if(strcmp(trans1->Value(), "position") == 0 || strcmp(trans1->Value(), "translate") == 0){
					
					TRANSFORM new_trans = new transform();
					const char* time_s = trans1->Attribute("time");
					if(time_s != NULL){
						// Animated translate
						new_trans->time  = atof(time_s);
						const char* align_s = trans1->Attribute("align");
						new_trans->align = strcmp(align_s, "True") == 0 || strcmp(align_s, "true") == 0 ? true: false;
						
						int point_count = 0;
						for(TiXmlElement* point = trans1->FirstChildElement(); point != nullptr; point = point->NextSiblingElement())
							point_count++;
						
						float (*points)[3] = new float[point_count][3];

						int i = 0;
						for(TiXmlElement* point = trans1->FirstChildElement();
							point != nullptr; point = point->NextSiblingElement(), i++){
							
							points[i][0] = atof(point->Attribute("x"));
							points[i][1] = atof(point->Attribute("y"));
							points[i][2] = atof(point->Attribute("z"));
						}
						new_trans->point_count = point_count;
						new_trans->points = points;
						new_trans->type  = 3;
					}else{
						// Normal 
						new_trans->p->x = atof(trans1->Attribute("x"));
						new_trans->p->y = atof(trans1->Attribute("y"));
						new_trans->p->z = atof(trans1->Attribute("z"));
						new_trans->type = 0;
					}
					transforms.push_back(new_trans);
				}
				else if(strcmp(trans1->Value(), "rotation") == 0 || strcmp(trans1->Value(), "rotate") == 0){
					
					TRANSFORM new_trans = new transform();
					const char* time_s = trans1->Attribute("time");
					int x = atoi(trans1->Attribute("x"));
					int y = atoi(trans1->Attribute("y"));
					int z = atoi(trans1->Attribute("z"));

					if (time_s != NULL){
						// Animated rotate
						new_trans->time = atof(time_s);
						new_trans->p->x = x;
						new_trans->p->y = y;
						new_trans->p->z = z;
						new_trans->type = 4;
					}else {
						// Normal rotate
						float angle = atof(trans1->Attribute("angle"));
						if (x == 1)
							new_trans->p->x = angle;
						if (y == 1)
							new_trans->p->y = angle;
						if (z == 1)
							new_trans->p->z = angle;
						new_trans->type = 1;
					}
					transforms.push_back(new_trans);
				}
				else if(strcmp(trans1->Value(), "scale") == 0){
					
					TRANSFORM new_trans = new transform();
					new_trans->p->x = atof(trans1->Attribute("x"));
					new_trans->p->y = atof(trans1->Attribute("y"));
					new_trans->p->z = atof(trans1->Attribute("z"));
					new_trans->type = 2;
					transforms.push_back(new_trans);
				}
			}
		}
		else if (strcmp(son->Value(), "group") == 0){
			read_group(son, transforms);
		}
		else if (strcmp(son->Value(), "models") == 0){
			
			TiXmlElement* model = son->FirstChildElement();
			for (; model!=nullptr; model = model->NextSiblingElement()){
	
				const char* centerables = model->Attribute("centerable");
				bool centerable = false;
				
				if(centerables != NULL && strcmp(centerables, "true") == 0) 
					centerable = true;
				
				read_object(model, model->Attribute("file"), centerable, transforms);
			}
		}
	}
}

void read_lights(TiXmlElement* lights_e){

	int l_idx = GL_LIGHT0;

	TiXmlElement* l = lights_e->FirstChildElement();

	for(; l!=nullptr; l = l->NextSiblingElement()){

		LIGHT light = new struct light();

		glEnable(l_idx);

		const char* px = l->Attribute("posx");
		const char* py = l->Attribute("posy");
		const char* pz = l->Attribute("posz");

		const char* dx = l->Attribute("dirx");
		const char* dy = l->Attribute("diry");
		const char* dz = l->Attribute("dirz");

		const char* ct = l->Attribute("cutoff");

		const char* tp = l->Attribute("type");

		const char* intens_char = l->Attribute("intensity");

		if (strcmp(tp, "spot") == 0){
			light->pos[0] = atof(px);
			light->pos[1] = atof(py);
			light->pos[2] = atof(pz);
			light->dir[0] = atof(dx);
			light->dir[1] = atof(dy);
			light->dir[2] = atof(dz);
			light->cutoff = atof(ct);
			light->type = 2;
		}
		else if (strcmp(tp, "point") == 0){
			light->pos[0] = atof(px);
			light->pos[1] = atof(py);
			light->pos[2] = atof(pz);
			light->type = 0;
		}
		else if (strcmp(tp, "directional") == 0){
			light->dir[0] = atof(dx);
			light->dir[1] = atof(dy);
			light->dir[2] = atof(dz);
			light->type = 1;
		}

		float intensity = intens_char == NULL ? 1 : atof(intens_char);

		float dark[4] = {0.1, 0.1, 0.1, 1.0};
		float white[4] = {intensity, intensity, intensity, 1.0};

		// light colors
		glLightfv(l_idx, GL_AMBIENT, dark);
		glLightfv(l_idx, GL_DIFFUSE, white);
		glLightfv(l_idx, GL_SPECULAR, white);

		lights.push_back(light);
		l_idx++;
	}
}

bool init_engine(char* file_path){

	TiXmlDocument xml_file(file_path);
	bool b = xml_file.LoadFile();
	if (!b){
		std::cout << "ERROR opening XML" << std::endl;
		return false;
	}

	TiXmlElement* root = xml_file.RootElement();

	TiXmlElement* cam = root->FirstChildElement();

	TiXmlElement* pos = cam->FirstChildElement();
	pos_x = atof(pos->Attribute("x"));
	pos_y = atof(pos->Attribute("y"));
	pos_z = atof(pos->Attribute("z"));

	TiXmlElement* lAt = pos->NextSiblingElement();
	lAt_x = atof(lAt->Attribute("x"));
	lAt_y = atof(lAt->Attribute("y"));
	lAt_z = atof(lAt->Attribute("z"));

	TiXmlElement* up = lAt->NextSiblingElement();
	up_x = atof(up->Attribute("x"));
	up_y = atof(up->Attribute("y"));
	up_z = atof(up->Attribute("z"));

	TiXmlElement* proj = up->NextSiblingElement();
	proj_fov = atof(proj->Attribute("fov"));	
	proj_near = atof(proj->Attribute("near"));
	proj_far = atof(proj->Attribute("far"));

	TiXmlElement* main_group = cam->NextSiblingElement();

	if (strcmp(main_group->Value(), "lights") == 0){
		read_lights(main_group);
		main_group = main_group->NextSiblingElement();
	}

	std::vector<TRANSFORM> transforms;
	const char* o_c;
	if ((o_c = main_group->Attribute("orbit_correction")) != NULL)
		orbit_correction = atof(o_c);
	else orbit_correction = 0;
	read_group(main_group, transforms);

	reset_position();
	return true;
}

int main(int argc, char** argv)
{
	if(argc < 2){
		std::cout << "Provide XML config file" << std::endl;
		return 0;
	}

	// GLUT init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width, height);
	glutCreateWindow("ENGINE");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glEnable(GL_RESCALE_NORMAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

	// Activate lighting
	glEnable(GL_LIGHTING);

	// Background color
	glClearColor(0.01, 0.01, 0.08, 1);

	// Iniciar glew
	glewInit();

	// Iniciar o DevIL
	ilInit();
    
	// Callback registry
	glutReshapeFunc( change_size );
	glutDisplayFunc( render_scene );
	glutIdleFunc( render_scene );

	// Input functions
	glutMotionFunc( drag_func );
	glutMouseFunc( mouse_func );
	glutKeyboardFunc( normal_key_func );
	glutSpecialFunc( special_key_func );

	// Configure engine
	if(!init_engine(argv[1])) return 0;
	
	// enter GLUTs main cycle
	glutMainLoop();
	
	return 1;
}