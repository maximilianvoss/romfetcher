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

#ifndef CONFIG_H
#define CONFIG_H

#define DATABASE_SCHEMA_VERSION 210
#define DATABASE_SCHEMA_SUPPORTED_LEGACY_VERSION 200

#define DATABASE_TABLE_ENGINES "engines"
#define DATABASE_TABLE_SYSTEMS "systems"

#define HOMEDIR_PATH ".romfetcher"
#define DATABASE_FILE "romfetcher.db"
#define ROM_BASE_DIR "RetroPie/roms"
#define THEMES_SYSTEM_DIRS { "/usr/share/romfetcher/themes", "/usr/local/share/romfetcher/themes"};
#define THEME_HOME "themes"

#ifndef CONTROLLER_MAPPING
#define CONTROLLER_MAPPING "00000000526574726f53746f6e653200,RetroStone2,a:b0,b:b1,x:b2,y:b3,dpdown:b11,dpleft:b12,dpright:b13,dpup:b10,leftshoulder:b4,lefttrigger:b6,rightshoulder:b5,righttrigger:b7,start:b9,back:b8,leftx:a0,lefty:a1,platform:Linux"
#endif

#define COPYRIGHT "C Copyright 2020 by Maximilian Voss (maximilian@voss.rocks)"

#endif
