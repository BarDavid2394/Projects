Bank Implementation in C++
This project implements a bank in C++, featuring multiple ATMs that can perform various operations such as checking balance, withdrawing, depositing, transferring money, opening, and closing accounts. The project utilizes threads for each ATM, as well as additional threads for printing the current bank status (Accounts) and deducting fees from the accounts. Mutex and readers-writers locks are used for concurrency control.

Features
Implementation of a bank system with multiple ATMs.
Each ATM can perform operations like checking balance, withdrawing, depositing, transferring money, opening, and closing accounts.
Use of threads for concurrency control, including threads for ATMs, printing bank status, and deducting fees.
Use of mutex and readers-writers locks for safe concurrent access to shared resources.
