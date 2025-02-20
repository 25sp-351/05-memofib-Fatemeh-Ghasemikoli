#include "cache.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void _noop(void) {}
CacheStat *_noop_stats(void) { return NULL; }

Cache *load_cache_module(const char *library) {
    void *lib_handle = dlopen(library, RTLD_NOW | RTLD_NODELETE);
    if (!lib_handle) {
        fprintf(stderr, "Error loading: %s\n", dlerror());
        return NULL;
    }

    Cache *cache_instance = malloc(sizeof(Cache));
    Void_fptr init_function = (Void_fptr)dlsym(lib_handle, "initialize");
    cache_instance->set_provider_func = (SetProvider_fptr)dlsym(lib_handle, "set_provider");
    cache_instance->get_statistics = (Stats_fptr)dlsym(lib_handle, "statistics");
    cache_instance->reset_statistics = (Void_fptr)dlsym(lib_handle, "reset_statistics");
    cache_instance->cache_cleanup = (Void_fptr)dlsym(lib_handle, "cleanup");

    dlclose(lib_handle);

    // Assign default functions if any of them are missing
    if (!cache_instance->get_statistics) cache_instance->get_statistics = _noop_stats;
    if (!cache_instance->reset_statistics) cache_instance->reset_statistics = _noop;
    if (!cache_instance->cache_cleanup) cache_instance->cache_cleanup = _noop;

    // Ensure essential function is present
    if (!cache_instance->set_provider_func) {
        fprintf(stderr, "Error: Required function missing\n");
        free(cache_instance);
        return NULL;
    }

    // Call initialization function if available
    if (init_function) init_function();

    return cache_instance;
}

void display_cache_statistics(int fd, CacheStat *stats) {
    if (!stats) {
        dprintf(fd, "No cache statistics available\n");
        return;
    }

    printf("Cache Statistics:\n");
    for (CacheStat *ptr = stats; ptr->type != END_OF_STATS; ptr++) {
        dprintf(fd, "%-10s (%d) %4d\n", CacheStatNames[ptr->type], ptr->type, ptr->value);
    }
}
