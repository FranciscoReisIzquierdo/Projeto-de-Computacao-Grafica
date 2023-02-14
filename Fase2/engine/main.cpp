#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <list>
#include <fstream>
#include <string>
#include <iostream>
#include "tinyxml/tinyxml.h"


struct point{
	float x = 0;
	float y = 0;
	float z = 0;
};

struct transform{
	point p;
	int type = -1;
};

struct object{
	std::list<point> vertices;
	std::list<transform> transforms;
	point position;
	point scale;
	point color;
	bool centerable = false;
};

// All objects loaded
std::list<object> objs;

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

// Last position
float last_pos_x;
float last_pos_y;
float last_pos_z;

// Projection
float proj_fov = 60;
float proj_near = 1;
float proj_far = 1000;
float ratio = 1;

// Camera position
int last_x = -1;
int last_y = -1;
float m_lat = 0.5;
float m_lon = 0.0;
float center_dist = 3.0;
float mouse_speed = 0.007;
bool fixed = false;
bool draw_all = true;
bool draw_axis = false;

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

void render_object(std::list<object>::iterator oit){
	std::list<point>::iterator vetit;
	srand(1);
	int i = 0;
	int f = 0;
	float c = 0.05;
	glPushMatrix();
	for(std::list<transform>::iterator trans = oit->transforms.begin(); trans !=  oit->transforms.end(); trans++){
		switch (trans->type){
			case 0:
				// Position
				glTranslatef(trans->p.x, trans->p.y, trans->p.z);
				break;
			case 1:
				// Rotation
				glRotatef(trans->p.x, 1, 0, 0);
				glRotatef(trans->p.y, 0, 1, 0);
				glRotatef(trans->p.z, 0, 0, 1);
				break;
			case 2:
				// Scale
				glScalef(trans->p.x, trans->p.y, trans->p.z);
				break;
			default:
				break;
		}
	}
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	oit->position.x = modelview[12];
	oit->position.y = modelview[13];
	oit->position.z = modelview[14];

	// Draw vertices
	glBegin(GL_TRIANGLES);
	for(vetit = oit->vertices.begin(); vetit != oit->vertices.end();i++, vetit++){
		// Set color
		if(i % 3 == 0) f++;
		switch (f % 3)
		{
		case 0:
			glColor3f(
				oit->color.x,
				oit->color.y,
				oit->color.z);
			break;
		case 1:
			glColor3f(
				oit->color.x + c,
				oit->color.y + c,
				oit->color.z + c);
			break;
		default:
			glColor3f(
				oit->color.x - c,
				oit->color.y - c,
				oit->color.z - c);
			break;
		}
		// Draw vertice
		glVertex3f(
			vetit->x,
			vetit->y,
			vetit->z);
	}
	glEnd();
	glPopMatrix();
}

void render_all_objects(){
	// Render all objects in scene
	std::list<object>::iterator oit;
	for(oit = objs.begin(); oit != objs.end(); oit++){
		render_object(oit);
	}
}

std::list<object>::iterator get_object(int n){
	auto b = objs.begin();
	std::advance(b, n);
	return b;
}

void render_axis() {

	float length = 1000.0;

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

    glEnd();
}

