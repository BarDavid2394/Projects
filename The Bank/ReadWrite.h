

#ifndef HW2_READWRITE_H
#define HW2_READWRITE_H
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <cstddef>
#include <pthread.h>

using namespace std;

extern void lock(pthread_mutex_t* mutex);
extern void unlock(pthread_mutex_t* mutex);
//Readers Writers lock struct
struct Read_Write_Lock{
    pthread_mutex_t read;
    pthread_mutex_t write;

    int readers;

    Read_Write_Lock(){
        readers = 0;
        if(pthread_mutex_init(&read, nullptr)){
            perror("Bank error: pthread_mutex_init failed");
            exit(1);
        }
        if(pthread_mutex_init(&write, nullptr)){
            perror("Bank error: pthread_mutex_init failed");
            exit(1);
        }

    }


    void enter_reader();
    void leave_reader();
    void enter_writer();
    void leave_writer();

}typedef rw_t;



#endif //HW2_READWRITE_H
