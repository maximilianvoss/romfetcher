#include "json2map.h"
#include "config.h"

#include <stdio.h>
#include <csafestring.h>
#include "jsmn.h"
#include "logging.h"
#include "stringlib.h"

static char *json2map_setTokenValue(char *jsonString, jsmntok_t *token);

static csafestring_t *json2map_concatPaths(char *parent, char *key, int arrayIdx);

static int json2map_calcEnd(jsmntok_t *token, int start, int end);

static int json2map_parseArray(json2map_t *obj, char *path, char *jsonString, jsmntok_t *token, int start, int end);

static int json2map_parseObject(json2map_t *obj, char *path, char *jsonString, jsmntok_t *token, int start, int end);

json2map_t *json2map_init(char saveSubobjectString) {
    LOGGING_DEBUG("START");
    json2map_t *obj = (json2map_t *) malloc(sizeof(json2map_t));
    obj->dataMapData = NULL;
    obj->dataMapHook = NULL;
    obj->saveSubobjectString = saveSubobjectString;
    LOGGING_DEBUG("DONE");
    return obj;
}

void json2map_destroy(json2map_t *obj) {
    LOGGING_DEBUG("START");
    free(obj);
    LOGGING_DEBUG("DONE");
}


int json2map_parse(json2map_t *obj, char *prefix, char *jsonString) {
    LOGGING_DEBUG("START");

    jsmn_parser p;
    jsmntok_t *token;

    if (jsonString == NULL) {
        LOGGING_DEBUG("jsonString == NULL");
        LOGGING_DEBUG("DONE");
        return -1;
    }

    jsmn_init(&p);

    int count = jsmn_parse(&p, jsonString, strlen(jsonString), NULL, 0);
    LOGGING_TRACE("Count = %d", count);
    if (count < 0) {
        LOGGING_WARN("no object found. jsonString=%s", jsonString);
        return -1;
    }

    token = (jsmntok_t *) malloc(sizeof(jsmntok_t) * count);
    p.pos = 0;
    jsmn_parse(&p, jsonString, strlen(jsonString), token, count);

    if (count < 1 || token[0].type != JSMN_OBJECT) {
        free(token);
        LOGGING_ERROR("first object needs to be a valid object. jsonString=%s", jsonString);
        return -1;
    }


    int retVal = json2map_parseObject(obj, prefix, jsonString, token, 1, count);
    free(token);
    LOGGING_TRACE("retVal=%d", retVal);
    LOGGING_DEBUG("DONE");
    return retVal;
}


void json2map_registerDataHook(json2map_t *obj, void *data, void *method) {
    LOGGING_DEBUG("START");
    obj->dataMapHook = method;
    obj->dataMapData = data;
    LOGGING_DEBUG("DONE");
}

static char *json2map_setTokenValue(char *jsonString, jsmntok_t *token) {
    LOGGING_DEBUG("START");
    char *buffer;
    buffer = calloc(sizeof(char), token->end - token->start + 1);
    memcpy(buffer, jsonString + token->start, token->end - token->start);

    LOGGING_TRACE("buffer=%s", buffer);
    LOGGING_DEBUG("DONE");
    return buffer;
}


static csafestring_t *json2map_concatPaths(char *parent, char *key, int arrayIdx) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("parent=%s, key=%s, arrayIdx=%d", parent, key, arrayIdx);

    char arrayIdxBuff[10];
    csafestring_t *buffer = safe_create(NULL);

    if (parent != NULL && *parent != '\0') {
        safe_strcpy(buffer, parent);
        safe_strchrappend(buffer, JSON2MAP_MAP_OBJECT_SEPARATOR);
        safe_strcat(buffer, key);
    } else {
        safe_strcpy(buffer, key);
    }

    if (arrayIdx >= 0) {
        sprintf(arrayIdxBuff, "%c%d%c", JSON2MAP_MAP_ARRAY_START, arrayIdx, JSON2MAP_MAP_ARRAY_END);
        safe_strcat(buffer, arrayIdxBuff);
    }
    if (arrayIdx == -2) {
        sprintf(arrayIdxBuff, "%c%c%c", JSON2MAP_MAP_ARRAY_START, JSON2MAP_MAP_ARRAY_COUNT, JSON2MAP_MAP_ARRAY_END);
        safe_strcat(buffer, arrayIdxBuff);
    }

    LOGGING_TRACE("buffer=%s", buffer->data);
    LOGGING_DEBUG("DONE");
    return buffer;
}


static int json2map_calcEnd(jsmntok_t *token, int start, int end) {
    // TODO: optimize search
    LOGGING_DEBUG("START");
    int i;
    for (i = start + 1; i < end; i++) {
        if (token[i].start > token[start].end) {
            LOGGING_TRACE("return value=%d", i - 1);
            LOGGING_DEBUG("DONE");
            return i - 1;
        }
    }
    LOGGING_TRACE("return value=%d", end - 1);
    LOGGING_DEBUG("DONE");
    return end - 1;
}


