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

#include <SDL_render.h>
#include "rendering.h"

void themes_setDrawColorField(app_t *app) {
    themes_setDrawColor(app, field);
}

void themes_setDrawColorBackground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, fieldActive);
    } else {
        themes_setDrawColor(app, fieldBackground);
    }
}

void themes_setDrawColor(app_t *app, enum theme_color color) {
    int r, g, b, a;
    switch (color) {
        case field:
            r = app->themes.enabled->colors.field.r;
            g = app->themes.enabled->colors.field.g;
            b = app->themes.enabled->colors.field.b;
            a = app->themes.enabled->colors.field.a;
            break;
        case fieldBackground:
            r = app->themes.enabled->colors.fieldBackground.r;
            g = app->themes.enabled->colors.fieldBackground.g;
            b = app->themes.enabled->colors.fieldBackground.b;
            a = app->themes.enabled->colors.fieldBackground.a;
            break;
        case fieldActive:
            r = app->themes.enabled->colors.fieldHighlight.r;
            g = app->themes.enabled->colors.fieldHighlight.g;
            b = app->themes.enabled->colors.fieldHighlight.b;
            a = app->themes.enabled->colors.fieldHighlight.a;
            break;
    }
    SDL_SetRenderDrawColor(app->renderer, r, g, b, a);
}