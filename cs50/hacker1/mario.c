#include <stdio.h>
#include <cs50.h>

// Print the double pyramid
void printDoubleRamp(int height)
{
    int r, c;
    
    for(r = 0; r < height; ++r)
    {
        for(c = 0; c < height - r - 1; ++c)
        {
           printf(" ");
        }
        
        for(c = height - r; c < height + 1; ++c)
        {
            printf("#");
        }
        
        printf("  ");
        
        for(c = 0; c < r + 1; ++c)
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
    
    // Main logic remains same as pset1    
    if(height > 0 && height < 24)
    {
        printDoubleRamp(height);
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
