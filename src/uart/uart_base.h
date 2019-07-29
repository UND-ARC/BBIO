/*
 * Michael Turnbull
 * 20190725
 * UART interface C <-> kernel driver
 */

#ifndef LIBRARY_BASICS
 	#include <stdio.h>
 	#include <stdlib.h>
 	#include <errno.h>
 	#include <string.h>
 	#include <unistd.h>
 	#define LIBRARY_BASICS
#endif

#ifndef TERM_IO
  #include <termios.h>
  #define TERM_IO
#endif

#ifndef DEVICE_DIR
  #define DEVICE_DIR "/dev/"
#endif

int does_exist(char* device);

int get_baudrate(char* device);
int set_baudrate(char* device, int baud);

int write_to(char* device, char* message);
char* read_from(char* device);
