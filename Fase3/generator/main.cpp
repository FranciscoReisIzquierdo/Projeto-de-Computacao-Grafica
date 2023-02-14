#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <cstring>
#include <math.h>
#include <vector>

typedef struct ponto {
    float x;
    float y;
    float z;
} *PONTO;

typedef struct objeto {
    std::vector<PONTO> vertices;
    std::vector<unsigned int> indices;
} *OBJETO;


float bezierM[4][4] = {{ -1.0f, 3.0f, -3.0f, 1.0f},
					   { 3.0f, -6.0f, 3.0f, 0.0f},
					   { -3.0f, 3.0f, 0.0f, 0.0f},
					   { 1.0f, 0.0f, 0.0f, 0.0f}};


void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


bool compare_pontos(PONTO p1, PONTO p2){
    if(p1->x == p2->x &&
       p1->y == p2->y &&
       p1->z == p2->z)
            return true;
    return false;
}

int ponto_in_vector(PONTO p, std::vector<PONTO> vec){
    for(int i = 0; i < vec.size(); i++)
        if(compare_pontos(p, vec[i]))
            return i;
    return -1;
}

void index_vertices(OBJETO obj){
    std::vector<PONTO> new_vert;

    for (PONTO p : obj->vertices){
        int idx = ponto_in_vector(p, new_vert);
        if (idx == -1){
            idx = new_vert.size();
            new_vert.push_back(p);
        }
        obj->indices.push_back(idx);
    }
    obj->vertices = new_vert;
}


// Função que escreve para ficheiro
void makeFile(std::string fich, OBJETO obj) {
    index_vertices(obj);
    std::ofstream file(fich);
    file << obj->vertices.size() << std::endl;
    file << obj->indices.size() << std::endl;

    // Write vertices
    for (PONTO it : obj->vertices){
        file << it->x << std::endl;
        file << it->y << std::endl;
        file << it->z << std::endl;
    }

    // Write indices
    for (unsigned int it : obj->indices){
        file << it << std::endl;
    }

    file.close();
}

// Função que adiciona pontos a uma std::list
void addPonto (float x, float y, float z, OBJETO obj) {
    PONTO p = new ponto();
    p->x = x;
    p->y = z;//y;
    p->z = y;//z;
    obj->vertices.push_back(p);
}


// Função do plano
void planeFunc(float tamanho, int div, std::string fich) {
    OBJETO obj = new objeto();
    float step = (float) tamanho/div;

    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=step ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=step) {
            //Triangulo 1
            addPonto(x,y,0,obj);
            addPonto(x+step,y+step,0,obj);
            addPonto(x+step,y,0,obj);

            //Triangulo 2
            addPonto(x,y,0,obj);
            addPonto(x,y+step,0,obj);
            addPonto(x+step,y+step,0,obj);
        }
    }
    
    makeFile(fich, obj);
}


// Função do box
void boxFunc(float tamanho, int div, std::string fich) {
    OBJETO obj = new objeto();
    float count = (float) tamanho/div;

    // Base inferior
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            addPonto(x,y,-tamanho/2.0f,obj);
            addPonto(x+count,y,-tamanho/2.0f,obj);
            addPonto(x+count,y+count,-tamanho/2.0f,obj);

            //Triangulo 2
            addPonto(x,y,-tamanho/2.0f,obj);
            addPonto(x+count,y+count,-tamanho/2.0f,obj);
            addPonto(x,y+count,-tamanho/2.0f,obj);
        }
    }

    // Base superior
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            addPonto(x,y,tamanho/2.0f,obj);
            addPonto(x+count,y+count,tamanho/2.0f,obj);
            addPonto(x+count,y,tamanho/2.0f,obj);

            //Triangulo 2
            addPonto(x,y,tamanho/2.0f,obj);
            addPonto(x,y+count,tamanho/2.0f,obj);
            addPonto(x+count,y+count,tamanho/2.0f,obj);
        }
    }

    // Base frontal
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count) {
            //Triangulo 1
            addPonto(x,-tamanho/2.0f,z,obj);
            addPonto(x+count,-tamanho/2.0f,z+count,obj);
            addPonto(x+count,-tamanho/2.0f,z,obj);

            //Triangulo 2
            addPonto(x,-tamanho/2.0f,z,obj);
            addPonto(x,-tamanho/2.0f,z+count,obj);
            addPonto(x+count,-tamanho/2.0f,z+count,obj);
        }
    }

    // Base de trás
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count) {
            //Triangulo 1
            addPonto(x,tamanho/2.0f,z,obj);
            addPonto(x+count,tamanho/2.0f,z,obj);
            addPonto(x+count,tamanho/2.0f,z+count,obj);

            //Triangulo 2
            addPonto(x,tamanho/2.0f,z,obj);
            addPonto(x+count,tamanho/2.0f,z+count,obj);
            addPonto(x,tamanho/2.0f,z+count,obj);
        }
    }

    // Base lateral direita
    for (float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            addPonto(-tamanho/2.0f,y,z,obj);
            addPonto(-tamanho/2.0f,y+count,z+count,obj);
            addPonto(-tamanho/2.0f,y,z+count,obj);

            //Triangulo 2
            addPonto(-tamanho/2.0f,y,z,obj);
            addPonto(-tamanho/2.0f,y+count,z,obj);
            addPonto(-tamanho/2.0f,y+count,z+count,obj);
        }
    }

    // Base lateral esquerdo
    for (float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            addPonto(tamanho/2.0f,y,z,obj);
            addPonto(tamanho/2.0f,y,z+count,obj);
            addPonto(tamanho/2.0f,y+count,z+count,obj);

            //Triangulo 2
            addPonto(tamanho/2.0f,y,z,obj);
            addPonto(tamanho/2.0f,y+count,z+count,obj);
            addPonto(tamanho/2.0f,y+count,z,obj);
        }
    }
    makeFile(fich, obj);
}


