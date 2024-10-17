#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <cstdio>
#include "irrkLang/include/irrKlang.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "irrKlang.lib")

#define VK_W 0x57
#define VK_S 0x53

using namespace std;
using namespace irrklang;

void draw();
void update(int value);
void enable2D(int width, int height);
void drawText(float x, float y, std::string text);
string int2str(int x);
void drawRect(float x, float y, float width, float height);
void keyboard();
void updateBall();
void vec2_norm(float& x, float& y);

ISoundEngine* SoundEngine = createIrrKlangDevice();

// largeur
int width = 500;
// hauteur
int height = 200;
// 60 fps
int interval = 1000 / 60;

// score gauche et score droite
int score_left = 0;
int score_right = 0;

int racket_width = 10;
int racket_height = 40;
int racket_speed = 3;

// raquette gauche
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

// raqauette droite
float racket_right_x = (float)width - racket_width - 10;
float racket_right_y = 50;

// balle
float ball_pos_x = (float)width / 2;
float ball_pos_y = (float)height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 4;

int main(int argc, char** argv) {

	SoundEngine->play2D("Armin van Buuren - Ping Pong.mp3", true);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Pong");

	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);

	enable2D(width, height);
	glColor3f(1.0f, 1.0f, 1.0f);

	glutMainLoop();

	return 0;
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	drawRect(racket_left_x, racket_left_y, (float)racket_width, (float)racket_height);
	drawRect(racket_right_x, racket_right_y, (float)racket_width, (float)racket_height);

	drawText((float)(width / 2 - 10), (float)(height - 15), int2str(score_left) + ":" + int2str(score_right));

	drawRect((float)ball_pos_x, (float)ball_pos_y, (float)ball_size, (float)ball_size);

	glutSwapBuffers();
}

void update(int value) {
	keyboard();

	updateBall();

	glutTimerFunc(interval, update, 0);

	glutPostRedisplay();
}

void enable2D(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawText(float x, float y, std::string text) {
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

/*
Conversion d'un entier en chaine de caractères
*/
string int2str(int x) {
	stringstream ss;
	ss << x;
	return ss.str();
}

void drawRect(float x, float y, float width, float height) {
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void keyboard() {
	if (GetAsyncKeyState(VK_W)) racket_left_y += racket_speed;
	if (GetAsyncKeyState(VK_S)) racket_left_y -= racket_speed;

	if (GetAsyncKeyState(VK_UP)) racket_right_y += racket_speed;
	if (GetAsyncKeyState(VK_DOWN)) racket_right_y -= racket_speed;
}

void updateBall() {
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;

	if (ball_pos_x < racket_left_x + racket_width &&
		ball_pos_x > racket_left_x &&
		ball_pos_y < racket_left_y + racket_height &&
		ball_pos_y > racket_left_y) {

		float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
		ball_dir_x = fabs(ball_dir_x);
		ball_dir_y = t;
	}

	if (ball_pos_x > racket_right_x &&
		ball_pos_x < racket_right_x + racket_width &&
		ball_pos_y < racket_right_y + racket_height &&
		ball_pos_y > racket_right_y) {

		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x);
		ball_dir_y = t;
	}

	if (ball_pos_x < 0) {
		++score_right;
		ball_pos_x = (float)width / 2;
		ball_pos_y = (float)height / 2;
		ball_dir_x = fabs(ball_dir_x);
		ball_dir_y = 0;
	}

	if (ball_pos_x > width) {
		++score_left;
		ball_pos_x = (float)width / 2;
		ball_pos_y = (float)height / 2;
		ball_dir_x = -fabs(ball_dir_x);
		ball_dir_y = 0;
	}

	if (ball_pos_y > height) {
		ball_dir_y = -fabs(ball_dir_y);
	}

	if (ball_pos_y < 0) {
		ball_dir_y = fabs(ball_dir_y);
	}

	vec2_norm(ball_dir_x, ball_dir_y);
}

void vec2_norm(float& x, float& y) {
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}
