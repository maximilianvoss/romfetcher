#ifndef __JSON2MAP_STRINGLIB_H__
#define __JSON2MAP_STRINGLIB_H__

#include "string.h"

typedef struct s_stringlib_token {
    long start;
    long length;
    struct s_stringlib_token *next;
} stringlib_tokens_t;

char *stringlib_getToken(stringlib_tokens_t *token, char *str);

stringlib_tokens_t *stringlib_splitTokens(char *str, char chr);

int stringlib_isInteger(char *str);

char *stringlib_longToString(char *buffer, long value);

void stringlib_freeTokens(stringlib_tokens_t *tokens);

#endif