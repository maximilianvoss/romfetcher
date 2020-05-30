#include <stdio.h>
#include <csafestring.h>
#include "map2json.h"
#include "config.h"
#include "logging.h"
#include "stringlib.h"

static void map2json_createJsonString(csafestring_t *buffer, map2json_tree_t *tree);

static long map2json_checkArrayObject(char *key);

static map2json_tree_t *map2json_findTreeNode(map2json_tree_t *root, char *key);

static map2json_tree_t *map2json_createEmptyTreeObject(char *key);

static long map2json_getArrayId(char *key);

static void map2json_storeValues(map2json_tree_t *obj, char *value);

static void map2json_createTree(map2json_t *obj);

static void map2json_createJsonStringArray(csafestring_t *buffer, map2json_tree_t *tree);

static void map2json_freeTreeMemory(map2json_tree_t *obj);

static void map2json_freePairsMemory(map2json_keyvalue_t *pair);

static map2json_tree_t *map2json_getArrayObject(map2json_tree_t *obj, long arrayId);

#define ARRAYID_NOT_SET -1
#define ARRAYID_IS_COUNT -2

map2json_t *map2json_init(char *prefix) {
    LOGGING_DEBUG("START");
    map2json_t *obj;

    obj = (map2json_t *) malloc(sizeof(map2json_t));
    obj->buffer = safe_create(NULL);
    obj->pairs = NULL;
    obj->tree = NULL;
    obj->prefix = prefix;

    LOGGING_DEBUG("DONE");
    return obj;
}


void map2json_push(map2json_t *obj, char *key, char *value) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("key=%s; value=%s", key, value);

    map2json_keyvalue_t *pair = (map2json_keyvalue_t *) malloc(sizeof(map2json_keyvalue_t));
    char *realKey;

    // TODO: Check and simplify
    if (obj->prefix == NULL) {
        realKey = key;
    } else {
        size_t prefixLength = strlen(obj->prefix);
        if (!strncmp(key, obj->prefix, prefixLength)) {
            realKey = &key[prefixLength + 1];
        } else {
            LOGGING_TRACE("prefix doesn't match");
            LOGGING_DEBUG("DONE");
            return;
        }
    }

    size_t keyLen = strlen(realKey) + 1;
    size_t valLen = strlen(value) + 1;

    if (realKey[keyLen - 3] == JSON2MAP_MAP_ARRAY_START && realKey[keyLen - 2] == JSON2MAP_MAP_OBJECT_CODE &&
        realKey[keyLen - 1] == JSON2MAP_MAP_ARRAY_END) {
        LOGGING_TRACE("array");
        LOGGING_DEBUG("DONE");
        return;
    }

    pair->key = (char *) calloc(sizeof(char), keyLen);
    pair->value = (char *) calloc(sizeof(char), valLen);

    memcpy(pair->key, realKey, keyLen);
    memcpy(pair->value, value, valLen);

    pair->next = obj->pairs;
    obj->pairs = pair;

    LOGGING_TRACE("key/value added");
    LOGGING_DEBUG("DONE");
}

char *map2json_create(map2json_t *obj) {
    LOGGING_DEBUG("START");

    map2json_freeTreeMemory(obj->tree);

    map2json_createTree(obj);
    safe_strcpy(obj->buffer, "");
    map2json_createJsonString(obj->buffer, obj->tree);

    LOGGING_DEBUG("DONE");
    return obj->buffer->data;
}


void map2json_destroy(map2json_t *obj) {
    LOGGING_DEBUG("START");
    map2json_freePairsMemory(obj->pairs);
    map2json_freeTreeMemory(obj->tree);
    safe_destroy(obj->buffer);
    free(obj);
    LOGGING_DEBUG("DONE");
}


static long map2json_checkArrayObject(char *key) {
    LOGGING_DEBUG("START");
    char *ptr = strchr(key, JSON2MAP_MAP_ARRAY_START);
    if (ptr && strchr(key, JSON2MAP_MAP_ARRAY_END)) {
        LOGGING_TRACE("return=%ld", ptr - key);
        LOGGING_DEBUG("DONE");
        return ptr - key;
    }
    LOGGING_TRACE("return=0");
    LOGGING_DEBUG("DONE");
    return 0;
}


static map2json_tree_t *map2json_findTreeNode(map2json_tree_t *root, char *key) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("key=%s", key);
    map2json_tree_t *obj;

    obj = root->children;
    while (obj != NULL) {
        if (!strcmp(obj->key, key)) {
            LOGGING_TRACE("key found");
            LOGGING_DEBUG("DONE");
            return obj;
        }
        obj = obj->next;
    }
    LOGGING_TRACE("key not found");
    LOGGING_DEBUG("DONE");
    return NULL;
}


