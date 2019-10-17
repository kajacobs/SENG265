import os
import sys

def main():
    #checking for correct number of arguments
    if (len(sys.argv) != 2):
        print("This program takes exactly one additional argument\n")
        exit(1)

    #Opening infile
    arg_file = sys.argv[1]
    text_file = open(arg_file)

    #Creating output file name
    input_name = sys.argv[1]
    suffix = '.mtf'
    output_name = (os.path.splitext(os.path.basename(input_name))[0])
    output_name = output_name + suffix


    #Creating and opening outfile
    mtf_file = open(output_name, "a+", encoding = "latin-1")
    #write magic numbers to file here
    magic_word = [0xba, 0x5e, 0xba, 0x11]
    encode_magic(magic_word, mtf_file)

    word_list = []
    count = 129
    #Iterating through each line of the text file
    for line in text_file:
        tokenized_line = line.split()
        #Iterating through each word of the line
        for word in tokenized_line:
            word_pos = find_word(word, word_list)
            if (word_pos == -1):
                write_word(count, word, mtf_file, word_list)
                count += 1
            else:
                move_to_front(word_pos, word_list, mtf_file, word)

        mtf_file.write("\n")

    text_file.close()
    mtf_file.close()


#Function that looks for word in list
#Returns -1 if word is not found (new word to encode)
#Returns index of word if found in list
def find_word(word, word_list):
    for element in word_list:
        if (element == word):
            return word_list.index(element)
    return -1

#Writes encoded number and new word to output file
def write_word(count, word, mtf_file, word_list):
    word_list.append(word)
    mtf_file.write(chr(count))
    mtf_file.write(word)

#Encodes magic number to file
def encode_magic(magic_word, mtf_file):
    for code in magic_word:
        mtf_file.write(chr(code))

# Function encodes position of existing word into output file
# moves word to front of list
def move_to_front(word_pos, word_list, mtf_file, word):
    dist_between_pos = word_list.index(word_list[-1]) - word_pos
    encode = 129 + dist_between_pos
    mtf_file.write(chr(encode))
    word_list.remove(word)
    word_list.append(word)


#Function Decodes a MTF file into text file
#Checks for correct magic number prior to decoding
def decode():
    #Opening infile
    arg_file = sys.argv[1]
    mtf_file = open(arg_file, "rb")


    #Testing if magic word is in mtf file
    magic_word = [0xba, 0x5e, 0xba, 0x11]
    bytes_from_file = []
    mw_from_file = []

    for x in range(4):
        bytes_from_file.append(mtf_file.read(1))

    for item in bytes_from_file:
        mw_from_file.append(int.from_bytes(item, "big"))

    if mw_from_file != magic_word:
            exit(1)

    #Converting mtf file to textfile
    word_list, dict = decode_mtf(mtf_file)
    mtf_file.close()
    length = len(word_list)

    #Creating output file name
    input_name = sys.argv[1]
    suffix = '.txt'
    output_name = (os.path.splitext(os.path.basename(input_name))[0])
    output_name = output_name + suffix

    #Creating and opening outfile
    txt_file = open(output_name, "a+")

    #Writing words to txt_file
    for i, num in enumerate(word_list):
        if num == 10:
            txt_file.write('\n')
        else:
            txt_file.write(dict[num])
            if word_list[i+1] != 10:
                txt_file.write(' ')

    #close txt_file
    txt_file.close()

#Decodes .mtf File
def decode_mtf(file):
    nums_from_file = []

    #reads bytes from file and adds to a list
    b = file.read(1)
    while b != b'':
        nums_from_file.append(int.from_bytes(b, "big"))
        b = file.read(1)

    #creating dictionary words/codes
    dict = {}
    word_list = []
    current_word = 128
    word = []

    #iterating through list of numbers
    for num in nums_from_file:
        #if number indicates new word
        if num == current_word + 1:
            #if word length is > 0, then add word to list
            if len(word) > 0:
                dict[current_word] = ''.join(chr(i) for i in word)
                word = []
            current_word += 1
            word_list.append(current_word)
        #if number is new line character
        elif num == 10:
            word_list.append(10)
        #if number is previously encoded word
        elif num > 128:
            newlist = []
            for n in word_list[::-1]:
                if n != 10 and n not in newlist:
                    newlist.append(n)
            word_list.append(newlist[num-129])
        #add character to current word
        else:
            word.append(num)

    # check for last word of file
    if len(word) > 0:
        dict[current_word] = ''.join(chr(i) for i in word)
        word = []
    return word_list, dict

command = os.path.basename(__file__)
#Invokes main function if text2mtf.py is arg[0]
if __name__ == "__main__" and command == "text2mtf.py":
    main()

#Invokes decode function i mtf2text.py is argv[0]
if __name__ == "__main__" and command == "mtf2text.py":
    decode()
