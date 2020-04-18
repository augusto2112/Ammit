#!/bin/bash
  
###############################################################################
# Driver for the decode_bo function.
# Author: Fernando Magno Quintao Pereira
# Date: April 19th, 2020
# Usage: run.sh SIZE CODE ITER COMP
# -------------------------------
# Results on 1.4GHz Intel Core i5:
# OFF4, S1,
###############################################################################

if [ $# -lt 4 ]
then
  echo Syntax: run.sh SIZE CODE ITER COMP
  exit 1
else
  SIZE=$1
  CODE=$2
  ITER=$3
  COMP=$4
  $COMP -O1 decode_ssro_OFF4.c -o decode_ssro_OFF4.exe
  $COMP -O1 decode_ssro_S1.c -o decode_ssro_S1.exe
  echo "OFF4, S1,"
  for (( c=0; c<=ITER; c++))
  do
    ./decode_ssro_OFF4.exe $SIZE $CODE
    ./decode_ssro_S1.exe $SIZE $CODE
    echo ""
  done
  rm -rf decode_ssro_OFF4.exe
  rm -rf decode_ssro_S1.exe
fi
