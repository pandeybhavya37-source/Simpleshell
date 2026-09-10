#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int seconds = atoi(argv[1]);
    sleep(seconds);
    printf("Sleepy has woken up after %d seconds!\n", seconds);
    return 0;
}
