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

#include "inputlist.h"
#include "../state/statehandler.h"


static void up(app_t *app);

static void down(app_t *app);

void inputlist_processUp(app_t *app) {
    up(app);
}

void inputlist_processDown(app_t *app) {
    down(app);
}

void inputlist_processLeft(app_t *app) {
    up(app);
}

void inputlist_processRight(app_t *app) {
    down(app);
}

void inputlist_processSelect(app_t *app) {
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

void inputlist_processBack(app_t *app) {
    statehandler_switch(app, 0);
}

void inputlist_processOtherButton(app_t *app, GameControllerState_t *state) {
}

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