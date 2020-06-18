#include "revert_string.h"

void RevertString(char *str)
{
	int sSize = 0;
    int tmp;
    while (str[sSize])
    {
        sSize++;
    }
    int i = 0;
    for (i = 0; i < sSize/2; i++)
    {
        tmp = str[i];
        str[i] = str[sSize-i-1];
        str[sSize-i] = tmp;
    }
}