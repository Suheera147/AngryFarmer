/*********
 CTIS164
----------
STUDENT : Suheera Tanvir
STUDENT ID: 21801211
SECTION : 02
HOMEWORK: 03
----------
PROBLEMS:
Very long code.
----------
ADDITIONAL FEATURES:
1) Lots of background
2) Added Gamestates. Explained in the initial 'HowToPlay' gamestate.
3) Added game levels. Easy, Medium, Hard. You can select in 'HowToPlay' gamestate.
Easy level has the computer moving slowly.Medium has it faster.
Hard moves fastest so you need to be quicker when answering
4) All five arithmetic equations possible.
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
#include <time.h>

#define WINDOW_WIDTH  1050
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  70 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define HOWTOPLAY 0   //gamestates
#define GAME 1
#define PAUSESCREEN 2
#define GAMEOVER 3

#define EASY 4   //levels
#define MEDIUM 5
#define HARD 6

int gameState = HOWTOPLAY;   //initital gamestate
int level = EASY;            //initial level

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight;

bool color = false;
int xBird = 300, yBird = -100;       // bird/target's initial position
bool birdIdle = true;               //flag to set bird not moving
double r = 227, g = 227, b = 811;    //bird's initial color

int xS = 0, yS = 0;  // shuriken's initial position
int incX, incY;
bool movement = false;             //shuriken shooting
int xA = 0, yA = 110;  //arrow head initial coordinates
float dirX = 0, dirY = 0;
float bAngle = 0;                       //Bird's angle
float angle = 0;                        //shooter angle
int speed = 1;


int point = 1; //points of target. will be randomized

char oper[5] = { '+','-','/','%','*' };   //array to store operand characters
int operIndex = 0;        //index for the operand array
int v1 = 1, v2 = 2, answer = 0;    //variables for equation
int options[4];      //array to hold options
int answerindex = 0; //index of option that will contain the answer
bool correctAnswer = false;
bool answerclick = false;
bool initial = false;

double r1 = 1, g1 = 1, b1 = 0;    //color value for  option boxes
double r2 = 1, g2 = 1, b2 = 0;
double r3 = 1, g3 = 1, b3 = 0;
double r4 = 1, g4 = 1, b4 = 0;

char name[10] = { 'P','L','A','Y','E','R','\0' };
int Pscore = 0;         //player score
int Cscore = 0;         //Computer score  
double timer = 6000;    //timer set to 60


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


void Equationfunction()   //function that generates new equation and calculates its answer
{

	operIndex = rand() % 4;
	v1 = (rand() % 8) + 1;
	v2 = (rand() % 8) + 1;
	while (v1 < v2)
	{
		v1 = (rand() % 8) + 1;
		v2 = (rand() % 8) + 1;
	}

	switch (oper[operIndex])
	{
	case '+': answer = v1 + v2; break;
	case '-': answer = v1 - v2; break;
	case '*': answer = v1 * v2; break;
	case '/': answer = v1 / v2; break;
	case '%': answer = v1 % v2; break;
	}


	options[0] = rand() % 20;
	while (options[0] == answer)
		options[0] = rand() % 20;
	options[1] = rand() % 20;
	while (options[1] == answer)
		options[1] = rand() % 20;
	options[2] = rand() % 20;
	while (options[2] == answer)
		options[2] = rand() % 20;
	options[3] = rand() % 20;
	while (options[3] == answer)
		options[3] = rand() % 20;


	answerindex = (rand() % 3);
	options[answerindex] = answer;
	correctAnswer = false;
	color = false;
	answerclick = false;
	initial = true;

}

void optionboxes()  //function that draws option boxes 
{
	if (initial == false) {
		Equationfunction();
	}
	glColor3f(r1, g1, b1);
	glRectf(0, -225, 80, -280);
	glLineWidth(2.5);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, -225);
	glVertex2f(0, -280);
	glVertex2f(80, -280);
	glVertex2f(80, -225);
	glEnd();
	glColor3f(0, 0, 0);
	vprint2(15, -270, 0.4, "%d", options[0]);
	glLineWidth(1);


	glColor3f(r2, g2, b2);
	glRectf(120, -225, 200, -280);
	glLineWidth(2.5);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(120, -225);
	glVertex2f(120, -280);
	glVertex2f(200, -280);
	glVertex2f(200, -225);
	glEnd();
	glColor3f(0, 0, 0);
	vprint2(135, -270, 0.4, "%d", options[1]);
	glLineWidth(1);

	glColor3f(r3, g3, b3);
	glRectf(240, -225, 320, -280);
	glLineWidth(2.5);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(240, -225);
	glVertex2f(240, -280);
	glVertex2f(320, -280);
	glVertex2f(320, -225);
	glEnd();
	glColor3f(0, 0, 0);
	vprint2(255, -270, 0.4, "%d", options[2]);
	glLineWidth(1);

	glColor3f(r4, g4, b4);
	glRectf(360, -225, 440, -280);
	glLineWidth(2.5);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(360, -225);
	glVertex2f(360, -280);
	glVertex2f(440, -280);
	glVertex2f(440, -225);
	glEnd();
	glColor3f(0, 0, 0);
	vprint2(375, -270, 0.4, "%d", options[3]);
	glLineWidth(1);

}

void shuriken()     //function for making shuriken to be shot at the targets
{
	glColor3f(0.329, 0.329, 0.388);
	glBegin(GL_QUADS);
	glVertex2f(xS, yS + 35);
	glVertex2f(xS + 10, yS + 10);
	glVertex2f(xS, yS);
	glVertex2f(xS - 10, yS + 10);
	glEnd();
	glColor3f(0.329, 0.329, 0.388);
	glBegin(GL_QUADS);
	glVertex2f(xS, yS - 35);
	glVertex2f(xS + 10, yS - 10);
	glVertex2f(xS, yS);
	glVertex2f(xS - 10, yS - 10);
	glEnd();
	glColor3f(0.329, 0.329, 0.388);
	glBegin(GL_QUADS);
	glVertex2f(xS + 35, yS);
	glVertex2f(xS + 10, yS + 10);
	glVertex2f(xS, yS);
	glVertex2f(xS + 10, yS - 10);
	glEnd();
	glColor3f(0.329, 0.329, 0.388);
	glBegin(GL_QUADS);
	glVertex2f(xS - 35, yS);
	glVertex2f(xS - 10, yS + 10);
	glVertex2f(xS, yS);
	glVertex2f(xS - 10, yS - 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xS, yS + 35);
	glVertex2f(xS + 10, yS + 10);
	glVertex2f(xS + 35, yS);
	glVertex2f(xS + 10, yS - 10);
	glVertex2f(xS, yS - 35);
	glVertex2f(xS - 10, yS - 10);
	glVertex2f(xS - 35, yS);
	glVertex2f(xS - 10, yS + 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS, yS + 35);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS, yS - 35);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS + 35, yS);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS - 35, yS);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS - 10, yS - 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS + 10, yS - 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS - 10, yS + 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xS, yS);
	glVertex2f(xS + 10, yS + 10);
	glEnd();
}

void shurikenmovement()    //function to move shuriken when it is to be shot at the target
{
	srand(time(NULL));

	if (movement == true)
	{
		incX = dirX;
		incY = dirY;
		xS += incX / 2;
		yS += incY / 2;


		if (xS > 600 || xS < -600 || yS > 350 || yS < -350)
		{
			movement = false;
			xS = 0;
			yS = 0;
		}

		if (xS > xBird - 40 && xS < xBird + 40 && yS > yBird - 40 && yS < yBird + 60)
		{
			movement = false;
			Cscore = Cscore + point;
			xBird = (rand() % 1000) - 500;
			yBird = (rand() % 500) - 200;
			point = (rand() % 5) + 1;
			r = (rand() % 1000);
			g = (rand() % 1000);
			b = (rand() % 1000);

			Equationfunction();

			if ((xBird > -140 && xBird < 140) && (yBird > -100 && yBird < 140))  //to ensure bird is not made on top of farmer
			{
				xBird = (rand() % 1000) - 500;
				yBird = (rand() % 500) - 200;
			}

			xS = 0;
			yS = 0;
		}

	}
}

void drawBird()     //function to draw bird/target
{

	//wings
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(xBird + 30, yBird + 20);
	glVertex2f(xBird + 30, yBird - 17);
	glVertex2f(xBird + 65, yBird + 35);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(xBird - 30, yBird + 20);
	glVertex2f(xBird - 30, yBird - 17);
	glVertex2f(xBird - 65, yBird + 35);
	glEnd();

	//face
	glColor3f(0.823, 0.823, 0.952);
	circle(xBird, yBird + 50, 18);
	glColor3f(0, 0, 0);
	circle_wire(xBird, yBird + 50, 18);

	glColor3f(0.901, 0.694, 0.176);
	glBegin(GL_TRIANGLES);
	glVertex2f(xBird - 5, yBird + 47);
	glVertex2f(xBird + 5, yBird + 47);
	glVertex2f(xBird, yBird + 35);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xBird - 5, yBird + 47);
	glVertex2f(xBird + 5, yBird + 47);
	glVertex2f(xBird, yBird + 35);
	glEnd();
	glColor3f(0, 0, 0);
	circle(xBird - 7.5, yBird + 55, 3);
	glColor3f(0, 0, 0);
	circle(xBird + 7.5, yBird + 55, 3);

	//body
	glColor3f(r / 1000, g / 1000, b / 1000);
	circle(xBird, yBird, 35);
	glColor3f(0, 0, 0);
	circle_wire(xBird, yBird, 35);

	glColor3f(0.698, 0.392, 0.121);
	glBegin(GL_TRIANGLES);
	glVertex2f(xBird - 20, yBird - 25);
	glVertex2f(xBird - 5, yBird - 25);
	glVertex2f(xBird - 12.5, yBird - 45);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xBird - 20, yBird - 25);
	glVertex2f(xBird - 5, yBird - 25);
	glVertex2f(xBird - 12.5, yBird - 45);
	glEnd();


	glColor3f(0.698, 0.392, 0.121);
	glBegin(GL_TRIANGLES);
	glVertex2f(xBird + 20, yBird - 25);
	glVertex2f(xBird + 5, yBird - 25);
	glVertex2f(xBird + 12.5, yBird - 45);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xBird + 20, yBird - 25);
	glVertex2f(xBird + 5, yBird - 25);
	glVertex2f(xBird + 12.5, yBird - 45);
	glEnd();

	bAngle = (atan2(yBird, xBird)) / D2R;
	if (bAngle < 0)
		bAngle = 360 + bAngle;

	vprint(xBird - 5, yBird - 2, GLUT_BITMAP_TIMES_ROMAN_24, "%d", point);
}

void drawFarmer()   //function to draw farmer
{
	//face
	glColor3f(1, 0.839, 0.721);
	circle(0, 10, 60);
	glColor3f(0, 0, 0);
	circle_wire(0, 10, 60);

	//mouth
	glColor3f(0.933, 0.525, 0.560);
	glBegin(GL_QUADS);
	glVertex2f(-35, -30);
	glVertex2f(-20, -40);
	glVertex2f(20, -40);
	glVertex2f(35, -30);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-35, -30);
	glVertex2f(-20, -40);
	glVertex2f(20, -40);
	glVertex2f(35, -30);
	glEnd();

	glColor3f(0.917, 0.549, 0.341);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 10);
	glVertex2f(-10, -15);
	glVertex2f(10, -15);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, 10);
	glVertex2f(-10, -15);
	glVertex2f(10, -15);
	glEnd();

	//straw
	glColor3f(0, 0, 0);
	glLineWidth(1.42);
	glBegin(GL_LINE_STRIP);
	glVertex2f(52, -60);
	glVertex2f(35, -60);
	glVertex2f(35, -75);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(55, -65);
	glVertex2f(40, -65);
	glVertex2f(40, -80);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(60, -70);
	glVertex2f(45, -70);
	glVertex2f(45, -85);
	glEnd();
	glLineWidth(1);

	glColor3f(0.749, 0.576, 0.411);
	glBegin(GL_QUADS);
	glVertex2f(0, -25);
	glVertex2f(50, -75);
	glVertex2f(53, -75);
	glVertex2f(3, -25);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0, -25);
	glVertex2f(50, -75);
	glVertex2f(53, -75);
	glVertex2f(3, -25);
	glEnd();

	//eyes
	glColor3f(0.858, 0.090, 0.078);
	circle(-25, 42, 12);
	glColor3f(0, 0, 0);
	circle_wire(-25, 42, 12);
	glColor3f(0, 0, 0);
	circle(-25, 42, 7);

	glColor3f(0.858, 0.090, 0.078);
	circle(25, 42, 12);
	glColor3f(0, 0, 0);
	circle_wire(25, 42, 12);
	glColor3f(0, 0, 0);
	circle(25, 42, 7);

	//moustache

	glColor3f(0.129, 0.031, 0.011);
	glBegin(GL_POLYGON);
	glVertex2f(-45, -30);
	glVertex2f(-50, -15);
	glVertex2f(-40, -25);
	glVertex2f(-25, -15);
	glVertex2f(-10, -15);
	glVertex2f(-10, -30);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(45, -30);
	glVertex2f(50, -15);
	glVertex2f(40, -25);
	glVertex2f(25, -15);
	glVertex2f(10, -15);
	glVertex2f(10, -30);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2f(10, -30);
	glVertex2f(10, -15);
	glVertex2f(0, -20);
	glVertex2f(-10, -15);
	glVertex2f(-10, -30);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-45, -30);
	glVertex2f(-50, -15);
	glVertex2f(-40, -25);
	glVertex2f(-25, -15);
	glVertex2f(-10, -15);
	glVertex2f(0, -20);
	glVertex2f(10, -15);
	glVertex2f(25, -15);
	glVertex2f(40, -25);
	glVertex2f(50, -15);
	glVertex2f(45, -30);
	glEnd();

	//hat
	glColor3f(0.698, 0.517, 0.219);
	glBegin(GL_QUADS);
	glVertex2f(-80, 50);
	glVertex2f(-50, 70);
	glVertex2f(50, 70);
	glVertex2f(80, 50);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-80, 50);
	glVertex2f(-50, 70);
	glVertex2f(50, 70);
	glVertex2f(80, 50);
	glEnd();

	glColor3f(0.556, 0.415, 0.180);
	glBegin(GL_QUADS);
	glVertex2f(-40, 80);
	glVertex2f(-50, 70);
	glVertex2f(50, 70);
	glVertex2f(40, 80);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-40, 80);
	glVertex2f(-50, 70);
	glVertex2f(50, 70);
	glVertex2f(40, 80);
	glEnd();

	glColor3f(0.717, 0.133, 0.082);
	glBegin(GL_QUADS);
	glVertex2f(-40, 80);
	glVertex2f(-40, 90);
	glVertex2f(40, 90);
	glVertex2f(40, 80);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-40, 80);
	glVertex2f(-40, 90);
	glVertex2f(40, 90);
	glVertex2f(40, 80);
	glEnd();

	glColor3f(0.698, 0.517, 0.219);
	glBegin(GL_POLYGON);
	glVertex2f(-40, 90);
	glVertex2f(-35, 100);
	glVertex2f(-25, 110);
	glVertex2f(-15, 115);
	glVertex2f(15, 115);
	glVertex2f(25, 110);
	glVertex2f(35, 100);
	glVertex2f(40, 90);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-40, 90);
	glVertex2f(-35, 100);
	glVertex2f(-25, 110);
	glVertex2f(-15, 115);
	glVertex2f(15, 115);
	glVertex2f(25, 110);
	glVertex2f(35, 100);
	glVertex2f(40, 90);
	glEnd();
	glColor3f(0.101, 0.098, 0.603);
	vprint(-13, 12, GLUT_BITMAP_TIMES_ROMAN_24, "%0.0f", angle);

}

void drawArrow()   //function that draws the rotating arrow on the farmer's face
{
	glColor3f(0, 0, 0);
	glLineWidth(2.3);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0, 10);
	glVertex2f(xA, yA);
	glEnd();
	glLineWidth(1);
	circle(xA, yA, 5);
}

void drawBackground()   //function to draw background
{
	//ScoreBoard
	glColor3f(0.784, 0.380, 0.176);
	glBegin(GL_QUADS);
	glVertex2f(-800, -290);
	glVertex2f(800, -290);
	glVertex2f(800, -360);
	glVertex2f(-800, -360);
	glEnd();
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-800, -290);
	glVertex2f(800, -290);
	glVertex2f(800, -360);
	glVertex2f(-800, -360);
	glEnd();
	glLineWidth(1);


	vprint(-500, -320, GLUT_BITMAP_TIMES_ROMAN_24, "%s : %d", name, Pscore);
	if (timer <= 0)
		vprint(-100, -320, GLUT_BITMAP_TIMES_ROMAN_24, "TIMER : 0.00");
	else
		vprint(-100, -320, GLUT_BITMAP_TIMES_ROMAN_24, "TIMER : %.2f", timer / 100);
	vprint(270, -320, GLUT_BITMAP_TIMES_ROMAN_24, "COMPUTER : %d", Cscore);

	//Light green strip of land
	glColor3f(0.407, 0.862, 0.337);
	glBegin(GL_QUADS);
	glVertex2f(-800, -110);
	glVertex2f(800, -110);
	glVertex2f(800, -220);
	glVertex2f(-800, -220);
	glEnd();



	//GameBoard
	glColor3f(0.831, 0.737, 0.329);
	glBegin(GL_QUADS);
	glVertex2f(-800, -290);
	glVertex2f(800, -290);
	glVertex2f(800, -220);
	glVertex2f(-800, -220);
	glEnd();
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-800, -290);
	glVertex2f(800, -290);
	glVertex2f(800, -220);
	glVertex2f(-800, -220);
	glEnd();
	glLineWidth(1);

	glLineWidth(3);
	vprint2(-500, -270, 0.3, "%d %c %d = ?", v1, oper[operIndex], v2);
	glLineWidth(1);

	optionboxes();

	//darker green strip of land
	glColor3f(0.274, 0.643, 0.215);
	glBegin(GL_QUADS);
	glVertex2f(-800, -110);
	glVertex2f(800, -110);
	glVertex2f(800, 0);
	glVertex2f(-800, 0);
	glEnd();

	//darkest green strip of land
	glColor3f(0.219, 0.411, 0.188);
	glBegin(GL_QUADS);
	glVertex2f(-800, 40);
	glVertex2f(800, 40);
	glVertex2f(800, 0);
	glVertex2f(-800, 0);
	glEnd();
	glColor3f(0, 0, 0);
	/*glBegin(GL_LINE_STRIP);
	glVertex2f(-800, 40);
	glVertex2f(800, 40);
	glEnd();*/

	//Sun
	glColor3f(0.949, 0.909, 0.192);
	circle(-300, 260, 60);
	glColor3f(0, 0, 0);
	circle_wire(-300, 260, 60);

	//trees
	//1
	glColor3f(0.607, 0.231, 0.109);
	glBegin(GL_QUADS);
	glVertex2f(-100, 80);
	glVertex2f(-80, 80);
	glVertex2f(-80, 0);
	glVertex2f(-100, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-100, 80);
	glVertex2f(-80, 80);
	glVertex2f(-80, 0);
	glVertex2f(-100, 0);
	glEnd();
	glColor3f(0.325, 0.721, 0.098);
	glBegin(GL_TRIANGLES);
	glVertex2f(-120, 60);
	glVertex2f(-60, 60);
	glVertex2f(-90, 160);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-120, 60);
	glVertex2f(-60, 60);
	glVertex2f(-90, 160);
	glEnd();

	//2
	glColor3f(0.352, 0.137, 0.047);
	glBegin(GL_QUADS);
	glVertex2f(-360, 80);
	glVertex2f(-340, 80);
	glVertex2f(-340, 0);
	glVertex2f(-360, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-360, 80);
	glVertex2f(-340, 80);
	glVertex2f(-340, 0);
	glVertex2f(-360, 0);
	glEnd();
	glColor3f(0.243, 0.505, 0.094);
	glBegin(GL_TRIANGLES);
	glVertex2f(-380, 60);
	glVertex2f(-320, 60);
	glVertex2f(-350, 160);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-380, 60);
	glVertex2f(-320, 60);
	glVertex2f(-350, 160);
	glEnd();

	//3
	glColor3f(0.396, 0.219, 0.141);
	glBegin(GL_QUADS);
	glVertex2f(-180, 60);
	glVertex2f(-160, 60);
	glVertex2f(-160, -20);
	glVertex2f(-180, -20);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-180, 60);
	glVertex2f(-160, 60);
	glVertex2f(-160, -20);
	glVertex2f(-180, -20);
	glEnd();
	glColor3f(0.207, 0.309, 0.152);
	glBegin(GL_TRIANGLES);
	glVertex2f(-210, 40);
	glVertex2f(-130, 40);
	glVertex2f(-170, 180);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-210, 40);
	glVertex2f(-130, 40);
	glVertex2f(-170, 180);
	glEnd();

	//4
	glColor3f(0.647, 0.333, 0.152);
	glBegin(GL_QUADS);
	glVertex2f(-280, 20);
	glVertex2f(-260, 20);
	glVertex2f(-260, -60);
	glVertex2f(-280, -60);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-280, 20);
	glVertex2f(-260, 20);
	glVertex2f(-260, -60);
	glVertex2f(-280, -60);
	glEnd();
	glColor3f(0.345, 0.607, 0.192);
	glBegin(GL_TRIANGLES);
	glVertex2f(-310, 0);
	glVertex2f(-230, 0);
	glVertex2f(-270, 120);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-310, 0);
	glVertex2f(-230, 0);
	glVertex2f(-270, 120);
	glEnd();


	//5
	glColor3f(0.564, 0.250, 0.074);
	glBegin(GL_QUADS);
	glVertex2f(-220, -20);
	glVertex2f(-200, -20);
	glVertex2f(-200, -100);
	glVertex2f(-220, -100);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-220, -20);
	glVertex2f(-200, -20);
	glVertex2f(-200, -100);
	glVertex2f(-220, -100);
	glEnd();
	glColor3f(0.184, 0.623, 0.078);
	glBegin(GL_TRIANGLES);
	glVertex2f(-250, -40);
	glVertex2f(-170, -40);
	glVertex2f(-210, 80);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-250, -40);
	glVertex2f(-170, -40);
	glVertex2f(-210, 80);
	glEnd();

	//6
	glColor3f(0.662, 0.313, 0.117);
	glBegin(GL_QUADS);
	glVertex2f(-400, -40);
	glVertex2f(-380, -40);
	glVertex2f(-380, -120);
	glVertex2f(-400, -120);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-400, -40);
	glVertex2f(-380, -40);
	glVertex2f(-380, -120);
	glVertex2f(-400, -120);
	glEnd();
	glColor3f(0.203, 0.674, 0.086);
	glBegin(GL_TRIANGLES);
	glVertex2f(-350, -60);
	glVertex2f(-430, -60);
	glVertex2f(-390, 40);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-350, -60);
	glVertex2f(-430, -60);
	glVertex2f(-390, 40);
	glEnd();

	//7
	glColor3f(0.768, 0.266, 0.090);
	glBegin(GL_QUADS);
	glVertex2f(-340, -60);
	glVertex2f(-320, -60);
	glVertex2f(-320, -140);
	glVertex2f(-340, -140);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-340, -60);
	glVertex2f(-320, -60);
	glVertex2f(-320, -140);
	glVertex2f(-340, -140);
	glEnd();
	glColor3f(0.192, 0.741, 0.156);
	glBegin(GL_TRIANGLES);
	glVertex2f(-380, -80);
	glVertex2f(-280, -80);
	glVertex2f(-330, 60);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-380, -80);
	glVertex2f(-280, -80);
	glVertex2f(-330, 60);
	glEnd();

	//8
	glColor3f(0.768, 0.266, 0.090);
	glBegin(GL_QUADS);
	glVertex2f(-140, -80);
	glVertex2f(-120, -80);
	glVertex2f(-120, -160);
	glVertex2f(-140, -160);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-140, -80);
	glVertex2f(-120, -80);
	glVertex2f(-120, -160);
	glVertex2f(-140, -160);
	glEnd();
	glColor3f(0.215, 0.690, 0.188);
	glBegin(GL_TRIANGLES);
	glVertex2f(-170, -100);
	glVertex2f(-90, -100);
	glVertex2f(-130, 20);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(-170, -100);
	glVertex2f(-90, -100);
	glVertex2f(-130, 20);
	glEnd();


	//clouds
	glColor3f(0.95, 0.95, 0.95);
	circle(-200, 260, 30);
	glColor3f(0.95, 0.95, 0.95);
	circle(-250, 260, 30);
	glColor3f(0.95, 0.95, 0.95);
	circle(-220, 300, 30);
	glColor3f(0.95, 0.95, 0.95);
	circle(-170, 290, 30);
	glColor3f(0.95, 0.95, 0.95);
	circle(-180, 240, 40);
	glColor3f(0.95, 0.95, 0.95);
	circle(-140, 270, 20);
	glColor3f(0.95, 0.95, 0.95);
	circle(-60, 260, 40);
	glColor3f(0.95, 0.95, 0.95);
	circle(-110, 260, 30);

	glColor3f(0.95, 0.95, 0.95);
	circle(300, 280, 30);
	glColor3f(0.95, 0.95, 0.95);
	circle(260, 260, 40);
	glColor3f(0.95, 0.95, 0.95);
	circle(240, 300, 40);
	glColor3f(0.95, 0.95, 0.95);
	circle(220, 320, 20);
	glColor3f(0.95, 0.95, 0.95);
	circle(200, 280, 40);
	glColor3f(0.95, 0.95, 0.95);
	circle(180, 300, 20);
	glColor3f(0.95, 0.95, 0.95);
	circle(160, 260, 30);
	glColor3f(0.95, 0.95, 0.95);
	circle(140, 300, 40);
	glColor3f(0.95, 0.95, 0.95);
	circle(120, 280, 40);


	//House
	//main
	glColor3f(0.968, 0.772, 0.631);
	glBegin(GL_QUADS);
	glVertex2f(140, 100);
	glVertex2f(220, 100);
	glVertex2f(220, 0);
	glVertex2f(140, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(140, 100);
	glVertex2f(220, 100);
	glVertex2f(220, 0);
	glVertex2f(140, 0);
	glEnd();

	glColor3f(1, 0.776, 0.619);
	glBegin(GL_QUADS);
	glVertex2f(380, 100);
	glVertex2f(220, 100);
	glVertex2f(220, 0);
	glVertex2f(380, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(380, 100);
	glVertex2f(220, 100);
	glVertex2f(220, 0);
	glVertex2f(380, 0);
	glEnd();

	glColor3f(0.792, 0.145, 0.086);
	glBegin(GL_QUADS);
	glVertex2f(180, 160);
	glVertex2f(340, 160);
	glVertex2f(380, 100);
	glVertex2f(220, 100);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(180, 160);
	glVertex2f(340, 160);
	glVertex2f(380, 100);
	glVertex2f(220, 100);
	glEnd();

	glColor3f(0.925, 0.592, 0.333);
	glBegin(GL_TRIANGLES);
	glVertex2f(180, 160);
	glVertex2f(220, 100);
	glVertex2f(140, 100);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(180, 160);
	glVertex2f(220, 100);
	glVertex2f(140, 100);
	glEnd();
	//windows
	//1
	glColor3f(0.729, 0.968, 0.952);
	glBegin(GL_QUADS);
	glVertex2f(240, 80);
	glVertex2f(280, 80);
	glVertex2f(280, 60);
	glVertex2f(240, 60);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(240, 80);
	glVertex2f(280, 80);
	glVertex2f(280, 60);
	glVertex2f(240, 60);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(240, 70);
	glVertex2f(280, 70);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(260, 60);
	glVertex2f(260, 80);
	glEnd();

	//2
	glColor3f(0.729, 0.968, 0.952);
	glBegin(GL_QUADS);
	glVertex2f(320, 80);
	glVertex2f(360, 80);
	glVertex2f(360, 60);
	glVertex2f(320, 60);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(320, 80);
	glVertex2f(360, 80);
	glVertex2f(360, 60);
	glVertex2f(320, 60);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(320, 70);
	glVertex2f(360, 70);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(340, 60);
	glVertex2f(340, 80);
	glEnd();

	//round window
	glColor3f(0.729, 0.968, 0.952);
	circle(180, 120, 10);
	glColor3f(0, 0, 0);
	circle_wire(180, 120, 10);

	//door
	glColor3f(0.533, 0.219, 0.086);
	glBegin(GL_QUADS);
	glVertex2f(165, 0);
	glVertex2f(165, 60);
	glVertex2f(195, 60);
	glVertex2f(195, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(165, 0);
	glVertex2f(165, 60);
	glVertex2f(195, 60);
	glVertex2f(195, 0);
	glEnd();

	//house fence
	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_QUADS);
	glVertex2f(60, 40);
	glVertex2f(800, 40);
	glVertex2f(800, 25);
	glVertex2f(60, 25);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(60, 40);
	glVertex2f(800, 40);
	glVertex2f(800, 25);
	glVertex2f(60, 25);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(60, 0);
	glVertex2f(60, 40);
	glVertex2f(70, 60);
	glVertex2f(80, 40);
	glVertex2f(80, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(60, 0);
	glVertex2f(60, 40);
	glVertex2f(70, 60);
	glVertex2f(80, 40);
	glVertex2f(80, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(100, 0);
	glVertex2f(100, 40);
	glVertex2f(110, 60);
	glVertex2f(120, 40);
	glVertex2f(120, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(100, 0);
	glVertex2f(100, 40);
	glVertex2f(110, 60);
	glVertex2f(120, 40);
	glVertex2f(120, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(140, 0);
	glVertex2f(140, 40);
	glVertex2f(150, 60);
	glVertex2f(160, 40);
	glVertex2f(160, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(140, 0);
	glVertex2f(140, 40);
	glVertex2f(150, 60);
	glVertex2f(160, 40);
	glVertex2f(160, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(180, 0);
	glVertex2f(180, 40);
	glVertex2f(190, 60);
	glVertex2f(200, 40);
	glVertex2f(200, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(180, 0);
	glVertex2f(180, 40);
	glVertex2f(190, 60);
	glVertex2f(200, 40);
	glVertex2f(200, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(220, 0);
	glVertex2f(220, 40);
	glVertex2f(230, 60);
	glVertex2f(240, 40);
	glVertex2f(240, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(220, 0);
	glVertex2f(220, 40);
	glVertex2f(230, 60);
	glVertex2f(240, 40);
	glVertex2f(240, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(260, 0);
	glVertex2f(260, 40);
	glVertex2f(270, 60);
	glVertex2f(280, 40);
	glVertex2f(280, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(260, 0);
	glVertex2f(260, 40);
	glVertex2f(270, 60);
	glVertex2f(280, 40);
	glVertex2f(280, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(300, 0);
	glVertex2f(300, 40);
	glVertex2f(310, 60);
	glVertex2f(320, 40);
	glVertex2f(320, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(300, 0);
	glVertex2f(300, 40);
	glVertex2f(310, 60);
	glVertex2f(320, 40);
	glVertex2f(320, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(340, 0);
	glVertex2f(340, 40);
	glVertex2f(350, 60);
	glVertex2f(360, 40);
	glVertex2f(360, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(340, 0);
	glVertex2f(340, 40);
	glVertex2f(350, 60);
	glVertex2f(360, 40);
	glVertex2f(360, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(380, 0);
	glVertex2f(380, 40);
	glVertex2f(390, 60);
	glVertex2f(400, 40);
	glVertex2f(400, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(380, 0);
	glVertex2f(380, 40);
	glVertex2f(390, 60);
	glVertex2f(400, 40);
	glVertex2f(400, 0);
	glEnd();


	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(420, 0);
	glVertex2f(420, 40);
	glVertex2f(430, 60);
	glVertex2f(440, 40);
	glVertex2f(440, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(420, 0);
	glVertex2f(420, 40);
	glVertex2f(430, 60);
	glVertex2f(440, 40);
	glVertex2f(440, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(460, 0);
	glVertex2f(460, 40);
	glVertex2f(470, 60);
	glVertex2f(480, 40);
	glVertex2f(480, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(460, 0);
	glVertex2f(460, 40);
	glVertex2f(470, 60);
	glVertex2f(480, 40);
	glVertex2f(480, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(420, 0);
	glVertex2f(420, 40);
	glVertex2f(430, 60);
	glVertex2f(440, 40);
	glVertex2f(440, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(420, 0);
	glVertex2f(420, 40);
	glVertex2f(430, 60);
	glVertex2f(440, 40);
	glVertex2f(440, 0);
	glEnd();

	glColor3f(0.921, 0.803, 0.517);
	glBegin(GL_POLYGON);
	glVertex2f(500, 0);
	glVertex2f(500, 40);
	glVertex2f(510, 60);
	glVertex2f(520, 40);
	glVertex2f(520, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(500, 0);
	glVertex2f(500, 40);
	glVertex2f(510, 60);
	glVertex2f(520, 40);
	glVertex2f(520, 0);
	glEnd();

}


void HowToPlay()   //function that draws how to play gamestate
{
	glColor3f(0.701, 0.709, 0.878);
	glRectf(-800, 500, 800, -500);

	glColor3f(0.486, 0.333, 0.725);
	glRectf(-650, 350, 600, -250);

	glColor3f(0.835, 0.701, 0.623);
	glRectf(-600, 300, 550, -200);

	glColor3f(0, 0, 0);
	vprint(-300, 250, GLUT_BITMAP_HELVETICA_18, "HELP. The farmer has gotten crazy and has started shooting birdies");
	vprint(-360, 225, GLUT_BITMAP_HELVETICA_18, "The birdies need your help to save them. Rescue the birdies through you mathematical skills");

	glLineWidth(5);
	vprint2(-190, 100, 0.4, "HOW TO PLAY");
	glLineWidth(1);
	vprint(-490, 50, GLUT_BITMAP_HELVETICA_18, "For each equation given at the bottom, click on the correct answer. points will be added to your score if you get it correct.");
	vprint(-525, 25, GLUT_BITMAP_HELVETICA_18, "if you answer wrong or fail to give an answer, the angry farmer will shoot a birdie and those points will be added to the computer");
	vprint(-250, 0, GLUT_BITMAP_HELVETICA_18, "press p to pause the game, u to unpause");
	vprint(-370, -25, GLUT_BITMAP_HELVETICA_18, "Choose the level you want to play. Easy has the slowest computer rotator and Hard has the fastest.");
	vprint(-315, -50, GLUT_BITMAP_HELVETICA_18, "The game will end in 60 seconds, so try to collect as many points as you can!");

	glColor3f(0.380, 1, 0.258);
	glBegin(GL_QUADS);
	glVertex2f(-350, -75);
	glVertex2f(-200, -75);
	glVertex2f(-200, -125);
	glVertex2f(-350, -125);
	glEnd();
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-350, -75);
	glVertex2f(-200, -75);
	glVertex2f(-200, -125);
	glVertex2f(-350, -125);
	glEnd();
	vprint(-310, -110, GLUT_BITMAP_TIMES_ROMAN_24, "EASY");
	glLineWidth(1);

	glColor3f(0.250, 0.282, 0.905);
	glBegin(GL_QUADS);
	glVertex2f(-75, -75);
	glVertex2f(75, -75);
	glVertex2f(75, -125);
	glVertex2f(-75, -125);
	glEnd();
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-75, -75);
	glVertex2f(75, -75);
	glVertex2f(75, -125);
	glVertex2f(-75, -125);
	glEnd();
	vprint(-50, -110, GLUT_BITMAP_TIMES_ROMAN_24, "MEDIUM");
	glLineWidth(1);

	glColor3f(0.964, 0.074, 0.094);
	glBegin(GL_QUADS);
	glVertex2f(350, -75);
	glVertex2f(200, -75);
	glVertex2f(200, -125);
	glVertex2f(350, -125);
	glEnd();
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(350, -75);
	glVertex2f(200, -75);
	glVertex2f(200, -125);
	glVertex2f(350, -125);
	glEnd();
	vprint(240, -110, GLUT_BITMAP_TIMES_ROMAN_24, "HARD");
	glLineWidth(1);

}

void PauseScreen()    //function that makes pausecreen gamestate
{
	if (gameState == PAUSESCREEN)
	{
		glColor3f(0, 0, 0);
		glLineWidth(10);
		vprint2(-550, 5, 1.2, "GAME PAUSED");
		glLineWidth(1);
	}
}

void GameoverScreen()  //function that makes gameover gamestate
{
	if (gameState == GAMEOVER)
	{
		glColor3f(0, 0, 0);
		glLineWidth(10);
		vprint2(-495, 5, 1.2, "GAME OVER");
		glLineWidth(1);
		glLineWidth(5);
		vprint2(-320, -100, 0.6, "YOUR SCORE : %d", Pscore);
		vprint2(-440, -200, 0.6, "COMPUTER SCORE : %d", Cscore);
		glLineWidth(1);

	}
}


void display()
{
	glClearColor(0.572, 0.968, 0.968, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//switch statement to switch between gamestates
	switch (gameState) {
	case HOWTOPLAY: HowToPlay(); break;
	case GAME: drawBackground(),
		shuriken(),
		drawFarmer(),
		drawArrow();
		drawBird();
		break;
	case PAUSESCREEN: drawBackground(),
		PauseScreen(); break;
	case GAMEOVER: drawBackground(),
		GameoverScreen(); break;
	}
	glutSwapBuffers();
}

void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	//pause game when p is pressed and current gamestate is game
	if ((key == 'p' || key == 'P') && gameState == GAME)
		gameState = PAUSESCREEN;

	//unpause game when u is pressed and current gamestate is pause
	if ((key == 'u' || key == 'U') && gameState == PAUSESCREEN)
		gameState = GAME;

	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

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

void onClick(int button, int stat, int x, int y)
{
	if (gameState == HOWTOPLAY)
	{
		if (x > 175 && x < 325 && y > 425 && y < 475)  //selecting easy
		{
			level = EASY;
			gameState = GAME;
		}
	}

	if (gameState == HOWTOPLAY)
	{
		if (x > 450 && x < 600 && y > 425 && y < 475)
		{
			level = MEDIUM;   //selecting medium
			gameState = GAME;
		}
	}

	if (gameState == HOWTOPLAY)
	{
		if (x > 725 && x < 875 && y > 425 && y < 475)
		{
			level = HARD;   //selecting hard
			gameState = GAME;
		}
	}


	if (answerclick == false)
	{
		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && y > 575 && y < 630 && x > 525 && x < 605)
		{
			color = true;
			if (answerindex == 0)
			{
				r1 = 0;
				g1 = 1;
				b1 = 0;
				birdIdle = false;
				correctAnswer = true;
				Pscore = Pscore + point;
			}
			else
			{
				r1 = 1;
				g1 = 0;
				b1 = 0;
				correctAnswer = false;
			}

			answerclick = true;
		}

		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && y > 575 && y < 630 && x > 645 && x < 725)
		{
			color = true;
			if (answerindex == 1)
			{
				r2 = 0;
				g2 = 1;
				b2 = 0;
				birdIdle = false;
				correctAnswer = true;
				Pscore = Pscore + point;
			}
			else
			{
				r2 = 1;
				g2 = 0;
				b2 = 0;
				correctAnswer = false;
			}

			answerclick = true;
		}

		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && y > 575 && y < 630 && x > 765 && x < 845)
		{
			color = true;
			if (answerindex == 2)
			{
				r3 = 0;
				g3 = 1;
				b3 = 0;
				birdIdle = false;
				correctAnswer = true;
				Pscore = Pscore + point;
			}
			else
			{
				r3 = 1;
				g3 = 0;
				b3 = 0;
				correctAnswer = false;
			}

			answerclick = true;
		}

		if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && y > 575 && y < 630 && x > 885 && x < 960)
		{
			color = true;
			if (answerindex == 3)
			{
				r4 = 0;
				g4 = 1;
				b4 = 0;
				birdIdle = false;
				correctAnswer = true;
				Pscore = Pscore + point;
			}
			else
			{
				r4 = 1;
				g4 = 0;
				b4 = 0;
				correctAnswer = false;
			}

			answerclick = true;
		}
	}



	glutPostRedisplay();
}

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

	glutPostRedisplay();
}

void onMove(int x, int y) {

	//used to change color of options to blue when mouse hovers over it
	if (color == false) {
		if (y > 575 && y < 630 && x > 525 && x < 605)
		{
			r1 = 0;
			g1 = 0;
			b1 = 1;
		}
		else
		{
			r1 = 1;
			g1 = 1;
			b1 = 0;
		}

		if (y > 575 && y < 630 && x > 645 && x < 725)
		{
			r2 = 0;
			g2 = 0;
			b2 = 1;
		}
		else
		{
			r2 = 1;
			g2 = 1;
			b2 = 0;
		}

		if (y > 575 && y < 630 && x > 765 && x < 845)
		{
			r3 = 0;
			g3 = 0;
			b3 = 1;
		}
		else
		{
			r3 = 1;
			g3 = 1;
			b3 = 0;
		}

		if (y > 575 && y < 630 && x > 885 && x < 960)
		{
			r4 = 0;
			g4 = 0;
			b4 = 1;
		}
		else
		{
			r4 = 1;
			g4 = 1;
			b4 = 0;
		}
	}
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {


	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (gameState == GAME)
	{

		timer -= 7;  //decrementing by 7 because time period set to 700

		if (timer <= 0)
			gameState = GAMEOVER;

		if (birdIdle == false)
			yBird += 10;

		if (yBird > 360)
		{
			birdIdle = true;
			Equationfunction();
			xBird = (rand() % 1000) - 500;
			yBird = (rand() % 500) - 200;
			point = (rand() % 5) + 1;
			r = (rand() % 1000);
			g = (rand() % 1000);
			b = (rand() % 1000);

		}

		if (level == EASY)
		{
			if (xBird < 0)
				angle += speed;
			else
				angle -= speed;
		}


		if (level == MEDIUM)
		{
			speed = 3;
			if (xBird < 0)
				angle += speed;
			else
				angle -= speed;
		}


		if (level == HARD)
		{
			speed = 5;
			if (xBird < 0)
				angle += speed;
			else
				angle -= speed;
		}

		xA = 100 * cos(angle * D2R);
		yA = 100 * sin(angle * D2R);

		if ((angle <= bAngle + 5 && angle >= bAngle - 5) && correctAnswer == false)
		{

			movement = true;
			dirX = xA;
			dirY = yA;
		}


		if (angle >= 360)
		{
			angle = 0;
			xA = 100 * cos(angle * D2R);
			yA = 100 * sin(angle * D2R);
		}

		else if (angle <= 0)
		{
			angle = 360;
			xA = 100 * cos(angle * D2R);
			yA = 100 * sin(angle * D2R);
		}

		shurikenmovement();
	}


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

	glutCreateWindow("HW-3 'Save the Birdies!!' by Suheera Tanvir - 21801211");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);


	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);


	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