void coneFunc(float radius, float height, int slices, int stacks, std::string fich) {
    
    OBJETO obj = new objeto();
    float angulo = (float)(2*M_PI) / ((float)slices);
    float anguloInicial = 0;
    float alturaBase = 0;//-height / 2.0;
    float rAtual = (float)radius;
    float rSeguinte = 0;
    float hAtual = (float)alturaBase;
    float hSeguinte = 0;


    //base
    while(abs(anguloInicial - 2 * M_PI) > 0.01) {
        addPonto(radius * cos(anguloInicial + angulo), radius * sin(anguloInicial + angulo), alturaBase, obj);
        addPonto(0, 0, alturaBase, obj);
        addPonto(radius * cos(anguloInicial), radius * sin(anguloInicial), alturaBase, obj);
        anguloInicial += angulo;
    }

    
    //altura
    anguloInicial = 0;
    for (int i = 0; i < stacks; i++) {
        rSeguinte = rAtual - (float)radius / (float)stacks;
        hSeguinte = hAtual + (float)height / (float)stacks;
        while (abs(anguloInicial - 2 * M_PI) > 0.01) {
            addPonto(rAtual * cos(anguloInicial), rAtual * sin(anguloInicial), hAtual, obj);
            addPonto(rSeguinte * cos(anguloInicial), rSeguinte * sin(anguloInicial), hSeguinte, obj);
            addPonto(rSeguinte * cos(anguloInicial + angulo), rSeguinte * sin(anguloInicial + angulo), hSeguinte, obj);

            addPonto(rAtual * cos(anguloInicial), rAtual * sin(anguloInicial), hAtual, obj);
            addPonto(rSeguinte * cos(anguloInicial + angulo), rSeguinte * sin(anguloInicial + angulo), hSeguinte, obj);
            addPonto(rAtual * cos(anguloInicial + angulo), rAtual * sin(anguloInicial + angulo), hAtual, obj);
   
            anguloInicial += angulo;
        }
        hAtual = hSeguinte;
        rAtual = rSeguinte;
        anguloInicial = 0;
    }

    makeFile(fich, obj);
}


void sphereFunc(float radius, int slices, int stacks, std::string fich){
    OBJETO obj = new objeto();
    float lat = 0.0;
    float lon = 0.0;
    float lat_step = (float) M_PI / (float) stacks;
    float lon_step = (float) 2 * M_PI / (float) slices;
    
    while(abs(lat - M_PI) > 0.01){
        float next_lat = lat + lat_step;

        while(abs(lon - 2 * M_PI) > 0.01){
            float next_lon = lon + lon_step;
            // First triangle
            addPonto(radius * cos(lon) * sin(lat), radius * sin(lon) * sin(lat), radius * cos(lat), obj);
            addPonto(radius * cos(next_lon) * sin(lat), radius * sin(next_lon) * sin(lat), radius * cos(lat), obj);
            addPonto(radius * cos(lon) * sin(next_lat), radius * sin(lon) * sin(next_lat), radius * cos(next_lat), obj);

            // Second triangle
            addPonto(radius * cos(next_lon) * sin(lat), radius * sin(next_lon) * sin(lat), radius * cos(lat), obj);
            addPonto(radius * cos(next_lon) * sin(next_lat), radius * sin(next_lon) * sin(next_lat), radius * cos(next_lat), obj);
            addPonto(radius * cos(lon) * sin(next_lat), radius * sin(lon) * sin(next_lat), radius * cos(next_lat), obj);

            lon = next_lon;
        }

        lon = 0.0;
        lat = next_lat;
    }
    makeFile(fich, obj);
}


