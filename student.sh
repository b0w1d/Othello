#!/bin/bash
g++ -std=c++11 -o exe -O3 main.cpp std1/* std2/*
./exe
rm exe
