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

#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

#define RGB565_RED 0xF800

uint8_t has_been_initialized = 0;
SenseHat g_sense_hat;

void delay(int t) {
    usleep(t * 1000);
}

SenseHat* get_sense_hat_pointer(char* address) {
  if (!has_been_initialized) {
      int fbfd;
      uint16_t *map;
      struct fb_fix_screeninfo fix_info;

      /* open the led frame buffer device */
      fbfd = open(address, O_RDWR);
      if (fbfd == -1) {
          perror("Error (call to 'open')");
          exit(EXIT_FAILURE);
      }

      /* read fixed screen info for the open device */
      if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fix_info) == -1) {
          perror("Error (call to 'ioctl')");
          close(fbfd);
          exit(EXIT_FAILURE);
      }

      /* now check the correct device has been found */
      if (strcmp(fix_info.id, "RPi-Sense FB") != 0) {
          printf("%s\n", "Error: RPi-Sense FB not found");
          close(fbfd);
          exit(EXIT_FAILURE);
      }

      /* map the led frame buffer device into memory */
      map =
          mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
      if (map == MAP_FAILED) {
          close(fbfd);
          perror("Error mmapping the file");
          exit(EXIT_FAILURE);
      }

          /* light it up! */
      for (int i = 0; i < NUM_WORDS; i++) {
          printf("map2 : %x\n", map);
          *(map + i) = 0xF;
          delay(25);
      }

      g_sense_hat = (SenseHat) { .map = map, .file_desc = fbfd };
      has_been_initialized = 1;
      clear_sense_hat(&g_sense_hat);
  }
  memset(g_sense_hat.map, 0, FILESIZE);

  print_sense_hat(&g_sense_hat);
  return &g_sense_hat;
}

void print_sense_hat(SenseHat* sense_hat) {
  printf("SenseHat(map: 0x%x, file_desc: %d)\n", sense_hat->map, sense_hat->file_desc);
}

void clear_sense_hat(SenseHat* to_be_cleared) {
  printf("Clearing sense hat\n");
  memset(to_be_cleared->map, 0, FILESIZE);
  delay(20);
}

void close_sense_hat(SenseHat* to_be_closed) {
   /* un-map and close */
    printf("Closing sense hat\n");
    if (munmap(to_be_closed->map, FILESIZE) == -1) {
        perror("Error un-mmapping the file");
    }
    close(to_be_closed->file_desc);
    to_be_closed = 0;
}
