#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cs50.h"

void encrypt(string plaintext, string key, string cipertext, int p_len);
int ctoi(char c);

int main(int argc, string argv[])
{
    // Run the validations prior to making any intializations etc.
    int i;
    if(argc != 2)
    {
        printf("Need a string key to run vigenere cipher!\n");
        return 1;
    }
    for(i = 0; i < strlen(argv[1]); ++i)
    {
        if(!isalpha(argv[1][i]))
        {
            printf("The keyword needs to be alphabetic only!\n");
            return 1;
        }
    }

    // Main logic begins here
    string key = argv[1], plaintext = GetString();
    string ciphertext = (string) malloc(strlen(plaintext) * sizeof(string));

    // Get the ciphertext
    encrypt(plaintext, key, ciphertext, strlen(plaintext));

    printf("%s\n", ciphertext);
    return 0;
}

// Shifts each point in plaintext by its corresponding point in key modulo 26
// Assumes plaintext, key and ciphertext have the same length
// The key pointer is only incremented on encountering an encipherable symbol
void encrypt(string p, string k, string c, int p_len)
{
    int i, j, k_len;
   
    for(i = 0, j = 0, k_len = strlen(k); i < p_len; ++i)
    {

        if(isupper(p[i]))
        {
            c[i] = (char) ((ctoi(p[i]) + ctoi(k[j])) % 26 + 'A');
            j = (j + 1) % k_len;
        }
        else if(islower(p[i]))
        {
            c[i] = (char) ((ctoi(p[i]) + ctoi(k[j])) % 26 + 'a');
            j = (j + 1) % k_len;
        }
        else
        {
            c[i] = p[i];
        }
    }
    return ;
}

// Does not check for errors
// Converts a character of the english alphabet to 
// a zero indexed number
// Returns (int)c if c is not part of the english alphabet
int ctoi(char c)
{
    if(isupper(c))
    {
        return (int)(c - 'A');
    }
    else if(islower(c))
    {
        return (int)(c - 'a');
    }
    else
    {
        return (int)c;
    }
}