static int json2map_parseArray(json2map_t *obj, char *path, char *jsonString, jsmntok_t *token, int start, int end) {
    LOGGING_DEBUG("START");
    int newEnd;
    char *buffer;
    csafestring_t *pathBuff;
    csafestring_t *objPathBuff;
    int count = 0;

    int i = start;
    while (i < end && i > 0) {

        pathBuff = json2map_concatPaths(NULL, path, count);
        count++;

        switch (token[i].type) {
            case JSMN_OBJECT:
                LOGGING_TRACE("JSMN_OBJECT");
                newEnd = json2map_calcEnd(token, i, end);

                if (obj->saveSubobjectString) {

                    objPathBuff = safe_clone(pathBuff);
                    safe_strchrappend(objPathBuff, JSON2MAP_MAP_ARRAY_START);
                    safe_strchrappend(objPathBuff, JSON2MAP_MAP_OBJECT_CODE);
                    safe_strchrappend(objPathBuff, JSON2MAP_MAP_ARRAY_END);

                    csafestring_t *tmp = safe_create(NULL);
                    safe_memset(tmp, '\0', token[i].end - token[i].start + 1);
                    safe_memcpy(tmp, &jsonString[token[i].start], token[i].end - token[i].start);

                    obj->dataMapHook(obj->dataMapData, objPathBuff->data, tmp->data);
                    safe_destroy(tmp);
                    safe_destroy(objPathBuff);
                }

                i = json2map_parseObject(obj, pathBuff->data, jsonString, token, i + 1, newEnd + 1);

                break;
            case JSMN_STRING:
            case JSMN_PRIMITIVE:
                LOGGING_TRACE("JSMN_PRIMITIVE");
                buffer = json2map_setTokenValue(jsonString, &token[i]);
                if (obj->dataMapHook != NULL) {
                    obj->dataMapHook(obj->dataMapData, pathBuff->data, buffer);
                }
                free(buffer);
                i++;
                break;
            default: LOGGING_ERROR("Not defined type. jsonString=%s", jsonString);
                LOGGING_DEBUG("DONE");
                return -1;
        }

        safe_destroy(pathBuff);
    }

    pathBuff = json2map_concatPaths(NULL, path, -2);

    char smallBuffer[25];
    stringlib_longToString(smallBuffer, count);
    if (obj->dataMapHook != NULL) {
        obj->dataMapHook(obj->dataMapData, pathBuff->data, smallBuffer);
    }
    safe_destroy(pathBuff);

    if (i < 0) {
        LOGGING_TRACE("return=%d", i)
        LOGGING_DEBUG("DONE");
        return i;
    }
    LOGGING_TRACE("return=%d", end)
    LOGGING_DEBUG("DONE");
    return end;
}


static int json2map_parseObject(json2map_t *obj, char *path, char *jsonString, jsmntok_t *token, int start, int end) {
    LOGGING_DEBUG("START");

    int newEnd;
    char *buffer;
    csafestring_t *pathBuff = NULL;
    csafestring_t *objPathBuff = NULL;

    int i = start;
    while (i < end && i > 0) {
        if (token[i].type == JSMN_STRING) {
            buffer = json2map_setTokenValue(jsonString, &token[i]);
            pathBuff = json2map_concatPaths(path, buffer, -1);
            free(buffer);
        } else {
            LOGGING_ERROR("Name of object has to be a string");
            LOGGING_DEBUG("DONE");
            return -1;
        }
        i++;

        switch (token[i].type) {
            case JSMN_OBJECT:
                LOGGING_TRACE("JSMN_OBJECT");
                newEnd = json2map_calcEnd(token, i, end);

                if (obj->saveSubobjectString) {
                    objPathBuff = safe_clone(pathBuff);
                    safe_strchrappend(objPathBuff, JSON2MAP_MAP_ARRAY_START);
                    safe_strchrappend(objPathBuff, JSON2MAP_MAP_OBJECT_CODE);
                    safe_strchrappend(objPathBuff, JSON2MAP_MAP_ARRAY_END);

                    csafestring_t *tmp = safe_create(NULL);
                    safe_memset(tmp, '\0', token[i].end - token[i].start + 1);
                    safe_memcpy(tmp, &jsonString[token[i].start], token[i].end - token[i].start);

                    obj->dataMapHook(obj->dataMapData, objPathBuff->data, tmp->data);
                    safe_destroy(tmp);
                    safe_destroy(objPathBuff);
                }

                i = json2map_parseObject(obj, pathBuff->data, jsonString, token, i + 1, newEnd + 1);
                break;
            case JSMN_STRING:
            case JSMN_PRIMITIVE:
                LOGGING_TRACE("JSMN_PRIMITIVE");
                buffer = json2map_setTokenValue(jsonString, &token[i]);
                if (obj->dataMapHook != NULL) {
                    obj->dataMapHook(obj->dataMapData, pathBuff->data, buffer);
                }
                free(buffer);
                i++;
                break;
            case JSMN_ARRAY:
                LOGGING_TRACE("JSMN_ARRAY");
                newEnd = json2map_calcEnd(token, i, end);
                i = json2map_parseArray(obj, pathBuff->data, jsonString, token, i + 1, newEnd + 1);
                break;
            default: LOGGING_ERROR("Not defined type");
                LOGGING_DEBUG("DONE");
                return -1;
        }
        safe_destroy(pathBuff);
    }

    if (i < 0) {
        LOGGING_TRACE("return=%d", i);
        LOGGING_DEBUG("DONE");
        return i;
    }
    LOGGING_TRACE("return=%d", end);
    LOGGING_DEBUG("DONE");
    return end;
}