static map2json_tree_t *map2json_createEmptyTreeObject(char *key) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("key=%s", key);
    map2json_tree_t *obj;

    obj = (map2json_tree_t *) malloc(sizeof(map2json_tree_t));
    obj->next = NULL;
    obj->value = NULL;
    obj->children = NULL;
    obj->arrayObjects = NULL;
    obj->type = JSMN_OBJECT;
    obj->arrayId = -1;
    obj->maxArrayId = -1;

    if (key == NULL) {
        LOGGING_TRACE("key == NULL");
        obj->key = NULL;
    } else {
        LOGGING_TRACE("key != NULL");
        obj->key = (char *) calloc(sizeof(char), strlen(key) + 1);
        memcpy(obj->key, key, strlen(key));
    }

    LOGGING_DEBUG("DONE");
    return obj;
}


static long map2json_getArrayId(char *key) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("key=%s", key);

    char *buffer;
    int value;

    if (key == NULL) {
        LOGGING_TRACE("key == NULL");
        LOGGING_TRACE("return=%d", ARRAYID_NOT_SET);
        LOGGING_DEBUG("DONE");
        return ARRAYID_NOT_SET;
    }
    key++;
    long length = strlen(key);

    buffer = calloc(sizeof(char), length);
    memcpy(buffer, key, length);
    buffer[length - 1] = '\0';

    if (*key == JSON2MAP_MAP_ARRAY_COUNT) {
        free(buffer);
        LOGGING_TRACE("*key == JSON2MAP_MAP_ARRAY_COUNT");
        LOGGING_TRACE("return=%d", ARRAYID_IS_COUNT);
        LOGGING_DEBUG("DONE");
        return ARRAYID_IS_COUNT;
    }

    value = atoi(buffer);
    free(buffer);
    LOGGING_TRACE("return=%d", value);
    LOGGING_DEBUG("DONE");
    return value;
}


static void map2json_storeValues(map2json_tree_t *obj, char *value) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("value=%s", value);

    if (obj->type == JSMN_ARRAY) {
        LOGGING_TRACE("object is array");
        obj->maxArrayId = atoi(value);
        LOGGING_DEBUG("DONE");
        return;
    }

    if (stringlib_isInteger(value) || !strcmp(value, "null") || !strcmp(value, "true") || !strcmp(value, "false") ||
        *value == JSON2MAP_PRIMITIVE_PREFIXER) {
        LOGGING_TRACE("object is primitive");
        obj->type = JSMN_PRIMITIVE;
        obj->value = value;
        if (*value == JSON2MAP_PRIMITIVE_PREFIXER) {
            obj->value++;
        }
    } else {
        LOGGING_TRACE("object is string");
        obj->type = JSMN_STRING;
        obj->value = value;
    }

    LOGGING_DEBUG("DONE");
}


static map2json_tree_t *map2json_getArrayObject(map2json_tree_t *obj, long arrayId) {
    LOGGING_DEBUG("START");
    map2json_tree_t *arrObj;

    if (arrayId == ARRAYID_IS_COUNT) {
        LOGGING_TRACE("arrayId == ARRAYID_IS_COUNT");
        LOGGING_DEBUG("DONE");
        return obj;
    }

    arrObj = obj->arrayObjects;
    while (arrObj != NULL) {
        if (arrObj->arrayId == arrayId) {
            LOGGING_TRACE("arrObj->arrayId == arrayId");
            LOGGING_DEBUG("DONE");
            return arrObj;
        }
        arrObj = arrObj->arrayObjects;

    }
    arrObj = map2json_createEmptyTreeObject(NULL);
    if (obj->maxArrayId < arrayId) {
        obj->maxArrayId = arrayId + 1;
    }
    arrObj->arrayId = arrayId;
    arrObj->arrayObjects = obj->arrayObjects;
    obj->arrayObjects = arrObj;

    LOGGING_DEBUG("DONE");
    return arrObj;
}


