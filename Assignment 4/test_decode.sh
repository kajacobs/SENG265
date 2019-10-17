#!/usr/bin/env bash
#--coding:utf-8 --

txt_files=(./Test_Files/test*.txt)
mtf_files=(./Source_Files/test*.mtf)
my_files=(./Source_Files/test*.txt)

make mtf2text2

for ((i=0; i<${#mtf_files[@]}; i+=1)); do
  chmod 777 ${txt_files[i]}
  chmod 777 ${mtf_files[i]}
  ./mtf2text2 ${mtf_files[i]}
  if diff ${txt_files[i]} ./Source_Files/test*${i}.txt
  then
    echo test ${i} passed
  else
    echo test ${i}  failed
  fi
done

make clean
rm ./Source_Files/test*.txt
