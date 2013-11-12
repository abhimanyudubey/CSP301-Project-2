#!/bin/bash
g++ -std=c++11 generator_studentfriends.cpp -o Generator -fopenmp
g++ TimeKeepernew3.cpp -o Timekeeper 
g++ Analyzer.cpp -o Analyzer
./Generator & ./Timekeeper
perl gyani.pl