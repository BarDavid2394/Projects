#include <string.h>
#include <iostream>
#include <list>
#include <vector>
#include <cstddef>
#include <stdio.h>
#include <pthread.h>
#include "ReadWrite.h"
#include "Bank.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#define DEBUG 1
#define BALANCE_ERROR 1
#define PASSWORD_ERROR 0


 void* fees(void* arg);
 void* print_accounts(void* arg);
//a struct to pass arguments for each Thread - in our case ATMs,Fees and Prints
struct ThreadArgs {
    int id;
    ifstream* file;
    ThreadArgs(): id(0){};
}typedef arg_t;

bank_t* Bank = new bank_t ;
bool done = false;
void* operation (void* arg);
void not_found(int atm_id, int account, int read_write);
pthread_t fees_thread;
pthread_t screen_thread;
fstream log_file;

int main(int argc, char* argv[]) {
    ifstream *files = new ifstream [argc-1];
    arg_t* arr_args = new arg_t [argc-1];
    log_file.open("log.txt",ios_base::out);
    Bank->atms.resize(argc-1);
    Bank->threads.resize(argc-1);
    //we open each file and check if it was opened correctly
    for(int i=0;i<argc-1;i++){
        files[i].open(argv[i+1]);
        if(!files[i].is_open()){ // if one of the files failed to be opened, we delete everything and exit
            cerr<<"Bank error : file.open failed"<<endl;
            delete[] files;
            delete[] arr_args;
            log_file.close();
            delete Bank;
            exit(1);
        }
        arr_args[i].id = i+1;
        arr_args[i].file = &files[i];


        Bank->atms[i].acc_map = &(Bank->accounts); // might move to be inside arg_t
        Bank->atms[i].id = i+1;
        if(pthread_create(&(Bank->threads[i]), NULL, operation, (void*)(&(arr_args[i])))){
            perror("Bank error: pthread_create failed");
            exit(1);
        }
    }// Create fees_thread
    if(pthread_create(&(fees_thread), NULL,fees, NULL)){
        perror("Bank error: pthread_create failed");
        exit(1);
    }//Create screen_thread for printing
    if(pthread_create(&(screen_thread), NULL,print_accounts, NULL)){
        perror("Bank error: pthread_create failed");
        exit(1);
    }
    for(int i=0;i<argc-1;i++){ //Waiting for all the ATM's to finish their work.
        if(pthread_join((Bank->threads[i]), NULL)){
            perror("Bank error: pthread_join failed");
            exit(1);
        }
    }
    done = true;
    if(pthread_join((fees_thread), NULL)){
        perror("Bank error: pthread_join failed");
        exit(1);
    }
    if(pthread_join((screen_thread), NULL)){
        perror("Bank error: pthread_join failed");
        exit(1);
    }
    for(int i=0;i<argc-1;i++){ //Closing all files
        files[i].close();
    }
    delete[] files;
    delete[] arr_args;
    log_file.close();
    delete Bank;
    return 0;

}
// Function to log an error and unlock the account
void log_and_unlock(int atm_id, int account_id,int amount,int error_type) {
    lock(&Bank->log_mutex);
    Bank->bank_lock.leave_reader();
    if(error_type==BALANCE_ERROR){
        log_file << "Error " << atm_id << ": Your transaction failed - account id "
                 << account_id << " balance is lower than " << amount << endl;
    }
    else if(error_type==PASSWORD_ERROR){
        log_file << "Error " << atm_id << ": Your transaction failed - password for account id "
                 << account_id << " is incorrect" << endl;
    }
    unlock(&Bank->log_mutex);
    sleep(1);
    Bank->accounts[account_id].account_lock.leave_writer();
}

// Function to log a transaction
void log_transaction(int atm_id, int amount, int source_acc, int target_acc) {
    lock(&Bank->log_mutex);
    log_file << atm_id << ": Transfer " << amount << " from account " <<
             source_acc << " to account " << target_acc << " new account balance is " <<
             Bank->accounts[source_acc].balance << " new target account balance is " <<
             Bank->accounts[target_acc].balance << endl;
    unlock(&Bank->log_mutex);
    sleep(1);
}


