#ifndef SENSE_HAT_H_   /* Include guard */
#define SENSE_HAT_H_

typedef struct {
    int file_desc;
    uint16_t *map;
} SenseHat;

SenseHat* get_sense_hat_pointer(char* address);

void print_sense_hat(SenseHat* sense_hat);

void fill_sense_hat(SenseHat* sense_hat, uint16_t color);

void close_sense_hat(SenseHat* to_be_closed);

#endif // FOO_H_
