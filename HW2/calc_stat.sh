#!/bin/bash
#checks if the no. of arguments is 1, as requested.
 if [[ "$#" -ne 1 ]]; then
    echo "Wrong number of arguments" >&2
    exit 1
fi
#checks if the course file is found inside the directory.
 if  ! find . -name "$1.txt" | grep -q .; then
     echo "Course not found" >&2
      exit 1
 fi
 #if the directory already exists, we delete it.
if [ -d "$1_stat" ]; then
   rm -rf "$1_stat"
fi
mkdir "$1_stat"
#running all the relevant c programs exe files.
./hist.exe "./$1.txt" -n_bins 10 > "./$1_stat/histogram.txt"
./mean.exe "./$1.txt" >> "./$1_stat/statistics.txt"
./median.exe "./$1.txt" >> "./$1_stat/statistics.txt"
./min.exe "./$1.txt" >> "./$1_stat/statistics.txt"
./max.exe "./$1.txt" >> "./$1_stat/statistics.txt"
#printing with the results the percentage of student who passed the exam (55 and above)
awk '$1>=55{p++} END {printf "%.0f%%\n", int(p/NR*100)}' "$1.txt" >> "./${1}_stat/statistics.txt"

