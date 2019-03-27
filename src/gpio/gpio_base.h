/*
 * Michael Turnbull
 * 20190327
 * GPIO pin usage headers
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef GPIO_PIN_DIR
	#define GPIO_PIN_DIR "/sys/class/gpio/"
	#define GPIO_PIN_DIRECTION "/direction"
#endif
