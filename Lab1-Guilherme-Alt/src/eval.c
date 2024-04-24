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
    int debug = open("debug.txt", O_WRONLY | O_APPEND, 0644);

    int pipefd[2][2];
    unsigned char curPipe = 0;
    pipe(pipefd[curPipe]);
    pipe(pipefd[curPipe + 1]);
    
    unsigned char isStart = 1, isEnd = 1;
    pid_t pid = fork();
    
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

            if (argVector[i][0] == '|' && argVector[i][1] == '\0') {
                //run the current command in the child process. Pipe it to the next
                isEnd = 0;
                free(argVector[i]);
                argVector[i++] = NULL;
                pid_t pid2 = fork();
                if (pid2 == 0) {
                    if (isStart) {
                        close(pipefd[1][0]);
                        close(pipefd[1][1]);
                        close(pipefd[0][0]);
                        outputFd = pipefd[0][1];
                    }
                    else {
                        close(pipefd[curPipe][0]);
                        close(pipefd[(curPipe + 1) % 2][1]);
                        outputFd = pipefd[curPipe][1];
                    }
                    break;
                }
                else { //parent. argv is different now
                    curPipe = (curPipe + 1) % 2;
                    isEnd = 1;
                    isStart = 0;
                    wait(NULL);
                    argVector = (char**) &argVector[i];
                    inputFd = pipefd[(curPipe + 1) % 2][0];
                    i = 0;
                }
            }
            write(debug,"a",1);
        }

        if (inputFd != 0) dup2(inputFd,0);
        if (outputFd != 0) dup2(outputFd,1);

        if (isEnd) {
            close(pipefd[curPipe][0]);
            close(pipefd[curPipe][1]);
            close(pipefd[(curPipe + 1) % 2][1]);
        }

        write(debug,argVector[0], sizeof(argVector[0]) + 3); //REMOVE THIS LATER
    
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
        close(pipefd[0][0]);
        close(pipefd[0][1]);
        close(pipefd[1][0]);
        close(pipefd[1][1]);
        wait(NULL);
        close(debug);
        write(1, "\n", 1);
        return 0;
    }
}