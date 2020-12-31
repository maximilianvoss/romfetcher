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


#include "uilist.h"
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "../constants.h"
#include "../helper/uihelper.h"

void uilist_renderDefault(app_t *app) {
    uilist_renderList(app, 50);
}

void uilist_renderList(app_t *app, int offset) {
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
    while (element != NULL && position <= height - 80) {
        uiElementRects_t rects = uihelper_generateRectsFullScreenWidth(20, position, width, LIST_ITEM_HEIGHT);
        themes_setDrawColorBackground(app, (element == app->list.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.outter);
        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &rects.inner);

        rendering_loadText(app->sdlRenderer, &texture, element->name, app->themes.active->fonts.font24,
                           &app->themes.active->colors.text);
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
