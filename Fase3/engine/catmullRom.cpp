#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "catmullRom.h"

#define TESSELATION 100.0

float prev_y[3] = {0,1,0};



void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
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


void multMatrixVector(float m[4][4], float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j][k];
		}
	}

}



void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};


	for(int i = 0; i < 3; i++) {

		float p[4] = {p0[i], p1[i], p2[i], p3[i]};
		float a[4];

		// Compute A = M * P
		multMatrixVector(m, p, a);
		
		// Compute pos = T * A
		pos[i] = powf(t,3.0) * a[0] + powf(t,2.0) * a[1] + t * a[2] + a[3];
		
		// compute deriv = T' * A
		deriv[i] = 3*powf(t,2.0) * a[0] + 2 * t * a[1] + a[2];
	}
}



void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, int point_count, float (*p)[3]) {

	float t = gt * point_count; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + point_count-1)%point_count;	
	indices[1] = (indices[0]+1)%point_count;
	indices[2] = (indices[1]+1)%point_count; 
	indices[3] = (indices[2]+1)%point_count;

	getCatmullRomPoint(t, p[indices[0]], p[indices[1]], p[indices[2]], p[indices[3]], pos, deriv);
}


void renderCatmullRomCurve(float (*p)[3], int point_count) {

	float pos[3];
	float deriv[3];

	// Draw curve using line segments with GL_LINE_LOOP
	glBegin(GL_LINE_LOOP);
    glColor3f(0.8f, 0.8f, 0.8f);
	float gt = 0;
	while (gt < 1) {
		getGlobalCatmullRomPoint(gt, pos, deriv, point_count, p);
		glVertex3f(pos[0],pos[1],pos[2]);
		gt += 1.0/TESSELATION;
	}
	glEnd();
}



void catmullRomAnimation(float time, bool align, int point_count, float (*p)[3]){

    float t = glutGet(GLUT_ELAPSED_TIME) / 1000.0f / time;

    float pos[3];
	float deriv[3];

    renderCatmullRomCurve(p, point_count);

	getGlobalCatmullRomPoint(t, pos, deriv, point_count, p);

	glTranslatef(pos[0], pos[1], pos[2]);

    if (align){

        float x[3] = {deriv[0], deriv[1], deriv[2]};
        normalize(x);
        float z[3];
        cross(x, prev_y, z);
        normalize(z);
        float y[3];
        cross(z,x,y);
        normalize(y);
        memcpy(prev_y, y, 3 * sizeof(float));

        float m[16];

        buildRotMatrix(x,y,z,m);

        glMultMatrixf(m);
    }
}