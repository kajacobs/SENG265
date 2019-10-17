#!/usr/bin/python3

import os
import sys
import re

MAGIC_NUMBER_1 = chr(0xBA) + chr(0x5E) + chr(0xBA) + chr(0x11)
MAGIC_NUMBER_2 = chr(0xBA) + chr(0x5E) + chr(0xBA) + chr(0x12)

#Start of encode
def encode(input_name):
    #creating outfile
    (base_name, _, _) = input_name.rpartition(".")
    output_name = base_name + "." + "mtf"

    #opening in and out files,
    # mtf_file set to w+, will erase current contents
    text_file = open(input_name)
    mtf_file = open(output_name, "w+", encoding = "latin-1", newline = "")
    mtf_file.write(MAGIC_NUMBER_2)

    list = []
    count = 129

    #Iterating through the infile
    #Checking if word is not unique
    for line in text_file:
        toke_line = line.split()
        for word in toke_line:
            position = find_word(word, list)
            if (position == -1):
                write_word(count, word, mtf_file, list)
                count += 1
            else:
                mtf(position, list, word, mtf_file)
        mtf_file.write("\n")

    text_file.close()
    mtf_file.close()

#Function searches for word in list, if exists
#returns index
def find_word(word, list):
    for element in list:
        if (element == word):
            return list.index(element)
    return -1
#Write unique words to file
def write_word(count, word, mtf_file, list):
    list.append(word)
    write_code(count, mtf_file)
    mtf_file.write(word)

#Writes code to file based on number of unique words
def write_code(count, mtf_file):
    code2 = count-249
    code3 = (count-504)//256
    code4 = (count-504)%256
    if (count < 249):
        mtf_file.write(chr(count))
    elif (count >= 249 and count <= 503):
        mtf_file.write(chr(249))
        mtf_file.write(chr(code2))
    elif (count > 503 and count <= 66040):
        mtf_file.write(chr(250))
        mtf_file.write(chr(code3))
        mtf_file.write(chr(code4))

#Move to front: searches for word in word_list
#Using index, moves word to front of list
def mtf(position, list, word, mtf_file):
    move_dist = list.index(list[-1]) - position
    code = 129 + move_dist
    write_code(code, mtf_file)
    list.remove(word)
    list.append(word)



#Start of Decode
def decode(input_name):
    (base_name, _, _) = input_name.rpartition(".")
    output_name = base_name + "." + "txt"

    #Opening mtf)file in read bytes
    mtf_file = open(input_name, "rb")

    bytes_from_file = []
    mw_from_file = []
    magic_word = [0xba, 0x5e, 0xba, 0x11]
    magic_word2 = [0xba, 0x5e, 0xba, 0x12]

    #Comparing Magic Numbers
    for x in range(4):
        bytes_from_file.append(mtf_file.read(1))

    for item in bytes_from_file:
        mw_from_file.append(int.from_bytes(item, "big"))

    if mw_from_file != magic_word:
        if mw_from_file != magic_word2:
            print("Incorrect magic number")
            exit(1)

    #Converting mtf file to textfile
    decode_mtf(mtf_file, output_name)
    mtf_file.close()


#Function opens outfile and iterates
#through infile, writes words to outfile
def decode_mtf(file, output_name):
    nums_from_file = []
    b = file.read(1)
    while b != b'':
        nums_from_file.append(int.from_bytes(b, "big"))
        b = file.read(1)

    txt_file = open(output_name, "w+")
    word = []
    word_list = []
    i = 0

    while i < len(nums_from_file):
        f = nums_from_file[i]
        if f > 128 and f < 249:
            if f - 128 <= len(word_list):
                f = f-128
                word_list.append(word_list.pop(-1*f))
                txt_file.write(word_list[-1])
                if nums_from_file[i+1] >128:
                    txt_file.write(' ')
            i += 1
        elif f == 249:
            f = nums_from_file[i+1]+121
            if f <= len(word_list):
                word_list.append(word_list.pop(-1*f))
                txt_file.write(word_list[-1])
                if nums_from_file[i+2] >128:
                    txt_file.write(' ')
            i += 2
        elif f == 250:
            f = 376 + (nums_from_file[i+1]*256) + (nums_from_file[i+2])
            if f <= len(word_list):
                word_list.append(word_list.pop(-1*f))
                txt_file.write(word_list[-1])
                if nums_from_file[i+3] >128:
                    txt_file.write(' ')
            i += 3
        elif f == 10:
            txt_file.write('\n')
            i += 1
        else:
            word.append(f)
            if nums_from_file[i+1] == 10:
                word_list.append(''.join(chr(i) for i in word))
                txt_file.write(''.join(chr(i) for i in word))
                word = []
            elif nums_from_file[i+1] >128:
                word_list.append(''.join(chr(i) for i in word))
                txt_file.write(''.join(chr(i) for i in word))
                txt_file.write(' ')
                word = []
            i += 1
    txt_file.close()
