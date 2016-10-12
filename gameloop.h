/*********************************************
 * CMPUT 201 ASSIGNMENT 3                    *
 * NAME: JANES, JOSHUA                       *
 * STUDENT ID: 1297283                       *
 * CCID: jjanes                              *
 * LAB: D04                                  *
 * TA; GAO, CHAO                             *
 * INSTRUCTOR: BURCH, NEIL                   *
 * DESCRIPTION: lander recreates the 1979    *
 * Atari arcade game Lunar Lander.           *
 ********************************************/

#define _XOPEN_SOURCE
#define YES		1
#define NO		0
#define X_START 	320
#define Y_START 	12
#define THRUST 		6
#define NO_THRUST 	4
#define THRUST_I        19
#define NO_THRUST_I     17
#define MAX_BUF 	256
#define PI              3.14159265
#define CRASH_SPEED     20

#include <stdio.h>
#include <string.h>             
#include <stdlib.h>             
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <assert.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/time.h>

typedef struct {
  double pos;
  double vel;
  double acc;
} PosVelocity;

typedef struct {
  PosVelocity x;
  PosVelocity y;
  double rotation;
  double gravity;
  double thrust;
} State;

State g_state;

/* these global variables were defined in lander.c */
extern double l_x1[20];
extern double l_y1[20];
extern double l_x2[20];
extern double l_y2[20];
extern int l_size;
extern int improved;

int no_thrust, crashed;
FILE * sketchpad;

double x1_trans[20], y1_trans[20], x2_trans[20], y2_trans[20];

/* line coordinates of the ship drawing */
double s_x1[6] = {310, 315, 325, 330, 310, 320};
double s_y1[6] = {20, 0, 0, 20, 20, 25};
double s_x2[6] = {315, 325, 330, 310, 320, 330};
double s_y2[6] = {0, 0, 20, 20, 25, 20};

/* improved ship drawing */
double si_x1[19] = {310, 312, 312, 326, 328, 328, 310, 330, 330, 310, 314, 310, 
		 310, 315, 325, 330, 330, 317, 320};
double si_y1[19] = {25, 25, 21, 25, 25, 21, 18, 18, 13, 13, 12, 8, 5, 0, 0, 5, 
		 8, 19, 25};
double si_x2[19] = {314, 312, 314, 330, 328, 326, 330, 330, 310, 310, 310, 310, 
		 315, 325, 330, 330, 326, 320, 323};
double si_y2[19] = {25, 21, 19, 25, 21, 19, 18, 13, 13, 18, 8, 5, 0, 0, 5, 8, 
		 12, 25, 19};

/* crashed ship drawing */
double cs_x1[19] = {168, 168, 172, 217, 225, 232, 234, 233, 224, 218, 216, 285,
		    297, 301, 289, 397, 399, 395, 0}; 
double cs_y1[19] = {425, 428, 432, 433, 426, 428, 430, 436, 442, 442, 439, 433,
		    448, 446, 429, 436, 433, 443, 0};
double cs_x2[19] = {168, 172, 174, 225, 232, 234, 233, 224, 218, 216, 217, 297,
		    301, 289, 285, 399, 402, 398, 0};
double cs_y2[19] = {428, 430, 428, 426, 428, 430, 436, 442, 442, 439, 433, 448,
		    446, 429, 433, 433, 433, 438, 0};

extern FILE *popen(const char *command, const char *modes);
extern int pclose(FILE *stream);
int drawShip();
void drawLandscape();
int gameLoop(double gravity, double thrust);
bool detectCollision(double Ax, double Ay, double Bx, double By,
		     double Cx, double Cy, double Dx, double Dy);
void handle_signal(int signal);
void shutdown_ncurses();
void init_ncurses();
