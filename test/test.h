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


#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <string.h>
#include <stdio.h>
#include "../src/helper/utils.h"

#define TESTCALL(TESTNAME, TESTMETHOD, DATA)\
    printf("\nTest case:\t%s\n", TESTNAME);\
    if ( argv[1] == NULL ) \
        TESTMETHOD(DATA);\
    else\
        if (!strcmp(argv[1], TESTNAME))\
            return TESTMETHOD(DATA);

#define ASSERTSTR(EXPECTED, ACTUAL)\
    printf("Expected: \t%s\nActual: \t%s\n\n", EXPECTED, ACTUAL);\
    if (strcmp(EXPECTED, ACTUAL))\
        return 1;

#define ASSERTINT(EXPECTED, ACTUAL)\
    printf("Expected: \t%d\nActual: \t%d\n\n", EXPECTED, ACTUAL);\
    if (EXPECTED != ACTUAL)\
        return 1;

#define ASSERTLONG(EXPECTED, ACTUAL)\
    printf("Expected: \t%ld\nActual: \t%ld\n\n", EXPECTED, ACTUAL);\
    if (EXPECTED != ACTUAL)\
        return 1;

#define ASSERTNOTNULL(EXPECTED)\
    if (EXPECTED == NULL) {\
        printf("Expected to be NOT NULL\n");\
        return 1;\
    }

#define ASSERTNULL(EXPECTED)\
    if (EXPECTED != NULL) {\
        printf("Expected to be NULL\n");\
        return 1;\
    }

#define ASSERTFILEEXISTS(PATH)\
    printf("Expects file to exist: %s\n", PATH);\
    if ( ! file_exists(PATH) ) {\
        printf("File: %s doesn't exist\n", PATH);\
        return 1;\
    }

#define ASSERTPTRNOTEQUAL(VALUE1, VALUE2)\
    if ( VALUE1 == VALUE2 ) {\
        printf("Expected that %p is not %p\n", VALUE1, VALUE2);\
        return 1;\
    }

void test_downloadCallback(void *app);

#endif
