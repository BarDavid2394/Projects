#!/usr/bin/env python3
import subprocess
import os

def run_tests():
    # Path to the Bank program
    bank_program = "./Bank"

    # %%%%%CHANGE DIRECTORY HERE%%%%%
    test_directory = "tests"
    # %%%%%CHANGE DIRECTORY HERE%%%%%

    # Collect test file paths
    test_files = []
    for file_name in os.listdir(test_directory):
        if file_name.startswith("atm") and file_name[3:].isdigit():
            test_files.append(os.path.join(test_directory, file_name))
    
    # Run Bank program with all test files as arguments
    subprocess.run([bank_program] + test_files)

if __name__ == "__main__":
    run_tests()
