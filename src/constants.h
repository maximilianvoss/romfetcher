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

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define DATABASE_SCHEMA_VERSION 300
#define DATABASE_SCHEMA_SUPPORTED_LEGACY_VERSION 300

#define DATABASE_TABLE_HOSTER "hoster"
#define DATABASE_TABLE_SYSTEMS "systems"

#define HOMEDIR_PATH ".romfetcher"
#define DATABASE_FILE "romfetcher.db"
#define ROM_BASE_DIR "RetroPie/roms"
#define THEMES_SYSTEM_DIRS { "/usr/share/romfetcher/themes", "/usr/local/share/romfetcher/themes"};
#define THEME_HOME "themes"

#ifndef CONTROLLER_MAPPING
#define CONTROLLER_MAPPING "00000000526574726f53746f6e653200,RetroStone2,a:b0,b:b1,x:b2,y:b3,dpdown:b11,dpleft:b12,dpright:b13,dpup:b10,leftshoulder:b4,lefttrigger:b6,rightshoulder:b5,righttrigger:b7,start:b9,back:b8,leftx:a0,lefty:a1,platform:Linux"
#endif

#ifndef DOWNLOADER_THREADS
#define DOWNLOADER_THREADS 2
#endif

#ifndef LIST_ITEM_HEIGHT
#define LIST_ITEM_HEIGHT 30
#endif

#ifndef PADDING_SIDES
#define PADDING_SIDES 20
#endif

#ifndef PADDING_TOP
#define PADDING_TOP 50
#endif

#ifndef PADDING_BOTTOM
#define PADDING_BOTTOM 50
#endif

#ifndef MAX_SEARCHTEXT_LENGTH
#define MAX_SEARCHTEXT_LENGTH 255
#endif

#ifndef RL_DYNAMIC_LIBRARY
#ifdef UNIX
#define RL_DYNAMIC_LIBRARY "libromlibrary.so"
#endif
#ifdef MACOS
#define RL_DYNAMIC_LIBRARY "libromlibrary.dylib"
#endif
#endif

#define COPYRIGHT "C Copyright 2020 - 2023 by Maximilian Voss (maximilian@voss.rocks)"

#endif
