#ifndef ERRORS

enum ErrorType {
    ERROR_READ_INPUT,
    ERROR_EXECV,
    ERROR_OPEN,
};

#define ERROR_READ_INPUT_MSG "Error: Unable to read command.\n"
#define ERROR_OPEN_MSG "Error: unable to open file.\n"
#define ERROR_UNHANDLED_MSG "Error: Unhandled error.\n"

typedef enum ErrorType ErrorType;

void handleError(ErrorType);

#define ERRORS
#endif