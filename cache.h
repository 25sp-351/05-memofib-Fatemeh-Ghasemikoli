#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>  // Include GMP for big integer support

// Key and value types used in caching
typedef int KeyType;
#define KEY_FMT "%d"

// Function type for the cached provider (Supports GMP)
typedef void (*ProviderFunction)(KeyType key, mpz_t result);  

// Structure to store cache statistics
typedef struct cachestat {
    enum Stat_type {
        END_OF_STATS=0,
        Cache_requests=1,
        Cache_hits=2,
        Cache_misses=3,
        Cache_evictions=4,
        Cache_size=5
    } type;
    int value;
} CacheStat;

static char *CacheStatNames[] = {
    "",
    "requests",
    "hits",
    "misses",
    "evictions",
    "size"
};

// Function pointer types
typedef void (*Void_fptr)(void);
typedef ProviderFunction (*SetProvider_fptr)(ProviderFunction);
typedef CacheStat* (*Stats_fptr)(void);

// Cache structure with function pointers for operations
typedef struct chooks {
    SetProvider_fptr set_provider_func;
    Stats_fptr get_statistics;
    Void_fptr reset_statistics;
    Void_fptr cache_cleanup;
} Cache;

// Loads a shared cache module, returns NULL on failure
Cache *load_cache_module(const char *libname);

// Utility functions for cache statistics
void print_cache_stats(int fd, CacheStat *stats);
void display_cache_statistics(int fd, CacheStat *stats);

/* HOW TO WRITE A LOADABLE CACHE MODULE */
#if CACHE_MODULE_REQUIREMENTS

// Called when loading the cache module
void initialize(void);

// Sets the provider function for caching
ProviderFunction set_provider(ProviderFunction downstream);

// Returns cache statistics (caller must free the result)
CacheStat* statistics(void);

// Resets cache statistics to default values
void reset_statistics(void);

// Cleans up the cache before program exit
void cleanup(void);

#endif
#endif  // CACHE_H
