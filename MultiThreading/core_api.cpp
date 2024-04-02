/* 046267 Computer Architecture - HW #4 */
#include "core_api.h"
#include "sim_api.h"
#include <vector>
#define BLOCKED_MT 1
#define FINE_MT 0
using namespace std;
///define each thread
struct Thread{
    int id;
    int reg[REGS_COUNT];
    bool halt;
    int pending;
    int pc;
    ///c'tor with parameters
    Thread(int id):id(id),pending(0),pc(0){
        halt = false;
        for(int i=0;i<REGS_COUNT;i++){
            reg[i] = 0;
        }
    }
}typedef thread_t;

////define the simulator
struct Simulator{
    vector<thread_t> threads;
    int threads_num;
    int processor_type;
    int total_cycles;
    int load_latency;
    int store_latency;
    int cs_overhead;
    int halt_num;
    int instruct_num;
    ////c'tor with parameter which get the type of MT
    Simulator(int type) :total_cycles(0), halt_num(0), instruct_num(0){
        processor_type = type;
        threads_num = SIM_GetThreadsNum();
        load_latency = SIM_GetLoadLat();
        store_latency = SIM_GetStoreLat();
        for(int i=0;i<threads_num;i++){
            threads.push_back(thread_t(i));
        }
        if(type == BLOCKED_MT){
            cs_overhead = SIM_GetSwitchCycles();
        }
        else {
            cs_overhead = 0;
        }
    }
    ////-----------------------memebers functions-----------------------------------////
    ///-----------------------------------------------------------------------------///

    ///given current running thread,the function find which thread should be run next and return it.
    thread_t* context_switch(thread_t* thread){
        //in case we should continue with the same thread(in Blocked MT)
        if(thread->pending == 0 && !thread->halt && processor_type == BLOCKED_MT){
            return thread;
        }
        //should be cyclic-so if we reached to the last thread, we initiate to zero
        int next_thread = ((thread->id + 1) % threads_num);
        for(int i=0;i<threads_num;i++){
            //find other thread to run
            if(threads[next_thread].pending == 0 && !threads[next_thread].halt){
                thread = (&(threads[next_thread]));
                break;
            }
            next_thread = ((next_thread + 1) % threads_num);
        }
        for(int i=0;i<cs_overhead;i++){
            add_cycle();
        }
        return thread;

    }
    ///update in each thread the pending counter;
    void add_cycle(){
        total_cycles++;
        for(int i=0;i<threads_num;i++){
            if(threads[i].pending > 0){
                threads[i].pending--;
            }
        }
    }
    ///according to the instruction we get, the function execute it in the specific thread.
    void run_thread(thread_t* thread){
        Instruction curr_inst;
        instruct_num++;
        SIM_MemInstRead(thread->pc,&curr_inst,thread->id);
        thread->pc++;
        switch (curr_inst.opcode) {
            case CMD_NOP:
                break;
            case CMD_ADD:
                thread->reg[curr_inst.dst_index] = (thread->reg[curr_inst.src1_index] +
                                                   thread->reg[curr_inst.src2_index_imm]);
                break;
            case CMD_SUB:
                thread->reg[curr_inst.dst_index] = (thread->reg[curr_inst.src1_index] -
                                                   thread->reg[curr_inst.src2_index_imm]);
                break;
            case CMD_ADDI:
                thread->reg[curr_inst.dst_index] = (thread->reg[curr_inst.src1_index] +
                                                    curr_inst.src2_index_imm);
                break;
            case CMD_SUBI:
                thread->reg[curr_inst.dst_index] = (thread->reg[curr_inst.src1_index] -
                                                    curr_inst.src2_index_imm);
                break;
            case CMD_LOAD:
                thread->pending = load_latency+1;
                uint32_t source;
                int32_t dest;
                if(curr_inst.isSrc2Imm){
                    source = (thread->reg[curr_inst.src1_index] + curr_inst.src2_index_imm);
                }
                else{
                    source = (thread->reg[curr_inst.src1_index] + thread->reg[curr_inst.src2_index_imm]);
                }
                SIM_MemDataRead(source,&dest);
                thread->reg[curr_inst.dst_index] = dest;
                break;
            case CMD_STORE:
                thread->pending = store_latency+1;
                int32_t save_dest;
                if(curr_inst.isSrc2Imm){
                    save_dest = thread->reg[curr_inst.dst_index] + curr_inst.src2_index_imm;
                }
                else{
                    save_dest = thread->reg[curr_inst.dst_index] + thread->reg[curr_inst.src2_index_imm];
                }
                SIM_MemDataWrite(save_dest,thread->reg[curr_inst.src1_index]);
                break;
            case CMD_HALT:
                thread->halt = true;
                halt_num++;
                break;
            default:
                break;
        }
    }
    ///check if we are on idle, which means to add cycle until other instruction get in
    bool check_if_idle(){
        if(threads_num == halt_num){
            return false;
        }
        while(true){
            for(int i=0;i<threads_num;i++){
                if(threads[i].pending == 0 && !threads[i].halt){
                    return true;
                }
            }
            add_cycle();
        }
    }
}typedef sim_t;
//create simulator for each type of MT
sim_t *Blocked;
sim_t *Fined;

///---------------------------Aux Functions-------------------------------//
///-----------------------------------------------------------------------//
void CORE_BlockedMT() {
    Blocked = new sim_t(BLOCKED_MT);
    thread_t* running_thread = &(Blocked->threads[0]); //start from thread-0
    while(Blocked->halt_num != Blocked->threads_num){ //continue until all threads finished them instructions
        while(running_thread->pending == 0 ){
            Blocked->run_thread(running_thread);
            Blocked->add_cycle();
            if(running_thread->halt){
                break;
            }
        }
        if(!Blocked->check_if_idle()){
            break;
        }
        running_thread = Blocked->context_switch(running_thread);
    }
}
void CORE_FinegrainedMT() {
    Fined = new sim_t(FINE_MT);
    thread_t* running_thread = &(Fined->threads[0]);
    while(Fined->halt_num != Fined->threads_num){
        Fined->run_thread(running_thread);
        Fined->add_cycle();
        if(!Fined->check_if_idle()){
            break;
        }
        running_thread = Fined->context_switch(running_thread);
    }
}
double CORE_BlockedMT_CPI(){
    double total_cycles = Blocked->total_cycles;
    double total_inst = Blocked->instruct_num;
    double CPI =  total_cycles/total_inst;
    delete Blocked;
    return CPI;

}
double CORE_FinegrainedMT_CPI(){
    double total_cycles = Fined->total_cycles;
    double total_inst = Fined->instruct_num;
    double CPI =  (total_cycles/total_inst);
    delete Fined;
    return CPI;

}
void CORE_BlockedMT_CTX(tcontext* context, int threadid) {
    for(int i=0;i<REGS_COUNT;i++){ // no copy_constructor to tcontext.
        context[threadid].reg[i] = Blocked->threads[threadid].reg[i];
    }

}
void CORE_FinegrainedMT_CTX(tcontext* context, int threadid) {
    for(int i=0;i<REGS_COUNT;i++){ // no copy_constructor to tcontext.
        context[threadid].reg[i] = Fined->threads[threadid].reg[i];
    }
}
