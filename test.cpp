/*
Click the left mouse button, it will switch between day and night.
Press q or Q, the program will quit.
Press l or L, the sun will change the direction of rotation
Press s or S, the sun will stop turning.
Press r or R, the sun will resume rotation.
Press p or P, the clouds will stop.
Press o or O, the clouds will resume move.
Press m or M, the lights will stop flashing.
Press n or N, the lights will resume flashing.
Press a or A, the fire will accelerate shaking.
Press b or B, the fire will slow down shaking.
Press c or C, the hands of the snowman will stop swinging.
Press d or D, the hands of the snowman will resume swinging.
Press e or E, the page will switch to the right view.
Press f or F, the page will switch to the main view.
Press up arrow key, the balloons will move up.
Press down arrow key, the balloons will move down.
Press left arrow key, the balloons will move left.
Press right arrow key, the balloons will move right.
*/


#define FREEGLUT_STATIC
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include <iostream>
#include "windows.h"
#pragma comment (linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define MAX_CHAR 128

typedef struct { GLfloat x, y; } point; // define a 2D point
point p0 = { 0,50 }; // set initial co-ordinate values of the point
GLfloat finishx = 300; // declare position of vertical axis
GLdouble pi = 3.1415926536f;

void drawString(const char* str)
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}
	free(wstring);
	glDeleteLists(list, 1);
}
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

struct Point
{
	int x;
	int y;
};

float r1 = 0;
GLfloat step1 = 10; // declare step
int time_interval_Sun = 16; // declare refresh interval in ms

//sun rotating
void OnTimer_Sun(int value) {
	r1 += step1;
	if (r1 >= 360)
		r1 = 0;
	else if (r1 <= 0)
		r1 = 359;
	glutTimerFunc(time_interval_Sun, OnTimer_Sun, 1);
}

//cloud moving
float r2 = 0;
GLfloat step2 = 8;
int time_interval_Cloud = 30;
void OnTimer_Cloud(int value) {
	r2 += step2;
	if (r2 >= 600)
		r2 = -600;
	else if (r2 <= -600)
		r2 = 600;
	glutTimerFunc(time_interval_Cloud, OnTimer_Cloud, 1);
}


//lights flashing
int time_interval_DD = 10;
float stepD = 0.02;
float color_selected[] = { 0.74,0.988,0.788 };
void OnTimer_DD(int value) {
	color_selected[0] -= stepD;
	color_selected[1] -= stepD;
	color_selected[2] -= stepD;
	//0.529,0.8,0.92
	if (color_selected[0] <0) {
		color_selected[0] = 0.529;
	}
	else if (color_selected[1] <0) {
		color_selected[1] = 0.8;
	}
	else if (color_selected[2] < 0) {
		color_selected[2] = 0.92;
	}
	glutTimerFunc(time_interval_DD, OnTimer_DD, 1);
}



float theta = 0;
bool flag = true; // flag=true, the line rotates clockwise
float speed_hand = 0.5;//control swing speed

GLfloat stepx = 0;
GLfloat stepy = 0;
bool direction = true;//direction=true, the fire turn right 
float speed_fire = 0.5;

float leng = 0;
float len = 5;
bool flag1 = true;//flag1=true, the hand move up
float speed_hand1 = 0.2;

GLfloat ct = 0;//transparency
GLfloat ct_balloon = 0;//transparency of balloon

Point heart_pos = { 350, 150 };
Point heart_pos1 = { 850, 150 };

//keyboard interaction
void keyboard_input(unsigned char key, int x, int y) {// keyboard interaction 
	if (key == 'q' || key == 'Q')//退出程序
		exit(0);
	else if (key == 'l' || key == 'L')//太阳改变转动方向
		step1 = -step1;
	else if (key == 's' || key == 'S')//太阳停止转动
		step1 = 0;
	else if (key == 'r' || key == 'R')//太阳恢复转动
		step1 = 10;
	else if (key == 'p' || key == 'P')// 云朵停止移动
		step2 = 0;
	else if (key == 'o' || key == 'O')// 云朵停止移动
		step2 = 8;
	else if (key == 'm' || key == 'M') //彩灯保持现有颜色
		stepD = 0;
	else if (key == 'n' || key == 'N')//彩灯闪烁
		stepD = 0.02;
	else if (key == 'a' || key == 'A')//火焰加快晃动
		speed_fire += 0.15;
	else if (key == 'b' || key == 'B')//火焰减慢晃动
		speed_fire -= 0.15;
	else if (key == 'c' || key == 'C')//雪人手停止摆动
		speed_hand = 0,
		speed_hand1 = 0;
	else if (key == 'd' || key == 'D')//雪人手重新开始摆动
		speed_hand = 0.5,
		speed_hand1 = 0.5;
	else if (key == 'e' || key == 'E')//切换右视角
		ct = 1,
		ct_balloon = 0.8;
	else if (key == 'f' || key == 'F')//回到主视角
		ct = 0,
		ct_balloon = 0;
}

//specialkeys interaction
void specialkeys(int key, int x, int y)//气球上下左右移动
{
	if (key == GLUT_KEY_RIGHT)
		heart_pos.x += 15,
		heart_pos1.x += 15;
	else if (key == GLUT_KEY_LEFT)
		heart_pos.x -= 15,
		heart_pos1.x -= 15;
	else if (key == GLUT_KEY_UP)
		heart_pos.y += 15,
		heart_pos1.y += 15;
	else if (key == GLUT_KEY_DOWN)
		heart_pos.y -= 15,
		heart_pos1.y -= 15;
	//键盘上下左右操作
	glutPostRedisplay();
}

