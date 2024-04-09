

#ifndef HW2_BANK_H
#define HW2_BANK_H
#include <string.h>
#include <iostream>
#include <time.h>
#include <list>
#include <string>
#include <vector>
#include <cstddef>
#include <pthread.h>
#include "ReadWrite.h"
#include <algorithm>
#include <unordered_map>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <fstream>




using namespace std;
//single account struct
struct account{
    int id;
    int password;
    double balance;
    rw_t account_lock;

    account(): id(0), password(0), balance(0) {};
    account(int id, int password,int balance) : id(id),password(password),balance(balance) {}
    account(const account& other) : id(other.id) ,password(other.password), balance(other.balance){}
    void print_account();


}typedef acc_t;
//single atm struct
struct ATM{
    int id;
    std::fstream* file;
    map<int,acc_t>* acc_map; // So we can access the right bank account


    ATM() : id(0), file(NULL),acc_map(NULL){};
    ATM(int id, fstream* file, map<int,acc_t>* acc_map) : id(id), file(file), acc_map(acc_map){};

}typedef atm_t;


//the whole bank struct
struct Bank{

    rw_t bank_lock;
    pthread_mutex_t log_mutex;
    pthread_mutex_t balance_mutex;
    vector<pthread_t> threads;
    int bank_balance;
    map<int,acc_t> accounts;
    vector<ATM> atms;
    Bank(){
        if(pthread_mutex_init(&balance_mutex,NULL)){

            perror("Bank error: pthread_mutex_init failed");
        }
        if(pthread_mutex_init(&log_mutex,NULL)){
            perror("Bank error: pthread_mutex_init failed");
        }
        bank_balance = 0;
    }
    ~Bank(){
        if(pthread_mutex_destroy(&balance_mutex)){
            cout << "balance mutex destroy" << endl;
            perror("Bank error: pthread_mutex_destroy failed");
        }
        if(pthread_mutex_destroy(&log_mutex)){
            cout << "log mutex destroy" << endl;
            perror("Bank error: pthread_mutex_destroy failed");
        }

    }
}typedef bank_t;
#endif //HW2_BANK_H
