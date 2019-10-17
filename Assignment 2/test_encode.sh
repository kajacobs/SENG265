#!/usr/bin/env bash
#--coding:utf-8 --

txt_files=(./a2tests/test*.txt)
mtf_files=(./a2tests/test*.mtf)

for ((i=0; i<${#txt_files[@]}; i+=1)); do
  printf -v j "%02d" $i
  chmod +x ${mtf_files[i]}
  python3 text2mtf.py ${txt_files[i]}
  if cmp ${mtf_files[i]} ./test${j}.mtf
  then
    echo test ${i} passed
  else 
    echo test ${i}  failed
  fi
  rm ./test${j}.mtf
done 