//Flame ignites at night, flame extinguishes at day
float color_fire[] = { 0.847,0.233,0.133 };//red
float color_fire1[] = { 0.847,0.233,0.133 };
float color_bg[] = { 0,0,0 };
float color_bgnight[] = { 0,0,0 };
float color_bgday[] = { 0.69,0.8784,0.9019 };//blue
float colorSunx[] = { 0,0,0 };
float colorSunx1[] = { 0.698, 0.133, 0.133 };
float colorSunx2[] = { 0,0,0 };
float colorSM[] = { 0.941,0.902,0.549 };
float colorSun[] = { 0.698, 0.133, 0.133 };
float colorMoon[] = { 0.941,0.902,0.549 };
float colorWood[] = { 0.384, 0.149, 0.07 };
//mouse interaction
void mouse_input(int button, int state, int x, int y) { //Left mouse button to switch between day and night
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && color_bg[0] == color_bgnight[0] && color_bg[1] == color_bgnight[1] && color_bg[2] == color_bgnight[2])
		color_fire[0] = color_bgday[0],
		color_fire[1] = color_bgday[1],
		color_fire[2] = color_bgday[2],
		color_bg[0] = color_bgday[0],
		color_bg[1] = color_bgday[1],
		color_bg[2] = color_bgday[2],
		colorSunx[0] = colorSunx1[0],
		colorSunx[1] = colorSunx1[1],
		colorSunx[2] = colorSunx1[2],
		colorSM[0] = colorSun[0],
		colorSM[1] = colorSun[1],
		colorSM[2] = colorSun[2],
		color_fire1[0] = colorWood[0],
		color_fire1[1] = colorWood[1],
		color_fire1[2] = colorWood[2];
	else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && color_fire[0] == color_bgday[0] && color_fire[1] == color_bgday[1] && color_fire[2] == color_bgday[2])

		color_fire[0] = 0.847,
		color_fire[1] = 0.233,
		color_fire[2] = 0.133,
		color_bg[0] = 0,
		color_bg[1] = 0,
		color_bg[2] = 0,
		colorSunx[0] = colorSunx2[0],
		colorSunx[1] = colorSunx2[1],
		colorSunx[2] = colorSunx2[2],
		colorSM[0] = colorMoon[0],
		colorSM[1] = colorMoon[1],
		colorSM[2] = colorMoon[2],
		color_fire1[0] = 0.847,
		color_fire1[1] = 0.233,
		color_fire1[2] = 0.133;

}

void BezierCurve(Point p1, Point p2, Point p3, Point p4, bool FillFlag = false)
{
	if (FillFlag)
	{
		//Use polygons to fit to achieve color filling
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
	}
	else
	{
		glBegin(GL_POINTS);
	}

	for (double t = 0.0; t <= 1.0; t += 0.0005) {
		double a1 = pow((1 - t), 3);
		double a2 = 3 * pow((1 - t), 2) * t;
		double a3 = 3 * pow(t, 2) * (1 - t);
		double a4 = pow(t, 3);
		float x = a1 * p1.x + a2 * p2.x + a3 * p3.x + a4 * p4.x;
		float y = a1 * p1.y + a2 * p2.y + a3 * p3.y + a4 * p4.y;

		glVertex2f(x, y);
	}
	glEnd();
}

//draw circle
void circle(int x, int y, int r)
{
	
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * pi * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void DrawBackground()
{
	glColor3f(color_bg[0], color_bg[1], color_bg[2]);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(900, 0);
	glVertex2f(900, 600);
	glVertex2f(0, 600);
	glEnd();
}

void DrawSunAndMoon()//sun rotating
{
	//angle
	glPushMatrix();
	glTranslatef(70, 545, 0);//Move the coordinate origin to (62348) in order to rotate around this point. The coordinate point of the lower triangle should also be set according to this origin
	glRotatef(r1, 0, 0, 1);
	glColor3f(colorSunx[0],colorSunx[1],colorSunx[2]);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();
	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex2f(8, 16);
	glVertex2f(0, 40);
	glVertex2f(-8, 16);
	glEnd();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//circle
	GLint r5 = 25;
	GLint x5 = 70;
	GLint y5 = 545;
	glColor3f(colorSM[0], colorSM[1], colorSM[2]);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 360)
	{
		glVertex2f(r5 * sin(i) + x5, r5 * cos(i) + y5);
	}
	glEnd();


}

void DrawCloud()//cloud moving
{
	glColor3f(1,1,1);
	circle(173 + r2, 516, 13);
	circle(193 + r2, 522, 13);
	circle(186 + r2, 504, 11);
	circle(213 + r2, 518, 11);
	circle(207 + r2, 505, 12);

	glColor3f(1,1,1);
	circle(303 + r2, 550, 13);
	circle(323 + r2, 556, 13);
	circle(316 + r2, 538, 11);
	circle(343 + r2, 552, 11);
	circle(337 + r2, 539, 12);

	glColor3f(1,1,1);
	circle(453 + r2, 520, 13);
	circle(473 + r2, 526, 13);
	circle(466 + r2, 508, 11);
	circle(493 + r2, 522, 11);
	circle(487 + r2, 507, 12);
}

void DrawSnowman() {// snowman shaking hands

	GLdouble pi = 3.1415926536f;
	//body
		//半径1
	GLint r1 = 75;
	GLint x1 = 660+100;
	GLint y1 = 80;
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     
	{
		glVertex2f(r1 * sin(i) + x1, r1 * cos(i) + y1);
	}
	glEnd();
	//head
	GLint r2 = 46;
	GLint x2 = 660+100;
	GLint y2 = 180;
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)    
	{
		glVertex2f(r2 * sin(i) + x2, r2 * cos(i) + y2);
	}
	glEnd();

	//left eye
	GLint r3 = 2;
	GLint x3 = 638+100;
	GLint y3 = 190;
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)
	{
		glVertex2f(r3 * sin(i) + x3+1, r3 * cos(i) + y3);
	}
	glEnd();

	//right eye
	GLint r4 = 2;
	GLint x4 = 682+100;
	GLint y4 = 190;
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)
	{
		glVertex2f(r4 * sin(i) + x4-1, r4 * cos(i) + y4);
	}
	glEnd();

	
	//Point line_pos = { 600, 400 };
	int length = 60;
	float angle = 10;//Linear swing amplitude
	
	//left hand
	glColor3f(0.627, 0.322, 0.176);
	glBegin(GL_LINES);
	glVertex2f(705,120);
	glVertex2f(705- length * sin((theta+60) * pi / 180),120+length * cos((theta+60) * pi / 180));
	glEnd();
	//right hand
	glColor3f(0.627, 0.322, 0.176);
	glBegin(GL_LINES);
	glVertex2f(815, 120);
	glVertex2f(815 + length * sin((theta + 60) * pi / 180), 120 + length * cos((theta + 60) * pi / 180));
	glEnd();
	//Swing clockwise to the maximum extent, and change it to counterclockwise
	if (theta > angle && flag == true)
	{
		flag = false;
	}
	// Swing counterclockwise to the maximum extent, and change it to clockwise
	if (theta < -1 * angle && flag == false) {
		flag = true;
	}
	theta = theta + (flag ? speed_hand : -1 * speed_hand);
	
}

