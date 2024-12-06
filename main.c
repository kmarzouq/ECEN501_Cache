/*************************************************************
 * cache simulator main body (programming assignment #2)
 *************************************************************/
#include "cache.h"
#include <string.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 200

int is_comment_or_empty(const char *line) {
    return line[0] == '#' || line[0] == '\0' || isspace(line[0]);
}

unsigned int parse_number(const char *str) {
    if (strstr(str, "0x") == str) {
        return (unsigned int)strtoul(str, NULL, 16); // hexadecimal
    } else {
        return (unsigned int)atoi(str); // decimal
    }
}

Cache * parse_config(FILE *file) {
    char line[MAX_LINE_LENGTH];
    unsigned int addr_width, line_size, mem_size, cache_size, assoc, policy;
    Cache * cache = NULL;

    for (int i = 0; i < 6; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error: Unexpected end of file while reading config\n");
            return cache;
        }

        // for debugging: printf("line %d: txt - %s\n",i,line);
        // ignoring comments or empty line
        if (is_comment_or_empty(line)) {
            i--;
            continue;
        }
        switch (i) {
            case 0: addr_width = parse_number(line); break;
            case 1: line_size = parse_number(line); break;
            case 2: mem_size = parse_number(line); break;
            case 3: cache_size = parse_number(line); break;
            case 4: assoc = parse_number(line); break;
            case 5: policy = parse_number(line); break;
        }
    }
    cache = initCache(addr_width,line_size,mem_size,cache_size, assoc,policy);
    return cache;
}


int parse_trace(FILE *file, Cache* cache) {
    char line[MAX_LINE_LENGTH];
    unsigned int num=0;
    while (fgets(line, sizeof(line), file) != NULL) {

        printf("trace #%u: %s",++num,line);

        // ignoring comments or empty line
        if (is_comment_or_empty(line)) {
            continue;
        }

        char command;
        unsigned int address, value;

        // parsing command
        if (sscanf(line, "%c", &command) != 1) {
            fprintf(stderr, "Error: Invalid trace command\n");
            return 0;
        }

        // Read 
        if (command == 'R') {
            if (sscanf(line + 1, "%x", &address) != 1) {
                fprintf(stderr, "Error: Invalid address format in READ command\n");
                return 0;
            }
            printf("READ: Address = 0x%x\n", address);

            /////////////////////////////////////////////////////
            // PART 1-4
            // Implement read here
            /////////////////////////////////////////////////////
            int in_cache=0;
            int num_lines = (1<<cache->cache_size)/(1<<cache->line_size); 
            int num_sets;
            if (cache->assoc==0){
                num_sets=1;
            }
            else{
                num_sets=num_lines/cache->assoc;
            }
            int i;
            int line_offset_bits = cache->line_size;
            int set_index_bits = cache->cache_size;
            int tag_size = cache->addr_width-set_index_bits-line_offset_bits;
            
            int tag;
            int index;
            int offset;

            BitArray * tagb= createBitArray(tag_size);
            BitArray * indexb=createBitArray(set_index_bits);
            BitArray * offsetb=createBitArray(line_offset_bits);

            int index_mask=0;
            int tag_mask=0;
            int offset_mask=0;

            for (i=line_offset_bits;i<set_index_bits+line_offset_bits;i++){
                index_mask = index_mask && (1<<i);
            }
            
            index = address & index_mask;
            int cnt=0;
            for (i=line_offset_bits;i<set_index_bits+line_offset_bits;i++){
                if (index & (1<<i))
                {
                    setBit(indexb,cnt);
                }
                else
                {
                    clearBit(indexb,cnt);
                }
                cnt++;
                
                
            }

            for (i=line_offset_bits+set_index_bits;i<tag_size+set_index_bits+line_offset_bits;i++){
                tag_mask = tag_mask && (1<<i);
            }

            tag = address & tag_mask;

            cnt=0;
            for (i=line_offset_bits+set_index_bits;i<tag_size+set_index_bits+line_offset_bits;i++){
                if (tag & (1<<i))
                {
                    setBit(tagb,cnt);
                }
                else
                {
                    clearBit(tagb,cnt);
                }
                cnt++;
                
                
            }

            for (i=0;i<line_offset_bits;i++){
                offset_mask = offset_mask || (1<<i);
            }

            offset = offset_mask & address;

            cnt=0;
            for (i=0;i<line_offset_bits;i++){
                if (offset & (1<<i))
                {
                    setBit(offsetb,cnt);
                }
                else
                {
                    clearBit(offsetb,cnt);
                }
                cnt++;
                
                
            }

            bool miss=true;
            int j;
            int match=0;
            int val=0;
            int nv=0;
            if (cache->assoc==0)
            {
                /* code */
            }
            else
            {

            for (i = 0; i < num_sets; i++)
            {

                if (cache->sets[index].lines->valid)
                   {
                    val++;
                    int j;
                    match=0;
                    for ( j = 0; j < tag_size; j++)
                    {
                        
                        if (getBit(cache->sets[index].lines->tag,j)!=getBit(tagb,j))
                        {
                            break;
                        }
                        else
                        {
                            match++;
                        }
                        
                        
                    }
                if (match==tag_size)
                    {
                        cache->stats->num_of_hits++;
                        miss=false;
                        break;
                    }
                else
                {
                    miss=true;
                }
                
                    
                    
                    
                    
                   }
                else
                   {
                    nv++;
                       
                   }
                    
                    
                
                
            }

                if (miss==true)
                {
                    if ((nv+val)==)
                    {
                        /* code */
                    }
                    
                }
                
            }
            
            




            //use utils.h bit array functions
        }
        // Write
        else if (command == 'W') {
            if (sscanf(line + 1, "%x %x", &address, &value) != 2) {
                fprintf(stderr, "Error: Invalid format in WRITE command\n");
                return 0;
            }
            printf("WRITE: Address = 0x%x, Value = 0x%x\n", address, value);

            /////////////////////////////////////////////////////
            // PART 1-4
            // Implement write here
            /////////////////////////////////////////////////////

        } else {
            fprintf(stderr, "Error: Unknown command '%c'\n", command);
            return 0;
        }
    }
    return 1;
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: cache_sim <config_file> <trace_file>\n");
        return 1;
    }

    // open configuration file
    FILE *config_file = fopen(argv[1], "r");
    if (config_file == NULL) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        printf("Usage: cache_sim <config_file> <trace_file>\n");
        return 1;
    }
    printf("Successfully opened the configuration file: %s\n", argv[1]);

    // open trace file
    FILE *trace_file = fopen(argv[2], "r");
    if (trace_file == NULL) {
        printf("Error: Could not open file '%s'\n", argv[2]);
        printf("Usage: cache_sim <config_file> <trace_file>\n");
        return 1;
    }
    printf("Successfully opened the trace file: %s\n", argv[2]);


    Cache *  my_cache = parse_config(config_file);
    if(my_cache==NULL) {
           printf("Configuration was not complte\n");
           return 1;
    }
    // close the config file
    fclose(config_file);

    // print out the configuration
    print_cache_config(my_cache);
    // parse trace
    parse_trace(trace_file,my_cache);
    // close the trace file
    fclose(trace_file);
    // print out the statistics
    print_stats(my_cache->stats);
    // freeing cache
    freeCache(my_cache);    

    
    return 0;
}
