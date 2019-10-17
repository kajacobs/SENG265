#!/usr/bin/env bash
#--coding:utf-8 --

txt_files=(./testsalt/test1*.txt)
mtf_files=(./testsalt/test1*.mtf)
echo testing text files 11-19

for ((i=1; i-1<${#mtf_files[@]}; i+=1)); do
  printf -v j "%01d" $i
  chmod +x ${mtf_files[i-1]}
  python3 decode.py ${mtf_files[i-1]}
  if diff ${txt_files[i-1]} ./tests/test1${j}.txt
  then
    echo test ${i} passed
  else
    echo test ${i}  failed
  fi
done
