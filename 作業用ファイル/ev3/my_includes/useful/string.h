#include <stdio.h>
#include <stdilb.h>

typedef struct {

    char *str;
    int len;

} String;

void string_constract(String *st)
{
    st -> len = 0;
    st -> str = (char *)malloc(sizeof(char));
}

void string_set(String *st,char *ch)
{
    st -> len = 0;
    st -> str = (char *)malloc(sizeof(char));
    while (ch[st -> len] != "\0")
    {
        st -> str = (char *)realloc(sizeof(char) * (st -> len + 1));
        st -> len += 1;
    }
}

void string_