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

struct ponto {
    float x;
    float y;
    float z;
} ;

struct objeto {
    std::list<ponto> vertices;
} ;




// Função que dá print
void print (std::string print) {
    std::cout << print << std::endl;
}

// Função que escreve para ficheiro

void makeFile(std::string fich, objeto obj) {
    std::ofstream file(fich);
    file << obj.vertices.size() << std::endl;
    file << "\n" << std::endl;
    std::list<ponto>::iterator it;
    for (it = obj.vertices.begin(); it != obj.vertices.end(); ++it){
        file << it->x << std::endl;
        file << it->y << std::endl;
        file << it->z << std::endl;
        file << "\n" << std::endl;
    }
    file.close();
}

// Função qur adiciona pontos a uma std::lista
objeto addPonto (float x, float y, float z, objeto obj) {
    ponto p;
    p.x = x;
    p.y = y;
    p.z = z;
    obj.vertices.push_back(p);
    return obj;
};

// Função do plano
void planeFunc(int tamanho, int div, std::string fich) {
    objeto obj;
    float count = (float) tamanho/div;

    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            obj = addPonto(x,y,0,obj);
            obj = addPonto(x+count,y,0,obj);
            obj = addPonto(x+count,y+count,0,obj);

            //Triangulo 2
            obj = addPonto(x,y,0,obj);
            obj = addPonto(x+count,y+count,0,obj);
            obj = addPonto(x,y+count,0,obj);
        }
    }
    
    makeFile(fich, obj);
}

// Função do plano
void boxFunc(int tamanho, int div, std::string fich) {
    objeto obj;
    float count = (float) tamanho/div;

    // Base inferior
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            obj = addPonto(x,y,-tamanho/2.0f,obj);
            obj = addPonto(x+count,y+count,-tamanho/2.0f,obj);
            obj = addPonto(x+count,y,-tamanho/2.0f,obj);

            //Triangulo 2
            obj = addPonto(x,y,-tamanho/2.0f,obj);
            obj = addPonto(x,y+count,-tamanho/2.0f,obj);
            obj = addPonto(x+count,y+count,-tamanho/2.0f,obj);
        }
    }

    // Base superior
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            obj = addPonto(x,y,tamanho/2.0f,obj);
            obj = addPonto(x+count,y,tamanho/2.0f,obj);
            obj = addPonto(x+count,y+count,tamanho/2.0f,obj);

            //Triangulo 2
            obj = addPonto(x,y,tamanho/2.0f,obj);
            obj = addPonto(x+count,y+count,tamanho/2.0f,obj);
            obj = addPonto(x,y+count,tamanho/2.0f,obj);
        }
    }

    // Base frontal
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count) {
            //Triangulo 1
            obj = addPonto(x,-tamanho/2.0f,z,obj);
            obj = addPonto(x+count,-tamanho/2.0f,z,obj);
            obj = addPonto(x+count,-tamanho/2.0f,z+count,obj);

            //Triangulo 2
            obj = addPonto(x,-tamanho/2.0f,z,obj);
            obj = addPonto(x+count,-tamanho/2.0f,z+count,obj);
            obj = addPonto(x,-tamanho/2.0f,z+count,obj);
        }
    }

    // Base de trás
    for (float x = -tamanho/2.0f; x < tamanho/2.0f; x+=count ) {
        for(float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count) {
            //Triangulo 1
            obj = addPonto(x,tamanho/2.0f,z,obj);
            obj = addPonto(x+count,tamanho/2.0f,z+count,obj);
            obj = addPonto(x+count,tamanho/2.0f,z,obj);

            //Triangulo 2
            obj = addPonto(x,tamanho/2.0f,z,obj);
            obj = addPonto(x,tamanho/2.0f,z+count,obj);
            obj = addPonto(x+count,tamanho/2.0f,z+count,obj);
        }
    }

    // Base lateral direita
    for (float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            obj = addPonto(-tamanho/2.0f,y,z,obj);
            obj = addPonto(-tamanho/2.0f,y,z+count,obj);
            obj = addPonto(-tamanho/2.0f,y+count,z+count,obj);

            //Triangulo 2
            obj = addPonto(-tamanho/2.0f,y,z,obj);
            obj = addPonto(-tamanho/2.0f,y+count,z+count,obj);
            obj = addPonto(-tamanho/2.0f,y+count,z,obj);
        }
    }

    // Base lateral esquerdo
    for (float z = -tamanho/2.0f; z < tamanho/2.0f; z+=count ) {
        for(float y = -tamanho/2.0f; y < tamanho/2.0f; y+=count) {
            //Triangulo 1
            obj = addPonto(tamanho/2.0f,y,z,obj);
            obj = addPonto(tamanho/2.0f,y+count,z+count,obj);
            obj = addPonto(tamanho/2.0f,y,z+count,obj);

            //Triangulo 2
            obj = addPonto(tamanho/2.0f,y,z,obj);
            obj = addPonto(tamanho/2.0f,y+count,z,obj);
            obj = addPonto(tamanho/2.0f,y+count,z+count,obj);
        }
    }
    makeFile(fich, obj);
}

