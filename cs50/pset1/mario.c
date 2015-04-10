#include <stdio.h>
#include <cs50.h>

void printPyramid(int height)
{
    int r, c;
    // Show h + 1 blocks in h rows
    // Start with  r = 2 to offset the pyramid to show 2 starting blocks
    for(r = 0; r < height; ++r)
    {
        for(c = 0; c < height - r - 1; ++c)
        {
            printf(" ");
        }
        for(c = height - r - 1; c <= height; ++c)
        {
            printf("#");
        }
        printf("\n");
    }
}

int main(void)
{    
    printf("Height: ");
    int height = GetInt();
    
    if(height > 0 && height < 24)
    {
        printPyramid(height);
    }
    else if(height == 0)
    {
        return 0;
    }
    else
    {
        main();
    }
    return 0;
}
