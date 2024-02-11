/*
 *  C code to demonstrate control of the LED matrix for the
 *  Raspberry Pi Sense HAT add-on board.
 *
 *  Uses the mmap method to map the led device into memory
 *
 *  Build with:  gcc -Wall -O2 led_matrix.c -o led_matrix
 *               or just 'make'
 *
 *  Tested with:  Sense HAT v1.0 / Raspberry Pi 3 B+ / Raspbian GNU/Linux 10 (buster)
 *
 */

#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sense_hat.h"
#include "util.h"

#define FILEPATH "/dev/fb0"
#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

#define RGB565_RED 0xF800

int main(void) {
  SenseHat* a = get_sense_hat_pointer(FILEPATH);
  printf("return sense hat: %x\n", a->map);
  print_sense_hat(a);
  fill_sense_hat(a, RGB565_RED);
  delay(1000);
  fill_sense_hat(a, 0);
  close_sense_hat(a); //printf("SenseHat(%x, %x)", a->file_desc, a->map);
}