void draw_string(float x, float y, float z, char *string) {

	glColor3f(1, 1, 1);
	glRasterPos3d(x, y, z);

	for (char* c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
}

void draw_ui(){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	float x2 = 0.99 - 377.0 / (float)width;
	float y2 = 0.99 - 592.0 / (float)height;

	char str0[100] = "HELP";
	draw_string(-0.98, 0.99 - (0.99 - y2)/15.0, 0, str0);
	char str1[100] = "Change Obj => ARROWS";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 2.5, 0, str1);
	char str2[100] = "Toggle drag => F";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 4, 0, str2);
	char str3[100] = "Reset Position => R";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 5.5, 0, str3);
	char str4[100] = "Zoom => ScrollWheel";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 7, 0, str4);
	char str5[100] = "Toggle Axis => E";
	draw_string(-0.98, 0.99 - (0.99 - y2)/14.0 * 8.5, 0, str5);

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
	std::list<object>::iterator cur_obj = get_object(current_obj);
	float delta_x, delta_y, delta_z, lAt_x_delta, lAt_y_delta, lAt_z_delta;
	if(current_obj != -1){

		delta_x = ((((cur_obj->scale.x + cur_obj->scale.y) / 2) * center_dist * cos(m_lon) * sin(m_lat) + cur_obj->position.x) - last_pos_x) / 10;

		if(up_y == 1){
			delta_y = ((((cur_obj->scale.x + cur_obj->scale.z) / 2) * center_dist * cos(m_lat) + cur_obj->position.y) - last_pos_y) / 10;
			delta_z = ((((cur_obj->scale.x + cur_obj->scale.z) / 2) * center_dist * sin(m_lon) * sin(m_lat) + cur_obj->position.z) - last_pos_z) / 10;
		}
		else if(up_z == 1){
			delta_y = ((((cur_obj->scale.x + cur_obj->scale.y) / 2) * center_dist * sin(m_lon) * sin(m_lat) + cur_obj->position.y) - last_pos_y) / 10;
			delta_z = ((((cur_obj->scale.x + cur_obj->scale.y) / 2) * center_dist * cos(m_lat) + cur_obj->position.z) - last_pos_z) / 10;
		}

		lAt_x_delta = (cur_obj->position.x - cur_lAt_x) / 10;
		lAt_y_delta = (cur_obj->position.y - cur_lAt_y) / 10;
		lAt_z_delta = (cur_obj->position.z - cur_lAt_z) / 10;

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


void render_scene(void)
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw UI
	draw_ui();
	//glDisable(GL_CULL_FACE);
	
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

	// Draw
	if (draw_axis) render_axis();
	if (draw_all) render_all_objects();
	else {
		if(current_obj == -1) current_obj = 0;
		render_object(get_object(current_obj));
	}

	// End of frame
	glutSwapBuffers();
}

