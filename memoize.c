#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "cache.h"

#define MAX_KEY 150  
#define CACHE_SIZE MAX_KEY + 1

char cache[CACHE_SIZE][310];  // Store Fibonacci numbers as strings

// Statistics
int cache_requests;
int cache_hits;
int cache_misses;

ProviderFunction _downstream = NULL; // Function pointer to downstream provider


/**
 * Initializes the memoization data structures.
 */
void initialize(void) {
    fprintf(stderr, __FILE__ " initialize()\n");

    cache_requests = 0;
    cache_hits     = 0;
    cache_misses   = 0;

    for (int ix = 0; ix < CACHE_SIZE; ix++)
        cache[ix][0] = '\0'; // Initialize empty strings
}

/**
 * Cleans up the memoization data structures.
 */
void cleanup(void) {
    fprintf(stderr, __FILE__ " cleanup()\n");
}

/**
 * Resets the statistics.
 */
void reset_statistics(void) {
    fprintf(stderr, __FILE__ " reset_statistics()\n");
    cache_requests = 0;
    cache_hits     = 0;
    cache_misses   = 0;
}

// Returns the statistics
CacheStat* statistics(void) {
    fprintf(stderr, __FILE__ " statistics()\n");

    CacheStat* stats_cache = malloc(4 * sizeof(CacheStat));
    stats_cache[0] = (CacheStat){Cache_requests, cache_requests};
    stats_cache[1] = (CacheStat){Cache_hits, cache_hits};
    stats_cache[2] = (CacheStat){Cache_misses, cache_misses};
    stats_cache[3] = (CacheStat){Cache_size, CACHE_SIZE};
    stats_cache[4] = (CacheStat){END_OF_STATS, 0};

    return stats_cache;
}

bool _is_present(KeyType key) { // Check if key is present in the cache
    return key >= 0 && key <= MAX_KEY && cache[key][0] != '\0';
}

void _insert(KeyType key, const char* value) { // Insert a value into the cache
    if (key < 0 || key > MAX_KEY)
        return;
    snprintf(cache[key], 310, "%s", value);  // Store as a string
}

void _get(KeyType key, mpz_t result) { // Get a value from the cache
    if (key < 0 || key > MAX_KEY)
        mpz_set_ui(result, 0);
    else
        mpz_set_str(result, cache[key], 10);
}

// Caching Fibonacci function
void _caching_provider(int key, mpz_t result) {
    cache_requests++;

    if (_is_present(key)) {
        cache_hits++;
        _get(key, result);
        return;
    } else {
        cache_misses++;
    }

    _downstream(key, result);
    char result_str[310];
    mpz_get_str(result_str, 10, result);
    _insert(key, result_str);
}

// Set the provider function for caching
ProviderFunction set_provider(ProviderFunction downstream) {
    fprintf(stderr, __FILE__ " set_provider()\n");
    _downstream = downstream;
    return _caching_provider;
}
