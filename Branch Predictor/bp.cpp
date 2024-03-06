/* 046267 Computer Architecture - HW #1                                 */
/* This file should hold your implementation of the predictor simulator */

#include "bp_api.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#define NUM_STATES 4
#define startBit 2
#define mid_bit 16
#define TARGET_PC 30
using namespace std;

//struct which describes each line in the BTB table
struct BTB_line{
    uint32_t branch_pc;
    uint32_t target;
    uint32_t tag;
    uint32_t index;
    int used; //valid bit
    uint32_t history; //local history
    uint32_t* fsm_l; //local fsm
    int history_l_size; //local history size
    BTB_line() : branch_pc(0), target(0), tag(0), index(0), used(0), history(0), history_l_size(0){}

    BTB_line(uint32_t branch_pc, uint32_t target_pc,
             uint32_t tag, uint32_t index, int used, uint32_t history) :
             branch_pc(branch_pc),target(target_pc), tag(tag), index(index), used(used), history(history) {
        fsm_l = nullptr;
    }

    BTB_line(const BTB_line& other)
            : branch_pc(other.branch_pc), target(other.target), tag(other.tag),
              index(other.index), used(other.used), history(other.history), history_l_size(other.history_l_size) {

        if (other.fsm_l) {
            int fsm_size = (1 << history_l_size); // Calculate size of fsm_l array
            fsm_l = new uint32_t[fsm_size];
            for (int i = 0; i < fsm_size; ++i) {
                fsm_l[i] = other.fsm_l[i];
            }
        } else {
            fsm_l = nullptr;
        }
    }

// Assignment operator
    BTB_line& operator=(const BTB_line& other) {
        if (this != &other) { // Avoid self-assignment
            // Copy member variables
            branch_pc = other.branch_pc;
            target = other.target;
            tag = other.tag;
            index = other.index;
            used = other.used;
            history = other.history;
            history_l_size=other.history_l_size;

            // Copy fsm_l array
            if (other.fsm_l) {
                int fsm_size = (1 << history_l_size); // Calculate size of fsm_l array
                fsm_l = new uint32_t[fsm_size];
                for (int i = 0; i < fsm_size; ++i) {
                    fsm_l[i] = other.fsm_l[i];
                }
            } else {
                fsm_l = nullptr;
            }
        }
        return *this;
    }

    ~BTB_line() {
        delete[] fsm_l;
    }

};typedef BTB_line line_t;

//struct which describes the BTB table
struct BTB_table{
    line_t*  table;
    int btb_size;
    int tag_size;
    int history_size;
    uint32_t history_all; //global history
    int fsm_state;
    uint32_t* fsm;
    int shared;
    bool global_hist;
    bool global_fsm;
    SIM_stats stats;

    BTB_table() : table(), btb_size(0),tag_size(0), history_size(0)
            , fsm_state(0), fsm(NULL), shared(0), global_hist(0),global_fsm(0){}

    BTB_table( int btb_size, int history_size,
               int fsm_state, int shared, bool global_hist,
               bool global_fsm, int tag_size) {
        this->btb_size = btb_size;
        this->tag_size = tag_size;
        this->global_hist = global_hist;
        this->global_fsm = global_fsm;
        this->history_size = history_size;
        this->fsm_state=fsm_state;
        this->shared = shared;
        this->table = new line_t[btb_size]; //should we intiliaize that or it calls the C'tor?
        int fsm_size =pow(2, history_size);

        if (global_hist) {
            this->history_all = 0;
        }
        this->fsm = new uint32_t[fsm_size];
        if (global_fsm) {
            for (int i = 0; i < fsm_size; i++) {
                this->fsm[i] = fsm_state;
            }
        } else {
            for (int i = 0; i < btb_size; i++) {
                this->table[i].fsm_l = new uint32_t[fsm_size];
            }
        }
    }