void torusFunc(float innerRadius, float outterRadius, int slices, int rings, std::string fich){
    OBJETO obj = new objeto();
    std::list<ponto> first_circle;
    float angle = 0;
    float slice_angle_step = (2 * M_PI) / (float) slices;
    float ring_angle_step = (2 * M_PI) / (float) rings;
    float radius = (outterRadius - innerRadius) / 2.0f;

    // draw the first circle
    while(abs(angle - 2 * M_PI) > 0.01){
        ponto p;
        p.x = cos(angle) * radius + (outterRadius - radius);
        p.y = cos(angle) * radius + (outterRadius - radius);
        p.z = sin(angle) * radius;
        first_circle.push_back(p);
        angle += slice_angle_step;
    }
    angle = 0;
    while(abs(angle - 2 * M_PI) > 0.01){
        for (std::list<ponto>::iterator p = first_circle.begin(); p != first_circle.end(); ++p){
            std::advance(p, 1);
            if(p == first_circle.end())
                p = first_circle.begin();
            std::list<ponto>::iterator p_next = p;
            if(p == first_circle.begin())
                p = first_circle.end();
            std::advance(p, -1);
            // Triangle 1
            addPonto(sin(angle) * p->x, cos(angle) * p->y, p->z, obj);
            addPonto(sin(angle + ring_angle_step) * p->x, cos(angle + ring_angle_step) * p->y, p->z, obj);
            addPonto(sin(angle + ring_angle_step) * p_next->x, cos(angle + ring_angle_step) * p_next->y, p_next->z, obj);
            // Triangle 2
            addPonto(sin(angle) * p->x, cos(angle) * p->y, p->z, obj);
            addPonto(sin(angle + ring_angle_step) * p_next->x, cos(angle + ring_angle_step) * p_next->y, p_next->z, obj);
            addPonto(sin(angle) * p_next->x, cos(angle) * p_next->y, p_next->z, obj);
        }
        angle += ring_angle_step;
    }
    makeFile(fich, obj);
}


// Read patch file for bezier surfaces
void read_patch_file(std::string file_path, std::vector<PONTO> *vertices, std::vector<int> *indices){
    
    std::ifstream file(file_path);
	std::string text;

    getline(file, text);
	int surface_count = atoi(text.c_str());

    // Read indices
    for (int i = 0; i < surface_count; i++){
        getline(file, text);
        char* ind_str = strtok((char*)text.c_str(), " ,\n");
        indices->push_back(atoi(ind_str));
        for (int j = 0; j < 15; j++){
            ind_str = strtok(NULL, " ,\n");
            indices->push_back(atoi(ind_str));
        }
    }

    getline(file, text);
    int vertice_count = atoi(text.c_str());

    // Read vertices
    for (int i = 0; i < vertice_count; i++){
        getline(file, text);
        PONTO p = new ponto(); 
        char* vert_str = strtok((char*)text.c_str(), " ,\n");
        p->x = atof(vert_str);
        vert_str = strtok(NULL, " ,\n");
        p->y = atof(vert_str);
        vert_str = strtok(NULL, " ,\n");
        p->z = atof(vert_str);
        vertices->push_back(p);
    }
}


