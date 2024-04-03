#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define HELLO "Enter a message: "

int main() {

    char buffer[1024] = {0};
    read(0, buffer, 1023);
    write(1, buffer, strlen(buffer));
    return 0;

}