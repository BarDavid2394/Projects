#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdint.h>
#include <vector>

using std::FILE;
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::stringstream;

//declarations and variables
void snoop(int evict_way);
bool was_snooped = false;
int snoop_way;

//define each line in the cache
struct Cache_block {
    unsigned long int address;
    uint32_t tag;
    int dirty_bit;
    int invalid;



    Cache_block(){
        tag=0;
        dirty_bit=0;
        invalid=1;
        address=0;

    };
}typedef block_t;

//define the whole cache
struct Cache {
    std::vector<std::vector<block_t> > blocks;
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
    std::vector<std::vector<int> > counters;

    //----------------------c'tors and d'tor------------------------------------------------//
    Cache():write_allocate(0), block_size(0),ways(0),cache_size(0),cycle_num(0),sets(0), read_miss(true),
            empty_cell(false), min_way_empty(0), fixed_set(0),fixed_tag(0),write_miss(true){}

    Cache(int write_allocate,int block_size,int ways,int cache_size,int cycle_num,int sets):
            write_allocate(write_allocate), block_size(block_size), ways(ways), cache_size(cache_size),
            cycle_num(cycle_num),sets(sets), read_miss(true), empty_cell(false), min_way_empty(ways-1), write_miss(true){
        blocks.resize(ways);
        for(int j=0;j<ways;j++){
            blocks[j].resize(sets);
        }
        counters.resize(sets);
        for(int i=0;i<sets;i++){
            counters[i].resize(ways);
            for(int j=0;j<ways;j++){
                counters[i][j] = ways-1;
            }
        }
    }

    ~Cache(){
        for(int i=0;i<sets;i++){
            blocks[i].clear();
        }
        blocks.clear(); // Clear the outer vector
    }
    //--------------------------member functions---------------------------------------------//
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

//define the statistics
struct Stats{
    double l1_access_num;
    double l2_access_num;
    double total_time_acc;
    double mem_cycles;
    double l1_miss;
    double l2_miss;