//This is the function that all the ATMs threads are pointing to. here we handle all the cases
void* operation (void* arg) {
    atm_t *curr_atm;
    int atm_id = ((arg_t *) arg)->id;
    ifstream& curr_file = *(((arg_t *) arg)->file);
    std::vector<atm_t>::iterator it_atm = Bank->atms.begin();
    string line;
    string word;

    for (; it_atm != Bank->atms.end(); it_atm++) { //looking for the right ATM
        if (it_atm->id == atm_id) {
            curr_atm = &(*it_atm);
            break;
        }
    }
    while (getline(curr_file,line)) {
        vector<string> input_arg;
        stringstream my_str(line);
        while (my_str >> word) {
            input_arg.push_back(word);
        }
        if (input_arg.empty()) {
            break;
        }
        string action = input_arg.front();
        input_arg.erase(input_arg.begin());
        int account = atoi(input_arg.front().c_str());
        input_arg.erase(input_arg.begin());
        int password = atoi(input_arg.front().c_str());
        input_arg.erase(input_arg.begin());
        sleep(0.1);
        if (action[0] == 'O') { //Open an account
            int initial_amount = atoi(input_arg.front().c_str());
            input_arg.erase(input_arg.begin());
            Bank->bank_lock.enter_writer();
            if (Bank->accounts.find(account) != Bank->accounts.end()) { // account is found
                lock(&Bank->log_mutex);
                Bank->bank_lock.leave_writer();
                log_file << "Error " << curr_atm->id << ": Your transaction failed"
                                                        " - account with the same id exists" << endl;
                unlock(&Bank->log_mutex);
                sleep(1);
            }else {
                acc_t new_acc(account, password, initial_amount);
                Bank->accounts[account] = new_acc;
                lock(&Bank->log_mutex);
                if (password < 10){
                    log_file << atm_id << ": New account id is " << account <<
                         " with password 000" << password << " and initial balance "
                         << initial_amount << endl;
                }
                else if (password < 100){
                    log_file << atm_id << ": New account id is " << account <<
                         " with password 00" << password << " and initial balance "
                         << initial_amount << endl;
                }
                else if (password < 1000){
                    log_file << atm_id << ": New account id is " << account <<
                         " with password 0" << password << " and initial balance "
                         << initial_amount << endl;
                }
                else {
                    log_file << atm_id << ": New account id is " << account <<
                         " with password " << password << " and initial balance "
                         << initial_amount << endl;
                }
                unlock(&Bank->log_mutex);
                sleep(1);
                Bank->bank_lock.leave_writer();
            }

        } else if (action[0] == 'D') { //Deposit to a specific account
            Bank->bank_lock.enter_reader();;
            bool exists_D = false;
            int d_amount = atoi(input_arg.front().c_str());
            input_arg.erase(input_arg.begin());
            if (Bank->accounts.find(account) != Bank->accounts.end()) { // account is found
                exists_D = true;
                Bank->accounts[account].account_lock.enter_reader();
                if (password != Bank->accounts[account].password) {
                    lock(&Bank->log_mutex);
                    Bank->bank_lock.leave_reader();
                    log_file << "Error " << curr_atm->id << ": Your transaction failed"
                                                            " - password for account id "<< account <<" is incorrect" << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                    Bank->accounts[account].account_lock.leave_reader();
                } else {
                    Bank->accounts[account].account_lock.leave_reader();
                    Bank->accounts[account].account_lock.enter_writer();
                    Bank->bank_lock.leave_reader();
                    Bank->accounts[account].balance += d_amount;
                    lock(&Bank->log_mutex);
                    log_file << curr_atm->id << ": Account " << account << " new balance is "
                             << Bank->accounts[account].balance << " after " << d_amount << " $ was deposited" << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                    Bank->accounts[account].account_lock.leave_writer();
                }
            }
            if (!exists_D) {
                not_found(curr_atm->id, account, 0);
            }
        }

        else if (action[0] == 'W') { //Withdraw from a specific account
            int w_amount = atoi(input_arg.front().c_str());
            input_arg.erase(input_arg.begin());
            Bank->bank_lock.enter_reader();
            bool exists_W = false;
            if (Bank->accounts.find(account) != Bank->accounts.end()) { // account is found
                exists_W = true;
                Bank->accounts[account].account_lock.enter_reader();
                if (password != Bank->accounts[account].password) {
                    Bank->accounts[account].account_lock.leave_reader();
                    lock(&Bank->log_mutex);
                    Bank->bank_lock.leave_reader();
                    log_file << "Error " << curr_atm->id << ": Your transaction failed"
                                                            " - password for account id "<< account <<
                             " is incorrect" << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                } else { //password is correct
                    Bank->accounts[account].account_lock.leave_reader();
                    Bank->accounts[account].account_lock.enter_writer();
                    Bank->bank_lock.leave_reader();
                    if (Bank->accounts[account].balance < w_amount) {
                        lock(&Bank->log_mutex);
                        log_file << "Error " << curr_atm->id << ": Your transaction failed"
                                                                " - account id " << account <<
                                 " balance is lower than " << w_amount << endl;
                        unlock(&Bank->log_mutex);
                        sleep(1);
                        Bank->accounts[account].account_lock.leave_writer();
                    } else { // w_amount is lower than balance - good!
                        Bank->accounts[account].balance -= w_amount;
                        lock(&Bank->log_mutex);
                        log_file << curr_atm->id << ": Account " << account << " new balance is "
                                 << Bank->accounts[account].balance << " after " << w_amount << " $ was withdrew"
                                 << endl;
                        unlock(&Bank->log_mutex);
                        sleep(1);
                        Bank->accounts[account].account_lock.leave_writer();
                    }
                }
            }
            if (!exists_W) {
                not_found(curr_atm->id, account, 0);
            }
        }



        else if (action[0] == 'B') { //Check the balance of a specific account.
            Bank->bank_lock.enter_reader();
            bool exists_B = false;
            if (Bank->accounts.find(account) != Bank->accounts.end()) { // account is found
                exists_B = true;
                Bank->accounts[account].account_lock.enter_reader();
                if (password != Bank->accounts[account].password) {
                    lock(&Bank->log_mutex);
                    Bank->bank_lock.leave_reader();
                    log_file << "Error " << curr_atm->id << ": Your transaction failed"
                                                            " - password for account id "
                             << account <<
                             " is incorrect" << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                    Bank->accounts[account].account_lock.leave_reader();
                } else { //password is correct
                    Bank->bank_lock.leave_reader();
                    lock(&Bank->log_mutex);
                    log_file << curr_atm->id << ": Account " << account
                             << " balance is "
                             << Bank->accounts[account].balance << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                    Bank->accounts[account].account_lock.leave_reader();
                };
            }
            if (!exists_B) {
                not_found(curr_atm->id, account, 0);
            }
        }


        else if (action[0] == 'Q') { //Close a specific account
            Bank->bank_lock.enter_writer();
            bool exists_Q = false;
            if (Bank->accounts.find(account) != Bank->accounts.end()) { // account is found
                exists_Q = true;
                Bank->accounts[account].account_lock.enter_reader();
                if (password != Bank->accounts[account].password) {
                    lock(&Bank->log_mutex);
                    Bank->bank_lock.leave_writer();
                    log_file << "Error " << curr_atm->id << ": Your transaction failed"
                                                            " - password for account id "
                             << account <<
                             " is incorrect" << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                    Bank->accounts[account].account_lock.leave_reader();
                } else {
                    int last_balance = Bank->accounts[account].balance;
                    Bank->accounts[account].account_lock.leave_reader();
                    Bank->accounts[account].account_lock.enter_reader();
                    Bank->accounts[account].account_lock.leave_writer();
                    Bank->accounts.erase(account);
                    if(last_balance < 0){
                        Bank->bank_lock.leave_writer();
                        continue;
                    }
                    lock(&Bank->log_mutex);
                    log_file << curr_atm->id << ": Account " << account
                             << " is now closed. Balance was "
                             << last_balance << endl;
                    unlock(&Bank->log_mutex);
                    sleep(1);
                    Bank->bank_lock.leave_writer();
                }
            }
            if (!exists_Q) {
                not_found(curr_atm->id, account, 1);
            }
        }
        else if (action[0] == 'T') { //Make a transfer between 2 different accounts
                bool T_exist=false;
                bool DT_exist=false;
                int target_acc = atoi(input_arg.front().c_str());
                input_arg.erase(input_arg.begin());
                int amount = atoi(input_arg.front().c_str());
                input_arg.erase(input_arg.begin());

                int smaller_id = std::min(account, target_acc);
                int greater_id = std::max(account, target_acc);

                Bank->bank_lock.enter_reader();

                if (Bank->accounts.find(smaller_id) != Bank->accounts.end()) { // Check if smaller account exists
                    T_exist=true;
                    Bank->accounts[smaller_id].account_lock.enter_writer(); // Lock smaller account

                    if (password != Bank->accounts[smaller_id].password) { // Check password
                    log_and_unlock(curr_atm->id, smaller_id,amount, PASSWORD_ERROR);
                        continue;
                    }

                    if (Bank->accounts[smaller_id].balance < amount) { // Check balance
                        log_and_unlock(curr_atm->id, smaller_id,amount,BALANCE_ERROR);
                        continue;
                    }

                    if (Bank->accounts.find(greater_id) != Bank->accounts.end()) { // Check if greater account exists
                        DT_exist=true;
                        Bank->accounts[greater_id].account_lock.enter_writer(); // Lock greater account

                        // Perform the transaction
                        Bank->accounts[smaller_id].balance -= amount;
                        Bank->accounts[greater_id].balance += amount;

                        log_transaction(curr_atm->id, amount, smaller_id, greater_id);

                        // Unlock accounts
                        Bank->accounts[greater_id].account_lock.leave_writer();
                    }
                    Bank->accounts[smaller_id].account_lock.leave_writer();
                }
            if (!T_exist) {
                not_found(curr_atm->id, account, 0);
                continue;
            }
            if (!DT_exist) {
                not_found(curr_atm->id, target_acc, 0);
                continue;
            }

        Bank->bank_lock.leave_reader();
            }
        fflush(stdout);
    }
    pthread_exit(NULL);
}

