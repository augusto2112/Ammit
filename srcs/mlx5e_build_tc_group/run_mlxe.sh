#!/bin/bash
  

if [ $# -lt 4 ]
then
  echo Syntax: run.sh SIZE CODE ITER COMP
  exit 1
else
  SIZE=$1
  CODE=$2
  ITER=$3
  COMP=$4
  $COMP -O1 mlx5e_build_tc_group_123.c -o mlx5e_build_tc_group_123.exe
  $COMP -O1 mlx5e_build_tc_group_132.c -o mlx5e_build_tc_group_132.exe
  $COMP -O1 mlx5e_build_tc_group_213.c -o mlx5e_build_tc_group_213.exe
  $COMP -O1 mlx5e_build_tc_group_231.c -o mlx5e_build_tc_group_231.exe
  $COMP -O1 mlx5e_build_tc_group_312.c -o mlx5e_build_tc_group_312.exe
  $COMP -O1 mlx5e_build_tc_group_321.c -o mlx5e_build_tc_group_321.exe

  echo -e "123,  132,  213,  231,  312,  321"
  for (( c=0; c<=ITER; c++))
  do
    ./mlx5e_build_tc_group_123.exe $SIZE $CODE 
    # echo -e "\t"
    ./mlx5e_build_tc_group_132.exe $SIZE $CODE
    # echo -e "\t"
    ./mlx5e_build_tc_group_213.exe $SIZE $CODE
    # echo -e "\t"
    ./mlx5e_build_tc_group_231.exe $SIZE $CODE
    # echo -e "\t"
    ./mlx5e_build_tc_group_312.exe $SIZE $CODE
    # echo -e "\t"
    ./mlx5e_build_tc_group_321.exe $SIZE $CODE
    # echo -e "\t"
    echo ""
  done
  rm -rf mlx5e_build_tc_group_123.exe
  rm -rf mlx5e_build_tc_group_132.exe
  rm -rf mlx5e_build_tc_group_213.exe
  rm -rf mlx5e_build_tc_group_231.exe
  rm -rf mlx5e_build_tc_group_312.exe
  rm -rf mlx5e_build_tc_group_321.exe
fi
