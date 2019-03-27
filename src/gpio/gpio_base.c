/*
 * Michael Turnbull
 * 20190327
 * Handles GPIO pin enable/disable stuff
 */

#include "gpio_base.h"

/*
 * Set the (gpio) pin mode to whatever it's supposed to be
 *
 * Arguments:
 *  char* pin : the pin name (e.g. "gpio66")
 *  int mode : 0 for input, 1 for output
 *
 * Returns 1 if successful, 0 if failed.
 */
int set_pin_mode(char* pin, int mode) {
	FILE *fp;
	char filepath[1024] = NULL;  // init with plenty of space
			      // TODO: dynamic allocation to save RAM
	strcpy(filepath, GPIO_PIN_DIR);
	strcat(filepath, pin);
	strcat(filepath, GPIO_PIN_DIRECTION);

	// First, check to see if we're already in the desired mode
	// Read file, check against argument.  Saves disk IO if not needed
	fp = fopen(filepath, "r");
	if (fp == NULL) {
		printf("[!] failed to read gpio pin mode, errno=%d, pin=%s", errno, pin);
		return 0;
	} else {
		// successfully opened file
		char contents[10] = NULL;
		char ch = NULL;
		while ( (ch = fgetc(fp)) != EOF ) {
			strcat(contents, ch);
		}
		if (mode == 0) {
			if (strcmp(contents, "in") == 0) {
				// Done!  Mode is already input
				return 1;
			} else {
				fclose(fp);
				free(contents);
			}
		} else if (mode == 1) {
			if (strcmp(contents, "out") == 0) {
				// Done! Mode is already output
				return 1;
			} else {
				fclose(fp);
				free(contents);
			}
		}
	}

	// if we're here, then the mode needs to be set, the file is
	// closed, and char* contents has been free'd

	fp = fopen(filepath, "w");
	if (fp == NULL) {
		printf("[!] failed to open gpio direction file for writing, errno=%d, pin=%s", errno, pin);
		return 0;
	}

	// file open, write to it
	// first, decide what we're going to write
	char write_out[3] = NULL;
	if (mode == 0) {
		strcat(write_out, "in");
	} else if (mode == 1) {
		strcat(write_out, "out");
	}

	// write it
	int res = fputs(write_out, fp);
	if (res == EOF) {
		printf("[!] failed to write to gpio direction file, errno=%d, pin=%s", errno, pin);
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
