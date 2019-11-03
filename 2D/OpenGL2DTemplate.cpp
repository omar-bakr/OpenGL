#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <glut.h>
#define PI 3.1415926535897932384626433832795

void Display();
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
void drawobstacle(GLfloat x, GLfloat y, GLfloat radius);

void displaywall();
void displaysun();
void time(int val);
void displaydefender();
void displayhelth1();
void displayhelth2();
void displayplayer1();
void displayplayer2();
void displaypower1();
void displaypower2();
void displaycurve();
void key(unsigned char k, int x, int y);
void keyUp(unsigned char k, int x, int y);
int* bezier(float t, int* p0, int* p1, int* p2, int* p3);
void print(int x, int y, char* string);


int p1damage = 0;
int p2damage = 0;
int xd = 0;//To move the defender 
float ballt = -0.2;
bool xdf = false;
int powerm1 = 0;
int powerm2 = 0;
bool powermf = false;
bool cp = true;//Currnt player where ture==player1
float factor;
bool curve = false;
bool ball = false;
bool gameover = false;
bool hit1 = false;
bool hit2 = false;
float sunangle = 0;
int ballxy[2];
bool ballf = true;
bool defenderf = true;
int deftime = 0;

//BEZIER
int p0[2];
int p1[2];
int p2[2];
int p3[2];

void displaycurve() {

	if (ball&&ballf)
	{
		int* p = bezier(ballt, p0, p1, p2, p3);
		ballxy[0] = p[0];
		ballxy[1] = p[1];
		drawobstacle(p[0], p[1], 20);
	}
}