static void map2json_createTree(map2json_t *obj) {
    LOGGING_DEBUG("START");
    map2json_tree_t *treeRoot;
    map2json_tree_t *treeObj;
    map2json_tree_t *treeChild;
    map2json_keyvalue_t *pair;
    stringlib_tokens_t *tokens;
    stringlib_tokens_t *token;
    char *buffer;

    obj->tree = map2json_createEmptyTreeObject(NULL);
    treeRoot = obj->tree;

    pair = obj->pairs;
    while (pair != NULL) {
        tokens = stringlib_splitTokens(pair->key, JSON2MAP_MAP_OBJECT_SEPARATOR);
        token = tokens;
        treeObj = treeRoot;

        while (token != NULL) {
            buffer = stringlib_getToken(token, pair->key);

            long arrayId = ARRAYID_NOT_SET;
            long pos = map2json_checkArrayObject(buffer);
            if (pos) {
                arrayId = map2json_getArrayId(&buffer[pos]);
                buffer[pos] = '\0';
            }

            treeChild = map2json_findTreeNode(treeObj, buffer);

            if (treeChild == NULL) {
                treeChild = map2json_createEmptyTreeObject(buffer);
                treeChild->next = treeObj->children;
                treeObj->children = treeChild;
                if (pos) {
                    treeChild->type = JSMN_ARRAY;
                }
            }

            if (pos) {
                treeObj = map2json_getArrayObject(treeChild, arrayId);
            } else {
                treeObj = treeChild;
            }
            free(buffer);
            token = token->next;
        }
        map2json_storeValues(treeObj, pair->value);
        pair = pair->next;
        stringlib_freeTokens(tokens);
    }

    LOGGING_DEBUG("DONE");
}

static void map2json_createJsonStringArray(csafestring_t *buffer, map2json_tree_t *tree) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("buffer=%s", buffer->data);

    int i;
    safe_strchrappend(buffer, JSON2MAP_MAP_ARRAY_START);

    for (i = 0; i < tree->maxArrayId; i++) {
        map2json_tree_t *arrayObj = tree->arrayObjects;
        while (arrayObj != NULL) {
            if (arrayObj->arrayId == i) {
                map2json_createJsonString(buffer, arrayObj);
            }
            arrayObj = arrayObj->arrayObjects;
        }
        if (i < tree->maxArrayId - 1) {
            safe_strchrappend(buffer, ',');
        }
    }
    safe_strchrappend(buffer, JSON2MAP_MAP_ARRAY_END);

    LOGGING_DEBUG("DONE");
}

static void map2json_createJsonString(csafestring_t *buffer, map2json_tree_t *tree) {
    LOGGING_DEBUG("START");
    LOGGING_TRACE("buffer=%s", buffer->data);

    if (tree == NULL) {
        LOGGING_TRACE("tree == NULL");
        LOGGING_DEBUG("DONE");
        return;
    }

    if (tree->key != NULL) {
        LOGGING_TRACE("tree->key != NULL");
        safe_strchrappend(buffer, '\"');
        safe_strcat(buffer, tree->key);
        safe_strchrappend(buffer, '\"');
        safe_strchrappend(buffer, ':');
    }

    if (tree->type == JSMN_OBJECT) {
        LOGGING_TRACE("tree->type == JSMN_OBJECT");
        safe_strchrappend(buffer, '{');
        map2json_createJsonString(buffer, tree->children);
        safe_strchrappend(buffer, '}');
    }

    if (tree->type == JSMN_ARRAY) {
        LOGGING_TRACE("tree->type == JSMN_ARRAY");
        map2json_createJsonStringArray(buffer, tree);
    }

    if (tree->type == JSMN_PRIMITIVE || tree->type == JSMN_STRING) {
        LOGGING_TRACE("tree->type == JSMN_PRIMITIVE || tree->type == JSMN_STRING");
        if (tree->type == JSMN_STRING) {
            safe_strchrappend(buffer, '\"');
        }
        safe_strcat(buffer, tree->value);
        if (tree->type == JSMN_STRING) {
            safe_strchrappend(buffer, '\"');
        }
    }

    if (tree->next != NULL) {
        LOGGING_TRACE("tree->next != NULL");
        safe_strchrappend(buffer, ',');
        map2json_createJsonString(buffer, tree->next);
    }
    LOGGING_DEBUG("DONE");
}


static void map2json_freeTreeMemory(map2json_tree_t *obj) {
    LOGGING_DEBUG("START");

    if (obj == NULL) {
        LOGGING_TRACE("obj == NULL");
        LOGGING_DEBUG("DONE");
        return;
    }

    map2json_freeTreeMemory(obj->arrayObjects);
    map2json_freeTreeMemory(obj->children);
    map2json_freeTreeMemory(obj->next);

    if (obj->key != NULL) {
        free(obj->key);
    }
    free(obj);
    LOGGING_DEBUG("DONE");
}


static void map2json_freePairsMemory(map2json_keyvalue_t *pair) {
    LOGGING_DEBUG("START");
    if (pair == NULL) {
        LOGGING_TRACE("pair == NULL");
        LOGGING_DEBUG("DONE");
        return;
    }
    map2json_freePairsMemory(pair->next);
    free(pair->key);
    free(pair->value);
    free(pair);
    LOGGING_DEBUG("DONE");
}