    BTB_table(const BTB_table& other)
            : btb_size(other.btb_size), tag_size(other.tag_size),
              history_size(other.history_size), history_all(other.history_all),
              fsm_state(other.fsm_state), shared(other.shared),
              global_hist(other.global_hist), global_fsm(other.global_fsm),
              stats(other.stats) {

        table = new line_t[btb_size];
        for (int i = 0; i < btb_size; ++i) {
            table[i] = other.table[i];
        }
        int fsm_size = pow(2, history_size);
        fsm = new uint32_t[fsm_size];
        for (int i = 0; i < fsm_size; ++i) {
            fsm[i] = other.fsm[i];
        }
    }
    BTB_table& operator=(const BTB_table& other) {
        if (this != &other) {
            btb_size = other.btb_size;
            tag_size = other.tag_size;
            history_size = other.history_size;
            history_all = other.history_all;
            fsm_state = other.fsm_state;
            shared = other.shared;
            global_hist = other.global_hist;
            global_fsm = other.global_fsm;
            stats = other.stats;
            table = new line_t[btb_size];
            for (int i = 0; i < btb_size; ++i) {
                table[i] = other.table[i]; // Assuming line_t has a proper copy constructor
            }
            int fsm_size = pow(2, history_size);
            fsm = new uint32_t[fsm_size];
            for (int i = 0; i < fsm_size; ++i) {
                fsm[i] = other.fsm[i];
            }
        }
        return *this;
    }
    ~BTB_table(){
        if(global_fsm){
            delete[] fsm;
        } else {
            for(int i = 0; i < btb_size; ++i){
                delete[] table[i].fsm_l;
            }
        }
    }
};typedef BTB_table table_t;
table_t* BTB;

//---------------------------------------------------------------------------------------//
//-----------------------------------AUX functions---------------------------------------//
//---------------------------------------------------------------------------------------//

//calculates the integer logarithm base 2 of an unsigned 32-bit integer.
uint32_t log2_uint(uint32_t a){
    int res = 0;
    while(pow(2,res) < a){
        res++;
    }
    return res;
}
//takes the branch PC and return the tag from it
uint32_t get_tag(uint32_t pc){
//    return ((pc >> 2) + log2_uint(BTB->btb_size));
      return (pc << (32 - 2 - log2_uint(BTB->btb_size) - BTB->tag_size)) >> (32- BTB->tag_size);
}
//searches for a specific tag within a BTB table and updates the provided line_t pointer if found
bool find_line(uint32_t tag, line_t *current){
    for(int i=0;i<BTB->btb_size;i++){
        if(tag == BTB->table[i].tag){
            *current = BTB->table[i];
            return true;
        }
    }
    return false;
}
//calculates the index of a branch in a BTB table using a mask derived from the starting bit
uint32_t get_index(uint32_t branch){
    int btb_size_log = log2_uint(BTB->btb_size);
    uint32_t end_bit = btb_size_log + startBit - 1; // Adjusted calculation for end_bit
    uint32_t mask = ((1 << (end_bit - startBit + 1)) - 1);
    mask <<= startBit;
    return ((branch & mask) >> startBit);
}
//determines if flush is needed based on branch prediction results.
bool flush_detector(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dest){
    return ((taken && targetPc != pred_dest) || (!taken && pred_dest != pc+4));
}
//verifies if the finite state machine cell allows for a transition based on the taken/not taken status
bool check_limits(uint32_t fsm_cell, bool taken){
    return (!((fsm_cell == 0 && !taken) || (fsm_cell == 3 && taken)));
}
//calculates the total size of a BTB
int calcTotalSize(unsigned btbSize, unsigned historySize, unsigned tagSize)
{
    int totalSize = btbSize * (1 + TARGET_PC + tagSize);
    totalSize += !BTB->global_hist ? btbSize * historySize : historySize;
    totalSize += !BTB->global_fsm ? btbSize * pow(2, historySize + 1) : pow(2, historySize + 1);
    return totalSize;
}
/*calculates the index for fsm, based on the PC.
 considering different configurations such as shared or global history in the BTB*/
unsigned calcFsmIndex(uint32_t pc) {
    unsigned shareValue = 0;
    unsigned fsmNum = 0;
    unsigned index = get_index(pc);
    if (BTB->shared== 1) {
        shareValue = pc >> 2;
        shareValue = shareValue % (unsigned(pow(2, BTB->history_size)));
    }
    if (BTB->shared == 2) {
        shareValue = pc >> 16;
        shareValue = shareValue % (unsigned(pow(2,BTB->history_size)));
    }
    if (BTB->global_hist) {
        fsmNum = BTB->history_all;
    }
    else {
        fsmNum = BTB->table[index].history;
    }
    return fsmNum ^ shareValue;
}


//---------------------------------------------------------------------------------------//
//-----------------------implementation of the main functions----------------------------//
//---------------------------------------------------------------------------------------//

int BP_init(unsigned btbSize, unsigned historySize, unsigned tagSize, unsigned fsmState,
            bool isGlobalHist, bool isGlobalTable, int Shared){
    BTB = new table_t(btbSize,historySize,fsmState,Shared,
                      isGlobalHist,isGlobalTable,tagSize);
    return 0;
}

