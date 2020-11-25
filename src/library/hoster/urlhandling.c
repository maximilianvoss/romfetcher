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

#include "urlhandling.h"
#include "../../common/utils.h"

char *urlhandling_substitudeVariables(char *templateString, system_t *system, char *(*mapping)(system_t *system),
                                      char *query, uint32_t page) {

    char *systemStr = mapping(system);
    if (systemStr == NULL) {
        LOG_ERROR("Found no mapping for system: %s\n", system->fullname);
        return NULL;
    }

    char *result;
    char *tmp;
    char pageStr[11];
    sprintf(pageStr, "%d", page);

    result = str_replace(templateString, "%page%", pageStr);

    tmp = result;
    result = str_replace(tmp, "%query%", query);
    free(tmp);

    tmp = result;
    result = str_replace(tmp, "%system%", systemStr);
    free(tmp);

    return result;
}
