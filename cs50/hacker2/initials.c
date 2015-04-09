#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs50.h"

#define DELIMS " -."

int main(void)
{
    string name = GetString();
    string token;
    token = strtok(name, DELIMS);

    // Keep pulling characters till no more tokens found
    // strtok() saves the day
    while(token != NULL)
    {
        printf("%c", toupper(token[0]));
        token = strtok(NULL, DELIMS);
    }

    printf("\n");
    
    return 0;
}
