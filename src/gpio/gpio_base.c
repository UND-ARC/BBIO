/*
 * Michael Turnbull
 * 20190327
 * Handles GPIO pin enable/disable stuff
 */

#include "gpio_base.h"


/*
 * Get the (gpio) pin direction, in or out.
 *
 * Arguments:
 *  char* pin : the pin name (e.g. "gpio66")
 *
 * Returns 1 if output, 0 if input.  -1 if error
 */
int get_pin_direction(char* pin) {
	FILE *fp;
	char* filepath = malloc(sizeof(char) * (strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_DIRECTION)));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_DIRECTION);

	// read file contents
	fp = fopen(filepath, "r");
	char contents[4];  // mode is only ever "in" or "out"
	if (fgets(contents, sizeof(contents), fp) == NULL) {
		// uh-oh
		printf("[!] failed to read gpio pin mode, errno=%d, pin=%s\n", errno, pin);
		return -1
	} else {
		// success!
		int 
		if (strcmp(contents, "in") == 0) {
			return 0;
		} else if (strcmp(contents, "out") == 0) {
			return 1;
		}
	}
	printf("[!] how the hell did I get here?? gpio_base.c:get_pin_mode");
	return -1
}


/*
 * Set the (gpio) pin mode to whatever it's supposed to be
 *
 * Arguments:
 *  char* pin : the pin name (e.g. "gpio66")
 *  int mode : 0 for input, 1 for output
 *
 * Returns 1 if successful, 0 if failed.
 */
int set_pin_direction(char* pin, int direction) {
	FILE *fp;
	char* filepath = malloc(sizeof(char) * (strlen(GPIO_PIN_DIR) + strlen(pin) + strlen(GPIO_PIN_DIRECTION)));

	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_DIRECTION);

	// First, check to see if we're already in the desired mode
	// Read file, check against argument.  Saves disk IO if not needed
	int current_dir = get_pin_direction(pin);
	if (curent_dir == -1) {
		printf("[!] unable to determine pin direction, goahead with overwrite\n");
	} else {
		if (direction == current_dir) {
			// no work to be done
			return 1
		}
	}

	// if we're here, then the mode needs to be set, the file is
	// closed, and char* contents has been free'd

	fp = fopen(filepath, "w");
	if (fp == NULL) {
		printf("[!] failed to open gpio direction file for writing, errno=%d, pin=%s\n", errno, pin);
		return 0;
	}

	// file open, write to it
	// first, decide what we're going to write
	char write_out[3] = "";
	if (mode == 0) {
		strcat(write_out, "in");
	} else if (mode == 1) {
		strcat(write_out, "out");
	}

	// write it
	int res = fputs(write_out, fp);
	if (res == EOF) {
		printf("[!] failed to write to gpio direction file, errno=%d, pin=%s\n", errno, pin);
		return 0;
	}

	// wrote it, done
	return 1;
}




/*
 * Tests:
 * Try to set GPIO66 to output, and turn it on
 */
int main(void) {
	set_pin_mode("gpio66", 1);
}
