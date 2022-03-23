/*********
CTIS164 - Project
----------
STUDENT : Ahmet Burak Yuksel
SECTION : 3
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         0 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define F1 0
#define F2 1
#define F3 2

#define MAX 20
#define MAXSTART 1
#define MAXEND 1

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

int corXL[MAX], corYL[MAX];
int corXS[MAX], corYS[MAX];
int corXE[MAX], corYE[MAX];
int inextL = 0;
int inextS = 1;
int inextE = 1;
int tempX;
int tempY;
int active = 0;
int mode = F1;

int LastcorXS;
int LastcorYS;
int LastcorXE;
int LastcorYE;

struct Point {
	int x, y;
};

struct line {
	Point p1, p2;
};

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//Circle for F1 
void drawF1() {
	glColor3f(1, 1, -0.1);
	circle_wire(-463, -285, 13);
}

//Circle for F2 
void drawF2() {
	glColor3f(1, 1, 0);
	circle_wire(-246, -285, 13);
}

//Circle for F3 
void drawF3() {
	glColor3f(1, 1, 0);
	circle_wire(-46, -285, 13);
}

//Start Circle
void drawStart(int x, int y) {
	//Circle
	glColor3f(0, 0, 0);
	circle(x, y, 30);
	glColor3f(1, 1, 0);
	circle(x, y, 28);

	//S Letter
	glColor3f(0, 0, 0);
	vprint(x - 12, y - 8, GLUT_BITMAP_TIMES_ROMAN_24, " S ");
}

//End Circle
void drawEnd(int x, int y) {
	//Circle
	glColor3f(0, 0, 0);
	circle(x, y, 30);
	glColor3f(1, 1, 0);
	circle(x, y, 28);

	//E Letter
	glColor3f(0, 0, 0);
	vprint(x - 14, y - 8, GLUT_BITMAP_TIMES_ROMAN_24, " E ");
}

//Small Circles
void drawLine(int x, int y) {
	//Circle
	glColor3f(0, 0, 0);
	circle(x, y, 5);

	//glColor3f(0, 0, 0);	
	//vprint(x, y, GLUT_BITMAP_8_BY_13, " # %d ", num);
}

//Line between Start and End
void showLine(float x1, float y1, float x2, float y2) {
	glColor3f(0, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1);

	glColor3f(1, 1, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1);
}

//Green line between small circles if line is not intersecting
void showLineGreen(float x1, float y1, float x2, float y2) {
	glColor3f(0, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1);

	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1);
}

//Red line between small circles if line is intersecting
void showLineRed(float x1, float y1, float x2, float y2) {
	glColor3f(0, 0, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1);

	glColor3f(1, 0, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glLineWidth(1);
}

//Checking if p is on line
bool checkonline(line l1, Point p) {
	if (p.x <= max(l1.p1.x, l1.p2.x) && p.x <= min(l1.p1.x, l1.p2.x) && (p.y <= max(l1.p1.y, l1.p2.y) && p.y <= min(l1.p1.y, l1.p2.y)))
		return true;

	return false;
}

int direction(Point a, Point b, Point c) {
	int num = (b.y - a.y)*(c.x - b.x) - (b.x - a.x)*(c.y - b.y);
	if (num == 0)
		return 0;   
	else if (num < 0)
		return 2;   
	return 1;    
}

//Checking if two lines are intersecting or not
bool Intersection(line line1, line line2) {
	int direction1 = direction(line1.p1, line1.p2, line2.p1);
	int direction2 = direction(line1.p1, line1.p2, line2.p2);
	int direction3 = direction(line2.p1, line2.p2, line1.p1);
	int direction4 = direction(line2.p1, line2.p2, line1.p2);

	if (direction1 != direction2 && direction3 != direction4)
		return true;
	if (direction1 == 0 && checkonline(line1, line2.p1))
		return true;
	if (direction2 == 0 && checkonline(line1, line2.p2))
		return true;
	if (direction3 == 0 && checkonline(line2, line1.p1))
		return true;
	if (direction4 == 0 && checkonline(line2, line1.p2))
		return true;
	return false;
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Black Background lines
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_LINES);
	for (int i = -500; i <= 500; i += 100) {
		glVertex2f(i, -300);
		glVertex2f(i, 300);
	}
	for (int c = -300; c <= 300; c += 100) {
		glVertex2f(-500, c);
		glVertex2f(500, c);
	}
	glEnd();

	//Dark Blue Background lines
	glColor3f(0.0f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	for (int i = -450; i <= 450; i += 100) {
		glVertex2f(i, -300);
		glVertex2f(i, 300);
	}
	for (int c = -250; c <= 250; c += 100) {
		glVertex2f(-500, c);
		glVertex2f(500, c);
	}
	glEnd();

	//Background for informations
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, 300, 500, 270);
	glColor3f(0.1f, 0.1f, 0.1f);
	glRectf(-500, -300, 500, -270);
	glEnd();

	//Showing current mode
	if (mode == F1)
		drawF1();
	if (mode == F2)
		drawF2();
	if (mode == F3)
		drawF3();

	for (int i = 1; i <= MAXSTART; i++) {
		showLine(corXS[i], corYS[i], corXE[i], corYE[i]);
		if (mode == F1) {
			drawStart(corXS[i], corYS[i]);
			LastcorXS = corXS[i];
			LastcorYS = corYS[i];
			drawEnd(corXE[i], corYE[i]);

			for (int i = 0; i < MAX; i++)
				drawLine(corXL[i], corYL[i]);
			line l1 = { {LastcorXS,LastcorYS}, {LastcorXE, LastcorYE} };
			line l2 = { {corXL[0], corYL[0]}, {corXL[1], corYL[1]} };
			line l3 = { {corXL[2], corYL[2]}, {corXL[3], corYL[3]} };
			line l4 = { {corXL[4], corYL[4]}, {corXL[5], corYL[5]} };
			line l5 = { {corXL[6], corYL[6]}, {corXL[7], corYL[7]} };
			line l6 = { {corXL[8], corYL[8]}, {corXL[9], corYL[9]} };
			line l7 = { {corXL[10], corYL[10]}, {corXL[11], corYL[11]} };
			line l8 = { {corXL[12], corYL[12]}, {corXL[13], corYL[13]} };
			line l9 = { {corXL[14], corYL[14]}, {corXL[15], corYL[15]} };
			line l10 = { {corXL[16], corYL[16]}, {corXL[17], corYL[17]} };
			line l11 = { {corXL[18], corYL[18]}, {corXL[19], corYL[19]} };

			if (Intersection(l1, l2))
				showLineRed(corXL[0], corYL[0], corXL[1], corYL[1]);
			else
				showLineGreen(corXL[0], corYL[0], corXL[1], corYL[1]);

			if (Intersection(l1, l3))
				showLineRed(corXL[2], corYL[2], corXL[3], corYL[3]);
			else
				showLineGreen(corXL[2], corYL[2], corXL[3], corYL[3]);

			if (Intersection(l1, l4))
				showLineRed(corXL[4], corYL[4], corXL[5], corYL[5]);
			else
				showLineGreen(corXL[4], corYL[4], corXL[5], corYL[5]);

			if (Intersection(l1, l5))
				showLineRed(corXL[6], corYL[6], corXL[7], corYL[7]);
			else
				showLineGreen(corXL[6], corYL[6], corXL[7], corYL[7]);

			if (Intersection(l1, l6))
				showLineRed(corXL[8], corYL[8], corXL[9], corYL[9]);
			else
				showLineGreen(corXL[8], corYL[8], corXL[9], corYL[9]);

			if (Intersection(l1, l7))
				showLineRed(corXL[10], corYL[10], corXL[11], corYL[11]);
			else
				showLineGreen(corXL[10], corYL[10], corXL[11], corYL[11]);

			if (Intersection(l1, l8))
				showLineRed(corXL[12], corYL[12], corXL[13], corYL[13]);
			else
				showLineGreen(corXL[12], corYL[12], corXL[13], corYL[13]);

			if (Intersection(l1, l9))
				showLineRed(corXL[14], corYL[14], corXL[15], corYL[15]);
			else
				showLineGreen(corXL[14], corYL[14], corXL[15], corYL[15]);

			if (Intersection(l1, l10))
				showLineRed(corXL[16], corYL[16], corXL[17], corYL[17]);
			else
				showLineGreen(corXL[16], corYL[16], corXL[17], corYL[17]);

			if (Intersection(l1, l11))
				showLineRed(corXL[18], corYL[18], corXL[19], corYL[19]);
			else
				showLineGreen(corXL[18], corYL[18], corXL[19], corYL[19]);
		}
	}

	for (int i = 1; i <= MAXEND; i++) {
		if (mode == F2) {
			showLine(corXS[i], corYS[i], corXE[i], corYE[i]);
			drawStart(corXS[i], corYS[i]);
			drawEnd(corXE[i], corYE[i]);
			LastcorXE = corXE[i];
			LastcorYE = corYE[i];

			for (int i = 0; i < MAX; i++)
				drawLine(corXL[i], corYL[i]);
			line l1 = { {LastcorXS,LastcorYS}, {LastcorXE, LastcorYE} };
			line l2 = { {corXL[0], corYL[0]}, {corXL[1], corYL[1]} };
			line l3 = { {corXL[2], corYL[2]}, {corXL[3], corYL[3]} };
			line l4 = { {corXL[4], corYL[4]}, {corXL[5], corYL[5]} };
			line l5 = { {corXL[6], corYL[6]}, {corXL[7], corYL[7]} };
			line l6 = { {corXL[8], corYL[8]}, {corXL[9], corYL[9]} };
			line l7 = { {corXL[10], corYL[10]}, {corXL[11], corYL[11]} };
			line l8 = { {corXL[12], corYL[12]}, {corXL[13], corYL[13]} };
			line l9 = { {corXL[14], corYL[14]}, {corXL[15], corYL[15]} };
			line l10 = { {corXL[16], corYL[16]}, {corXL[17], corYL[17]} };
			line l11 = { {corXL[18], corYL[18]}, {corXL[19], corYL[19]} };

			if (Intersection(l1, l2))
				showLineRed(corXL[0], corYL[0], corXL[1], corYL[1]);
			else
				showLineGreen(corXL[0], corYL[0], corXL[1], corYL[1]);

			if (Intersection(l1, l3))
				showLineRed(corXL[2], corYL[2], corXL[3], corYL[3]);
			else
				showLineGreen(corXL[2], corYL[2], corXL[3], corYL[3]);

			if (Intersection(l1, l4))
				showLineRed(corXL[4], corYL[4], corXL[5], corYL[5]);
			else
				showLineGreen(corXL[4], corYL[4], corXL[5], corYL[5]);

			if (Intersection(l1, l5))
				showLineRed(corXL[6], corYL[6], corXL[7], corYL[7]);
			else
				showLineGreen(corXL[6], corYL[6], corXL[7], corYL[7]);

			if (Intersection(l1, l6))
				showLineRed(corXL[8], corYL[8], corXL[9], corYL[9]);
			else
				showLineGreen(corXL[8], corYL[8], corXL[9], corYL[9]);

			if (Intersection(l1, l7))
				showLineRed(corXL[10], corYL[10], corXL[11], corYL[11]);
			else
				showLineGreen(corXL[10], corYL[10], corXL[11], corYL[11]);

			if (Intersection(l1, l8))
				showLineRed(corXL[12], corYL[12], corXL[13], corYL[13]);
			else
				showLineGreen(corXL[12], corYL[12], corXL[13], corYL[13]);

			if (Intersection(l1, l9))
				showLineRed(corXL[14], corYL[14], corXL[15], corYL[15]);
			else
				showLineGreen(corXL[14], corYL[14], corXL[15], corYL[15]);

			if (Intersection(l1, l10))
				showLineRed(corXL[16], corYL[16], corXL[17], corYL[17]);
			else
				showLineGreen(corXL[16], corYL[16], corXL[17], corYL[17]);

			if (Intersection(l1, l11))
				showLineRed(corXL[18], corYL[18], corXL[19], corYL[19]);
			else
				showLineGreen(corXL[18], corYL[18], corXL[19], corYL[19]);
		}
	}

	for (int i = 0; i < MAX; i++) {
		showLine(LastcorXS, LastcorYS, LastcorXE, LastcorYE);
		drawStart(LastcorXS, LastcorYS);
		drawEnd(LastcorXE, LastcorYE);
		if (mode == F3) {

			line l1 = { {LastcorXS,LastcorYS}, {LastcorXE, LastcorYE} };
			line l2 = { {corXL[0], corYL[0]}, {corXL[1], corYL[1]} };
			line l3 = { {corXL[2], corYL[2]}, {corXL[3], corYL[3]} };
			line l4 = { {corXL[4], corYL[4]}, {corXL[5], corYL[5]} };
			line l5 = { {corXL[6], corYL[6]}, {corXL[7], corYL[7]} };
			line l6 = { {corXL[8], corYL[8]}, {corXL[9], corYL[9]} };
			line l7 = { {corXL[10], corYL[10]}, {corXL[11], corYL[11]} };
			line l8 = { {corXL[12], corYL[12]}, {corXL[13], corYL[13]} };
			line l9 = { {corXL[14], corYL[14]}, {corXL[15], corYL[15]} };
			line l10 = { {corXL[16], corYL[16]}, {corXL[17], corYL[17]} };
			line l11 = { {corXL[18], corYL[18]}, {corXL[19], corYL[19]} };

			if (Intersection(l1, l2))
				showLineRed(corXL[0], corYL[0], corXL[1], corYL[1]);
			else
				showLineGreen(corXL[0], corYL[0], corXL[1], corYL[1]);

			if (Intersection(l1, l3))
				showLineRed(corXL[2], corYL[2], corXL[3], corYL[3]);
			else
				showLineGreen(corXL[2], corYL[2], corXL[3], corYL[3]);

			if (Intersection(l1, l4))
				showLineRed(corXL[4], corYL[4], corXL[5], corYL[5]);
			else
				showLineGreen(corXL[4], corYL[4], corXL[5], corYL[5]);

			if (Intersection(l1, l5))
				showLineRed(corXL[6], corYL[6], corXL[7], corYL[7]);
			else
				showLineGreen(corXL[6], corYL[6], corXL[7], corYL[7]);

			if (Intersection(l1, l6))
				showLineRed(corXL[8], corYL[8], corXL[9], corYL[9]);
			else
				showLineGreen(corXL[8], corYL[8], corXL[9], corYL[9]);

			if (Intersection(l1, l7))
				showLineRed(corXL[10], corYL[10], corXL[11], corYL[11]);
			else
				showLineGreen(corXL[10], corYL[10], corXL[11], corYL[11]);

			if (Intersection(l1, l8))
				showLineRed(corXL[12], corYL[12], corXL[13], corYL[13]);
			else
				showLineGreen(corXL[12], corYL[12], corXL[13], corYL[13]);

			if (Intersection(l1, l9))
				showLineRed(corXL[14], corYL[14], corXL[15], corYL[15]);
			else
				showLineGreen(corXL[14], corYL[14], corXL[15], corYL[15]);

			if (Intersection(l1, l10))
				showLineRed(corXL[16], corYL[16], corXL[17], corYL[17]);
			else
				showLineGreen(corXL[16], corYL[16], corXL[17], corYL[17]);

			if (Intersection(l1, l11))
				showLineRed(corXL[18], corYL[18], corXL[19], corYL[19]);
			else
				showLineGreen(corXL[18], corYL[18], corXL[19], corYL[19]);

			drawLine(corXL[i], corYL[i]);
		}
	}

	//Name and id
	glColor3f(1, 1, 0);
	vprint(-470, 280, GLUT_BITMAP_8_BY_13, "Ahmet Burak Yuksel - 21803003");
	vprint(280, 280, GLUT_BITMAP_8_BY_13, "HW #2 - 2020-2021 Spring");

	//Mode Status
	glColor3f(1, 1, 1);
	vprint(-470, -290, GLUT_BITMAP_8_BY_13, "F1 : Set Start Position    F2 : Set End Position    F3 : Add Lines");

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
		mode = F1;
	if (key == GLUT_KEY_F2)
		mode = F2;
	if (key == GLUT_KEY_F3)
		mode = F3;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	//Taking mouse coordinates to an array to spawn them in display function
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

		if (mode == F1) {
			corXS[inextS] = tempX - winWidth / 2;
			corYS[inextS] = winHeight / 2 - tempY;
			inextL++;
			if (inextL == MAXSTART)
				inextL = 1;
		}


		if (mode == F2) {
			corXE[inextE] = tempX - winWidth / 2;
			corYE[inextE] = winHeight / 2 - tempY;
			if (inextE == MAXEND)
				inextE = 1;
		}

		if (mode == F3) {
			corXL[inextL] = tempX - winWidth / 2;
			corYL[inextL] = winHeight / 2 - tempY;
			inextL++;
			if (inextL == MAX)
				inextL = 0;
		}

	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	tempX = x;
	tempY = y;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Calculation of Intersections - Ahmet Burak Yuksel");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
