#include <unistd.h>
#include "errors.h"
#include "eval.h"

#define STDOUT 1
#define STDIN  0

#define HELLO_MESSAGE "Welcome to the Atlas shell :)\nA very powerful tool for interacting with your OS!\n\n"
#define CMD_MSG "Atlash-1.0> "

#define MAX_CMD_LENGTH 1024

// This is the main loop of the shell

int main(int argc, char* argv[]) {
    char hello[] = HELLO_MESSAGE;
    char cmdMsg[] = CMD_MSG;
    char input[MAX_CMD_LENGTH]; 
    int result;
    char* cmdArgv[] = {"/usr/bin/ls", "-l", NULL};

    ssize_t inputSize = 0;

    write(STDOUT, hello, sizeof(hello));
    while (1) {

        write(STDOUT, cmdMsg, sizeof(cmdMsg));

        inputSize = read(STDIN, input, MAX_CMD_LENGTH);
        if (inputSize == -1) {
            handleError(ERROR_READ_INPUT);
        }
        evaluate(input, inputSize);
        
    }
    return 0;
}