void DrawTree() {//Tree   light flashing
	glBegin(GL_POLYGON);
	glColor3f(0.384,0.149,0.07);
	glVertex2f(180,0);
	glVertex2f(230,0);//Midpoint 205
	glVertex2f(230,130);
	glVertex2f(180,130);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0.012,0.137,0.055);
	glVertex2f(30,120);
	glVertex2f(380,120);
	glVertex2f(290,200);
	glVertex2f(140,200);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(0.012, 0.137, 0.055);
	glVertex2f(60, 200);
	glVertex2f(350, 200);
	glVertex2f(250, 280);
	glVertex2f(160, 280);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.012, 0.137, 0.055);
	glVertex2f(120, 280);
	glVertex2f(290, 280);
	glVertex2f(205, 360);
	glEnd();
	//star
	GLint rs = 12;
	GLint Rs = 22;
	//sin(18)=0.309 cos(18)=0.95 cos(36)=0.809 sin(36)=0.588  sin(54)=0.809 
	//lower right
	glColor3f(1, 0.843, 0);
	glBegin(GL_POLYGON);
	glVertex2f(205, 360);
	glVertex2f(205 + Rs *0.588, 360 - Rs * 0.809 + rs);
	glVertex2f(rs * 0.95 + 205, 360 + rs - rs *0.309);
	glVertex2f(205, 360 + rs);
	glEnd();
	//lower left
	glColor3f(1, 0.843, 0);
	glBegin(GL_POLYGON);
	glVertex2f(205, 360);
	glVertex2f(205 -Rs * 0.588, 360 - Rs * 0.809 + rs);
	glVertex2f(205-rs * 0.95, 360 + rs - rs * 0.309);
	glVertex2f(205, 360 + rs);
	glEnd();
	//left
	glColor3f(1, 0.843, 0);
	glBegin(GL_POLYGON);
	glVertex2f(205 - rs * 0.95, 360 + rs - rs * 0.309);
	glVertex2f(205-rs*0.588-(Rs*0.588)/0.809,360+rs+rs*0.809);
	glVertex2f(205-rs*0.588, 360 + rs + rs * 0.809);
	glVertex2f(205,360+rs);
	glEnd();
	//top
	glColor3f(1, 0.843, 0);
	glBegin(GL_POLYGON);
	glVertex2f(205,360+rs);
	glVertex2f(205-rs*0.588, 360 + rs + rs * 0.809);
	glVertex2f(205,360+rs+Rs);
	glVertex2f(205+rs*0.588, 360 + rs + rs * 0.809);
	glEnd();

	//right
	glColor3f(1, 0.843, 0);
	glBegin(GL_POLYGON);
	glVertex2f(205,360+rs);
	glVertex2f(205 + rs * 0.588, 360 + rs + rs * 0.809);
	glVertex2f(205+rs*0.588+(Rs*0.588)/0.809,360+rs+rs*0.809);
	glVertex2f(rs*0.95+205,360+rs-rs*0.309);
	glEnd();

	//lights
	GLint rD = 6;
	//light1 (100,140)
	glColor3f(color_selected[0], color_selected[1], color_selected[2]);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 100, rD * cos(i) + 140);
	}
	glEnd();
	//light2 (160,200)
	glColor3f(color_selected[0]+0.2, color_selected[1], color_selected[2]+0.1);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 160, rD * cos(i) + 200);
	}
	glEnd();
	//light3 (200,150)
	glColor3f(color_selected[0] -0.2, color_selected[1]-0.3, color_selected[2] + 0.1);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 200, rD * cos(i) + 150);
	}
	glEnd();

	//light4 (230,300)
	glColor3f(color_selected[0]+0.51, color_selected[1] -0.25, color_selected[2] - 0.034);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 230, rD * cos(i) + 300);
	}
	glEnd();

	//light5 (140,240)
	glColor3f(color_selected[0] + 0.31, color_selected[1] - 0.55, color_selected[2] - 0.034);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 140, rD * cos(i) + 240);
	}
	glEnd();
	//light6 (300,170)
	glColor3f(color_selected[0] + 0.3, color_selected[1] - 0.4, color_selected[2] - 0.134);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 260, rD * cos(i) + 200);
	}
	glEnd();

	//light7 (210,250)
	glColor3f(color_selected[0]-0.65, color_selected[1] - 0.27, color_selected[2] +0.28);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 210, rD * cos(i) + 250);
	}
	glEnd();

	//light8 (290,150)
	glColor3f(color_selected[0] - 0.45, color_selected[1] + 0.27, color_selected[2] - 0.47);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 290, rD * cos(i) + 150);
	}
	glEnd();

	//light9 (200,150)
	glColor3f(color_selected[0] - 0.2, color_selected[1] - 0.4, color_selected[2] + 0.2);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 170, rD * cos(i) + 310);
	}
	glEnd();



	


}

void DrawWood() {
	glBegin(GL_POLYGON);
	glColor3f(0.384, 0.149, 0.07);
	glVertex2f(348, 12);
	glVertex2f(355, 0);
	glVertex2f(510, 80);
	glVertex2f(505, 85);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.384, 0.149, 0.07);
	glVertex2f(421, 0);
	glVertex2f(433, -5);
	glVertex2f(503, 90);
	glVertex2f(495, 92);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.384, 0.149, 0.07);
	glVertex2f(496, -5);
	glVertex2f(506, 0);
	glVertex2f(488, 96);
	glVertex2f(480, 95);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.384, 0.149, 0.07);
	glVertex2f(556, -5);
	glVertex2f(570, 0);
	glVertex2f(475, 93);
	glVertex2f(472, 90);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.384, 0.149, 0.07);
	glVertex2f(620, 0);
	glVertex2f(630, 10);
	glVertex2f(468, 85);
	glVertex2f(462, 80);
	glEnd();

}