void getBezierPoint(float u, float v, float** matrixX, float** matrixY, float** matrixZ, float* pos) {
    float bezierMatrix[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
                               { 3.0f, -6.0f, 3.0f, 0.0f },
                               { -3.0f, 3.0f, 0.0f, 0.0f },
                               { 1.0f,  0.0f, 0.0f, 0.0f } };

    float vetorU[4] = { u * u * u, u * u, u, 1 };
    float vetorV[4] = { v * v * v, v * v, v, 1 };

    float vetorAux[4];
    float px[4];
    float py[4];
    float pz[4];

    float mx[4];
    float my[4];
    float mz[4];

    multMatrixVector((float*)bezierMatrix, vetorV, vetorAux);
    multMatrixVector((float*)matrixX, vetorAux, px);
    multMatrixVector((float*)matrixY, vetorAux, py);
    multMatrixVector((float*)matrixZ, vetorAux, pz);

    multMatrixVector((float*)bezierMatrix, px, mx);
    multMatrixVector((float*)bezierMatrix, py, my);
    multMatrixVector((float*)bezierMatrix, pz, mz);


    pos[0] = (vetorU[0] * mx[0]) + (vetorU[1] * mx[1]) + (vetorU[2] * mx[2]) + (vetorU[3] * mx[3]);
    pos[1] = (vetorU[0] * my[0]) + (vetorU[1] * my[1]) + (vetorU[2] * my[2]) + (vetorU[3] * my[3]);
    pos[2] = (vetorU[0] * mz[0]) + (vetorU[1] * mz[1]) + (vetorU[2] * mz[2]) + (vetorU[3] * mz[3]);

}



// Make bezier model
void make_bezier_model(std::vector<PONTO>* vertices, std::vector<int>* indices, int tessellation, std::string file){

    OBJETO obj = new objeto();
    
    float pos[4][3];
    float matrixX[4][4];
    float matrixY[4][4];
    float matrixZ[4][4];

    float u = 0;
    float v = 0;
    float inc = 1 / (float)tessellation;

    for (size_t p = 0; p < (*indices).size(); p += 16) {
        for (size_t i = 0; i < tessellation; i++) {
            for (size_t j = 0; j < tessellation; j++) {
                u = inc * i;
                v = inc * j;
                float u2 = inc * (i + 1);
                float v2 = inc * (j + 1);


                for (size_t a = 0; a < 4; a++) {
                    for (size_t b = 0; b < 4; b++) {

                        matrixX[a][b] = (*vertices).at((*indices).at(p + a * 4 + b))->x;
                        matrixY[a][b] = (*vertices).at((*indices).at(p + a * 4 + b))->y;
                        matrixZ[a][b] = (*vertices).at((*indices).at(p + a * 4 + b))->z;

                    }
                }

                getBezierPoint(u, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[0]);
                getBezierPoint(u, v2, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[1]);
                getBezierPoint(u2, v, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[2]);
                getBezierPoint(u2, v2, (float**)matrixX, (float**)matrixY, (float**)matrixZ, pos[3]);

                addPonto(pos[3][0], pos[3][1], pos[3][2], obj);
                addPonto(pos[0][0], pos[0][1], pos[0][2], obj);
                addPonto(pos[2][0], pos[2][1], pos[2][2], obj);

                addPonto(pos[0][0], pos[0][1], pos[0][2], obj);
                addPonto(pos[3][0], pos[3][1], pos[3][2], obj);
                addPonto(pos[1][0], pos[1][1], pos[1][2], obj);

            }
        }
    }

    makeFile(file, obj);
}


// Função main que recebe os argumentos passados no terminal
int main(int argc, char **argv) {

    if(argc == 0) {
        printf("Arguments invalid !!!");
        return 0;
    }

    if (strcmp(argv[1], "plane") == 0){
        if(argc != 5) {
            printf("Wrong number of arguments !!! ");
            return 0;
        }
        planeFunc(atof(argv[2]),atoi(argv[3]),argv[4]);
    }
    else if (strcmp(argv[1], "box") == 0){
        if(argc != 5) {
            printf("Wrong number of arguments !!! ");
            return 0;
        }
        boxFunc(atof(argv[2]),atoi(argv[3]),argv[4]);
    }
    else if (strcmp(argv[1], "cone") == 0){
        if(argc != 7) {
            printf("Wrong number of arguments !!! ");
            return 0;
        }
        coneFunc(atof(argv[2]),atof(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
    }
    else if (strcmp(argv[1], "sphere") == 0){
        if(argc != 6) {
            printf("Wrong number of arguments !!! ");
            return 0;
        }
        sphereFunc(atof(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5]);
    }
    else if (strcmp(argv[1], "torus") == 0){
        if(argc != 7) {
            printf("Wrong number of arguments !!! ");
            return 0;
        }
        torusFunc(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
    }
    else if (strcmp(argv[1], "bezier") == 0){
        if(argc != 5){
            printf("Wrong number of arguments !!!");
            return 0;
        }
        std::vector<PONTO> vertices;
        std::vector<int> indices;
        read_patch_file(argv[2], &vertices, &indices);
        make_bezier_model(&vertices, &indices, atoi(argv[3]), argv[4]);
    }

    return 0;
}