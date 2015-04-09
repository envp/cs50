#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cs50.h"

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("You need to supply the key as a command line argument!\n");
        return 1;
    }
    
    int i, key = atoi(argv[1]);
    string p = GetString();
    
    // Alter plain text in-place since we don't need to re-use it
    for(i = 0; i < strlen(p); ++i)
    {
        if(isupper(p[i]))
        {
            p[i] = (char)(((int)(p[i] - 'A') + key) % 26 + 'A');
        }
        else if(islower(p[i]))
        {
            p[i] = (char)(((int)(p[i] - 'a') + key) % 26 + 'a');
        }

    }
    printf("%s\n", p);
    return 0;
}


