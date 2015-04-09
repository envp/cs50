#define _XOPEN_SOURCE       /* See feature_test_macros(7) */
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PTXT_LEN 128
#define MAX_CTXT_LEN 13
#define MAX_SALT_LEN 2
#define CANDIDATE_SIZE 128
#define ASC_PRT_STRT 32
#define ASC_PRT_END 126
#define DICT_FILE_NAME "/usr/share/dict/words"

int brute_force(char* ctxt, char* salt, char* ptxt);
int dictionary(char* ctxt, char* salt, char* ptxt, const char* dictfname);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./crack <ciphertext>\n");
		return 1;
	}
    
    // Crypt ensures that the highest 2 bytes of the hash are the salt itself
    // Save some space for that null byte
    char salt[MAX_SALT_LEN + 1] = {0}, 
    ctxt[MAX_CTXT_LEN + 1] = {0}, 
    ptxt[MAX_PTXT_LEN + 1] = {0};

    strncpy(salt, argv[1], MAX_SALT_LEN);
    strncpy(ctxt, argv[1], MAX_CTXT_LEN);
    
    if(dictionary(ctxt, salt, ptxt, DICT_FILE_NAME) == 0)
    {
        printf("%s\n", ptxt);
    }
    else if(brute_force(ctxt, salt, ptxt) == 0)
    {
        printf("%s\n", ptxt);
    }

    return 0;
}

// Performs a literal "dictionary" attack on the ciphertext
// uses the wordlist provided by cs50 by default
// Returns non-zero int on failure
int dictionary(char* ctxt, char* salt, char* ptxt, const char* dictfname)
{
    FILE* dict = fopen(dictfname, "r");
    char candidate[CANDIDATE_SIZE] = {0};
    if(dict == NULL || feof(dict))
    {
        return -1;
    }

    // fgetc till newline much faster?
    while(!feof(dict))
    {
        // Vulnerable to overflow attacks!
        fscanf(dict, "%s", candidate);
        if(strcmp(ctxt, crypt(candidate, salt)) == 0)
        {
            strncpy(ptxt, candidate, strlen(candidate));
            fclose(dict);
            return 0;
        }
    }
    fclose(dict);
    return 1;
}

// Fallback mechanism, brute force all passwords up to 8 bytes long
// O(N^8), gotta run this make make meself a cuppa tea, some dinner
// and dessert for all of Uganda
int brute_force(char *ctxt, char* salt, char* ptxt)
{
    // Lol counters
    int a, b, c, d, e, f, g, h, attempts = 0;
    for(a = ASC_PRT_STRT; a <= ASC_PRT_END; ++a)
    {
        ptxt[0] = (char) a;
        for(b = ASC_PRT_STRT; b <= ASC_PRT_END; ++b)
        {
            ptxt[1] = (char) b;
            for(c = ASC_PRT_STRT; c <= ASC_PRT_END; ++c)
            {
                ptxt[2] = (char) c;
                for(d = ASC_PRT_STRT; d <= ASC_PRT_END; ++d)
                {
                    ptxt[3] =(char) d;
                    for(e = ASC_PRT_STRT; e <= ASC_PRT_END; ++e)
                    {
                        ptxt[4] =(char) e;
                        for(f = ASC_PRT_STRT; f <= ASC_PRT_END; ++f)
                        {
                            ptxt[5] =(char) f;
                            for(g = ASC_PRT_STRT; g <= ASC_PRT_END; ++g) 
                            {
                                ptxt[6] =(char) g;
                                for(h = ASC_PRT_STRT; h <= ASC_PRT_END; ++h)
                                {
                                    ptxt[7] =(char) h;
                                    ++attempts;
                                    if(strcmp(ctxt, crypt(ptxt, salt)) == 0)
                                    {
                                        // An alternative to goto
                                        // ptxt is already set, it's all we need
                                        return 0;
                                    }
                                }
                                // Notify once every 95 * 95 attempts
                                printf("%d\r", attempts);
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}