//Lock a mutex

void lock(pthread_mutex_t* mutex){
    if(pthread_mutex_lock(mutex)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(1);
    }
}
//unLock a mutex
void unlock(pthread_mutex_t* mutex){
    if(pthread_mutex_unlock(mutex)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(1);
    }
}
//if the account is not found, we print to the log. we handle if we lock before to write or read.
void not_found(int atm_id, int account, int read_write) { // if we send 0 in read_write - its read. else write
    lock(&Bank->log_mutex);
    if(read_write){
        Bank->bank_lock.leave_writer();
    }
    else{
        Bank->bank_lock.leave_reader();
    }
    log_file << "Error " << atm_id << ": Your transaction failed"
                                                   " - account id " << account <<
                    " does not exist" << endl;
    unlock(&Bank->log_mutex);
    sleep(1);

}

//Hnadling the accounts prints to the screen each 0.5 seconds.
void*  print_accounts(void* arg){
    while(!done){
        sleep(0.5);
        printf("\033[2J");
        printf("\033[1;1H");
        cout << "Current Bank Status" << endl;
        Bank->bank_lock.enter_reader();
        if(Bank->accounts.empty()){
            Bank->bank_lock.leave_reader();
            continue;
        }
        for (auto it_acc = Bank->accounts.begin(); it_acc != Bank->accounts.end() ; it_acc++) {
            it_acc->second.account_lock.enter_reader();
            it_acc->second.print_account();
            it_acc->second.account_lock.leave_reader();
            lock(&Bank->balance_mutex);
            cout << " The Bank has " << Bank->bank_balance << " $" << endl;
            unlock(&Bank->balance_mutex);
        }
        Bank->bank_lock.leave_reader();
    }
    pthread_exit(NULL);
}


