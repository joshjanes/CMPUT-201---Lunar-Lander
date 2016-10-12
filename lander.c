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

#include "lander.h"

int main(int argc, char * argv[])
{
  FILE * fp;
  char filename[MAX_BUF];
  memset(filename, '\0', MAX_BUF);
  
  bool = errorCheck(&argc, argv, filename);
  
  l_size = 0;
  memset(l_x1, 0, LSIZE);
  memset(l_y1, 0, LSIZE);
  memset(l_x2, 0, LSIZE);
  memset(l_y2, 0, LSIZE);
  
  /* open specified file */
  fp = fopen(filename, "r");
  if(fp == NULL)
    {
      printf("could not open %s\n", filename);
      /* frees the memory that was in use and exits */
      exit(EXIT_FAILURE);
    }
  else
    {
      processFile(fp);
      gameLoop(gravityvalue, thrustvalue);
      fclose(fp);
    }
  exit(EXIT_SUCCESS);
}

/* goes through each line of file and adds it to the landscape coordinates */
void processFile(FILE * fp)
{
  int rval;
  rval = fscanf(fp, "%lf %lf\n", &l_x1[l_size], &l_y1[l_size]);

  if (rval != 2)
    {
      printf("Error in input file\n");
      exit(EXIT_FAILURE);
    }
  
  rval = fscanf(fp, "%lf %lf\n", &l_x2[l_size], &l_y2[l_size]);
  /* goes through the lines of landscape.txt adds the values to the landscape
     variables */
  while(!feof(fp))
    {
      l_size += 1;
      rval = fscanf(fp, "%lf %lf\n", &l_x2[l_size], &l_y2[l_size]);
      l_x1[l_size] = l_x2[l_size-1];
      l_y1[l_size] = l_y2[l_size-1];
    }
  return;
} 

int errorCheck(int * argc, char * argv[], char * filename){
   /* error checking input */
  int argg = 0, argt = 0, argf = 0, argi = 0, i;
  double tempdouble;
  
  /* checks to see if the necessary arguments are present */
  for (i = 1; i<*argc; i++){
    if (strcmp(argv[i], GRAVITY_ARG) == 0){
      argg = 1;
    }
    
    if (strcmp(argv[i], THRUST_ARG) == 0){
      argt = 1;
    }
    
    if (strcmp(argv[i], FILE_ARG) == 0){
      argf = 1;
    }
    
    if (strcmp(argv[i], IMPROVED_ARG) == 0){
      argi = 1;
    }
  }
  if (argg != 1 || argt != 1 || argf != 1){
    printf("need flags -g gravity, -f landscape.txt, -t thrust\n");
    exit(EXIT_SUCCESS);
    return 0;
  }
  if (argi == 1){
    improved = 1;
  }

  /* checks to see if the values after the arguments are present */
  for (i = 1; i<*argc; i++){
    if (strcmp(argv[i], GRAVITY_ARG) == 0){
      if (i != (*argc-1) ) {
	i++;
	tempdouble = atof(argv[i]);
	if ( (strcmp(argv[i], "0") != 0) && (tempdouble == 0) ){
	  printf("bad '%s', or good option but no value.\n", argv[i]);
   	  exit(EXIT_SUCCESS);
	}
	gravityvalue = tempdouble;
      }
      else{
	printf("bad '%s', or good option but no value.\n", argv[i]);
	exit(EXIT_SUCCESS);
      } 
    }
    
    if (strcmp(argv[i], THRUST_ARG) == 0){
      if (i != (*argc-1) ) {
	i++;
	tempdouble = atof(argv[i]);
	if ( (strcmp(argv[i], "0") != 0) && (tempdouble == 0) ){
	  printf("bad '%s', or good option but no value.\n", argv[i]);
   	  exit(EXIT_SUCCESS);
	}
	if (tempdouble > 0 || tempdouble < -20){
	  printf("thrust > 0, < -20 not allowed\n");
   	  exit(EXIT_SUCCESS);
	}
        thrustvalue = tempdouble;
      }
      else{
	printf("bad '%s', or good option but no value.\n", argv[i]);
	exit(EXIT_SUCCESS);
      } 
    }
    
    if (strcmp(argv[i], FILE_ARG) == 0){
      if (i != (*argc-1) ) {
	i++;
        strcpy(filename, argv[i]);
      }
      else{
	printf("bad '%s', or good option but no value.\n", argv[i]);
	exit(EXIT_SUCCESS);
      } 
    }
  }
  return 0;
}
  