void DrawFire() {//bonfire  Flame extinguished->night   burn->day
	Point fire_pos = { 490, 21 };
	glColor3f(color_fire[0], color_fire[1], color_fire[2]);
	Point f1 = { fire_pos.x , fire_pos.y };//bottom point
	Point f2 = { fire_pos.x - 75 , fire_pos.y + 5 };
	Point f3 = { fire_pos.x - 85  , fire_pos.y + 90 };
	Point f4 = { fire_pos.x - 90 + stepx / 2, fire_pos.y + 110 };//linking point

	Point f5 = { fire_pos.x - 75 + stepx / 2 , fire_pos.y + 105 };
	Point f6 = { fire_pos.x - 70 + stepx / 2 , fire_pos.y + 96 };
	Point f7 = { fire_pos.x - 50, fire_pos.y + 95 };//linking point

	Point f8 = { fire_pos.x - 35  , fire_pos.y + 135 + stepy };
	Point f9 = { fire_pos.x - 20 + stepx  , fire_pos.y + 160 + stepy };
	Point f10 = { fire_pos.x + stepx  , fire_pos.y + 185 + stepy };//top point

	Point f11 = { fire_pos.x + 20 + stepx,fire_pos.y + 160 + stepy };
	Point f12 = { fire_pos.x + 35,fire_pos.y + 135 + stepy };
	Point f13 = { fire_pos.x + 50,fire_pos.y + 95 };//linking point

	Point f14 = { fire_pos.x + 70 + stepx / 2,fire_pos.y + 96 };
	Point f15 = { fire_pos.x + 75 + stepx / 2,fire_pos.y + 105 };
	Point f16 = { fire_pos.x + 90 + stepx / 2,fire_pos.y + 110 };//linking point

	Point f17 = { fire_pos.x + 85 ,fire_pos.y + 90 };
	Point f18 = { fire_pos.x + 75 ,fire_pos.y + 5 };
	BezierCurve(f1, f2, f3, f4, true);
	BezierCurve(f4, f5, f6, f7, true);
	BezierCurve(f7, f8, f9, f10, true);
	BezierCurve(f10, f11, f12, f13, true);
	BezierCurve(f13, f14, f15, f16, true);
	BezierCurve(f16, f17, f18, f1, true);

	glColor3f(color_fire[0], color_fire[1], color_fire[2]);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x - 90 + stepx / 2-1, fire_pos.y + 110);
	glVertex2f(fire_pos.x - 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();

	glColor3f(color_fire[0], color_fire[1], color_fire[2]);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x + stepx, fire_pos.y + 185 + stepy+1.5);
	glVertex2f(fire_pos.x - 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();

	glColor3f(color_fire[0], color_fire[1], color_fire[2]);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x + stepx, fire_pos.y + 185 + stepy+1.5);
	glVertex2f(fire_pos.x + 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();

	glColor3f(color_fire[0], color_fire[1], color_fire[2]);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x + 90 + stepx / 2+1, fire_pos.y + 110);
	glVertex2f(fire_pos.x + 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();

	//Swing counterclockwise to the maximum extent, and change it to clockwise
	if (stepx > 20 && direction == true)
	{
		direction = false;
	}
	//
	if (stepx < -20 && direction == false) {
		direction = true;
	}
	stepx = stepx + (direction ? speed_fire : -1 * speed_fire);

	
}

void DrawWoodsbj() {
	glBegin(GL_POLYGON);
	glColor3f(color_fire1[0], color_fire1[1], color_fire1[2]);
	glVertex2f(430.5,49.8);
	glVertex2f(438.4,42.2);
	glVertex2f(510, 80);
	glVertex2f(505, 85);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(color_fire1[0], color_fire1[1], color_fire1[2]);
	glVertex2f(448.7,34.3);
	glVertex2f(457.9,28.6);
	glVertex2f(503, 90);
	glVertex2f(495, 92);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(color_fire1[0], color_fire1[1], color_fire1[2]);
	glVertex2f(491.8,21);
	glVertex2f(501.9,22);
	glVertex2f(488, 96);
	glVertex2f(480, 95);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(color_fire1[0], color_fire1[1], color_fire1[2]);
	glVertex2f(524.8,30.1);
	glVertex2f(533.3,35.8);
	glVertex2f(475, 93);
	glVertex2f(472, 90);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(color_fire1[0], color_fire1[1], color_fire1[2]);
	glVertex2f(539.3,40.3);
	glVertex2f(546.3,47.7);
	glVertex2f(468, 85);
	glVertex2f(462, 80);
	glEnd();


}

void DrawWord1() {
	glColor3f(0.753,0.1568,0.106);
	selectFont(75, ANSI_CHARSET, "Calibri");
	glRasterPos2f(380,350);
	drawString("HHappy New Year!");
	glEnd();
}

void DrawSmile() {//smile of snowman use beziercurve
	Point face_pos = { 760, 183 };
	glColor3f(0.0, 0.0, 0.0);
	Point p1 = { face_pos.x - 18, face_pos.y - 15 };
	Point p2 = { face_pos.x - 10, face_pos.y - 28 };
	Point p3 = { face_pos.x + 10, face_pos.y - 28 };
	Point p4 = { face_pos.x + 18, face_pos.y - 15 };
	BezierCurve(p1, p2, p3, p4);
}

