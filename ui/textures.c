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

#include "textures.h"
#include <SDL_image.h>
#include "rendering.h"

void textures_init(app_t *app) {
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    app->textures.backgroundImage = rendering_loadImage(app, app->theme->images.background);
    app->textures.searchChevron = rendering_loadImage(app, app->theme->images.selectorIcon);
    app->textures.settingsIcon = rendering_loadImage(app, app->theme->images.settingsIcon);
    app->textures.checkboxChecked = rendering_loadImage(app, app->theme->images.checkboxChecked);
    app->textures.checkboxUnchecked = rendering_loadImage(app, app->theme->images.checkboxUnchecked);
}

void textures_destroy(app_t *app) {
    SDL_DestroyTexture(app->textures.backgroundImage);
    SDL_DestroyTexture(app->textures.searchChevron);
    SDL_DestroyTexture(app->textures.settingsIcon);
    SDL_DestroyTexture(app->textures.checkboxChecked);
    SDL_DestroyTexture(app->textures.checkboxUnchecked);
}
