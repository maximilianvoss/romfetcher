#ifndef ROMFETCHER_H
#define ROMFETCHER_H

#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

// HTTP Method Enum
typedef enum {
    GET,
    POST
} httpmethod_t;

// Gaming System
typedef struct system_s {
    struct system_s *prev;
    struct system_s *next;
    char *fullname;
    uint8_t active;
    char *name;
    char *path;
} system_t;

// Callback to run downloads
typedef void (*downloadCallback_t)(void *appData, struct system_s *system, char *title, char *url, char *data,
                                   char *filename,
                                   httpmethod_t method);

// Memory Image or any other data object where data & size is needed
typedef struct memimage_s {
    char *binary;
    size_t size;
} memimage_t;

// Hoster of ROMs
typedef struct hoster_s {
    struct hoster_s *prev;
    struct hoster_s *next;
    char *fullname;
    uint8_t active;
    char *name;
    struct cache_s *cacheHandler;
    struct memimage_s *favicon;

    struct result_s *(*search)(struct system_s *system, char *searchString);

    void (*download)(struct result_s *item, downloadCallback_t downloadCallbackFunction, void *appData);
} hoster_t;

// Search Result
typedef struct result_s {
    struct result_s *prev;
    struct result_s *next;
    char *title;
    uint8_t active;
    char *url;
    struct system_s *system;
    struct hoster_s *hoster;
} result_t;

// Caching of search results or hoster's content
typedef struct cache_s {
    void *appData;

    uint8_t (*isValid)(struct hoster_s *hoster, struct system_s *system, void *appData);

    void (*clear)(struct hoster_s *hoster, struct system_s *system, void *appData);

    void (*add)(struct hoster_s *hoster, struct system_s *system, char *title, char *link, void *appData);

    result_t *(*get)(struct hoster_s *hoster, struct system_s *system, char *searchString, void *appData);

    void (*touch)(struct hoster_s *hoster, struct system_s *system, void *appData);
} cache_t;

// Definition of a linked list
typedef struct {
    void *prev;
    void *next;
    char *name;
    uint8_t active;
} linkedlist_t;

// Definition of a curl response
typedef struct curl_response_s {
    size_t size;
    char *data;
} curl_response_t;

// Exported methods
system_t *systems_init();

void systems_destroy(system_t *systems);

hoster_t *hosterhandler_init(cache_t *cacheHandler);

result_t *hosterhandler_search(hoster_t *hosters, system_t *system, char *searchString);

void hosterhandler_download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

void hosterhandler_destroy(hoster_t *hoster);

void result_freeList(result_t *resultList);

void *linkedlist_appendElement(void *list, void *element);

void *linkedlist_pop(void *list, void **element);

void *linkedlist_push(void *list, void *element);

void linkedlist_freeList(void *list, void (*callback)(void *));

uint32_t linkedlist_getElementCount(void *list);

void *linkedlist_findElementByName(void *ptr, char *name);

uint8_t linkedlist_isElementInList(void *list, void *element);

void *linkedlist_deleteElement(void *list, void *element, void (*callback)(void *));

void *linkedlist_removeElement(void *list, void *element);

void *linkedlist_sort(void *ptr);

void *linkedlist_clone(void *ptr, size_t size, void (*callback)(void *));

void *linkedlist_getFirstActive(void *ptr);

void *linkedlist_getPrevActive(void *ptr);

void *linkedlist_getNextActive(void *ptr);

void *linkedlist_getLastElement(void *list);

int
curlling_download(char *url, char *data, httpmethod_t method, char *filename, curl_off_t *current, curl_off_t *total,
                  volatile uint8_t *cancellation);

curl_response_t *curlling_fetch(char *url, char *postData, httpmethod_t method, long throwHeaderOut);

void curl_freeResponse(curl_response_t *response);

// nice macros for method calling
#define loadSystems systems_init
#define destroySystems(systems) systems_destroy(systems)
#define loadHosters hosterhandler_init
#define destroyHosters(hosters) hosterhandler_destroy(hosters)
#define searchHosters(hosters, system, searchString) hosterhandler_search(hosters, system, searchString)
#define downloadItem(item, downloadCallbackFunction, appData) hosterhandler_download(item, downloadCallbackFunction, appData)
#define destroyResults(results) result_freeList(results)

#define ll_append(list, element) linkedlist_appendElement(list, element)
#define ll_pop(list, element) linkedlist_pop(list, element)
#define ll_push(list, element) linkedlist_push(list, element)
#define ll_free(list, callback) linkedlist_freeList(list, callback)
#define ll_count(list) linkedlist_getElementCount(list)
#define ll_findByName(list, name) linkedlist_findElementByName(list, name)
#define ll_isInList(list, element) linkedlist_isElementInList(list, element)
#define ll_delete(list, element, callback) linkedlist_deleteElement(list, element, callback)
#define ll_remove(list, element) linkedlist_removeElement(list, element)
#define ll_sort(list) linkedlist_sort(list)
#define ll_clone(list, size, callback) linkedlist_clone(list, size, callback)
#define ll_get1stActive(list) linkedlist_getFirstActive(list)
#define ll_getActivePrev(element) linkedlist_getPrevActive(element)
#define ll_getActiveNext(element) linkedlist_getNextActive(element)
#define ll_getLast(list) linkedlist_getLastElement(list)

// define the number of parallel threads to fetch content from hosters
#ifndef HOSTER_FETCH_THREADS
#define HOSTER_FETCH_THREADS 5
#endif

// default loglevel
#ifndef LOG_LEVEL
#define LOG_LEVEL 2
#endif

// log output
#ifndef LOG_DEBUG
#define LOG_DEBUG(fmt, ...) if ( LOG_LEVEL > 1) { printf("DEBUG: "); printf(fmt, ##__VA_ARGS__);}
#endif

#ifndef LOG_INFO
#define LOG_INFO(fmt, ...) if ( LOG_LEVEL > 0 ) { printf("INFO: "); printf(fmt, ##__VA_ARGS__); }
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(fmt, ...) { printf("ERROR: "); printf(fmt, ##__VA_ARGS__);}
#endif

#endif