    Stats():l1_access_num(0), l2_access_num(0), total_time_acc(0),
            mem_cycles(0),l1_miss(0),l2_miss(0){};
}typedef stat_t;


stat_t* stats;
cache_t* l1_cache;
cache_t* l2_cache;
bool double_miss = false;

//----------------------------------------------------------------------------------//
//------------------------------Aux_functions---------------------------------------//
//----------------------------------------------------------------------------------//

//calculate the number of sets we have in the cache
int set_cal(int size, int ways, int block_size){
    return pow(2,((size)-(block_size+ways)));
};
//extract the right set from the address we get
int extract_set(int num, int sets,int block_size){

        // Calculate the set mask
        uint32_t set_mask = ((1 << static_cast<int>(log2(sets))) - 1) << block_size;
        // Extract and return the set value
        return (num & set_mask) >> block_size;
    }

//extract the right tag from the address we get
uint32_t  extract_tag(int num, int sets, int block_size){
    return num >> (static_cast<int>(log2(sets))+block_size);
}
//implementation of the LRU1 with the counters vectors
void update_LRU1 (int way){ //return int for dirty bit check.
    int current = l1_cache->counters[l1_cache->fixed_set][way]; //this is the way we want to update
    l1_cache->counters[l1_cache->fixed_set][way] = ((l1_cache->ways)-1);
    for(int i=0;i<l1_cache->ways;i++){
        if(way != i && l1_cache->counters[l1_cache->fixed_set][i] >= current){
            l1_cache->counters[l1_cache->fixed_set][i]--;
        }
    }
}
//implementation of the LRU1 with the counters vectors
void update_LRU2(int way){
    int current = l2_cache->counters[l2_cache->fixed_set][way];
    l2_cache->counters[l2_cache->fixed_set][way] = l2_cache->ways-1;
    for(int i=0;i<l2_cache->ways;i++){
        if(way != i && l2_cache->counters[l2_cache->fixed_set][i] >= current){
            l2_cache->counters[l2_cache->fixed_set][i]--;
        }
    }
}
//insert the element to the specific location in the L1 cache
void insert_l1_element(int way,int set,int address,int dirty){
    l1_cache->blocks[way][set].dirty_bit = dirty;
    l1_cache->blocks[way][set].tag = l1_cache->fixed_tag;
    l1_cache->blocks[way][set].address = address; //saving address for snoop later
    l1_cache->blocks[way][set].invalid = 0;
}
//insert the element to the specific location in the L2 cache
void insert_l2_element(int way,int set,int address,int dirty){
    l2_cache->blocks[way][set].dirty_bit = dirty;
    l2_cache->blocks[way][set].tag = l2_cache->fixed_tag;
    l2_cache->blocks[way][set].address = address; //saving address for snoop later
    l2_cache->blocks[way][set].invalid = 0;
}
//the eviction and insertion function in L1 cache when read operation is executed
void insert_l1_read(int way2, int address){
    int current_way;
    if(was_snooped) {
        current_way = snoop_way;
    }
    else{
        current_way = l1_cache->findEvictionWay(l1_cache->fixed_set);
    }
    if (l1_cache->empty_cell) { //There is an unoccupied way
        if(was_snooped){
            l1_cache->min_way_empty = current_way; // if was empty cell but snooped, then insert at snoop way.
        }
        insert_l1_element(l1_cache->min_way_empty,l1_cache->fixed_set,address,0);
        update_LRU1(l1_cache->min_way_empty);
        was_snooped = false;
    }
    else{
        //check if the address exists in the L2 cache
        if (l1_cache->blocks[current_way][l1_cache->fixed_set].dirty_bit) {
            int tag_2 = extract_tag(l1_cache->blocks[current_way][l1_cache->fixed_set].address, l2_cache->sets,
                                    l2_cache->block_size);
            for (int i = 0; i < l2_cache->ways; i++) {
                if (tag_2 == l2_cache->blocks[i][l2_cache->fixed_set].tag) {
                    l2_cache->blocks[i][l2_cache->fixed_set].dirty_bit = 1;
                    update_LRU2(i);
                    break;
                }
            }
        }
        insert_l1_element(current_way,l1_cache->fixed_set,address,0);
        update_LRU1(current_way);
        was_snooped = false;
    }
}
//the eviction and insertion function in L2 cache when read operation is executed
void insert_l2_read(int address) {
    if (l2_cache->empty_cell) { //Just insert data
        insert_l2_element(l2_cache->min_way_empty,l2_cache->fixed_set,address,0);
        update_LRU2(l2_cache->min_way_empty);
    }
    else{
        int evict_way = l2_cache->findEvictionWay(l2_cache->fixed_set);
        snoop(evict_way);
        insert_l2_element(evict_way,l2_cache->fixed_set,address,
                          l2_cache->blocks[evict_way][l2_cache->fixed_set].dirty_bit);
        update_LRU2(evict_way);
    }
}
//the eviction and insertion function in L1 cache when write operation is executed
void insert_l1_write(int way2, int address){
    int current_way;
    if(was_snooped) {
        current_way = snoop_way;
    }
    else{
        current_way = l1_cache->findEvictionWay(l1_cache->fixed_set);
    }
    if (l1_cache->empty_cell) { //There is an unoccupied way
        if(was_snooped){
            l1_cache->min_way_empty = current_way;
        }
        insert_l1_element(l1_cache->min_way_empty,l1_cache->fixed_set,address,1);
        update_LRU1(l1_cache->min_way_empty);
        was_snooped = false;
    }
    else{
        if (l1_cache->write_allocate) {
            if (l1_cache->blocks[current_way][l1_cache->fixed_set].dirty_bit) {
                int tag_2 = extract_tag(l1_cache->blocks[current_way][l1_cache->fixed_set].address, l2_cache->sets,
                                        l2_cache->block_size);
                for (int i = 0; i < l2_cache->ways; i++) {
                    if (tag_2 == l2_cache->blocks[i][l2_cache->fixed_set].tag) {
                        l2_cache->blocks[i][l2_cache->fixed_set].dirty_bit = 1;
                        update_LRU2(i);
                        break;
                    }
                }
            }
        }
        insert_l1_element(current_way,l1_cache->fixed_set,address,1);
        update_LRU1(current_way);
        was_snooped = false;
    }
}
//the eviction and insertion function in L2 cache when write operation is executed
void insert_l2_write( int address){
    if (l2_cache->empty_cell) { //Just insert data
        insert_l2_element(l2_cache->min_way_empty,l2_cache->fixed_set,address,1);
        update_LRU2(l2_cache->min_way_empty);
    }
    else{
        int evict_way2 = l2_cache->findEvictionWay(l2_cache->fixed_set);
        if(l2_cache->write_allocate){
            snoop(evict_way2);
            insert_l2_element(evict_way2,l2_cache->fixed_set,address,1);
            update_LRU2(evict_way2);
        }
    }

}
//snoop from L2 to L1 and check the specific address is exist
void snoop(int evict_way) {
    uint32_t tag = extract_tag(
            l2_cache->blocks[evict_way][l2_cache->fixed_set].address,
            l1_cache->sets, l2_cache->block_size);
    int set = extract_set(l2_cache->blocks[evict_way][l2_cache->fixed_set].address,
                          l1_cache->sets, l2_cache->block_size);
    for (int i = 0; i < l1_cache->ways; i++) {
        if (!l1_cache->blocks[i][set].invalid) {
            if (l1_cache->blocks[i][set].tag == tag) {
                snoop_way = i;
                was_snooped = true;
            }
        }
    }
}

//------------------------------------------------------------------------------//
//-------------------------------Main-------------------------------------------//
//------------------------------------------------------------------------------//

int main(int argc, char **argv) {
	if (argc < 19) {
        cerr << "Not enough arguments" << endl;
        return 0;
    }

    char* fileString = argv[1];

    ifstream file(fileString); //input file stream
    string line;
    if (!file || !file.good()) {
        // File doesn't exist or some other error
        cerr << "File not found" << endl;
        return 0;
    }


    unsigned MemCyc = 0, BSize = 0, L1Size = 0, L2Size = 0, L1Assoc = 0,
			L2Assoc = 0, L1Cyc = 0, L2Cyc = 0, WrAlloc = 0;
    for (int i = 2; i < 19; i += 2) {
        string s(argv[i]);
        if (s == "--mem-cyc") {
            MemCyc = atoi(argv[i + 1]);
        } else if (s == "--bsize") {
			BSize = atoi(argv[i + 1]);
		} else if (s == "--l1-size") {
			L1Size = atoi(argv[i + 1]);
		} else if (s == "--l2-size") {
			L2Size = atoi(argv[i + 1]);
		} else if (s == "--l1-cyc") {
			L1Cyc = atoi(argv[i + 1]);
		} else if (s == "--l2-cyc") {
			L2Cyc = atoi(argv[i + 1]);
		} else if (s == "--l1-assoc") {
			L1Assoc = atoi(argv[i + 1]);
		} else if (s == "--l2-assoc") {
			L2Assoc = atoi(argv[i + 1]);
		} else if (s == "--wr-alloc") {
			WrAlloc = atoi(argv[i + 1]);
		} else {
			cerr << "Error in arguments" << endl;
			return 0;
		}

	}

    stats = new Stats();
    stats->mem_cycles = MemCyc;
    //create the caches
    l1_cache = new Cache(WrAlloc,BSize, pow(2,L1Assoc),L1Size,L1Cyc, set_cal(L1Size,L1Assoc,BSize));
    l2_cache = new Cache(WrAlloc,BSize, pow(2,L2Assoc),L2Size,L2Cyc, set_cal(L2Size,L2Assoc,BSize));
    while (getline(file, line)) {

		stringstream ss(line);
		string address;
		char operation = 0; // read (R) or write (W)
		if (!(ss >> operation >> address)) {
			// Operation appears in an Invalid format
			cout << "Command Format error" << endl;
			return 0;
		}
		string cutAddress = address.substr(2); // Removing the "0x" part of the address

		unsigned long int num = 0;

		num = strtoul(cutAddress.c_str(), NULL, 16);
        l1_cache->fixed_set =  extract_set(num,l1_cache->sets,BSize);
        l2_cache->fixed_set =  extract_set(num,l2_cache->sets,BSize);
        l1_cache->fixed_tag = extract_tag(num,l1_cache->sets,BSize);
        l2_cache->fixed_tag = extract_tag(num,l2_cache->sets,BSize);
        //Only for Reading!
        l2_cache->empty_cell = false;
        l1_cache->empty_cell = false;
        l1_cache->min_way_empty = l1_cache->ways-1;
        l2_cache->min_way_empty = l2_cache->ways-1;

        //----------------------------------------------------------------------------------//
        //----------------------------------Read operation----------------------------------//
        if(operation == 'r') {
            stats->l1_access_num++;
            l1_cache->read_miss = true;
            l2_cache->read_miss = true;
            for (int i = 0; i < l1_cache->ways; i++) {
                if (!l1_cache->blocks[i][l1_cache->fixed_set].invalid) {
                    if (l1_cache->fixed_tag == l1_cache->blocks[i][l1_cache->fixed_set].tag)
                    { //READ HIT
                        stats->total_time_acc += l1_cache->cycle_num;
                        update_LRU1(i);
                        l1_cache->read_miss = false;
                        break;
                    }
                } else { //The current cell is invalid
                    l1_cache->empty_cell = true;
                    if (i <= l1_cache->min_way_empty) {
                        l1_cache->min_way_empty = i;
                    }
                }
            }
            if (l1_cache->read_miss) {
                stats->l1_miss++;
                stats->l2_access_num++;
                for (int i = 0; i < l2_cache->ways; i++) {
                    if (!l2_cache->blocks[i][l2_cache->fixed_set].invalid) {
                        if (l2_cache->fixed_tag ==l2_cache->blocks[i][l2_cache->fixed_set].tag)
                        { //L1 MISS L2 HIT (READ)
                            stats->total_time_acc += (l1_cache->cycle_num+l2_cache->cycle_num);
                            update_LRU2(i);
                            l2_cache->read_miss = false;
                            insert_l1_read(i, num);
                            break;
                        }
                    } else {
                        l2_cache->empty_cell = true;
                        if (i <= l2_cache->min_way_empty) {
                            l2_cache->min_way_empty = i;
                        }
                    }
                }
                if (l2_cache->read_miss) { //L1 MISS L2 MISS
                    stats->l2_miss++;
                    stats->total_time_acc += (l1_cache->cycle_num+l2_cache->cycle_num+stats->mem_cycles);
                    if (l2_cache->empty_cell) {
                        insert_l2_read(num);
                        insert_l1_read(l2_cache->min_way_empty, num);
                    } else {
                        insert_l2_read(num);
                        insert_l1_read(l2_cache->findEvictionWay(l2_cache->fixed_set), num);
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------//
        //----------------------------------write operation---------------------------------//
        if(operation == 'w') {
            stats->l1_access_num++;
            l1_cache->write_miss = true;
            l2_cache->write_miss = true;
            for (int i = 0; i < l1_cache->ways; i++) {
                if (!l1_cache->blocks[i][l1_cache->fixed_set].invalid) {
                    if (l1_cache->fixed_tag == l1_cache->blocks[i][l1_cache->fixed_set].tag)
                    { //WRITE HIT
                          stats->total_time_acc += l1_cache->cycle_num;
                        update_LRU1(i);
                        l1_cache->blocks[i][l1_cache->fixed_set].dirty_bit = 1;
                        l1_cache->write_miss = false;
                        break;
                    }
                } else { //The current cell is invalid
                    l1_cache->empty_cell = true;
                    if (i <= l1_cache->min_way_empty)
                        l1_cache->min_way_empty = i;
                }
            }
            if (l1_cache->write_miss) {
                stats->l1_miss++;
                stats->l2_access_num++;
                for (int i = 0; i < l2_cache->ways; i++) {
                    if (!l2_cache->blocks[i][l2_cache->fixed_set].invalid) {
                        if (l2_cache->fixed_tag == l2_cache->blocks[i][l2_cache->fixed_set].tag)
                        { //L1 MISS L2 HIT (WRITE)
                            stats->total_time_acc += (l1_cache->cycle_num+l2_cache->cycle_num);
                            update_LRU2(i);
                            l2_cache->write_miss = false;
                            if (WrAlloc) {
                                insert_l1_write(i, num);
                            } else {
                                l2_cache->blocks[i][l2_cache->fixed_set].dirty_bit = 1;
                            }
                            break;
                        }
                    } else {
                        l2_cache->empty_cell = true;
                        if (i <= l2_cache->min_way_empty)
                            l2_cache->min_way_empty = i;
                    }
                }
                if (l2_cache->write_miss) { //L1 MISS L2 MISS (WRITE)
                    stats->l2_miss++;
                    stats->total_time_acc += (l1_cache->cycle_num + l2_cache->cycle_num + stats->mem_cycles);
                    if(WrAlloc){
                        if (l2_cache->empty_cell) {
                            insert_l2_write(num);
                            insert_l1_write(l2_cache->min_way_empty, num);
                        } else {
                            insert_l2_write(num);
                            insert_l1_write(l2_cache->findEvictionWay(l2_cache->fixed_set), num);
                        }
                    }
                }
            }
        }
	}
    //calculate the stats
    double Numerator=stats->total_time_acc;
    double Denominator = stats->l1_access_num;
	double L1MissRate = (stats->l1_miss / stats->l1_access_num);
	double L2MissRate = (stats->l2_miss / stats->l2_access_num);
	double avgAccTime = (Numerator/Denominator);
	printf("L1miss=%.03f ", L1MissRate);
	printf("L2miss=%.03f ", L2MissRate);
	printf("AccTimeAvg=%.03f\n", avgAccTime);
	return 0;
}