void Drawballoon() {
	//Elliptical balloon
	glEnable(GL_BLEND);//set transparency
	glColor4f(0.4,0.59,1,0.8);
	//linking point (heart_pos.x-260,heart_pos.y+150)
	GLint Ba = 30;
	GLint Bb = 40;
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 360)
	{
		glVertex2f(Ba * cos(i) + heart_pos.x - 260, Bb * sin(i) + heart_pos.y + 170+Bb);
	}
	glEnd();
	glDisable(GL_BLEND);

	//Draw a rope
	glEnable(GL_BLEND);
	glColor4f(0.4, 0.59, 1, 0.2);
	Point n1 = { heart_pos.x - 260, heart_pos.y + 170 };//linking point

	Point n2 = { heart_pos.x + 30 - 260, heart_pos.y + 110 };
	Point n3 = { heart_pos.x +10 - 260, heart_pos.y + 80 };

	Point n4 = { heart_pos.x - 260, heart_pos.y+60 };

	Point n5 = { heart_pos.x -10 - 260, heart_pos.y +40 };
	Point n6 = { heart_pos.x -30 - 260, heart_pos.y +10 };

	Point n7 = { heart_pos.x - 260, heart_pos.y - 30};

	Point n8 = {heart_pos.x-260+30,heart_pos.y-80};
	Point n9 = {heart_pos.x-260+10,heart_pos.y-110};

	Point n10 = {heart_pos.x-260,heart_pos.y-150};

	BezierCurve(n1, n2, n3, n4);
	BezierCurve(n4, n5, n6, n7);
	BezierCurve(n7, n8, n9, n10);
	glDisable(GL_BLEND);

	//heart balloon
	glEnable(GL_BLEND);
	glColor4f(0.98,0.4745,0.645,0.8);
	Point h1 = { heart_pos.x - 300, heart_pos.y + 200 };//linking point
	Point h2 = { heart_pos.x - 35 - 300, heart_pos.y + 30 + 200 };
	Point h3 = { heart_pos.x - 50 - 300, heart_pos.y - 20 + 200 };
	Point h4 = { heart_pos.x - 300, heart_pos.y - 50 + 200 };//linking point
	Point h5 = { heart_pos.x + 35 - 300, heart_pos.y + 30 + 200 };
	Point h6 = { heart_pos.x + 50 - 300, heart_pos.y - 20 + 200 };
	BezierCurve(h1, h2, h3, h4, true);
	BezierCurve(h1, h5, h6, h4, true);
	glDisable(GL_BLEND);
	// draw a rope
	glEnable(GL_BLEND);
	glColor4f(0.98, 0.4745, 0.645, 0.1);
	Point s1 = { heart_pos.x - 300, heart_pos.y + 150 };//linking point
	Point s2 = { heart_pos.x - 30 - 300, heart_pos.y + 100 };
	Point s3 = { heart_pos.x - 10 - 300, heart_pos.y + 30 };
	Point s4 = { heart_pos.x - 300, heart_pos.y };
	Point s5 = { heart_pos.x + 10 - 300, heart_pos.y - 30 };
	Point s6 = { heart_pos.x + 30 - 300, heart_pos.y - 100 };
	Point s7 = { heart_pos.x - 300, heart_pos.y - 150 };
	BezierCurve(s1, s2, s3, s4);
	BezierCurve(s4, s5, s6, s7);
	glDisable(GL_BLEND);

}

