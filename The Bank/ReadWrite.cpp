#include "ReadWrite.h"

using namespace std;
void rw_t :: enter_reader(){
    lock(&read);
    readers++;
    if(readers == 1){
        lock(&write);
    }
    unlock(&read);
}
void rw_t :: leave_reader(){
    lock(&read);
    readers--;
    if(readers == 0){
        unlock(&write);
    }
    unlock(&read);
}
void rw_t :: enter_writer(){
    lock(&write);

}
void rw_t :: leave_writer(){
    unlock(&write);
}

