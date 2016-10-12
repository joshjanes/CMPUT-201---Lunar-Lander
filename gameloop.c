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

#include "gameloop.h"

int gameLoop(double gravity, double thrust){
  int c, running = 1;
  crashed = 0;
  struct sigaction handler;
  struct itimerval timer;
  if (improved){
    no_thrust = NO_THRUST_I;
  }
  else{
    no_thrust = NO_THRUST;
  }

  /* opens sketchpad */
  sketchpad = popen("java -jar Sketchpad.jar", "w");
  drawLandscape();

  if (improved){
    fprintf(sketchpad, "setColor 255 0 0\n");
  }

  init_ncurses();

  mvprintw(5, 10, "Press any key to start.");
  mvprintw(6, 10, 
	 "(Then press arrow keys to rotate, space for thrust, 'q' to quit.)\n");
  refresh(); // printed output doesn't appear until we refresh
  c = getch();
  erase();

  nodelay(stdscr, TRUE); // have getch() return ERR if input isn't ready

  // set up signal handler for alarm
  handler.sa_handler = handle_signal;
  sigemptyset( &handler.sa_mask );
  handler.sa_flags = 0;
  if( sigaction( SIGALRM, &handler, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }

  // set up alarm timer
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 50000;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 50000;
  if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 ) {
    exit( EXIT_FAILURE );
  }

  /* initializes values */
  g_state.x.pos = X_START;
  g_state.x.vel = 0;
  g_state.y.pos = Y_START;
  g_state.y.vel = 0;
  g_state.y.acc = gravity;
  g_state.rotation = PI/2;
  g_state.gravity = gravity;
  g_state.thrust = thrust;

  drawShip();
  mvprintw(5, 0, "left arrow key rotates counter-clockwise, right clockwise, "
	   "space for thrust, q to quit.");

  char filename[MAX_BUF];
  memset(filename, '\0', MAX_BUF);

  do {
    c = getch();
    if( c == ERR ) { continue; } // keep looping until we get input

    switch( c ) {
    case KEY_LEFT:
      g_state.rotation += 0.1;
      break;

    case KEY_RIGHT:
      g_state.rotation -= 0.1;
      break;

    case ' ':
      g_state.x.acc = g_state.thrust * cos(g_state.rotation); 
      g_state.y.acc = (g_state.gravity + g_state.thrust) * 
	sin(g_state.rotation);
      if (improved){
	no_thrust = THRUST_I;
      }
      else{
	no_thrust = THRUST;
      }
      break;

    case 'q':
      running = 0;
      break;

    default:
      // do nothing
      break;
    }
    refresh();
    /* reset thrust value */
  } while( running );

  pclose(sketchpad);

  // must do this or else the terminal will be unusable
  shutdown_ncurses();

  exit(EXIT_SUCCESS);
}

/* rotates the ship coordinates and then draws the ship, with or without the 
   jet engines on, depending on the value of thrust*/
