#include <GL/freeglut.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

const int point_num = 4;
const float PI = 3.14159265358979323846;
GLfloat length = 0.02f;

class coordinate {//定义点
public:
	coordinate() {}
	coordinate(float Cx, float Cy) {
		x = Cx;
		y = Cy;
	}
	float x;
	float y;
};

static vector<coordinate> points;
static coordinate vertices[point_num] = {};
static float u = 0.0;
static vector<coordinate> beizer;
static vector<coordinate> mousecoordinate;

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mousecoordinate.push_back(coordinate(((int)x - 250) / (float)250, (250 - (int)y) / (float)250));
		if (mousecoordinate.size() == point_num) {
			for (int i = 0; i < point_num; i++) {
				vertices[i] = mousecoordinate[i];
			}
			u = 1.0;
		}
		else {
			if (mousecoordinate.size() > point_num) {
				mousecoordinate.pop_back();
			}
		}
	}
}

void mousemove(int x, int y) {
	float aX = (x - 500 / 2) / (float)250;
	float aY = (500 / 2 - y) / (float)250;
	for (int i = 0; i < mousecoordinate.size(); i++) {
		if (pow((aX - mousecoordinate[i].x), 2) + pow((aY - mousecoordinate[i].y), 2) < pow(length * 2, 2)) {
			mousecoordinate[i].x = aX;
			mousecoordinate[i].y = aY;
			vertices[i].x = aX;
			vertices[i].y = aY;
			u = 1.0f;
		}
	}
}

void Coor() {
	vector<coordinate> temp(vertices, vertices + point_num);
	vector<coordinate> temp_;
	for (int i = 0; i < point_num - 1; i++) {
		for (int j = 0; j < point_num - 1 - i; j++) {
			float X = temp[j].x + (temp[j + 1].x - temp[j].x) * u;
			float Y = temp[j].y + (temp[j + 1].y - temp[j].y) * u;
			coordinate tempC(X, Y);
			temp_.push_back(tempC);
		}
		temp = temp_;
		temp_.clear();
	}
	beizer.push_back(temp[0]);
}

const int n = 1000;
void drawmousecoordinate() {
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < mousecoordinate.size(); i++) {
		glVertex2f(mousecoordinate[i].x, mousecoordinate[i].y);
	}
	glEnd();
	//顶点处加粗
	for (int i = 0; i < mousecoordinate.size(); i++) {
		glBegin(GL_POLYGON);
		for (int j = 0; j<n; ++j)
			glVertex2f(0.01f*cos(2 * PI / n*j)+ mousecoordinate[i].x, 0.01f*sin(2 * PI / n*j) + mousecoordinate[i].y);
		glEnd();
	}
}

float a = 0.0, b = 0.0, c = 0.0;
void drawBeizer() {
	glColor3f(a, b, c);
	glBegin(GL_POINTS);
	for (int i = 0; i < beizer.size(); i++) {
		glVertex2f(beizer[i].x, beizer[i].y);
	}
	glEnd();
}

void drawLineStrip() {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < point_num; i++) {
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

void display()
{
	glEnable(GL_POLYGON_SMOOTH_HINT);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(0, 0, 0);
	drawLineStrip();
	Coor();
	drawBeizer();
	drawmousecoordinate();
	glFlush();
	glutSwapBuffers();
}

void idle(){
	if (u < 1.0)
		u = (u + 0.001);
	else {
		u = 0.0;
		beizer.clear();
	}
	display();
}

//键盘控制颜色
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
	{
		a = 1.0;
		b = 0.0;
		c = 0.0;
		break;
	}
	case 'g':
	{
		a = 0.0;
		b = 1.0;
		c = 0.0;
		break;
	}
	case 'b':
	{
		a = 0.0; 
		b = 0.0;
		c = 1.0;
		break;
	}
	default:
		break;
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("bezier");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
	glutMainLoop();
	return 0;
}
