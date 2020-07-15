/*
 * Copyright 2020 Maximilian Voss (maximilian@voss.rocks)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dirent.h>
#include <csafestring.h>
#include <sys/stat.h>
#include <json2map.h>
#include "loading.h"
#include "../config.h"
#include "../helper/path.h"
#include "../helper/map.h"
#include "../helper/utils.h"

static void colorSetter(SDL_Color *color, int r, int g, int b, int a);

static void loadThemes(app_t *app, char *path);

static void loadTheme(app_t *app, char *path);

static void *loadJsonMap(char *json);

static void json2maphook(void *data, char *key, char *value);

static theme_t *createThemeByMap(char *path, void *map);

static char *cloneString(char *strIn);

static char *createFullQualifiedPath(char *path, char *file);

static void loadColor(SDL_Color *color, void *map, char *key);

static int mapToInt(void *map, char *key);

static void freetheme(void *ptr);

void themes_init(app_t *app) {
    char themedirs[255][255] = THEMES_SYSTEM_DIRS;
    for (int i = 0; i < 255 && *themedirs[i] != '\0'; ++i) {
        loadThemes(app, themedirs[i]);
    }

    csafestring_t *path = path_romfetchersHome();
    safe_strcat(path, THEME_HOME);
    loadThemes(app, path->data);
    safe_destroy(path);

    if (app->themes.all == NULL) {
        printf("Was not able to init themes\n");
        exit(1);
    }
    app->themes.active = app->themes.all;
}

void themes_destroy(app_t *app) {
    linkedlist_freeList(app->themes.all, &freetheme);
}

theme_t *themes_getByFileReference(app_t *app, char *fileReference) {
    if (fileReference == NULL) {
        return app->themes.all;
    }
    theme_t *ptr = app->themes.all;
    while (ptr != NULL) {
        if (ptr->fileReference != NULL && !strcmp(fileReference, ptr->fileReference)) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

static void freetheme(void *ptr) {
    theme_t *theme = (theme_t *) ptr;

    FREENOTNULL(theme->fileReference);
    FREENOTNULL(theme->font);
    FREENOTNULL(theme->images.background);
    FREENOTNULL(theme->images.checkboxChecked);
    FREENOTNULL(theme->images.checkboxUnchecked);
    FREENOTNULL(theme->images.selectorIcon);
    FREENOTNULL(theme->images.settingsIcon);
    FREENOTNULL(theme->images.downloadManagerIcon);
    FREENOTNULL(theme->name);
}

static void colorSetter(SDL_Color *color, int r, int g, int b, int a) {
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}


static void loadThemes(app_t *app, char *path) {
    DIR *directoryHandler;
    struct dirent *entry;

    directoryHandler = opendir(path);
    if (directoryHandler == NULL) {
        return;
    }

    while ((entry = readdir(directoryHandler)) != NULL) {
        if (*(entry->d_name) == '.') {
            continue;
        }

        if (entry->d_type == DT_DIR) {
            csafestring_t *themePath = safe_create(path);
            safe_strchrappend(themePath, '/');
            safe_strcat(themePath, entry->d_name);
            loadTheme(app, themePath->data);
            safe_destroy(themePath);
        }
    }
    closedir(directoryHandler);
}

static void loadTheme(app_t *app, char *path) {
    csafestring_t *filepath = safe_create(path);
    safe_strchrappend(filepath, '/');
    safe_strcat(filepath, "theme.json");

    struct stat st = {0};
    if (stat(filepath->data, &st) == -1) {
        SDL_Log("theme.json not found at: %s", filepath->data);
        safe_destroy(filepath);
        return;
    }

    char buff[255];
    csafestring_t *content = safe_create(NULL);
    FILE *themeJson = fopen(filepath->data, "r");
    while (fgets(buff, 255, themeJson)) {
        safe_strcat(content, buff);
    }
    fclose(themeJson);

    void *map = loadJsonMap(content->data);
    theme_t *theme = createThemeByMap(path, map);
    app->themes.all = linkedlist_appendElement(app->themes.all, theme);
    hash_destroyMap(map);

    safe_destroy(content);
    safe_destroy(filepath);
}

static theme_t *createThemeByMap(char *path, void *map) {
    theme_t *theme = (theme_t *) calloc(1, sizeof(theme_t));
    loadColor(&theme->colors.background, map, "colors.background");
    loadColor(&theme->colors.field, map, "colors.field");
    loadColor(&theme->colors.fieldBackground, map, "colors.fieldBackground");
    loadColor(&theme->colors.fieldHighlight, map, "colors.fieldHighlight");
    loadColor(&theme->colors.text, map, "colors.text");
    loadColor(&theme->colors.textHighlight, map, "colors.textHighlight");
    loadColor(&theme->colors.textInverted, map, "colors.textInverted");

    loadColor(&theme->colors.modalBackground, map, "colors.modalBackground");
    loadColor(&theme->colors.modalBackgroundInactive, map, "colors.modalBackgroundInactive");
    loadColor(&theme->colors.modalFrame, map, "colors.modalFrame");
    loadColor(&theme->colors.modalText, map, "colors.modalText");
    loadColor(&theme->colors.modalButtonFrame, map, "colors.modalButtonFrame");
    loadColor(&theme->colors.modalButtonFrameActive, map, "colors.modalButtonFrameActive");

    theme->fileReference = cloneString(path);
    theme->name = cloneString(hash_get(map, "name"));

    theme->font = createFullQualifiedPath(path, hash_get(map, "font"));
    theme->images.background = createFullQualifiedPath(path, hash_get(map, "images.background"));
    theme->images.checkboxChecked = createFullQualifiedPath(path, hash_get(map, "images.checkboxChecked"));
    theme->images.checkboxUnchecked = createFullQualifiedPath(path, hash_get(map, "images.checkboxUnchecked"));
    theme->images.selectorIcon = createFullQualifiedPath(path, hash_get(map, "images.selectorIcon"));
    theme->images.settingsIcon = createFullQualifiedPath(path, hash_get(map, "images.settingsIcon"));
    theme->images.downloadManagerIcon = createFullQualifiedPath(path, hash_get(map, "images.downloadManagerIcon"));

    return theme;
}

static void loadColor(SDL_Color *color, void *map, char *key) {
    csafestring_t *tmp = safe_create(key);
    safe_strcat(tmp, "[x]");
    int count = mapToInt(map, tmp->data);

    if (count != 4) {
        colorSetter(color, 0, 0, 0, 0);
        safe_destroy(tmp);
        return;
    }

    safe_strcpy(tmp, key);
    safe_strcat(tmp, "[0]");
    int r = mapToInt(map, tmp->data);

    safe_strcpy(tmp, key);
    safe_strcat(tmp, "[1]");
    int g = mapToInt(map, tmp->data);

    safe_strcpy(tmp, key);
    safe_strcat(tmp, "[2]");
    int b = mapToInt(map, tmp->data);

    safe_strcpy(tmp, key);
    safe_strcat(tmp, "[3]");
    int a = mapToInt(map, tmp->data);

    if (r != -1 && g != -1 && b != -1 && a != -1) {
        colorSetter(color, r, g, b, a);
    }

    safe_destroy(tmp);
}

static int mapToInt(void *map, char *key) {
    char *countStr = hash_get(map, key);
    if (countStr == NULL) {
        return -1;
    }

    int count = atol(countStr);
    return count;
}

static char *createFullQualifiedPath(char *path, char *file) {
    if (file == NULL) {
        return NULL;
    }
    if (*file == '/') {
        return cloneString(file);
    }
    csafestring_t *filepath = safe_create(path);
    safe_strchrappend(filepath, '/');
    safe_strcat(filepath, file);

    char *str = cloneString(filepath->data);
    safe_destroy(filepath);
    return str;
}

static char *cloneString(char *strIn) {
    if (strIn == NULL) {
        return NULL;
    }
    char *str = (char *) calloc(strlen(strIn) + 1, sizeof(char));
    strcpy(str, strIn);
    return str;
}

static void json2maphook(void *data, char *key, char *value) {
    hash_set(data, key, value);
}

static void *loadJsonMap(char *json) {
    void *map = hash_createMap();
    json2map_t *json2mapObj = json2map_init(0);
    json2map_registerDataHook(json2mapObj, map, &json2maphook);
    json2map_parse(json2mapObj, NULL, json);
    json2map_destroy(json2mapObj);
    return map;
}

