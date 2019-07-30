/*
 * Michael Turnbull
 * 20190729
 * Handles UART interfacing/messaging
 */

#include "uart_base.h"

struct {
  int rawrate;
  int termiosrate;
} conversiontable[] = {
  {0, B0},          {50, B50},        {75, B75},
  {110, B110},      {134, B134},      {150, B150},
  {200, B200},      {300, B300},      {600, B600},
  {1200, B1200},    {1800, B1800},    {2400, B2400},
  {4800, B4800},    {9600, B9600},    {19200, B19200},
  {38400, B38400}
};

int convert_raw_to_termios(int rawrate) {
  for (int i = 0; i < sizeof(conversiontable)/sizeof(conversiontable[0]); i++) {
    if (conversiontable[i].rawrate == rawrate) {
      return conversiontable[i].termiosrate;
    }
  }
  return -1;
}

int convert_termios_to_raw(int termiosrate) {
  for (int i = 0; i < sizeof(conversiontable)/sizeof(conversiontable[0]); i++) {
    if (conversiontable[i].termiosrate == termiosrate) {
      return conversiontable[i].rawrate;
    }
  }
  return -1;
}

/*
 * Determine whether or not a serial device exists.
 *
 * Arguments:
 *  char* device: the device name (e.g. "ttyS0")
 *
 * Returns 1 if exists, 0 if not.  -1 if error
 */
int does_exist(char* device) {
  char* filepath = malloc(sizeof(char) * (strlen(DEVICE_DIR) + strlen(device) + 3));
  strcpy(filepath, DEVICE_DIR);
  strcat(filepath, device);

  // access is from <unistd.h>, checks for file existance :)
  if (access(filepath, F_OK) != -1) {
    // file exists
    return 1;
  } else {
    // file does not exist
    return 0;
  }
}


/*
 * Get the baud rate on a particular serial device.
 *
 * Arguments:
 *  char* device : the device name (e.g. "ttyS0")
 *
 * Returns an integer represting the baud rate or -1 in error
 */
int get_baudrate(char* device) {
  char* filepath = malloc(sizeof(char) * (strlen(DEVICE_DIR) + strlen(device) + 3));
  strcpy(filepath, DEVICE_DIR);
  strcat(filepath, device);

  struct termios *attribs;
  speed_t speed_in, speed_out;

  // open device in read/write mode, non-controlling terminal, with no non-blocking IO
  dev_fd = open(filepath, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    printf("[!] get_baudrate failed to open port %s for testing", filepath);
    return -1;
  }

  tcgetattr(dev_fd, &attribs);
  speed_in = cfgetispeed(*attribs);
  speed_out = cfgetospeed(*attribs);

  if (speed_in != speed_out) {
    print("[!] out-of-sync input/output baud rates for device %s", filepath);
    return -1
  }

  int actual_baud = convert_termios_to_raw(speed_in);
  return actual_baud;

  close(dev_fd);

}