void displaysun() {
	int i;
	int triangleAmount = 50; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;



	glPushMatrix();
	glTranslated(500, 900,0);
	glRotated(sunangle, 0, 0, 1);
	glTranslated(-500, -900, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1,1,0);
	glVertex2f(500, 900); // center of circle
	for (i = 0; i <= triangleAmount;i++) {
		glVertex2f(
			500 + (30 * cos(i * twicePi / triangleAmount)),
			900 + (30 * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(500.0f, 900.0f, 0.0f);
	glVertex3f(500.0f, 930.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(500.0f, 900.0f, 0.0f);
	glVertex3f(530.0f, 900.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(500.0f, 900.0f, 0.0f);
	glVertex3f(500.0f, 870.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(500.0f, 900.0f, 0.0f);
	glVertex3f(470.0f, 900.0f, 0.0f);
	glEnd();
	glPopMatrix();

}
int* bezier(float t, int* p0, int* p1, int* p2, int* p3)
{
	int res[2];
	res[0] = pow((1 - t), 3) * p0[0] + 3 * t * pow((1 - t), 2) * p1[0] + 3 * pow(t, 2) * (1 - t) * p2[0] + pow(t, 3) * p3[0];
	res[1] = pow((1 - t), 3) * p0[1] + 3 * t * pow((1 - t), 2) * p1[1] + 3 * pow(t, 2) * (1 - t) * p2[1] + pow(t, 3) * p3[1];
	return res;
}

void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void main(int argc, char** argr) {
	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Assignment 1");
	glutDisplayFunc(Display);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(0, time, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 1000.0, 0.0, 1000.0);
	glLineWidth(3);
	glutMainLoop();
}

void key(unsigned char k, int x, int y) {
	if (k == 32) {
		if (cp) {
			if (powerm1 >= 250)
				powermf = true;
			if (powerm1 <= 0)
				powermf = false;
			if (powermf)
				powerm1 -= 10;
			else
				powerm1 += 10;
		}
		else {
			if (powerm2 >= 250)
				powermf = true;
			if (powerm2 <= 0)
				powermf = false;
			if (powermf)
				powerm2 -= 10;
			else
				powerm2 += 10;

		}


	}


	glutPostRedisplay();
}

void keyUp(unsigned char k, int x, int y)
{
	if (k == 32) {
		if (cp) {
			factor = ((float)powerm1 / 250);
			powerm1 = 0;
			curve = true;

		}
		else {
			factor = ((float)powerm2 / 250);
			powerm2 = 0;
			curve = true;
		}
	}


	glutPostRedisplay();
}

void Display() {

	glClear(GL_COLOR_BUFFER_BIT);
	if (!gameover) {
		displaysun();
		displaycurve();
		displaywall();
		displayhelth1();
		displayhelth2();
		displayplayer1();
		displayplayer2();
		displaypower1();
		displaypower2();
		displaydefender();
	}
	else
	{
		if (p1damage >= 150)
			print(400, 500, "Player 2 is the winner");
		if(p2damage>=150)
			print(400, 500, "Player 1 is the winner");
	}
	glFlush();
	
	

}

void displaywall() {


	glBegin(GL_POLYGON);

	glColor3f(1, 1, 1);
	glVertex3f(475.0f, 0.0f, 0.0f);
	glVertex3f(475.0f, 550.0f, 0.0f);
	glVertex3f(525.0f, 550.0f, 0.0f);
	glVertex3f(525.0f, 0.0f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(1, 1, 1);
	glVertex3f(475.0f, 550.0f, 0.0f);
	glVertex3f(500.0f, 600.0f, 0.0f);
	glVertex3f(525.0f, 550.0f, 0.0f);

	glEnd();
}

void displaydefender() {
	if (defenderf) {
		glPushMatrix();
		glTranslated(xd, 0, 0);
		glBegin(GL_POLYGON);

		glColor3f(1, 1, 1);
		glVertex3f(350.0f, 725.0f, 0.0f);
		glVertex3f(350.0f, 770.0f, 0.0f);
		glVertex3f(650.0f, 770.0f, 0.0f);
		glVertex3f(650.0f, 725.0f, 0.0f);

		glEnd();

		glBegin(GL_TRIANGLES);

		glColor3f(1, 1, 1);
		glVertex3f(325.0f, 747.5f, 0.0f);
		glVertex3f(350.0f, 770.0f, 0.0f);
		glVertex3f(350.0f, 725.0f, 0.0f);

		glEnd();

		glBegin(GL_TRIANGLES);

		glColor3f(1, 1, 1);
		glVertex3f(650.0f, 725.0f, 0.0f);
		glVertex3f(650.0f, 770.0f, 0.0f);
		glVertex3f(675.0f, 747.5f, 0.0f);

		glEnd();
		glPopMatrix();

	}



}

void displayhelth1() {

	glBegin(GL_POLYGON);

	glColor3f(1, 1, 1);
	glVertex3f(50.0f, 900.0f, 0.0f);
	glVertex3f(50.0f, 925.0f, 0.0f);
	glVertex3f(200.0f, 925.0f, 0.0f);
	glVertex3f(200.0f, 900.0f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);

	glColor3f(1, 0, 0);
	glVertex3f(50.0f, 900.0f, 0.0f);
	glVertex3f(50.0f, 925.0f, 0.0f);
	glVertex3f(200.0f-p1damage, 925.0f, 0.0f);
	glVertex3f(200.0f-p1damage, 900.0f, 0.0f);

	glEnd();
}

void displayhelth2() {
	glBegin(GL_POLYGON);

	glColor3f(1, 1, 1);
	glVertex3f(800.0f, 900.0f, 0.0f);
	glVertex3f(800.0f, 925.0f, 0.0f);
	glVertex3f(950.0f, 925.0f, 0.0f);
	glVertex3f(950.0f, 900.0f, 0.0f);

	glEnd();

	glBegin(GL_POLYGON);

	glColor3f(1, 0, 0);
	glVertex3f(800.0f, 900.0f, 0.0f);
	glVertex3f(800.0f, 925.0f, 0.0f);
	glVertex3f(950.0f-p2damage, 925.0f, 0.0f);
	glVertex3f(950.0f-p2damage, 900.0f, 0.0f);

	glEnd();
}

void displayplayer1() {
	glPushMatrix();
	if (hit1) {
		glTranslated(0, 50, 0);
	}
	//left foot
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex3f(60.0f, 0.0f, 0.0f);
	glVertex3f(60.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 0.0f, 0.0f);
	glEnd();
	//right foot
	glPushMatrix();
	glTranslated(40, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex3f(60.0f, 0.0f, 0.0f);
	glVertex3f(60.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();
	//Touch
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(95.0f, 25.0f, 0.0f);
	glVertex3f(60.0f, 72.0f, 0.0f);
	glVertex3f(130.0f, 72.0f, 0.0f);
	glEnd();
	//left leg 
	glBegin(GL_POLYGON);

	glColor3f(0, 0, 1);
	glVertex3f(70.0f, 20.0f, 0.0f);
	glVertex3f(70.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 20.0f, 0.0f);

	glEnd();
	//right leg
	glPushMatrix();
	glTranslated(40, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex3f(70.0f, 20.0f, 0.0f);
	glVertex3f(70.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 20.0f, 0.0f);
	glEnd();
	glPopMatrix();
	//body
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex3f(60.0f, 72.0f, 0.0f);
	glVertex3f(60.0f, 170.0f, 0.0f);
	glVertex3f(130.0f, 170.0f, 0.0f);
	glVertex3f(130.0f, 72.0f, 0.0f);
	glEnd();
	//right hand
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(135.0f, 60.0f, 0.0f);
	glVertex3f(135.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 60.0f, 0.0f);
	glEnd();

	glColor3f(1, 1, 1);
	drawFilledCircle(137.5, 50, 4);
	//left hand 
	glPushMatrix();
	glTranslated(-85, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(135.0f, 60.0f, 0.0f);
	glVertex3f(135.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 60.0f, 0.0f);
	glEnd();

	glColor3f(1, 1, 1);
	drawFilledCircle(137.5, 50, 4);
	glPopMatrix();
	//head
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex3f(75.0f, 175.0f, 0.0f);
	glVertex3f(75.0f, 215.0f, 0.0f);
	glVertex3f(115.0f, 215.0f, 0.0f);
	glVertex3f(115.0f, 175.0f, 0.0f);
	glEnd();

	drawFilledCircle(90, 200, 5);
	drawFilledCircle(100, 200, 5);

	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(117.0f, 195.0f, 0.0f);
	glVertex3f(127.0f, 195.0f, 0.0f);
	glEnd();

	glPushMatrix();
	glTranslated(-52, 0, 0);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(117.0f, 195.0f, 0.0f);
	glVertex3f(127.0f, 195.0f, 0.0f);
	glEnd();
	glPopMatrix();
	glPopMatrix();





}

void displayplayer2() {
	glPushMatrix();
	glTranslated(800, 0, 0);
	if (hit2) {
		glRotated(15, 0, 0, 1);
	}

	//left foot
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(60.0f, 0.0f, 0.0f);
	glVertex3f(60.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 0.0f, 0.0f);
	glEnd();
	//right foot
	glPushMatrix();
	glTranslated(40, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex3f(60.0f, 0.0f, 0.0f);
	glVertex3f(60.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 20.0f, 0.0f);
	glVertex3f(90.0f, 0.0f, 0.0f);
	glEnd();
	glPopMatrix();
	//Touch
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(95.0f, 25.0f, 0.0f);
	glVertex3f(60.0f, 72.0f, 0.0f);
	glVertex3f(130.0f, 72.0f, 0.0f);
	glEnd();
	//left leg 
	glBegin(GL_POLYGON);

	glColor3f(0, 1, 0);
	glVertex3f(70.0f, 20.0f, 0.0f);
	glVertex3f(70.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 20.0f, 0.0f);

	glEnd();
	//right leg
	glPushMatrix();
	glTranslated(40, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex3f(70.0f, 20.0f, 0.0f);
	glVertex3f(70.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 70.0f, 0.0f);
	glVertex3f(80.0f, 20.0f, 0.0f);
	glEnd();
	glPopMatrix();
	//body
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex3f(60.0f, 72.0f, 0.0f);
	glVertex3f(60.0f, 170.0f, 0.0f);
	glVertex3f(130.0f, 170.0f, 0.0f);
	glVertex3f(130.0f, 72.0f, 0.0f);
	glEnd();
	//right hand
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(135.0f, 60.0f, 0.0f);
	glVertex3f(135.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 60.0f, 0.0f);
	glEnd();

	glColor3f(1, 1, 1);
	drawFilledCircle(137.5, 50, 4);
	//left hand 
	glPushMatrix();
	glTranslated(-85, 0, 0);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(135.0f, 60.0f, 0.0f);
	glVertex3f(135.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 165.0f, 0.0f);
	glVertex3f(140.0f, 60.0f, 0.0f);
	glEnd();

	glColor3f(1, 1, 1);
	drawFilledCircle(137.5, 50, 4);
	glPopMatrix();
	//head
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex3f(75.0f, 175.0f, 0.0f);
	glVertex3f(75.0f, 215.0f, 0.0f);
	glVertex3f(115.0f, 215.0f, 0.0f);
	glVertex3f(115.0f, 175.0f, 0.0f);
	glEnd();

	drawFilledCircle(90, 200, 5);
	drawFilledCircle(100, 200, 5);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(117.0f, 195.0f, 0.0f);
	glVertex3f(127.0f, 195.0f, 0.0f);
	glEnd();

	glPushMatrix();
	glTranslated(-52, 0, 0);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(117.0f, 195.0f, 0.0f);
	glVertex3f(127.0f, 195.0f, 0.0f);
	glEnd();
	glPopMatrix();
	glPopMatrix();

}

void displaypower1() {
	if (cp) {
		glBegin(GL_LINE_LOOP);

		glColor3f(1, 1, 1);
		glVertex3f(15.0f, 0.0f, 0.0f);
		glVertex3f(15.0f, 250.0f, 0.0f);
		glVertex3f(25.0f, 250.0f, 0.0f);
		glVertex3f(25.0f, 0.0f, 0.0f);

		glEnd();

		glPushMatrix();
		glBegin(GL_POLYGON);

		glColor3f(1, 0, 0);
		glVertex3f(15.0f, 0.0f, 0.0f);
		glVertex3f(15.0f, powerm1, 0.0f);
		glVertex3f(25.0f, powerm1, 0.0f);
		glVertex3f(25.0f, 0.0f, 0.0f);

		glEnd();
		glPopMatrix();
	}
}

void displaypower2() {
	if (!cp) {
		glPushMatrix();
		glTranslated(950, 0, 0);
		glBegin(GL_LINE_LOOP);

		glColor3f(1, 1, 1);
		glVertex3f(15.0f, 0.0f, 0.0f);
		glVertex3f(15.0f, 250.0f, 0.0f);
		glVertex3f(25.0f, 250.0f, 0.0f);
		glVertex3f(25.0f, 0.0f, 0.0f);

		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslated(950, 0, 0);
		glBegin(GL_POLYGON);

		glColor3f(1, 0, 0);
		glVertex3f(15.0f, 0.0f, 0.0f);
		glVertex3f(15.0f, powerm2, 0.0f);
		glVertex3f(25.0f, powerm2, 0.0f);
		glVertex3f(25.0f, 0.0f, 0.0f);

		glEnd();
		glPopMatrix();

	}
	
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 50; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	

	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0, 1, 1);
	glVertex2f(x,y); // center of circle
	for (i = 0; i <= triangleAmount;i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
	glPopMatrix();
}

void drawobstacle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 50; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;



	

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0, 1, 1);
	glVertex2f(x,y); // center of circle
	for (i = 0; i <= triangleAmount;i++) {
		glVertex2f(
			x + (radius * cos(i * twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 0);
	glVertex2f(x , y - 20);
	glVertex2f(x-20, y );
	glVertex2f(x, y + 20);
	glVertex2f(x+20, y);
	glEnd();
	glPopMatrix();
}

void time(int val) {
	deftime++;
	sunangle+=1;

	if (deftime >= 500) {
		defenderf = true;
		deftime = 0;
	}

	if (ballt >= 1.3)
	{
		ball = false;
		curve = false;
		ballf = true;
		cp = !cp;
		ballt = -0.2;
		hit1 = false;
		hit2 = false;

	}
	if (ballt < 1.3 && ball)
	{
		ballt += 0.003;
	}

	if (xdf)
		xd--;
	else
		xd++;

	if (xd + 675 > 1000)
		xdf = true;

	if (xd + 325 < 0)
		xdf = false;

	if (curve) {
		if (cp) {
			p0[0] = 150;
			p0[1] = 0;

			p1[0] = 150 * factor + 150;
			p1[1] = 600 * factor + 500;

			p2[0] = 300 * factor + 150;
			p2[1] = 600 * factor + 500;

			p3[0] = 850 * factor + 150;
			p3[1] = 0;
			ball = true;


		}
		else
		{
			p0[0] = 850;
			p0[1] = 0;

			p1[0] = 850-(325 * factor);
			p1[1] = 400 + (600 * factor);

			p2[0] = 850 - (425 * factor);
			p2[1] = 400 + (600 * factor);

			p3[0] = 850 - 900*factor;
			p3[1] = 0;
			ball = true;

		}
		

	}

	if (ballxy[0] > 475 && ballxy[0] < 525 && ballxy[1]>0 && ballxy[1] < 600)
	{
		ballf = false;
		ballxy[0] = -1;
		ballxy[1] = -1;
		ballt = 1.3;
	}

	if (ballxy[0] > 325+xd && ballxy[0] < 675+xd && ballxy[1]>705 && ballxy[1] < 770&& defenderf)
	{
		ballf = false;
		ballxy[0] = -1;
		ballxy[1] = -1;
		ballt = 1.3;
		defenderf = false;
		deftime = 0;
		
	}
	if (ballxy[0] > 60  && ballxy[0] < 140 && ballxy[1]>0 && ballxy[1] < 235)
	{
		ballf = false;
		ballxy[0] = -1;
		ballxy[1] = -1;
		ballt = 1.3;
		p1damage += 37.5;
		hit1 = true;

	}
	if (ballxy[0] > 860  && ballxy[0] < 940  && ballxy[1]>0 && ballxy[1] < 235)
	{
		ballf = false;
		ballxy[0] = -1;
		ballxy[1] = -1;
		ballt = 1.3;
		p2damage += 37.5;
		hit2 = true;
	}
	if (p1damage > 149||p2damage > 149) {
		gameover = true;
		   
	}
	

	

	


	glutTimerFunc(10, time, 0);
	glutPostRedisplay();
}
