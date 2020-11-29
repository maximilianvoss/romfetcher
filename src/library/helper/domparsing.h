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
