#********************************************
# CMPUT 201 ASSIGNMENT 3                    *
# NAME: JANES, JOSHUA                       *
# STUDENT ID: 1297283                       *
# CCID: jjanes                              *
# LAB: D04                                  *
# TA; GAO, CHAO                             *
# INSTRUCTOR: BURCH, NEIL                   *
# DESCRIPTION: lander recreate the 1979     *
# Atari arcade game Lunar Lander.           *
#********************************************/

lander: lander.o gameloop.o
	gcc -Wall -std=c99 -o lander lander.o gameloop.o -lm -lncurses

lander.o: lander.c lander.h
	gcc -Wall -std=c99 -c lander.c -g

gameloop.o: gameloop.c gameloop.h
	gcc -Wall -std=c99 -c gameloop.c -g

clean:
	-rm -f *.o lander core

tar:
	tar cvf lander.tar lander.c lander.h gameloop.c gameloop.h Makefile README
