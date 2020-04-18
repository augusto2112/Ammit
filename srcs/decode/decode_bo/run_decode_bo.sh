#!/bin/bash
  
###############################################################################
# Driver for the decode_bo function.
# Author: Fernando Magno Quintao Pereira
# Date: April 19th, 2020
# Usage: run.sh SIZE CODE ITER
# -------------------------------
# Results on 1.4GHz Intel Core i5:
# OFF10, S2, S1_D,
# 3.14, 2.81, 2.84,
# 2.92, 2.85, 2.87,
# 2.95, 2.85, 2.84,
# 2.92, 2.92, 2.79,
# 2.94, 2.74, 2.75,
# 2.85, 2.73, 2.73,
###############################################################################

if [ $# -lt 3 ]
then
  echo Syntax: run.sh SIZE CODE ITER
  exit 1
else
  clang++ -O1 decode_bo_OFF10.c -o decode_bo_OFF10.exe
  clang++ -O1 decode_bo_S2.c -o decode_bo_S2.exe
  clang++ -O1 decode_bo_S1_D.c -o decode_bo_S1_D.exe
  SIZE=$1
  CODE=$2
  ITER=$3
  echo "OFF10, S2, S1_D,"
  for (( c=0; c<=ITER; c++))
  do
    ./decode_bo_OFF10.exe $SIZE $CODE
    ./decode_bo_S2.exe $SIZE $CODE
    ./decode_bo_S1_D.exe $SIZE $CODE
    echo ""
  done
  rm -rf decode_bo_OFF10.exe
  rm -rf decode_bo_S2.exe
  rm -rf decode_bo_S1_D.exe
fi
