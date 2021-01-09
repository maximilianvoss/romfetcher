/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
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
#include "../constants.h"
#include "../helper/path.h"
#include "../../common/utils.h"
#include "../helper/map.h"

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
        LOG_ERROR("Was not able to init themes");
        exit(1);
    }
    app->themes.active = app->themes.all;
}

void themes_destroy(app_t *app) {
    // FIXME: destroy Textures  & Fonts
    linkedlist_freeList(app->themes.all, &freetheme);
}

theme_t *themes_getByFileReference(theme_t *theme, char *fileReference) {
    if (fileReference == NULL) {
        return NULL;
    }
    theme_t *ptr = ll_get1st(theme);
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
    FREENOTNULL(theme->fonts.font);
    FREENOTNULL(theme->images.backgroundPath);
    FREENOTNULL(theme->images.checkboxCheckedPath);
    FREENOTNULL(theme->images.checkboxUncheckedPath);
    FREENOTNULL(theme->images.selectorIconPath);
    FREENOTNULL(theme->images.settingsIconPath);
    FREENOTNULL(theme->images.downloadManagerIconPath);
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
        LOG_ERROR("theme.json not found at: %s", filepath->data);
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

    // window
    loadColor(&theme->colors.windowBackgroundActive, map, "colors.window.background.active");
    loadColor(&theme->colors.windowBackgroundInactive, map, "colors.window.background.inactive");
    loadColor(&theme->colors.windowCopyright, map, "colors.window.copyright");

    // field
    loadColor(&theme->colors.fieldTextActive, map, "colors.field.active.text");
    loadColor(&theme->colors.fieldForegroundActive, map, "colors.field.active.foreground");
    loadColor(&theme->colors.fieldBackgroundActive, map, "colors.field.active.background");
    loadColor(&theme->colors.fieldTextInactive, map, "colors.field.inactive.text");
    loadColor(&theme->colors.fieldForegroundInactive, map, "colors.field.inactive.foreground");
    loadColor(&theme->colors.fieldBackgroundInactive, map, "colors.field.inactive.background");

    // button
    loadColor(&theme->colors.buttonTextActive, map, "colors.button.active.text");
    loadColor(&theme->colors.buttonForegroundActive, map, "colors.button.active.foreground");
    loadColor(&theme->colors.buttonBackgroundActive, map, "colors.button.active.background");
    loadColor(&theme->colors.buttonTextInactive, map, "colors.button.inactive.text");
    loadColor(&theme->colors.buttonForegroundInactive, map, "colors.button.inactive.foreground");
    loadColor(&theme->colors.buttonBackgroundInactive, map, "colors.button.inactive.background");

    // Search Result list
    loadColor(&theme->colors.resultHeaderBackbround, map, "colors.results.header.background");
    loadColor(&theme->colors.resultHeaderForeground, map, "colors.results.header.foreground");
    loadColor(&theme->colors.resultHeaderText, map, "colors.results.header.text");
    loadColor(&theme->colors.resultDivider, map, "colors.results.divider");
    loadColor(&theme->colors.resultRowTextActive, map, "colors.results.row.active.text");
    loadColor(&theme->colors.resultRowForegroundActive, map, "colors.results.row.active.foreground");
    loadColor(&theme->colors.resultRowBackgroundActive, map, "colors.results.row.active.background");
    loadColor(&theme->colors.resultRowTextInactive, map, "colors.results.row.inactive.text");
    loadColor(&theme->colors.resultRowForegroundInactive, map, "colors.results.row.inactive.foreground");
    loadColor(&theme->colors.resultRowBackgroundInactive, map, "colors.results.row.inactive.background");

    // modal frame
    loadColor(&theme->colors.modalBackground, map, "colors.modal.background");
    loadColor(&theme->colors.modalForeground, map, "colors.modal.foreground");
    loadColor(&theme->colors.modalText, map, "colors.modal.text");
    loadColor(&theme->colors.modalHeadText, map, "colors.modal.head.text");
    loadColor(&theme->colors.modalHeadForeground, map, "colors.modal.head.foreground");

    // modal button
    loadColor(&theme->colors.modalButtonTextActive, map, "colors.modal.button.active.text");
    loadColor(&theme->colors.modalButtonForegroundActive, map, "colors.modal.button.active.foreground");
    loadColor(&theme->colors.modalButtonBackgroundActive, map, "colors.modal.button.active.background");
    loadColor(&theme->colors.modalButtonTextInactive, map, "colors.modal.button.inactive.text");
    loadColor(&theme->colors.modalButtonForegroundInactive, map, "colors.modal.button.inactive.foreground");
    loadColor(&theme->colors.modalButtonBackgroundInactive, map, "colors.modal.button.inactive.background");

    // icons
    loadColor(&theme->colors.iconForegroundColorActive, map, "colors.icon.active.foreground");
    loadColor(&theme->colors.iconBackgroundColorActive, map, "colors.icon.active.background");
    loadColor(&theme->colors.iconDownloadsTextActive, map, "colors.icon.active.downloadsText");
    loadColor(&theme->colors.iconDownloadsCircleActive, map, "colors.icon.active.downloadsCircle");

    loadColor(&theme->colors.iconForegroundColorInactive, map, "colors.icon.inactive.foreground");
    loadColor(&theme->colors.iconBackgroundColorInactive, map, "colors.icon.inactive.background");
    loadColor(&theme->colors.iconDownloadsTextInactive, map, "colors.icon.inactive.downloadsText");
    loadColor(&theme->colors.iconDownloadsCircleInactive, map, "colors.icon.inactive.downloadsCircle");

    // keyboard
    loadColor(&theme->colors.keyboardText, map, "colors.keyboard.text");
    loadColor(&theme->colors.keyboardActiveChar, map, "colors.keyboard.activeChar");

    // Downloads
    loadColor(&theme->colors.dwnldsItemForegroundInactive, map, "colors.downloads.item.inactive.foreground");
    loadColor(&theme->colors.dwnldsItemBackgroundInactive, map, "colors.downloads.item.inactive.background");
    loadColor(&theme->colors.dwnldsItemTextInactive, map, "colors.downloads.item.inactive.text");
    loadColor(&theme->colors.dwnldsItemForegroundActive, map, "colors.downloads.item.active.foreground");
    loadColor(&theme->colors.dwnldsItemBackgroundActive, map, "colors.downloads.item.active.background");
    loadColor(&theme->colors.dwnldsItemTextActive, map, "colors.downloads.item.active.text");
    loadColor(&theme->colors.dwnldsBarForegroundInactive, map, "colors.downloads.progressbar.inactive.foreground");
    loadColor(&theme->colors.dwnldsBarBackgroundInactive, map, "colors.downloads.progressbar.inactive.background");
    loadColor(&theme->colors.dwnldsBarTextInactive, map, "colors.downloads.progressbar.inactive.text");
    loadColor(&theme->colors.dwnldsBarProgressBarInactive, map, "colors.downloads.progressbar.inactive.bar");
    loadColor(&theme->colors.dwnldsBarForegroundActive, map, "colors.downloads.progressbar.active.foreground");
    loadColor(&theme->colors.dwnldsBarBackgroundActive, map, "colors.downloads.progressbar.active.background");
    loadColor(&theme->colors.dwnldsBarTextActive, map, "colors.downloads.progressbar.active.text");
    loadColor(&theme->colors.dwnldsBarProgressBarActive, map, "colors.downloads.progressbar.active.bar");

    theme->fileReference = cloneString(path);
    theme->name = cloneString(hash_get(map, "name"));

    theme->fonts.font = createFullQualifiedPath(path, hash_get(map, "font"));
    theme->images.backgroundPath = createFullQualifiedPath(path, hash_get(map, "images.background"));
    theme->images.checkboxCheckedPath = createFullQualifiedPath(path, hash_get(map, "images.checkboxChecked"));
    theme->images.checkboxUncheckedPath = createFullQualifiedPath(path, hash_get(map, "images.checkboxUnchecked"));
    theme->images.selectorIconPath = createFullQualifiedPath(path, hash_get(map, "images.selectorIcon"));
    theme->images.settingsIconPath = createFullQualifiedPath(path, hash_get(map, "images.settingsIcon"));
    theme->images.downloadManagerIconPath = createFullQualifiedPath(path,
                                                                    hash_get(map, "images.downloadManagerIcon"));

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

