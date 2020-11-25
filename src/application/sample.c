//
// Created by voss on 2020-11-24.
//

#include "../library/romfetcher.h"
#include "../library/helper/linkedlist.h"


void test(void *appData, system_t *system, char *title, char *url, char *data, char *filename,
          httpmethod_t method) {
    printf("\n");
    printf("AppData: %s\n", appData);
    printf("System: %s\n", system->fullname);
    printf("Title: %s\n", title);
    printf("URL: %s\n", url);
    printf("DATA: %s\n", data);
    printf("Filename: %s\n", filename);
}

uint8_t isValid(struct hoster_s *hoster, struct system_s *system, void *appData) {
    LOG_DEBUG("is valid\n");
    return 0;
}

void clear(struct hoster_s *hoster, struct system_s *system, void *appData) {
    LOG_DEBUG("clear\n");
}

void add(struct hoster_s *hoster, struct system_s *system, char *title, char *link, void *appData) {
    LOG_DEBUG("add: %s - %s\n", title, link);
}

result_t *get(struct hoster_s *hoster, struct system_s *system, char *searchString, void *appData) {
    LOG_DEBUG("get\n");
    return NULL;
}

void touch(struct hoster_s *hoster, struct system_s *system, void *appData) {
    LOG_DEBUG("touch\n");
}


int main() {

    cache_t cacheHandler;
    cacheHandler.appData = "Hello World";
    cacheHandler.add = add;
    cacheHandler.get = get;
    cacheHandler.touch = touch;
    cacheHandler.clear = clear;
    cacheHandler.isValid = isValid;

    system_t *systems = loadSystems();
    hoster_t *hosters = loadHosters(&cacheHandler);

//    hoster_t *myhoster = linkedlist_findElementByName(hosters, "https://romhustler.org");
//    hoster_t *myhoster = linkedlist_findElementByName(hosters, "https://www.roms-download.com");
//    hoster_t *myhoster = linkedlist_findElementByName(hosters, "https://www.romsemulator.net");
//    hoster_t *myhoster = linkedlist_findElementByName(hosters, "https://www.romsmania.cc");
//    hoster_t *myhoster = linkedlist_findElementByName(hosters, "https://www.wowroms.com");
    hoster_t *myhoster = linkedlist_findElementByName(hosters, "https://freeroms.com");
    system_t *mysystem = linkedlist_findElementByName(systems, "Game Boy");

    if (myhoster == NULL) printf("myhoster null\n");
    if (mysystem == NULL) printf("mysystem null\n");

    result_t *results = searchHosters(myhoster, mysystem, "Mario");
    if (results == NULL) printf("results null\n");

    LOG_INFO("%s\n", results->title);
    LOG_ERROR("%s\n", results->title);

    downloadItem(results, &test, "hello world");

    destroySystems(systems);
    destroyHosters(hosters);
}

