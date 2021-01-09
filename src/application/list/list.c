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
#include "../constants.h"
#include "../helper/uihelper.h"

static void up(app_t *app);

static void down(app_t *app);

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
        if (app->list.cursor->active) {
            app->list.cursor->active = 0;
        } else {
            app->list.cursor->active = 1;
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
        linkedlist_t *prev = linkedlist_getPrevActive(app->list.cursor);
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
        linkedlist_t *next = linkedlist_getNextActive(app->list.cursor);
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

    int deviceCountToDisplay = (height - offset - 80) / LIST_ITEM_HEIGHT + 1;

    linkedlist_t *element = (app->list.cursor != NULL) ? app->list.cursor : app->list.all;
    if (element == NULL) {
        return;
    }

    int i = 0;
    while (i < deviceCountToDisplay / 2 - 1) {
        if (app->list.filterActive) {
            if (linkedlist_getPrevActive(element) == NULL) {
                break;
            }
            element = linkedlist_getPrevActive(element);
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

        rendering_loadText(app->sdlRenderer, &texture, element->name, app->themes.active->fonts.font24,
                           themes_getDrawColorFieldText(app, (element == app->list.cursor)));
        if (app->list.checkbox) {
            uiElementRects_t checkboxRects = uihelper_generateRects(rects.inner.x + 3, rects.content.y, rects.content.h,
                                                                    rects.content.h);
            uihelper_renderSDLTexture(app->sdlRenderer,
                                      element->active ? app->themes.active->images.checkboxChecked
                                                      : app->themes.active->images.checkboxUnchecked,
                                      &checkboxRects.inner);
            rects.content.x += rects.content.h;
            rects.content.w -= rects.content.h;
            uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        } else {
            uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        }
        uihelper_destroyTexture(&texture);

        if (app->list.filterActive) {
            element = linkedlist_getNextActive(element);
        } else {
            element = element->next;
        }
        position += LIST_ITEM_HEIGHT;
    }
}