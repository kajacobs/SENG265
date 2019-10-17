#!/usr/bin/env bash
#--coding:utf-8 --

txt_files=(./a2tests/test*.txt)
mtf_files=(./a2tests/test*.mtf)

for ((i=0; i<${#mtf_files[@]}; i+=1)); do
  printf -v j "%02d" $i
  chmod +x ${txt_files[i]}
  python3 mtf2text.py ${mtf_files[i]}
  if diff ${txt_files[i]} ./test${j}.txt
  then
    echo test ${i} passed
  else
    echo test ${i}  failed
  fi
  rm ./test${j}.txt
done