void coneFunc(int radius, int height, int slices, int stacks, std::string fich) {
    
    float angulo = (float)(2*M_PI) / ((float)slices);
    float anguloInicial = 0;
    float alturaBase = -height / 2.0;
    float rAtual = (float)radius;
    float rSeguinte = 0;
    float hAtual = (float)alturaBase;
    float hSeguinte = 0;
    objeto obj;


    //base
    while(anguloInicial < 2 * M_PI - angulo) {
        obj = addPonto(radius * cos(anguloInicial + angulo), radius * sin(anguloInicial + angulo), alturaBase, obj);
        obj = addPonto(radius * cos(anguloInicial), radius * sin(anguloInicial), alturaBase, obj);
        obj = addPonto(0, 0, alturaBase, obj);
        anguloInicial += angulo;
    }

    
    //altura
    anguloInicial = 0;
    for (int i = 0; i < stacks; i++) {
        rSeguinte = rAtual - (float)radius / (float)stacks;
        hSeguinte = hAtual + (float)height / (float)stacks;
        while (anguloInicial < 2 * M_PI - angulo) {
            obj = addPonto(rAtual * cos(anguloInicial), rAtual * sin(anguloInicial), hAtual, obj);
            obj = addPonto(rSeguinte * cos(anguloInicial + angulo), rSeguinte * sin(anguloInicial + angulo), hSeguinte, obj);
            obj = addPonto(rSeguinte * cos(anguloInicial), rSeguinte * sin(anguloInicial), hSeguinte, obj);

            obj = addPonto(rAtual * cos(anguloInicial), rAtual * sin(anguloInicial), hAtual, obj);
            obj = addPonto(rAtual * cos(anguloInicial + angulo), rAtual * sin(anguloInicial + angulo), hAtual, obj);
            obj = addPonto(rSeguinte * cos(anguloInicial + angulo), rSeguinte * sin(anguloInicial + angulo), hSeguinte, obj);
   
            anguloInicial += angulo;
        }
        hAtual = hSeguinte;
        rAtual = rSeguinte;
        anguloInicial = 0;
    }

    makeFile(fich, obj);
}


void sphereFunc(int radius, int slices, int stacks, std::string fich){
    objeto obj;
    float lat = 0.0;
    float lon = 0.0;
    float lat_step = (float) M_PI / (float) stacks;
    float lon_step = (float) 2 * M_PI / (float) slices;
    
    while(lat < M_PI - lat_step){
        float next_lat = lat + lat_step;

        while(lon < 2 * M_PI - lon_step){
            float next_lon = lon + lon_step;
            // First triangle
            obj = addPonto(radius * cos(lon) * sin(lat), radius * sin(lon) * sin(lat), radius * cos(lat), obj);
            obj = addPonto(radius * cos(lon) * sin(next_lat), radius * sin(lon) * sin(next_lat), radius * cos(next_lat), obj);
            obj = addPonto(radius * cos(next_lon) * sin(lat), radius * sin(next_lon) * sin(lat), radius * cos(lat), obj);

            // Second triangle
            obj = addPonto(radius * cos(next_lon) * sin(lat), radius * sin(next_lon) * sin(lat), radius * cos(lat), obj);
            obj = addPonto(radius * cos(lon) * sin(next_lat), radius * sin(lon) * sin(next_lat), radius * cos(next_lat), obj);
            obj = addPonto(radius * cos(next_lon) * sin(next_lat), radius * sin(next_lon) * sin(next_lat), radius * cos(next_lat), obj);

            lon = next_lon;
        }

        lon = 0.0;
        lat = next_lat;
    }
    makeFile(fich, obj);
}