int drawShip(){
  double xstart=(double)X_START, ystart=(double)Y_START;
  int i = 0;
  double tempx1[20], tempy1[20], tempx2[20], tempy2[20];

  for(i=0; i<no_thrust; i++){
    /* rotates and translates the points of the spaceship */
    if (improved){
      tempx1[i] = ( ( (si_x1[i]-xstart) * sin(g_state.rotation) - 
		      (si_y1[i]-ystart) * cos(g_state.rotation) )
		    + g_state.x.pos);
      tempy1[i] = ( ( (si_x1[i]-xstart) * cos(g_state.rotation) + 
		      (si_y1[i]-ystart) * sin(g_state.rotation) )
		    + g_state.y.pos);
      tempx2[i] = ( ( (si_x2[i]-xstart) * sin(g_state.rotation) - 
		      (si_y2[i]-ystart) * cos(g_state.rotation) )
		    + g_state.x.pos);
      tempy2[i] = ( ( (si_x2[i]-xstart) * cos(g_state.rotation) + 
		      (si_y2[i]-ystart) * sin(g_state.rotation) )
		    + g_state.y.pos);
    }
    else{
      tempx1[i] = ( ( (s_x1[i]-xstart) * sin(g_state.rotation) - 
		      (s_y1[i]-ystart) * cos(g_state.rotation) ) 
		    + g_state.x.pos);
      tempy1[i] = ( ( (s_x1[i]-xstart) * cos(g_state.rotation) + 
		      (s_y1[i]-ystart) * sin(g_state.rotation) ) 
		    + g_state.y.pos);
      tempx2[i] = ( ( (s_x2[i]-xstart) * sin(g_state.rotation) - 
		      (s_y2[i]-ystart) * cos(g_state.rotation) ) 
		    + g_state.x.pos);
      tempy2[i] = ( ( (s_x2[i]-xstart) * cos(g_state.rotation) + 
		      (s_y2[i]-ystart) * sin(g_state.rotation) ) 
		    + g_state.y.pos);
    }
  }

  if (improved && crashed){
    /* draws crashed ship */
    for(i=0; i<THRUST_I; i++){
      fprintf(sketchpad, "eraseSegment %.0lf %0.lf %.0lf %.0lf\n", 
	      x1_trans[i], y1_trans[i], x2_trans[i], y2_trans[i]);
      fprintf(sketchpad, "drawSegment %.0lf %0.lf %.0lf %.0lf\n", 
	      cs_x1[i], cs_y1[i], cs_x2[i], cs_y2[i]);
    }
    return 0;
  }
  else{
    for(i=0; i<no_thrust; i++){
      fprintf(sketchpad, "eraseSegment %.0lf %0.lf %.0lf %.0lf\n", 
	      x1_trans[i], y1_trans[i], x2_trans[i], y2_trans[i]);
      fprintf(sketchpad, "drawSegment %.0lf %0.lf %.0lf %.0lf\n", 
	      tempx1[i], tempy1[i], tempx2[i], tempy2[i]);
      x1_trans[i] = tempx1[i];
      y1_trans[i] = tempy1[i];
      x2_trans[i] = tempx2[i];
      y2_trans[i] = tempy2[i];
    }
  }
  fflush(sketchpad);
  /* detects possible collisions between line segments */
  for (int j=0; j<l_size; j++){
    if (improved){
      for (i=0; i<THRUST_I; i++){
	if(detectCollision(l_x1[j], l_y1[j], l_x2[j], l_y2[j], 
			   x1_trans[i], y1_trans[i], x2_trans[i],
			   y2_trans[i])){
	  return 0;
	}
      }
    }
    else{
      for (i=0; i<THRUST; i++){
	if(detectCollision(l_x1[j], l_y1[j], l_x2[j], l_y2[j], 
			   x1_trans[i], y1_trans[i], 
			   x2_trans[i], y2_trans[i])){
	  return 0;
	}
      }
    }
  }
    
  /* erases the jet trail */
    if (improved){
      if( no_thrust < THRUST_I ){
	fprintf(sketchpad, 
		"eraseSegment %.0lf %0.lf %.0lf %.0lf\n", 
		x1_trans[THRUST_I-1], y1_trans[THRUST_I-1], 
		x2_trans[THRUST_I-1], y2_trans[THRUST_I-1]);
	fprintf(sketchpad, 
		"eraseSegment %.0lf %0.lf %.0lf %.0lf\n", 
		x1_trans[THRUST_I-2], y1_trans[THRUST_I-2], 
		x2_trans[THRUST_I-2], y2_trans[THRUST_I-2]);
	fflush(sketchpad);
      }
    }
    else{
      if( no_thrust < THRUST ){
	fprintf(sketchpad, 
		"eraseSegment %.0lf %0.lf %.0lf %.0lf\n", 
		x1_trans[THRUST-1], y1_trans[THRUST-1], 
		x2_trans[THRUST-1], y2_trans[THRUST-1]);
	fprintf(sketchpad, 
		"eraseSegment %.0lf %0.lf %.0lf %.0lf\n", 
		x1_trans[THRUST-2], y1_trans[THRUST-2], 
		x2_trans[THRUST-2], y2_trans[THRUST-2]);
	fflush(sketchpad);
      }
    }
    return 1;
}

