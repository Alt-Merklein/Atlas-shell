To compile the shell, use:
"make atlash"

To compile a test file that accepts input from fd 0 and outputs it to fd 0, use:
"make test"


The files input.txt and output.txt can be used to try some commands that mess
with file descriptors, such as "./bin/test < input.txt > output.txt"


Some problems that were not solved for the time being:

    - cd command is not changing the path where the shell is at.

    - when writing to a file, like "/usr/bin/echo helloworld > file.txt", the
      file contents are not deleted, just ovewritten. If the original
      contents of the file were "File Contents", the contents after
      writing to it will be "helloworldnts".