void torusFunc(float innerRadius, float outterRadius, int slices, int rings, std::string fich){
    objeto obj;
    std::list<ponto> first_circle;
    float angle = 0;
    float slice_angle_step = (2 * M_PI) / (float) slices;
    float ring_angle_step = (2 * M_PI) / (float) rings;
    float radius = (outterRadius - innerRadius) / 2.0f;

    // draw the first circle
    while(angle < 2 * M_PI - slice_angle_step){
        ponto p;
        p.x = cos(angle) * radius + (outterRadius - radius);
        p.y = cos(angle) * radius + (outterRadius - radius);
        p.z = sin(angle) * radius;
        first_circle.push_back(p);
        angle += slice_angle_step;
    }
    angle = 0;
    while(angle < 2 * M_PI - ring_angle_step){
        for (std::list<ponto>::iterator p = first_circle.begin(); p != first_circle.end(); ++p){
            std::advance(p, 1);
            if(p == first_circle.end())
                p = first_circle.begin();
            std::list<ponto>::iterator p_next = p;
            if(p == first_circle.begin())
                p = first_circle.end();
            std::advance(p, -1);
            // Triangle 1
            obj = addPonto(sin(angle) * p->x, cos(angle) * p->y, p->z, obj);
            obj = addPonto(sin(angle + ring_angle_step) * p_next->x, cos(angle + ring_angle_step) * p_next->y, p_next->z, obj);
            obj = addPonto(sin(angle + ring_angle_step) * p->x, cos(angle + ring_angle_step) * p->y, p->z, obj);
            // Triangle 2
            obj = addPonto(sin(angle) * p->x, cos(angle) * p->y, p->z, obj);
            obj = addPonto(sin(angle) * p_next->x, cos(angle) * p_next->y, p_next->z, obj);
            obj = addPonto(sin(angle + ring_angle_step) * p_next->x, cos(angle + ring_angle_step) * p_next->y, p_next->z, obj);
        }
        angle += ring_angle_step;
    }
    makeFile(fich, obj);
}

// Função main que recebe os argumentos passados no terminal
int main(int argc, char **argv) {

    if(argc == 0) {
        print("Arguments invalid !!!");
        return 0;
    }

    if (strcmp(argv[1], "plane") == 0){
            if(argc != 5) {
                print("Wrong number of arguments !!! ");
                return 0;
            }
            planeFunc(atoi(argv[2]),atoi(argv[3]),argv[4]);
    }
    else if (strcmp(argv[1], "box") == 0){
            if(argc != 5) {
                print("Wrong number of arguments !!! ");
                return 0;
            }
            boxFunc(atoi(argv[2]),atoi(argv[3]),argv[4]);
    }
    else if (strcmp(argv[1], "cone") == 0){
            if(argc != 7) {
                print("Wrong number of arguments !!! ");
                return 0;
            }
            coneFunc(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
    }
    else if (strcmp(argv[1], "sphere") == 0){
            if(argc != 6) {
                print("Wrong number of arguments !!! ");
                return 0;
            }
            sphereFunc(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5]);
    }
    else if (strcmp(argv[1], "torus") == 0){
            if(argc != 7) {
                print("Wrong number of arguments !!! ");
                return 0;
            }
            torusFunc(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
    }

    return 0;
}