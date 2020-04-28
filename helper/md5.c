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

#include "md5.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_BUFFER_SIZE 4096

char *md5_file2md5(char *filename) {
    int i;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char *) calloc(sizeof(char), 33);
    char *buffer = calloc(sizeof(char), FILE_BUFFER_SIZE);

    MD5_Init(&c);

    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        return NULL;
    }
    while (!feof(stream)) {
        size_t size = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, stream);
        MD5_Update(&c, buffer, size);
    }
    fclose(stream);
    free(buffer);

    MD5_Final(digest, &c);
    for (i = 0; i < 16; ++i) {
        snprintf(&(out[i * 2]), 16 * 2, "%02x", (unsigned int) digest[i]);
    }

    return out;
}

char *md5_str2md5(char *string) {
    int i;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char *) calloc(sizeof(char), 33);

    MD5_Init(&c);
    MD5_Update(&c, string, strlen(string));
    MD5_Final(digest, &c);

    for (i = 0; i < 16; ++i) {
        snprintf(&(out[i * 2]), 16 * 2, "%02x", (unsigned int) digest[i]);
    }

    return out;
}