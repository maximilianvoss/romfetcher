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

#include "themes.h"


#ifdef DEVELOP
#define START_IN_WINDOW
#define BACKGROUND_IMAGE "/usr/local/share/romfetcher/wallpaper.jpg"
#define SELECTOR_ICON "/usr/local/share/romfetcher/selector.png"
#define SETTINGS_ICON "/usr/local/share/romfetcher/settings.png"
#define CHECKBOX_CHECKED_ICON "/usr/local/share/romfetcher/checkbox_checked.png"
#define CHECKBOX_UNCHECKED_ICON "/usr/local/share/romfetcher/checkbox_unchecked.png"
#define FONTS "/usr/local/share/romfetcher/FreeSans.ttf"
#else
#define BACKGROUND_IMAGE "/usr/share/romfetcher/wallpaper.jpg"
#define SELECTOR_ICON "/usr/share/romfetcher/selector.png"
#define SETTINGS_ICON "/usr/share/romfetcher/settings.png"
#define CHECKBOX_CHECKED_ICON "/usr/share/romfetcher/checkbox_checked.png"
#define CHECKBOX_UNCHECKED_ICON "/usr/share/romfetcher/checkbox_unchecked.png"
#define FONTS "/usr/share/romfetcher/FreeSans.ttf"
#endif

static void colorSetter(SDL_Color *color, int r, int g, int b, int a);

void themes_init(app_t *app) {
    app->theme = (theme_t *) calloc(1, sizeof(theme_t));
    colorSetter(&app->theme->colors.text, 0, 0, 0, 255);
    colorSetter(&app->theme->colors.textHighlight, 0, 0, 255, 255);
    colorSetter(&app->theme->colors.textInverted, 255, 255, 255, 255);
    app->theme->font = FONTS;
    app->theme->images.background = BACKGROUND_IMAGE;
    app->theme->images.checkboxChecked = CHECKBOX_CHECKED_ICON;
    app->theme->images.checkboxUnchecked = CHECKBOX_UNCHECKED_ICON;
    app->theme->images.selectorIcon = SELECTOR_ICON;
    app->theme->images.settingsIcon = SETTINGS_ICON;
}

void themes_destroy(app_t *app) {
    free(app->theme);
}

static void colorSetter(SDL_Color *color, int r, int g, int b, int a) {
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;
}