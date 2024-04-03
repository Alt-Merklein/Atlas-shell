
#include "parse.h"
#include <unistd.h>
#include <stdlib.h>

char** inputStringToArgv(char* input, int inputSize) {    
    // First: allocate memory to store each argument. 
    // Number of char* allocated = number of spaces - 1. (Ignore repeated spaces and spaces at the end of command) 
    // We implement a FSM for input parsing

    int i = 0, j = 0, k = 0;
    ParsingState state = START;
    char cur;
    int startIndex = 0, endIndex = 0;
    int numArgs = countArgNumber(input, inputSize);

    char** argVector = (char**) malloc((numArgs + 1) * sizeof(char*)); //+1 becuase we need a NULL at the end
    argVector[numArgs] = NULL;

    while (i < inputSize) {
        cur = input[i];
        switch (state) {
            case START:
                if (cur != ' ' && cur != '\n'){
                    startIndex = i; //start of the next argv separated by space to be allocated
                    state = READ_ARG;
                }

                i++;
                break;

            case READ_ARG:
                if (cur == ' ' || cur == '\n') {
                  endIndex = i - 1; //last one was a character
                  state = ALLOCATE;  
                } 
                else i++;
                break;

            case ALLOCATE:
                argVector[j] = (char *) malloc((2 + endIndex - startIndex) * sizeof(char)); //allocate memory for one of the args
                for (k = 0; k < endIndex - startIndex + 1; k++) {
                    argVector[j][k] = input[startIndex + k];
                }
                argVector[j][k] = '\0'; //terminate string with 0
                j++;
                i++;
                state = WAIT_FOR_ARG;
                break;

            case WAIT_FOR_ARG:
                if (cur != ' ' && cur != '\n') {
                    startIndex = i;
                    state = READ_ARG;
                }
                i++;
                break;

            default:
                break;
        }
    }
    return argVector;
}

int countArgNumber(char* input, int size) {
    int i = 0, count = 0;
    int insideArg = 0; //flag

    for (i = 0; i < size; i++) {
        if (input[i] != ' ' && input[i] != '\n' && !insideArg) {
            count++;
            insideArg = 1;
        }

        if (input[i] == ' ' && insideArg) {
            insideArg = 0;
        }
    }

    return count;
}