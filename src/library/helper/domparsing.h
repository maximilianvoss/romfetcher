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

#ifndef HELPER_DOMPARSING_H
#define HELPER_DOMPARSING_H

#include <source/lexbor/html/html.h>

lxb_dom_collection_t *domparsing_getElementsCollectionByClassName(char *htmlDOM, lxb_html_document_t **document,
                                                                  char *className);

lxb_dom_collection_t *domparsing_getElementsCollectionByTagName(char *htmlDOM, lxb_html_document_t **document,
                                                                char *tagName);

void domparsing_findChildElementsByTagName(lxb_dom_collection_t *collection, lxb_dom_element_t *element, char *tagName,
                                           uint8_t recursive);

lxb_dom_element_t *
domparser_findFirstChildElementByTagName(lxb_dom_element_t *element, char *tagName, uint8_t recursive);

void domparsing_findChildElementsByAttribute(lxb_dom_collection_t *collection, lxb_dom_element_t *element,
                                             char *attributeName,
                                             char *attributeValue, uint8_t contains,
                                             uint8_t recursive);

lxb_dom_element_t *
domparser_findFirstChildElementByAttribute(lxb_dom_element_t *element,
                                           char *attributeName,
                                           char *attributeValue, uint8_t contains,
                                           uint8_t recursive);

char *domparsing_getAttributeValue(lxb_dom_element_t *element, char *attributeName);

char *domparsing_getText(lxb_dom_element_t *element);

lxb_dom_collection_t *domparsing_createCollection(lxb_html_document_t *document);

#endif