void  Drawfirework1() {
	//glColor3f(0.49, 1, 0.83);//green
	//glColor3f(1, 0.75, 0.796);//pink
	//glColor3f(1, 0.843, 0);//glod yellow 
	GLint L1 = 50;
	glPushMatrix();
	glTranslatef(780, 405, 0);
	glColor3f(1, 0.75, 0.796); 
	glBegin(GL_LINES);
	glVertex2f(0,0);
	glVertex2f(0,L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L1);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();





}
void  Drawfirework2() {
	GLint L2 = 44;
	glPushMatrix();
	glColor3f(1, 0.843, 0);
	glTranslatef(780, 405, 0);
	glRotatef(15, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L2);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();




}
void  Drawfirework3() {
	GLint L3 = 38;
	glPushMatrix();
	glColor3f(1, 0.843, 0);
	glTranslatef(370, 350, 0);
	glRotatef(15, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L3);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
void  Drawfirework4() {
	GLint L4 = 32;
	glPushMatrix();
	glColor3f(0.49, 1, 0.83);
	glTranslatef(370, 350, 0);
	glRotatef(15, 0, 0, 1);
	glRotatef(15, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, L4);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

//the new background of new view way
void DrawBackground1()
{
	glEnable(GL_BLEND);
	glColor4f(color_bg[0], color_bg[1], color_bg[2],ct);
	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	glVertex2f(900, 0);
	glVertex2f(900, 600);
	glVertex2f(0, 600);
	glEnd();
	glDisable(GL_BLEND);
}

void Aballoon() {
	//Elliptical balloon
	glEnable(GL_BLEND);
	glColor4f(0.4, 0.59, 1, ct_balloon);
	//linking point (heart_pos.x-260,heart_pos.y+150)
	GLint Ba = 30;
	GLint Bb = 40;
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 360)
	{
		glVertex2f(Ba * cos(i) + heart_pos1.x - 260, Bb * sin(i) + heart_pos1.y + 170 + Bb);
	}
	glEnd();
	glDisable(GL_BLEND);

	//draw a rope
	glEnable(GL_BLEND);
	glColor4f(0.4, 0.59, 1, ct_balloon);
	Point n1 = { heart_pos1.x - 260, heart_pos1.y + 170 };//linking point

	Point n2 = { heart_pos1.x + 30 - 260, heart_pos1.y + 110 };
	Point n3 = { heart_pos1.x + 10 - 260, heart_pos1.y + 80 };

	Point n4 = { heart_pos1.x - 260, heart_pos1.y + 60 };

	Point n5 = { heart_pos1.x - 10 - 260, heart_pos1.y + 40 };
	Point n6 = { heart_pos1.x - 30 - 260, heart_pos1.y + 10 };

	Point n7 = { heart_pos1.x - 260, heart_pos1.y - 30 };

	Point n8 = { heart_pos1.x - 260 + 30,heart_pos1.y - 80 };
	Point n9 = { heart_pos1.x - 260 + 10,heart_pos1.y - 110 };

	Point n10 = { heart_pos1.x - 260,heart_pos1.y - 150 };

	BezierCurve(n1, n2, n3, n4);
	BezierCurve(n4, n5, n6, n7);
	BezierCurve(n7, n8, n9, n10);
	glDisable(GL_BLEND);

	//heart balloon
	glEnable(GL_BLEND);
	glColor4f(0.98, 0.4745, 0.645, ct_balloon);
	Point h1 = { heart_pos1.x - 300, heart_pos1.y + 200 };//linking point
	Point h2 = { heart_pos1.x - 35 - 300, heart_pos1.y + 30 + 200 };
	Point h3 = { heart_pos1.x - 50 - 300, heart_pos1.y - 20 + 200 };
	Point h4 = { heart_pos1.x - 300, heart_pos1.y - 50 + 200 };//linking point
	Point h5 = { heart_pos1.x + 35 - 300, heart_pos1.y + 30 + 200 };
	Point h6 = { heart_pos1.x + 50 - 300, heart_pos1.y - 20 + 200 };
	BezierCurve(h1, h2, h3, h4, true);
	BezierCurve(h1, h5, h6, h4, true);
	glDisable(GL_BLEND);
	// draw a rope
	glEnable(GL_BLEND);
	glColor4f(0.98, 0.4745, 0.645, ct_balloon);
	Point s1 = { heart_pos1.x - 300, heart_pos1.y + 150 };//linking point
	Point s2 = { heart_pos1.x - 30 - 300, heart_pos1.y + 100 };
	Point s3 = { heart_pos1.x - 10 - 300, heart_pos1.y + 30 };
	Point s4 = { heart_pos1.x - 300, heart_pos1.y };
	Point s5 = { heart_pos1.x + 10 - 300, heart_pos1.y - 30 };
	Point s6 = { heart_pos1.x + 30 - 300, heart_pos1.y - 100 };
	Point s7 = { heart_pos1.x - 300, heart_pos1.y - 150 };
	BezierCurve(s1, s2, s3, s4);
	BezierCurve(s4, s5, s6, s7);
	glDisable(GL_BLEND);

}
void Acloud() {
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1,ct);
	circle(173 + r2, 516, 13);
	circle(193 + r2, 522, 13);
	circle(186 + r2, 504, 11);
	circle(213 + r2, 518, 11);
	circle(207 + r2, 505, 12);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1,ct);
	circle(303 + r2, 550, 13);
	circle(323 + r2, 556, 13);
	circle(316 + r2, 538, 11);
	circle(343 + r2, 552, 11);
	circle(337 + r2, 539, 12);
	glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1,ct);
	circle(453 + r2, 520, 13);
	circle(473 + r2, 526, 13);
	circle(466 + r2, 508, 11);
	circle(493 + r2, 522, 11);
	circle(487 + r2, 507, 12);
	glDisable(GL_BLEND);

}
void Atree() {
	//tree
	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.384, 0.149, 0.07, ct);
	glVertex2f(180 + 245, 0);
	glVertex2f(230 + 245, 0);//middle point 205   450 245
	glVertex2f(230 + 245, 130);
	glVertex2f(180 + 245, 130);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.012, 0.137, 0.055, ct);
	glVertex2f(30 + 245, 120);
	glVertex2f(380 + 245, 120);
	glVertex2f(290 + 245, 200);
	glVertex2f(140 + 245, 200);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.012, 0.137, 0.055, ct);
	glVertex2f(60 + 245, 200);
	glVertex2f(350 + 245, 200);
	glVertex2f(250 + 245, 280);
	glVertex2f(160 + 245, 280);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBegin(GL_TRIANGLES);
	glColor4f(0.012, 0.137, 0.055, ct);
	glVertex2f(120 + 245, 280);
	glVertex2f(290 + 245, 280);
	glVertex2f(205 + 245, 360);
	glEnd();
	glDisable(GL_BLEND);
	//star
	GLint rs = 12;
	GLint Rs = 22;
	//sin(18)=0.309 cos(18)=0.95 cos(36)=0.809 sin(36)=0.588  sin(54)=0.809 
	//lower right
	glEnable(GL_BLEND);
	glColor4f(1, 0.843, 0, ct);
	glBegin(GL_POLYGON);
	glVertex2f(205 + 245, 360);
	glVertex2f(205 + Rs * 0.588 + 245, 360 - Rs * 0.809 + rs);
	glVertex2f(rs * 0.95 + 205 + 245, 360 + rs - rs * 0.309);
	glVertex2f(205 + 245, 360 + rs);
	glEnd();
	glDisable(GL_BLEND);

	//lower left
	glEnable(GL_BLEND);
	glColor4f(1, 0.843, 0, ct);
	glBegin(GL_POLYGON);
	glVertex2f(205 + 245, 360);
	glVertex2f(205 - Rs * 0.588 + 245, 360 - Rs * 0.809 + rs);
	glVertex2f(205 - rs * 0.95 + 245, 360 + rs - rs * 0.309);
	glVertex2f(205 + 245, 360 + rs);
	glEnd();
	glDisable(GL_BLEND);
	//left
	glEnable(GL_BLEND);
	glColor4f(1, 0.843, 0, ct);
	glBegin(GL_POLYGON);
	glVertex2f(205 - rs * 0.95 + 245, 360 + rs - rs * 0.309);
	glVertex2f(205 - rs * 0.588 - (Rs * 0.588) / 0.809 + 245, 360 + rs + rs * 0.809);
	glVertex2f(205 - rs * 0.588 + 245, 360 + rs + rs * 0.809);
	glVertex2f(205 + 245, 360 + rs);
	glEnd();
	glDisable(GL_BLEND);
	//top
	glEnable(GL_BLEND);
	glColor4f(1, 0.843, 0, ct);
	glBegin(GL_POLYGON);
	glVertex2f(205 + 245, 360 + rs);
	glVertex2f(205 - rs * 0.588 + 245, 360 + rs + rs * 0.809);
	glVertex2f(205 + 245, 360 + rs + Rs);
	glVertex2f(205 + rs * 0.588 + 245, 360 + rs + rs * 0.809);
	glEnd();
	glDisable(GL_BLEND);
	//right
	glEnable(GL_BLEND);
	glColor4f(1, 0.843, 0, ct);
	glBegin(GL_POLYGON);
	glVertex2f(205 + 245, 360 + rs);
	glVertex2f(205 + rs * 0.588 + 245, 360 + rs + rs * 0.809);
	glVertex2f(205 + rs * 0.588 + (Rs * 0.588) / 0.809 + 245, 360 + rs + rs * 0.809);
	glVertex2f(rs * 0.95 + 205 + 245, 360 + rs - rs * 0.309);
	glEnd();
	glDisable(GL_BLEND);

	//lights
	GLint rD = 6;
	//light1 (100,140)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0], color_selected[1], color_selected[2], ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 100 + 245, rD * cos(i) + 140);
	}
	glEnd();
	glDisable(GL_BLEND);
	//light2 (160,200)

	glEnable(GL_BLEND);
	glColor4f(color_selected[0] + 0.2, color_selected[1], color_selected[2] + 0.1, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 160 + 245, rD * cos(i) + 200);
	}
	glEnd();
	glDisable(GL_BLEND);

	//light3 (200,150)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] - 0.2, color_selected[1] - 0.3, color_selected[2] + 0.1, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 200 + 245, rD * cos(i) + 150);
	}
	glEnd();
	glDisable(GL_BLEND);
	//light4 (230,300)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] + 0.51, color_selected[1] - 0.25, color_selected[2] - 0.034, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 230 + 245, rD * cos(i) + 300);
	}
	glEnd();
	glDisable(GL_BLEND);

	//light5 (140,240)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] + 0.31, color_selected[1] - 0.55, color_selected[2] - 0.034, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 140 + 245, rD * cos(i) + 240);
	}
	glEnd();
	glDisable(GL_BLEND);
	//light6 (300,170)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] + 0.3, color_selected[1] - 0.4, color_selected[2] - 0.134, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 260 + 245, rD * cos(i) + 200);
	}
	glEnd();
	glDisable(GL_BLEND);

	//light7 (210,250)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] - 0.65, color_selected[1] - 0.27, color_selected[2] + 0.28, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 210 + 245, rD * cos(i) + 250);
	}
	glEnd();
	glDisable(GL_BLEND);

	//light8 (290,150)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] - 0.45, color_selected[1] + 0.27, color_selected[2] - 0.47, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 290 + 245, rD * cos(i) + 150);
	}
	glEnd();
	glDisable(GL_BLEND);

	//light9 (200,150)
	glEnable(GL_BLEND);
	glColor4f(color_selected[0] - 0.2, color_selected[1] - 0.4, color_selected[2] + 0.2, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     //通过数学计算来画多边形的点
	{
		glVertex2f(rD * sin(i) + 170 + 245, rD * cos(i) + 310);
	}
	glEnd();
	glDisable(GL_BLEND);

}

