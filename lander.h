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

#include <stdio.h>
#include <string.h>             
#include <stdlib.h>             
#include <math.h>
#include <assert.h>

#define LSIZE 20
#define GRAVITY_ARG "-g"
#define FILE_ARG "-f"
#define THRUST_ARG "-t"
#define IMPROVED_ARG "-i"
#define MAX_BUF 256


int improved;

double thrustvalue, gravityvalue;

/* coordinates for the landscape will be stored in these */
double l_x1[20], l_y1[20], l_x2[20], l_y2[20];
/* landscape size */
int l_size;
int bool = 1;

extern FILE *popen( const char *command, const char *modes);
extern int pclose(FILE *stream);
void processFile(FILE * fp);
int errorCheck(int * argc, char * argv[], char * filename);
int gameLoop(double gravity, double thrust);
int gameLoopi(double gravity, double thrust);
