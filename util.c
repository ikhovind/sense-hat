#include <unistd.h>

void delay(int t) {
    usleep(t * 1000);
}
