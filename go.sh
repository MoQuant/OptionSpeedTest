#!/bin/bash
echo "Option Pricing Battle"
gcc -o c sim.c
g++ -o s sim.cpp -std=c++14
echo "Compiled Successfully"
echo "___________________________"
./s
echo "---------------------------"
./c
echo "---------------------------"
echo "Run Successful"
exit 0
