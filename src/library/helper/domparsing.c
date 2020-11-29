#include <romfetcher.h>
#include "domparsing.h"
#include "../../common/utils.h"

#define COLLECTION_INIT_SIZE 20

static lxb_html_document_t *readDocument(char *htmlDOM);


//lxb_dom_collection_destroy(collection, true);
//lxb_html_document_destroy(document);

lxb_dom_collection_t *domparsing_getElementsCollectionByClassName(char *htmlDOM, lxb_html_document_t **document,
                                                                  char *className) {
    lxb_status_t status;
    *document = readDocument(htmlDOM);

    lxb_dom_collection_t *collection = lxb_dom_collection_make(&(*document)->dom_document, COLLECTION_INIT_SIZE);
    if (collection == NULL) {
        LOG_ERROR("Failed to create Collection object");
        return NULL;
    }

    status = lxb_dom_elements_by_class_name(lxb_dom_interface_element((*document)->body), collection,
                                            (const lxb_char_t *) className, strlen(className));
    if (status != LXB_STATUS_OK) {
        LOG_ERROR("Failed to get elements by class name");
        return NULL;
    }

    return collection;
}

lxb_dom_collection_t *domparsing_getElementsCollectionByTagName(char *htmlDOM, lxb_html_document_t **document,
                                                                char *tagName) {
    lxb_status_t status;
    *document = readDocument(htmlDOM);

    lxb_dom_collection_t *collection = lxb_dom_collection_make(&(*document)->dom_document, COLLECTION_INIT_SIZE);
    if (collection == NULL) {
        LOG_ERROR("Failed to create Collection object");
        return NULL;
    }

    status = lxb_dom_elements_by_tag_name(lxb_dom_interface_element((*document)->body), collection,
                                          (const lxb_char_t *) tagName, strlen(tagName));
    if (status != LXB_STATUS_OK) {
        LOG_ERROR("Failed to get elements by class name");
        return NULL;
    }

    return collection;
}

lxb_dom_collection_t *domparsing_createCollection(lxb_html_document_t *document) {
    lxb_dom_collection_t *collection = lxb_dom_collection_make(&document->dom_document, COLLECTION_INIT_SIZE);
    if (collection == NULL) {
        LOG_ERROR("Failed to create Collection object");
    }
    return collection;
}

void domparsing_findChildElementsByTagName(lxb_dom_collection_t *collection, lxb_dom_element_t *element, char *tagName,
                                           uint8_t recursive) {
    size_t len;
    lxb_dom_node_t *node = element->node.first_child;

    while (node != NULL) {
        if (node->type != LXB_DOM_NODE_TYPE_ELEMENT) {
            node = node->next;
            continue;
        }

        lxb_dom_element_t *childElement = lxb_dom_interface_element(node);

        char *nodeName = (char *) lxb_dom_node_name(node, &len);
        if (!strcmp(nodeName, tagName)) {
            lxb_dom_collection_append(collection, childElement);
        }

        if (recursive) {
            domparsing_findChildElementsByTagName(collection, childElement, tagName, recursive);
        }
        node = node->next;
    }
}

lxb_dom_element_t *
domparser_findFirstChildElementByTagName(lxb_dom_element_t *element, char *tagName, uint8_t recursive) {
    size_t len;
    lxb_dom_node_t *node = element->node.first_child;

    while (node != NULL) {
        if (node->type != LXB_DOM_NODE_TYPE_ELEMENT) {
            node = node->next;
            continue;
        }

        lxb_dom_element_t *childElement = lxb_dom_interface_element(node);

        char *nodeName = (char *) lxb_dom_node_name(node, &len);
        if (!strcmp(nodeName, tagName)) {
            return childElement;
        }

        if (recursive) {
            return domparser_findFirstChildElementByTagName(childElement, tagName, recursive);
        }
        node = node->next;
    }
    return NULL;
}