bool BP_predict(uint32_t pc, uint32_t *dst) {
    uint32_t tag = get_tag(pc);
    uint32_t index = get_index(pc);
    BTB->table[index].history_l_size=BTB->history_size;
    line_t current = BTB->table[index];
    uint32_t tmp_hist;
    /*check If the BTB table entry at the calculated index is unused or its tag mismatches the provided tag
    and update the destination address pointer to point to the next instruction address and return false */
    if(!BTB->table[index].used || tag != BTB->table[index].tag){
        *dst = pc + 4;
        return false;
    }
    else {
        //check which history we are using
        tmp_hist = (BTB->global_hist ? BTB->history_all : current.history);
        if(!BTB->global_fsm) {
            if (current.fsm_l[tmp_hist] < 2) {
                *dst = pc + 4;
                return false;
            } else {
                *dst = current.target;
                return true;
            }
        }
        else {
            uint32_t curr_fsm;
            curr_fsm = calcFsmIndex(pc);
            if (BTB->fsm[curr_fsm] < 2) {
                *dst = pc + 4;
                return false;
            } else {
                *dst = current.target;
                return true;
            }
        }
    }
}

void BP_update(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dst){
    /* Increment the branch counter in the BTB statistics,
       then if a flush is detected based on the provided parameters,
       increment the flush counter in the BTB statistics as well. */
    BTB->stats.br_num++;
    if(flush_detector(pc, targetPc, taken, pred_dst)){
        BTB->stats.flush_num++;
    }

    uint32_t index = get_index(pc);
    uint32_t tag = get_tag(pc);

    // If the entry at the calculated index is not used, initialize it.
    if(!BTB->table[index].used) {
        BTB->table[index].branch_pc = pc;
        BTB->table[index].target = targetPc;
        BTB->table[index].history = 0;
        BTB->table[index].tag = tag;
        BTB->table[index].index = index;
        BTB->table[index].used = 1;

        // Initialize FSM values if not using global FSM.
        if(!BTB->global_fsm){
            for (int i = 0; i < pow(2, BTB->history_size); i++) {
                BTB->table[index].fsm_l[i] = BTB->fsm_state;
            }
        }

        // Initialize history if not using global history.
        if(!BTB->global_hist){
            BTB->table[index].history = 0;
        }
    }
    else {
        // If tag mismatches, update tag and PC.
        if(tag != BTB->table[index].tag) {
            BTB->table[index].tag = tag;
            BTB->table[index].branch_pc = pc;

            // Update FSM values if not using global FSM.
            if(!BTB->global_fsm){
                for (int i = 0; i < pow(2, BTB->history_size); i++) {
                    BTB->table[index].fsm_l[i] = BTB->fsm_state;
                }
            }

            // Initialize history if not using global history.
            if(!BTB->global_hist){
                BTB->table[index].history = 0;
            }
        }
        // Update target PC.
        BTB->table[index].target = targetPc;
    }

    // Update FSM and history based on configuration.
    if(BTB->global_hist && BTB->global_fsm){
        if(check_limits(BTB->fsm[calcFsmIndex(pc)], taken)){
            taken ? (BTB->fsm[calcFsmIndex(pc)]++) : (BTB->fsm[calcFsmIndex(pc)]--);
        }
        BTB->history_all = ((BTB->history_all << 1) | taken) & ((1 << BTB->history_size) - 1);
    }
    else if(!BTB->global_hist && BTB->global_fsm){
        if(check_limits(BTB->fsm[calcFsmIndex(pc)], taken)){
            taken ? (BTB->fsm[calcFsmIndex(pc)]++) : (BTB->fsm[calcFsmIndex(pc)]--);
        }
        BTB->table[index].history = ((BTB->table[index].history << 1) | taken) & ((1 << BTB->history_size) - 1);
    }
    else if(!BTB->global_fsm){
        if(BTB->global_hist){
            if(check_limits(BTB->table[index].fsm_l[BTB->history_all],taken)){
                taken? (BTB->table[index].fsm_l[BTB->history_all]++) : (BTB->table[index].fsm_l[BTB->history_all]--);
            }
            BTB->history_all = ((BTB->history_all << 1) | taken) & ((1 << BTB->history_size) - 1);
        }
        else{
            if(check_limits(BTB->table[index].fsm_l[BTB->table[index].history],taken)){
                taken ? (BTB->table[index].fsm_l[BTB->table[index].history]++) : (BTB->table[index].fsm_l[BTB->table[index].history]--);
            }
            BTB->table[index].history = ((BTB->table[index].history << 1) | taken) & ((1 << BTB->history_size) - 1);
        }
    }
}


void BP_GetStats(SIM_stats *curStats){
    curStats->size = calcTotalSize(BTB->btb_size, BTB->history_size,BTB->tag_size);
    curStats->flush_num = BTB->stats.flush_num;
    curStats->br_num = BTB->stats.br_num;
    delete BTB;
    return;
}