#ifndef __JSON2MAP_MAP2JSON_H__
#define __JSON2MAP_MAP2JSON_H__

#include <csafestring.h>
#include "jsmn.h"

typedef struct s_map2json_keyvalue {
    char *key;
    char *value;
    struct s_map2json_keyvalue *next;
} map2json_keyvalue_t;

typedef struct s_map2json_tree {
    char *key;
    char *value;
    long arrayId;
    long maxArrayId;
    jsmntype_t type;
    struct s_map2json_tree *next;
    struct s_map2json_tree *children;
    struct s_map2json_tree *arrayObjects;
} map2json_tree_t;

typedef struct s_map2json {
    map2json_keyvalue_t *pairs;
    map2json_tree_t *tree;
    csafestring_t *buffer;
    char *prefix;
} map2json_t;

map2json_t *map2json_init(char *prefix);

void map2json_push(map2json_t *obj, char *key, char *value);

char *map2json_create(map2json_t *obj);

void map2json_destroy(map2json_t *obj);

#endif