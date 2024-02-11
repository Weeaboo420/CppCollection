#!/bin/bash
result=$(g++ -std=c++17 main.cpp -o terrain.out)
if [ -z "$result" ]
then
	./terrain.out
else
	:
fi
