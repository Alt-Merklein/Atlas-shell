#ifndef PARSE


enum ParsingState {
    START,
    READ_ARG,
    ALLOCATE,
    WAIT_FOR_ARG,
};

typedef enum ParsingState ParsingState;

char** inputStringToArgv(char*, int);
int countArgNumber(char*, int);

#define PARSE
#endif