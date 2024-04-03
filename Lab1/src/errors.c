#include "errors.h"
#include <unistd.h>

void handleError(ErrorType type) {
    switch (type)
    {
        case ERROR_READ_INPUT:
            write(2, ERROR_READ_INPUT_MSG, sizeof(ERROR_READ_INPUT_MSG));
            break;

        case ERROR_OPEN:
            write(2, ERROR_OPEN_MSG, sizeof(ERROR_OPEN_MSG));
            break;

        default:
            write(2, ERROR_UNHANDLED_MSG, sizeof(ERROR_UNHANDLED_MSG));
            break;
    }
}