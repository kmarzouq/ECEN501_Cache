/*******************************************************
 * cache data structure for programming assignment #2
 *******************************************************/
#include "cache.h"

/* initalize the cache memory with the given set of parameters */
Cache* initCache(unsigned int addr_width, unsigned int line_size, unsigned int mem_size, unsigned int cache_size, unsigned int assoc, unsigned int policy) {

    Cache* cache = (Cache*)malloc(sizeof(Cache));

    printf("addr_width %u, line_size %u, mem_size %u, cache_size %u, assoc %u, pol %u\n",
                    addr_width,
                    line_size,
                    mem_size,
                    cache_size,
                    assoc,
                    policy);

    
    cache->addr_width = addr_width;
    cache->line_size = line_size;
    cache->mem_size = mem_size;
    cache->cache_size = cache_size;
    cache->assoc = assoc;
    cache->policy = policy;

    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    // TO BE COMPLETED FOR PART 0
    // How many sets?
    int num_lines = (1<<cache_size)/(1<<line_size); // total number of cache lines
    //int num_sets = num_lines/(assoc+1); // total number of sets (fully-associative: 1 set)
    int num_sets;
    if (assoc==0){
        num_sets=1;
    }
    else{
        num_sets=num_lines/assoc;
    }
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////

    // Memory allocation for sets
    cache->sets = (CacheSet*)malloc(num_sets * sizeof(CacheSet));

    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    // TO BE COMPLETED FOR PART 0
    // Set initialization
    for (int i = 0; i < num_sets; i++) {

        // TO BE
        if (assoc==0){
            cache->sets[i].lines = (CacheLine*)malloc(sizeof(CacheLine)*num_lines);
        }
        else{
            cache->sets[i].lines = (CacheLine*)malloc(sizeof(CacheLine)*assoc);
        }
        
        //add for assoc = 0;

        // cache line initialization
        for (int j = 0; j < assoc; j++) {

            // initialization for Valid
            cache->sets[i].lines[j].valid = false;

            // calculate the number of bits in a Tag (addr_width - set_index_bits - line_offset_bits)
            int line_offset_bits = line_size;
            int set_index_bits = cache_size;
            int tag_size = addr_width-set_index_bits-line_offset_bits;

            // Initalize Tag/Data //changed from ... to -1
            cache->sets[i].lines[j].tag = createBitArray(tag_size);//create bit array here
            cache->sets[i].lines[j].data = 0;//
        }
    }
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////

    // Stats initialization
    cache->stats = (CacheStats*)malloc(sizeof(CacheStats));
    cache->stats->num_of_total_accesses = 0;
    cache->stats->num_of_hits = 0;
    cache->stats->num_of_total_misses = 0;
    cache->stats->num_of_compulsory_misses = 0;
    cache->stats->num_of_capacity_misses = 0;
    cache->stats->num_of_conflict_misses = 0;
    cache->stats->hit_rate = 0.0;

    //print_cache_config(cache);

    return cache;
}

void freeCache(Cache* cache) {
    int num_lines = (1 << (cache->cache_size - cache->line_size));
    int num_sets = cache->assoc == 0 ? 1 : num_lines / cache->assoc;

    for (int i = 0; i < num_sets; i++) {
        for (int j = 0; j < cache->assoc; j++) {
            freeBitArray(cache->sets[i].lines[j].tag);
            free(cache->sets[i].lines[j].data);
        }
        free(cache->sets[i].lines);
    }
    free(cache->sets);
    free(cache->stats);
    free(cache);
}

bool isPowerOfTwo(int x) { return (x !=0) && ((x & (x-1))==0); }

void print_cache_config(Cache * cache){
        printf("================= Cache Configuration =======================\n");
        printf("address width: %u bit \n",cache->addr_width);
        printf("line size: %u Bytes\n",(unsigned int)pow(2,cache->line_size));
        printf("primary memory size: %u Bytes\n",(unsigned int)pow(2,cache->mem_size));
        printf("cache size: %u Bytes\n",(unsigned int)pow(2,cache->cache_size));
        printf("associativity: ");
        if(cache->assoc==0) printf("fully associative\n");
        else if(cache->assoc==1) printf("direct-mapped\n");
        else if(isPowerOfTwo(cache->assoc)) printf("%d-way\n",cache->assoc);
        else printf("error\n");
        printf("replacement policy: ");
        if(cache->policy==0) printf("random\n");
        else if(cache->policy==1) printf("LRU\n");
        else if(cache->policy==1) printf("optimal\n");
        else printf("error\n");

        return;
}
void print_stats(CacheStats * stat){
        printf("================= Cache Statistics  =======================\n");
        printf("for %lu accesses ... \n",stat->num_of_total_accesses);
        printf("hits: %lu \n",stat->num_of_hits);
        printf("cumpolsory misses: %lu \n",stat->num_of_compulsory_misses);
        printf("capacity misses: %lu \n",stat->num_of_compulsory_misses);
        printf("conflict misses: %lu \n",stat->num_of_compulsory_misses);
        printf("cache hit rate: %Lf\n",stat->hit_rate); 
        return;
}
