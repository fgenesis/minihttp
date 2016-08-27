// Example 1: Dead-simple one-shot file download API

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "minihttp.h"

int main(int argc, char *argv[])
{
    char *content = minihttp::Download("motherfuckingwebsite.com");
    if(!content)
    {
        puts("ERROR!");
        return 1;
    }

    puts(content);
    free(content);

    return 0;
}