void read_object(std::string path, float r, float g, float b, bool centerable, std::list<transform> orders){
	std::ifstream file(path);
	std::string text;
	getline(file, text);
	int num = atoi(text.c_str());
	getline(file, text);
	getline(file, text);
	object obj;
	for(int i = 0; i < num; i++){
		point p;
		getline(file, text);
		p.x = atof(text.c_str());
		getline(file, text);
		p.y = atof(text.c_str());
		getline(file, text);
		p.z = atof(text.c_str());
		obj.vertices.push_back(p);
		getline(file, text);
		getline(file, text);
	}
	// Color
	point cor1; cor1.x = r; cor1.y = g; cor1.z = b;
	obj.color = cor1;
	// Centering
	obj.centerable = centerable;
	// Get World Position and Scale
	point scale;
	scale.x = 1;
	scale.y = 1;
	scale.z = 1;
	for(std::list<transform>::iterator trans = orders.begin(); trans != orders.end(); trans++){
		switch (trans->type){
			case 0:
				// Position
				break;
			case 1:
				// Rotation
				break;
			case 2:
				// Scale
				scale.x *= trans->p.x;
				scale.y *= trans->p.y;
				scale.z *= trans->p.z;
				break;
			default:
				break;
		}
	}
	obj.scale = scale;
	// Transformation order
	obj.transforms.assign(orders.begin(), orders.end());

	objs.push_back(obj);
	file.close();
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

void obj_dow(){
	if(!draw_all){
		if(current_obj - 1 < 0)
			current_obj = objs.size() - 1;
		else current_obj--;
	}else{
		int first = -1;
		for(int i = objs.size() - 1; i >= 0; i--)
			if(get_object(i)->centerable){
				first = i;
				break;
			}
		for(int i = current_obj - 1; i >= 0; i--)
			if(get_object(i)->centerable){
				first = i;
				break;
			}
		if(first != -1) current_obj = first;
	}
}

void obj_up(){
	if(!draw_all){
		if(current_obj + 1 >= objs.size())
			current_obj = 0;
		else current_obj++;
	}else{
		int first = -1;
		for(int i = 0; i < objs.size(); i++)
			if(get_object(i)->centerable){
				first = i;
				break;
			}
		for(int i = current_obj + 1; i < objs.size(); i++)
			if(get_object(i)->centerable){
				first = i;
				break;
			}
		if(first != -1) current_obj = first;
	}
}

void normal_key_func(unsigned char c, int x, int y){
	if (c == 'a' || c == 's'){
		obj_dow();
	}
	else if (c == 'd' || c == 'w'){
		obj_up();
	}
	else if (c == 'f'){ // fixed position
		fixed = !fixed;
	}
	else if(c == 'r'){ // reset position
		reset_position();
	}
	else if (c == 'o'){ // toggle draw all objects
		draw_all = !draw_all;
	}
	else if(c == 'e'){ // toggle draw axis
		draw_axis = !draw_axis;
	}
}

void special_key_func(int c, int x, int y){
	switch (c)
	{
	case 100: // Left
		obj_dow();
		break;
	case 102: // Right
		obj_up();
		break;
	case 103: // Down
		obj_dow();
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

	switch (button)
	{
	case 3:
		center_dist -= 0.1;
		break;
	case 4:
		center_dist += 0.1;
		break;
	default:
		break;
	}
}

void read_group(TiXmlElement* group, std::list<transform> transforms){//point pos, point rot, point scale){

	TiXmlElement* son = group->FirstChildElement();

	for (; son!=nullptr; son = son->NextSiblingElement()){
		if (strcmp(son->Value(), "transform") == 0){
			TiXmlElement* trans1 = son->FirstChildElement();
			for(; trans1!=nullptr; trans1 = trans1->NextSiblingElement()){
				if(strcmp(trans1->Value(), "position") == 0 || strcmp(trans1->Value(), "translate") == 0){
					transform new_trans;
					new_trans.p.x = atof(trans1->Attribute("x"));
					new_trans.p.y = atof(trans1->Attribute("y"));
					new_trans.p.z = atof(trans1->Attribute("z"));
					new_trans.type = 0;
					transforms.push_back(new_trans);
				}
				else if(strcmp(trans1->Value(), "rotation") == 0 || strcmp(trans1->Value(), "rotate") == 0){
					transform new_trans;
					int x = atoi(trans1->Attribute("x"));
					int y = atoi(trans1->Attribute("y"));
					int z = atoi(trans1->Attribute("z"));
					float angle = atof(trans1->Attribute("angle"));
					if (x == 1)
						new_trans.p.x = angle;
					if (y == 1)
						new_trans.p.y = angle;
					if (z == 1)
						new_trans.p.z = angle;
					new_trans.type = 1;
					transforms.push_back(new_trans);
				}
				else if(strcmp(trans1->Value(), "scale") == 0){
					transform new_trans;
					new_trans.p.x = atof(trans1->Attribute("x"));
					new_trans.p.y = atof(trans1->Attribute("y"));
					new_trans.p.z = atof(trans1->Attribute("z"));
					new_trans.type = 2;
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
				const char* rs = model->Attribute("r");
				const char* gs = model->Attribute("g");
				const char* bs = model->Attribute("b");
				const char* centerables = model->Attribute("centerable");
				float r = 0.5f, g = 0.0f, b = 0.0f;
				bool centerable = false;
				if(centerables != NULL && strcmp(centerables, "true") == 0) centerable = true;
				if(rs != NULL && gs != NULL && bs != NULL){
					r = atof(model->Attribute("r"));
					g = atof(model->Attribute("g"));
					b = atof(model->Attribute("b"));
				}
				read_object(model->Attribute("file"), r, g, b, centerable, transforms);
			}
		}
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
	std::list<transform> transforms;
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
	glClearColor(0.01, 0.01, 0.08, 1);
    
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