//Taking fess from all the accounts every 3 seconds.
void* fees(void* arg){
    while(!done){
        sleep(3);
        Bank->bank_lock.enter_reader();
        double random = std::rand() % 5 + 1;
        double fee = (1-((double)(random/100)));
        double money_taken = 0.0;
        double curr_money = 0;
        double curr_account;// used to calculate the new account balance after round
        for (auto it_acc = Bank->accounts.begin(); it_acc != Bank->accounts.end() ; it_acc++) {
            it_acc->second.account_lock.enter_writer();// Maybe add enter + leave reader to the read part
            curr_money = ::round(it_acc->second.balance * (1-fee));
            money_taken = curr_money; // balance * fee in %
            if(it_acc->second.balance < 0){
                it_acc->second.account_lock.leave_writer();
                continue;
            }
                it_acc->second.balance *= fee;
                curr_account = round(it_acc->second.balance);
                it_acc->second.balance = curr_account;
                lock(&Bank->balance_mutex);
                Bank->bank_balance +=money_taken;
                unlock(&Bank->balance_mutex);
                lock(&Bank->log_mutex);
                log_file <<"Bank: commissions of " << random << " % were charged, the bank gained " <<
                         money_taken <<" $ from account " <<  it_acc->first << endl;
                unlock(&Bank->log_mutex);
                it_acc->second.account_lock.leave_writer();
        }
        Bank->bank_lock.leave_reader();
    }
    pthread_exit(NULL);
}