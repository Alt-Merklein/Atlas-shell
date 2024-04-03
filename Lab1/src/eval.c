#include "errors.h"
#include "parse.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

//Evaluates the input using execv syscall
int evaluate(char* input, int inputSize) {
    int i = 0; //inFileIdx = -1, outFileIdx = -1;
    int inputFd = 0, outputFd = 1; //Default values
    pid_t pid = fork();

    //Child Process. For now it's using the default stdin and stdout
    if (pid == 0) {
        char** argVector = inputStringToArgv(input, inputSize);
        
        //Set file descriptors based on argVectors' contents. Remove them from argVector later.
        for (i = 0; argVector[i] != NULL; i++) {
            if (argVector[i][0] == '<' && argVector[i][1] == '\0') {

                //The actual args of the command should not get past this, so we set a NULL in the middle of the arg vector
                free(argVector[i]);
                argVector[i++] = NULL;
                //Expect a file path as the next arg
                inputFd = open(argVector[i], O_RDONLY);
                if (inputFd == -1) handleError(ERROR_OPEN);
            }
            if (argVector[i][0] == '>' && argVector[i][1] == '\0') {
                free(argVector[i]);
                argVector[i++] = NULL;
                outputFd = open(argVector[i], O_WRONLY | O_CREAT, 0644);
                if (outputFd == -1) handleError(ERROR_OPEN);
            }
        }
        if (inputFd != 0)  dup2(inputFd,0);
        if (outputFd != 1) dup2(outputFd,1);

        //execve syscall
        int result = execv(argVector[0], argVector);

        //Free the memory (useless right now, since the execve kills the process)
        i = 0;
        while (argVector[i] != NULL) {
            free(argVector[i]);
            i++;
        }
        free(argVector);
    }

    //Parent process
    else {
        wait(NULL);
        write(1, "\n", 1);
        return 0;
    }
}