

#ifndef HW2_CACHE_H
#define HW2_CACHE_H
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#define START_BIT 2

struct Cache_block {
    unsigned long int address;
    uint32_t tag;
    int dirty_bit;
    int invalid;

    Cache_block():tag(0),dirty_bit(0),invalid(0),address(0){};
    Cache_block(uint32_t tag, int dirty_bit, int invalid): tag(tag), dirty_bit(dirty_bit),invalid(invalid){};
    ~Cache_block() = default;

}typedef block_t;

struct Cache {
    block_t** blocks;
    int write_allocate;
    int block_size;
    int ways;
    int cache_size;
    int cycle_num;
    int sets;
    bool read_miss;
    bool empty_cell;
    int min_way_empty;
    int fixed_set;
    uint32_t fixed_tag;
    bool write_miss;
    int* counters[];


    Cache():write_allocate(0), block_size(0),ways(0),cache_size(0),cycle_num(0),sets(0), read_miss(1),
    empty_cell(0), min_way_empty(0), fixed_set(0),fixed_tag(0),write_miss(1){}
    Cache(int write_allocate,int block_size,int ways,int cache_size,int cycle_num,int sets):
    write_allocate(write_allocate), block_size(block_size), ways(ways), cache_size(cache_size),
    cycle_num(cycle_num),sets(sets), read_miss(1), empty_cell(0), min_way_empty(ways-1), write_miss(1){
        blocks = new block_t * [ways];
        for(int j=0;j<ways;j++){
            blocks[j] = new block_t[sets];
        }

        for(int i=0;i<sets;i++){
            counters[i] = new int[ways];
            for(int j=0;j<ways;j++){
                counters[i][j] = ways-1;
            }
        }
    }
    ~Cache(){
        for(int i=0;i<ways;i++){
            delete[] blocks[i];
        }
        delete[] blocks;
        for(int j =0;j<sets;j++){
            delete counters[j];
        }
    }
    // Find the way with the lowest counter value
int  findEvictionWay(int sets) {
        int minCounter = ways;
        int evictWay = 0;
        for (int i = 0; i < ways; ++i) {
            if (counters[sets][i] < minCounter) {
                minCounter = counters[sets][i];
                evictWay = i;
            }
        }
        return evictWay;
    }
}typedef cache_t;

struct Stats{
    int l1_access_num;
    int l2_access_num;
    int total_time_acc;
    int main_access;
    int main_cycles;

    Stats():l1_access_num(0), l2_access_num(0), total_time_acc(0), main_access(0), main_cycles(0){};
//    Stats(int main_cycles):l1_access_num(0), l2_access_num(0), total_time_acc(0), main_access(0), main_cycles(main_cycles){};

}typedef stat_t;

int set_cal(int size, int ways, int block_size){
    return pow(2,((size)/(block_size*ways)));
};
uint32_t get_index(uint32_t branch){

}
int extract_set(int num, int sets,int block_size){
    uint32_t set_mask = ((1 << sets) - 1) << (START_BIT+ block_size);
    return (num & set_mask) >> (START_BIT + block_size);

}
uint32_t  extract_tag(int num, int sets, int block_size){
    uint32_t tag_size = 32 - START_BIT - sets - block_size;
    uint32_t tag_mask = ((1 << tag_size) - 1) << (START_BIT + sets + block_size);
    return (num & tag_mask) >> (START_BIT + sets + block_size);
    }





#endif //HW2_CACHE_H