void domparsing_findChildElementsByAttribute(lxb_dom_collection_t *collection, lxb_dom_element_t *element,
                                             char *attributeName,
                                             char *attributeValue, uint8_t contains,
                                             uint8_t recursive) {

    size_t len;
    lxb_dom_node_t *node = element->node.first_child;
    while (node != NULL) {
        if (node->type != LXB_DOM_NODE_TYPE_ELEMENT) {
            node = node->next;
            continue;
        }

        lxb_dom_element_t *childElement = lxb_dom_interface_element(node);
        lxb_dom_attr_t *attr = childElement->first_attr;
        while (attr != NULL) {
            char *name = (char *) lxb_dom_attr_local_name(attr, &len);
            if (name != NULL && !strcmp(name, attributeName)) {
                char *value = (char *) lxb_dom_attr_value(attr, &len);
                if (value != NULL) {
                    if (contains) {
                        if (str_contains(value, attributeValue)) {
                            lxb_dom_collection_append(collection, childElement);
                        }
                    } else {
                        if (!strcmp(value, attributeValue)) {
                            lxb_dom_collection_append(collection, childElement);
                        }
                    }
                }
            }
            attr = attr->next;
        }
        if (recursive) {
            domparsing_findChildElementsByAttribute(collection, childElement, attributeName, attributeValue, contains,
                                                    recursive);
        }

        node = node->next;
    }
}

lxb_dom_element_t *
domparser_findFirstChildElementByAttribute(lxb_dom_element_t *element,
                                           char *attributeName,
                                           char *attributeValue, uint8_t contains,
                                           uint8_t recursive) {
    size_t len;
    lxb_dom_node_t *node = element->node.first_child;
    while (node != NULL) {
        if (node->type != LXB_DOM_NODE_TYPE_ELEMENT) {
            node = node->next;
            continue;
        }

        lxb_dom_element_t *childElement = lxb_dom_interface_element(node);
        lxb_dom_attr_t *attr = childElement->first_attr;
        while (attr != NULL) {
            char *name = (char *) lxb_dom_attr_local_name(attr, &len);
            if (name != NULL && !strcmp(name, attributeName)) {
                char *value = (char *) lxb_dom_attr_value(attr, &len);
                if (value != NULL) {
                    if (contains) {
                        if (str_contains(value, attributeValue)) {
                            return childElement;
                        }
                    } else {
                        if (!strcmp(value, attributeValue)) {
                            return childElement;
                        }
                    }
                }
            }
            attr = attr->next;
        }
        if (recursive) {
            return domparser_findFirstChildElementByAttribute(childElement, attributeName, attributeValue, contains,
                                                              recursive);
        }

        node = node->next;
    }
    return NULL;
}

char *domparsing_getAttributeValue(lxb_dom_element_t *element, char *attributeName) {
    size_t len;
    lxb_dom_attr_t *attr = element->first_attr;
    while (attr != NULL) {
        char *name = (char *) lxb_dom_attr_local_name(attr, &len);
        if (name != NULL && !strcmp(name, attributeName)) {
            char *value = (char *) lxb_dom_attr_value(attr, &len);
            return value;
        }
        attr = attr->next;
    }
    return NULL;
}

char *domparsing_getText(lxb_dom_element_t *element) {
    size_t len;
    char *text = (char *) lxb_dom_node_text_content(&element->node, &len);
    return text;
}

static lxb_html_document_t *readDocument(char *htmlDOM) {
    lxb_html_parser_t *parser = lxb_html_parser_create();
    lxb_status_t status = lxb_html_parser_init(parser);
    if (status != LXB_STATUS_OK) {
        LOG_ERROR("Failed to create HTML parser");
        return NULL;
    }

    lxb_html_document_t *document = lxb_html_parse(parser, (const lxb_char_t *) htmlDOM, strlen(htmlDOM));
    if (document == NULL) {
        LOG_ERROR("Failed to create Document object");
        return NULL;
    }

    lxb_html_parser_destroy(parser);
    return document;
}