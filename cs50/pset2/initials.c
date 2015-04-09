#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs50.h"

int main(void)
{
    string name = GetString();
    int i, j, spaces = 0;
    
    // Count the number of spaces
    for(i = 0; i < strlen(name); ++i)
    {
        if(name[i] == ' ')
        {
            spaces++;
        }
    }
    
    string initials = (string) malloc((spaces + 1) * sizeof(string));
    initials[0] = toupper(name[0]);
    
    // Similar logic for getting the initials
    for(i = 0, j = 1; i < strlen(name); ++i)
    {
        if(name[i] == ' ')
        {
            initials[j] = toupper(name[i + 1]);
            j++;
        }
    }
    
    printf("%s\n", initials);
    return 0;
}