void Awood() {
	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.384, 0.149, 0.07,ct);
	glVertex2f(348-40, 12);
	glVertex2f(355-40, 0);
	glVertex2f(510-40, 80);
	glVertex2f(505-40, 85);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.384, 0.149, 0.07,ct);
	glVertex2f(421-40, 0);
	glVertex2f(433-40, -5);
	glVertex2f(503-40, 90);
	glVertex2f(495-40, 92);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.384, 0.149, 0.07,ct);
	glVertex2f(496-40, -5);
	glVertex2f(506-40, 0);
	glVertex2f(488-40, 96);
	glVertex2f(480-40, 95);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.384, 0.149, 0.07,ct);
	glVertex2f(556-40, -5);
	glVertex2f(570-40, 0);
	glVertex2f(475-40, 93);
	glVertex2f(472-40, 90);
	glEnd();
	glDisable(GL_BLEND);
	
	glEnable(GL_BLEND);
	glBegin(GL_POLYGON);
	glColor4f(0.384, 0.149, 0.07,ct);
	glVertex2f(620-40, 0);
	glVertex2f(630-40, 10);
	glVertex2f(468-40, 85);
	glVertex2f(462-40, 80);
	glEnd();
	glDisable(GL_BLEND);
}

void Afire() {
	//fire
	glEnable(GL_BLEND);
	Point fire_pos = { 490-40, 21-11 };
	glColor4f(color_fire[0], color_fire[1], color_fire[2], ct);
	Point f1 = { fire_pos.x , fire_pos.y };//bottom point
	Point f2 = { fire_pos.x - 75 , fire_pos.y + 5 };
	Point f3 = { fire_pos.x - 85  , fire_pos.y + 90 };
	Point f4 = { fire_pos.x - 90 + stepx / 2, fire_pos.y + 110 };//linking point

	Point f5 = { fire_pos.x - 75 + stepx / 2 , fire_pos.y + 105 };
	Point f6 = { fire_pos.x - 70 + stepx / 2 , fire_pos.y + 96 };
	Point f7 = { fire_pos.x - 50, fire_pos.y + 95 };//linking point

	Point f8 = { fire_pos.x - 35  , fire_pos.y + 135 + stepy };
	Point f9 = { fire_pos.x - 20 + stepx  , fire_pos.y + 160 + stepy };
	Point f10 = { fire_pos.x + stepx  , fire_pos.y + 185 + stepy };//top point

	Point f11 = { fire_pos.x + 20 + stepx,fire_pos.y + 160 + stepy };
	Point f12 = { fire_pos.x + 35,fire_pos.y + 135 + stepy };
	Point f13 = { fire_pos.x + 50,fire_pos.y + 95 };//linking point

	Point f14 = { fire_pos.x + 70 + stepx / 2,fire_pos.y + 96 };
	Point f15 = { fire_pos.x + 75 + stepx / 2,fire_pos.y + 105 };
	Point f16 = { fire_pos.x + 90 + stepx / 2,fire_pos.y + 110 };//linking point

	Point f17 = { fire_pos.x + 85 ,fire_pos.y + 90 };
	Point f18 = { fire_pos.x + 75 ,fire_pos.y + 5 };
	BezierCurve(f1, f2, f3, f4, true);
	BezierCurve(f4, f5, f6, f7, true);
	BezierCurve(f7, f8, f9, f10, true);
	BezierCurve(f10, f11, f12, f13, true);
	BezierCurve(f13, f14, f15, f16, true);
	BezierCurve(f16, f17, f18, f1, true);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glColor4f(color_fire[0], color_fire[1], color_fire[2], ct);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x - 90 + stepx / 2-1, fire_pos.y + 110);
	glVertex2f(fire_pos.x - 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();
	glDisable(GL_BLEND);


	glEnable(GL_BLEND);
	glColor4f(color_fire[0], color_fire[1], color_fire[2], ct);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x + stepx, fire_pos.y + 185 + stepy);
	glVertex2f(fire_pos.x - 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glColor4f(color_fire[0], color_fire[1], color_fire[2], ct);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x + stepx, fire_pos.y + 185 + stepy);
	glVertex2f(fire_pos.x + 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glColor4f(color_fire[0], color_fire[1], color_fire[2], ct);
	glBegin(GL_TRIANGLES);
	glVertex2f(fire_pos.x + 90 + stepx / 2+1, fire_pos.y + 110);
	glVertex2f(fire_pos.x + 50, fire_pos.y + 95);
	glVertex2f(fire_pos.x, fire_pos.y);
	glEnd();
	glDisable(GL_BLEND);


	if (stepx > 10 && direction == true)
	{
		direction = false;
	}
	// Swing counterclockwise to the maximum extent, and change it to clockwise
	if (stepx < -10 && direction == false) {
		direction = true;
	}
	stepx = stepx + (direction ? (speed_fire-0.3) : -1 * (speed_fire-0.3));

}

