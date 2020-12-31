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

#include "uidownloadmanager.h"
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "downloadpipeline.h"

static void renderProgressBar(app_t *app, download_t *element, int position);

void uidownloadmanager_render(app_t *app) {
    int width, height;
    int offsetX = 50;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;

    int deviceCountToDisplay = (height - offsetX - 80) / 65 + 1;

    download_t *element = (app->download.cursor != NULL) ? app->download.cursor : app->download.active;
    if (element == NULL) {
        return;
    }

    int i = 0;
    while (i < deviceCountToDisplay / 2 - 1) {
        download_t *prev = downloadpipeline_getPrev(app, element);
        if (prev == NULL) {
            break;
        }
        element = prev;
        i++;
    }

    int position = offsetX;
    while (element != NULL && position <= height - 80) {
        rendering_loadText(app, &texture, element->title, app->themes.active->fonts.font26,
                           &app->themes.active->colors.text);

        SDL_Rect r2 = {48, position - 2, width - 96, 70};
        themes_setDrawColorBackground(app, (element == app->download.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &r2);

        SDL_Rect r = {50, position, width - 100, 68};
        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &r);

        SDL_Rect srcQuad = {0, 0, width - 120, texture.h};
        SDL_Rect renderQuad = {60, position + 3, (texture.w > width - 120) ? width - 120 : texture.w, texture.h};

        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);

        renderProgressBar(app, element, position);

        element = downloadpipeline_getNext(app, element);
        position += 65;
    }
}


static void renderProgressBar(app_t *app, download_t *element, int position) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect rect1 = {100, position + 35, width - 200, 20};
    themes_setDrawColor(app, fieldBackground);
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {100 + 2, position + 35 + 2, width - 200 - 4, 20 - 4};
    themes_setDrawColor(app, field);
    SDL_SetRenderDrawColor(app->sdlRenderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);

    double percentage;
    char percentText[10];
    if (linkedlist_isElementInList(app->download.active, element)) {
        percentage = element->total != 0 ? (double) element->current / (double) element->total : 0.0;
        snprintf(percentText, 5, "%.0f%c", percentage * 100, '%');
    } else if (linkedlist_isElementInList(app->download.queue, element)) {
        percentage = 0.0f;
        strcpy (percentText, "QUEUED");
    } else {
        if (element->cancelled) {
            percentage = 0.0f;
            strcpy (percentText, "CANCELLED");
        } else {
            percentage = 1.0f;
            strcpy (percentText, "DONE");
        }
    }

    SDL_Rect rect3 = {100 + 2, position + 35 + 2, (width - 200 - 4) * percentage, 20 - 4};
    themes_setDrawColor(app, fieldActive);
    SDL_RenderFillRect(app->sdlRenderer, &rect3);

    texture_t texture;
    rendering_loadText(app, &texture, percentText, app->themes.active->fonts.font16, &app->themes.active->colors.text);
    SDL_Rect renderQuad = {width / 2 - texture.w / 2, position + 35 + 3, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture.texture);
}