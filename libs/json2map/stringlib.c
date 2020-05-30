#include <stdio.h>
#include <stdlib.h>
#include "stringlib.h"
#include "logging.h"


char *stringlib_getToken(stringlib_tokens_t *token, char *str) {
    LOGGING_DEBUG("START");
    char *buffer;

    buffer = calloc(sizeof(char), token->length + 1);
    memcpy(buffer, str + token->start, token->length);

    LOGGING_TRACE("buffer=%s", buffer);
    LOGGING_DEBUG("DONE");
    return buffer;
}


stringlib_tokens_t *stringlib_splitTokens(char *str, char chr) {
    LOGGING_DEBUG("START");
    char *lastPos;
    char *newPos;

    stringlib_tokens_t *tokens = NULL;
    stringlib_tokens_t *currentToken;
    stringlib_tokens_t *newToken;

    newPos = str;
    lastPos = str;

    while (*newPos != '\0') {
        if (*newPos == chr) {
            newToken = (stringlib_tokens_t *) malloc(sizeof(stringlib_tokens_t));
            newToken->next = NULL;

            if (tokens == NULL) {
                tokens = newToken;
                currentToken = newToken;
            } else {
                currentToken->next = newToken;
                currentToken = newToken;
            }

            currentToken->start = (lastPos - str);
            currentToken->length = (newPos - lastPos);
            lastPos = newPos + 1;
        }
        newPos++;
    }
    if (newPos != str) {
        newToken = (stringlib_tokens_t *) malloc(sizeof(stringlib_tokens_t));
        newToken->next = NULL;

        if (tokens == NULL) {
            tokens = newToken;
            currentToken = newToken;
        } else {
            currentToken->next = newToken;
            currentToken = newToken;
        }

        currentToken->start = (lastPos - str);
        currentToken->length = (newPos - lastPos);
    }
    LOGGING_DEBUG("DONE");
    return tokens;
}

void stringlib_freeTokens(stringlib_tokens_t *tokens) {
    LOGGING_DEBUG("START");
    stringlib_tokens_t *token;
    while (tokens != NULL) {
        token = tokens;
        tokens = tokens->next;
        LOGGING_TRACE("freeing %X", (unsigned int) token);
        free(token);
    }
    LOGGING_DEBUG("DONE");
}

int stringlib_isInteger(char *str) {
    LOGGING_DEBUG("START");
    char *ptr = str;

    if (*ptr == '\0') {
        LOGGING_TRACE("return=0");
        LOGGING_DEBUG("DONE");
        return 0;
    }

    while (*ptr != '\0') {
        if ((*ptr < 48 || *ptr > 57) && *ptr != '.') {
            LOGGING_TRACE("return=0");
            LOGGING_DEBUG("DONE");
            return 0;
        }
        ptr++;
    }
    LOGGING_TRACE("return=1");
    LOGGING_DEBUG("DONE");
    return 1;
}

char *stringlib_longToString(char *buffer, long value) {
    LOGGING_DEBUG("START");
    sprintf(buffer, "%ld", value);
    LOGGING_TRACE("buffer=%s", buffer);
    LOGGING_DEBUG("DONE");
    return buffer;
}