/* draws the landscape defined by the input file */
void drawLandscape(){
  for (int i=0; i<(l_size+1); i++){
    fprintf(sketchpad, "drawSegment %.0lf %0.lf %.0lf %.0lf\n",
	    l_x1[i], l_y1[i], l_x2[i], l_y2[i]);
  }
}

bool detectCollision(double Ax, double Ay, double Bx, double By,
		     double Cx, double Cy, double Dx, double Dy) {
  double  distAB, theCos, theSin, newX, ABpos;
  if ( (Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy) ) return NO;
  if ( (Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy) || 
       (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy) ) {
    return NO; }
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;
  distAB=sqrt(Bx*Bx+By*By);
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;
  if ( (Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.) ) return NO;
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);
  if ((ABpos<0) || (ABpos>distAB) ) return NO;
  return YES;
}

void init_ncurses()
{
  int r;

  initscr();
  r = cbreak(); assert( r != ERR );
  r = noecho(); assert( r != ERR );
  r = nonl(); assert( r != ERR );
  r = curs_set( 0 ); assert( r != ERR );
  r = leaveok( stdscr, TRUE ); assert( r != ERR );
  r = intrflush( stdscr, FALSE ); assert( r != ERR );
  r = keypad( stdscr, TRUE ); assert( r != ERR );
}

void shutdown_ncurses()
{
  endwin();
}

/* handles the alarm signal and re-draws */
void handle_signal( int signal )
{
  char ch = 0;
  if( signal != SIGALRM ) { return; }

  g_state.y.pos +=  g_state.y.vel * 0.05 + 0.5 * g_state.y.acc * 0.05 * 0.05;
  g_state.y.vel += g_state.y.acc * 0.05;

  g_state.x.pos -= g_state.x.vel * 0.05 + 0.5 * g_state.x.acc * 0.05 * 0.05; 
  g_state.x.vel += g_state.x.acc * 0.05;

  /* resets the accelerations (assumes the thrusters are turned off) */
  g_state.y.acc = g_state.gravity;
  g_state.x.acc = 0;

  if(drawShip()){
    if(improved){
      no_thrust = NO_THRUST_I;
    }      
    else{
      no_thrust = NO_THRUST;
    }
    refresh();
  }
  else{
    if(g_state.x.vel <= CRASH_SPEED && g_state.y.vel <= CRASH_SPEED){
      printf("\n\tLANDED!!!\n");
    }
    else{
      crashed = 1;
      drawShip();
      fflush(sketchpad);
      printf("\n\tCRASHED!!!\n");
    }

    while (ch != 'q'){
      ch = getch();
    }
    fprintf(sketchpad, "end");
    pclose(sketchpad);
    shutdown_ncurses();
    exit(EXIT_FAILURE);
    /* do nothing until q is pressed */
  }
  if(improved){
    /* gives a notification and ends the game if the lander goes offscreen */
    if(g_state.x.pos <= -15 || g_state.x.pos >= 655 || g_state.y.pos <= -15){
      printf("\n\tLOST CONTACT WITH LANDER!!!\n");
      while (ch != 'q'){
	ch = getch();
      }
      fprintf(sketchpad, "end");
      pclose(sketchpad);
      shutdown_ncurses();
      exit(EXIT_FAILURE);
    }
  }
  else{
    /* user gets stuch if not playing the improved version */
    if(g_state.x.pos <= 0 || g_state.x.pos >= 640 || g_state.y.pos <= 0){
      /* freeze the game until q is entered */
      while (ch != 'q'){
	ch = getch();
      }
      fprintf(sketchpad, "end");
      pclose(sketchpad);
      shutdown_ncurses();
      exit(EXIT_FAILURE);
    }
  }
}