void Asnowman() {
//snowman
	GLdouble pi = 3.1415926536f;
	//body
	GLint r1 = 75;
	GLint x1 = 450;
	GLint y1 = 80;
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     
	{
		glVertex2f(r1 * sin(i) + x1, r1 * cos(i) + y1);
	}
	glEnd();
	glDisable(GL_BLEND);
	//head
	GLint r2 = 46;
	GLint x2 = 450;
	GLint y2 = 180;
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)     
	{
		glVertex2f(r2 * sin(i) + x2, r2 * cos(i) + y2);
	}
	glEnd();
	glDisable(GL_BLEND);

	//eye
	glEnable(GL_BLEND);
	GLint re = 2;
	GLint xe =420;
	GLint ye = 190;
	glColor4f(0, 0, 0,ct);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * pi; i = i + pi / 180)
	{
		glVertex2f(re * sin(i) + xe + 1, re * cos(i) + ye);
	}
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	Point face_pos = { 420, 190 };
	glColor4f(0.0, 0.0, 0.0,ct);
	Point p1 = { face_pos.x -10, face_pos.y -25 };
	Point p2 = { face_pos.x , face_pos.y-24 };
	Point p3 = { face_pos.x+10 , face_pos.y-20 };
	Point p4 = { face_pos.x + 12, face_pos.y - 18 };
	BezierCurve(p1, p2, p3, p4);
	glDisable(GL_BLEND);

}

void handce() {
	
	glEnable(GL_BLEND);
	glColor4f(0.627, 0.322, 0.176, ct);
	glBegin(GL_LINES);
	glVertex2f(450, 110);
	glVertex2f(450, 130 + leng);
	glEnd();
	glDisable(GL_BLEND);

	if (leng > len && flag1 == true)
	{
		flag1 = false;
	}
	// Up to maximum, change to down
	if (leng < -1 * len && flag1 == false) {
		flag1 = true;
	}
	leng = leng + (flag1 ? speed_hand1 : -1 * speed_hand1);
}

void display(void)
{
	glMatrixMode(GL_PROJECTION);//Assign the current matrix as the projection matrix
	glLoadIdentity();//Set matrix as identity matrix
	gluOrtho2D(0, 900, 0, 600);//For example, create an OpenGL window with a size of 800 * 600 pixels, and then call gluOrtho2D (0400, 0300). When drawing points and lines, specify the coordinates. The coordinate range should be [0400] on the X axis and [0300] on the Y axis. In this way, no matter the X axis or the Y axis, each unit is represented by two pixels.
	glClearColor(1,1,1, 1);//  glClearColor：red、green、blue、alpha
	glClear(GL_COLOR_BUFFER_BIT);//Perform cleanup task
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	DrawBackground();
	DrawSunAndMoon();
	DrawCloud();
	Drawballoon();
	DrawSnowman();
	DrawSmile();
	DrawTree();
	DrawWood();
	DrawFire();
	DrawWoodsbj();
	Drawfirework1();
	Drawfirework2();
	Drawfirework3();
	Drawfirework4();
	DrawWord1();
	DrawBackground1();
	Aballoon();
	Acloud();
	Atree();
	Awood();
	Afire();
	Asnowman();
	handce();
	glutSwapBuffers();//Swap buffers. All drawings are drawn into a background buffer. If you don't swap buffers, you won't see anything drawn
}

void when_in_mainloop() // idle callback function
{
	glutPostRedisplay(); // force OpenGL to redraw the current window
	//glutPostRedisplay marks that the current window needs to be redrawn. The window display will be called back to redisplay the normal panel of the window when the next cycle is passed through glutMainLoop.
}

int main(int argc, char** argv)
{
	//This main function will complete the necessary initialization and open the event processing loop. All GLUT functions have a glut prefix and those that have completed some initialization have a glutInit prefix. The first thing you need to do is call the function glutInit().
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//glutInitDisplayMode() function is mainly used to specify the type of display mode when creating a window.
	//GLUT_DOUBLE indicates the use of double buffered windows, generally used to generate animation effects; GLUT_ RGB color mode
	glutInitWindowPosition(100, 100);//Set the position of the initial window (the upper left corner of the window is relative to the desktop coordinates (x, y)). The coordinates (0,0) refer to the position of the upper left corner of the screen
	glutInitWindowSize(900, 600);//Set window size
	glutCreateWindow("My New Year Card");//Create a window with a title
	glutDisplayFunc(display);//GlutDisplayFunc (display) is automatically called when the program is running, that is, the program will automatically call the display function to redraw the window
	glutIdleFunc(when_in_mainloop);
	/*glutIdleFunc sets the global idle callback to func, so that the GLUT program can
		Perform background processing tasks or continuous animation.If enabled, the idle callback is called continuously when no event is received.
		The callback routine has no parameters.The current windowand menu do not change until the idle callback occurs.Has multiple windowsand /or
		The menu program should explicitly set the current window and /or the current menu, rather than depending on its current settings.Minimize
		The amount of computationand rendering in the idle callback to avoid affecting the interactive response of the program.In general, in an idle callback
		You can render only one frame.Assigning NULL to glutIdleFunc disables the generation of idle callbacks.
		The callback function calls the function in the main function, but the parameters passed are the previously defined functions*/
	glutTimerFunc(time_interval_Sun, OnTimer_Sun, 1);
	glutTimerFunc(time_interval_Cloud, OnTimer_Cloud, 1);
	glutTimerFunc(time_interval_DD, OnTimer_DD, 1);
	glutMouseFunc(mouse_input); // mouse callback function
	glutKeyboardFunc(keyboard_input);
	glutSpecialFunc(specialkeys);
	glutMainLoop();
}








