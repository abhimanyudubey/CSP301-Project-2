#!/bin/bash
make all
./build/Generator & ./build/Timekeeper
cp gyani.pl build
perl gyani.pl