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

#include "list.h"
#include "../state/statehandler.h"
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "../helper/uihelper.h"
#include "../definitions/listitem.h"
#include "../helper/utils.h"

static void up(app_t *app);

static void down(app_t *app);

static void addElement(sqlite3 *db, char *tableName, char *name, int active);

static int loadActivitiesCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

static int filterName(void *payload, void *input);

void ll_processUp(app_t *app) {
    up(app);
}

void ll_processDown(app_t *app) {
    down(app);
}

void ll_processLeft(app_t *app) {
    up(app);
}

void ll_processRight(app_t *app) {
    down(app);
}

void ll_processSelect(app_t *app) {
    if (app->list.multi) {
        if (getListItem(app->list.cursor)->active) {
            getListItem(app->list.cursor)->active = 0;
        } else {
            getListItem(app->list.cursor)->active = 1;
        }
    } else {
        app->list.active = app->list.cursor;
    }
    statehandler_switch(app, 1);
}

void ll_processBack(app_t *app) {
    statehandler_switch(app, 0);
}

void ll_processOtherButton(app_t *app, controller_t *state) {
}

void ll_processOtherKey(app_t *app, SDL_Scancode scancode) {}

static void up(app_t *app) {
    if (app->list.filterActive) {
        acll_t *prev = acll_prevFilter(app->list.cursor, ll_filterActive, NULL);
        if (prev != NULL) {
            app->list.cursor = prev;
        }
    } else {
        if (app->list.cursor->prev != NULL) {
            app->list.cursor = app->list.cursor->prev;
        }
    }
}

static void down(app_t *app) {
    if (app->list.filterActive) {
        acll_t *next = acll_nextFilter(app->list.cursor, ll_filterActive, NULL);
        if (next != NULL) {
            app->list.cursor = next;
        }
    } else {
        if (app->list.cursor->next != NULL) {
            app->list.cursor = app->list.cursor->next;
        }

    }
}

void ll_renderDefault(app_t *app) {
    ll_renderList(app, PADDING_TOP);
}

void ll_renderList(app_t *app, int offset) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;

    int elementCountToDisplay = (height - offset - 80) / LIST_ITEM_HEIGHT + 1;

    acll_t *element = (app->list.cursor != NULL) ? app->list.cursor : app->list.all;
    if (element == NULL) {
        return;
    }

    int i = 0;
    while (i < elementCountToDisplay / 2 - 1) {
        if (app->list.filterActive) {
            if (acll_prevFilter(element, ll_filterActive, NULL) == NULL) {
                break;
            }
            element = acll_prevFilter(element, ll_filterActive, NULL);
        } else {
            if (element->prev == NULL) {
                break;
            }
            element = element->prev;
        }
        i++;
    }

    int position = offset;
    while (element != NULL && position <= height - PADDING_BOTTOM) {
        uiElementRects_t rects = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, position, width,
                                                                       LIST_ITEM_HEIGHT);
        themes_setDrawColorFieldBackground(app, (element == app->list.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.outter);
        themes_setDrawColorFieldForeground(app, (element == app->list.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.inner);

        rendering_loadText(app->sdlRenderer, &texture, getListItem(element)->name, getActiveTheme(app)->fonts.font24,
                           themes_getDrawColorFieldText(app, (element == app->list.cursor)));
        if (app->list.checkbox) {
            uiElementRects_t checkboxRects = uihelper_generateRects(rects.inner.x + 3, rects.content.y, rects.content.h,
                                                                    rects.content.h);
            uihelper_renderSDLTexture(app->sdlRenderer,
                                      getListItem(element)->active ? getActiveTheme(app)->images.checkboxChecked
                                                                   : getActiveTheme(app)->images.checkboxUnchecked,
                                      &checkboxRects.inner);
            rects.content.x += rects.content.h;
            rects.content.w -= rects.content.h;
            uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        } else {
            uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        }
        uihelper_destroyTexture(&texture);

        if (app->list.filterActive) {
            element = acll_nextFilter(element, ll_filterActive, NULL);
        } else {
            element = element->next;
        }
        position += LIST_ITEM_HEIGHT;
    }
}

void ll_dbInit(sqlite3 *db, char *tableName) {
    char *err_msg = 0;
    char *queryTmpl = "CREATE TABLE %tablename% (id INT PRIMARY KEY, name TEXT, active INT)";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
    free(query);
}


void ll_dbPersist(sqlite3 *db, char *tableName, acll_t *list) {
    char *err_msg = 0;
    char *queryTmpl = "DELETE FROM %tablename%";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }

    while (list != NULL) {
        addElement(db, tableName, getListItem(list)->name, getListItem(list)->active);
        list = list->next;
    }
    free(query);
}

void ll_dbLoad(sqlite3 *db, char *tableName, acll_t *list) {
    char *errmsg;
    char *queryTmpl = "SELECT name, active FROM %tablename%";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    int ret = sqlite3_exec(db, query, loadActivitiesCallback, list, &errmsg);
    if (ret != SQLITE_OK) {
        LOG_ERROR("Error in select statement %s [%s].", query, errmsg);
        free(query);
        return;
    }
}

static void addElement(sqlite3 *db, char *tableName, char *name, int active) {
    char *queryTmpl = "INSERT INTO %tablename% (name, active) VALUES (@name, @active)";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@name");
        sqlite3_bind_text(stmt, idx, name, strlen(name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@active");
        sqlite3_bind_int(stmt, idx, active);
    } else {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        LOG_ERROR("insert statement didn't return DONE (%i): %s", rc, sqlite3_errmsg(db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    free(query);
}

static int loadActivitiesCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
    acll_t *element = acll_find(p_data, filterName, p_fields[0]);
    if (element != NULL) {
        if (!strcmp(p_fields[1], "1")) {
            getListItem(element)->active = 1;
        } else {
            getListItem(element)->active = 0;
        }
    }
    return 0;
}

int ll_filterActive(void *payload, void *input) {
    listitem_t *item = payload;
    if (item->active) {
        return 1;
    }
    return 0;
}

static int filterName(void *payload, void *input) {
    listitem_t *item = payload;
    char *name = input;
    return !strcmp(item->name, name);
}