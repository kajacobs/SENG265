#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char byte;

typedef struct node{
    byte* word;
    struct node* next;
    struct node* prev;
} node;


node* create (byte* word);
node* next (node* ref);
node* prev (node* ref);
node* first (node* ref);
void freeList (node* ref);
node* push (node* ref, byte* word);
byte* moveToFront(node* ref, int number);

void printList(node* ref) {
    if (!ref) {
        printf("Empty List\n");
    } else {
        ref = first(ref);
        while (ref) {
            printf("%s\n",ref->word);
            ref = next(ref);
        }
    }
}

char  magic_number_1[] = {0xBA, 0x5E, 0xBA, 0x11, 0x00};
char  magic_number_2[] = {0xBA, 0x5E, 0xBA, 0x12, 0x00};

/*
* Encode function: no implementation required for A4
*/

int encode(FILE *input, FILE *output) {
    return 0;
}


int decode(FILE *input, FILE *output) {

    /* Retrieves magic number from file
    * compares to magic_number_1 and magic_number_2
    */

    byte magic_code[4];
    fread (magic_code, sizeof(byte), 4, input);
    for (int i = 0; i < 4; i++){
        if (magic_code[i] != (byte)magic_number_1[i]){
            if (magic_code[i] != (byte)magic_number_2[i]){
                printf("Incorrect Magic Code\n");
                //return 0;
            }
        }
    }

    int len = 0;
    char* newline = "\n";
    int wordcount = 0;
    node* wordlist = NULL;
    int code = 0;
    byte* word = NULL;
    char* space = " ";
    byte *curr_byte = (byte*)calloc(1, sizeof(byte));
    byte *curr_word = (byte*)calloc(0, sizeof(byte));


    /* Parses through input file byte by byte
    * determines what to do based on decimal value
    */
    fread(curr_byte, 1, 1, input);
    while (curr_byte){
        //If EOF return 0
        fread(curr_byte, 1, 1, input);
        if (feof(input)){
            break;
        }
        if (*curr_byte == 10){
            // we have a new line character
            if (len != 0){
                if (curr_word != NULL) {
                    wordlist = push(wordlist, curr_word);
                    wordcount++;
                    fwrite(curr_word, 1, len, output);
                    curr_word = NULL;
                }
                len = 0;
            }
            fwrite(newline,1, 1, output);
        } else if (*curr_byte < 129){
            //We have a letter
            if (len < 0) len = 0;
            len++;
            curr_word = (byte*)realloc(curr_word, len*sizeof(byte)+1);
            memcpy(curr_word+len-1, curr_byte, 1);
        } else if (*curr_byte > 128 && *curr_byte <249){
            //we have a single encoded word
            if (len != 0){
                if (curr_word != NULL) {
                    wordlist = push(wordlist, curr_word);
                    wordcount++;
                    fwrite(curr_word, 1, len, output);
                    curr_word = NULL;
                }
                fwrite(space, 1, 1, output);
                len = 0;
            }
            len--;
            code = *curr_byte - 128;
            if (code <= wordcount){
                word = moveToFront(wordlist, code);
                fwrite(word, 1, strlen((char*)word), output);
            }
        } else if (*curr_byte == 249){
            // we have a double encoded word
            if (len != 0){
                if (curr_word != NULL) {
                    wordlist = push(wordlist, curr_word);
                    wordcount++;
                    fwrite(curr_word, 1, len, output);
                    curr_word = NULL;
                }
                fwrite(space, 1, 1, output);
                len = 0;
            }
            len--;
            fread(curr_byte, 1, 1, input);
            code = *curr_byte + 121;
            if (code <= wordcount){
                word = moveToFront(wordlist, code);
                fwrite(word, 1, strlen((char*)word), output);
            }
        } else if (*curr_byte == 250){
            // we have a triple encoded word
            if (len != 0){
                if (curr_word != NULL) {
                    wordlist = push(wordlist, curr_word);
                    wordcount++;
                    fwrite(curr_word, 1, len, output);
                    curr_word = NULL;
                }
                fwrite(space, 1, 1, output);
                len = 0;
            }
            len--;
            fread(curr_byte, 1, 1, input);
            code = 376 + (*curr_byte)*256;
            fread(curr_byte, 1, 1, input);
            code += *curr_byte;
            if (code <= wordcount){
                word = moveToFront(wordlist, code);
                fwrite(word, 1, strlen((char*)word), output);
            }
        }
    }
    /* Free all malloc'ed memory */
    free(curr_word);
    free(curr_byte);
    freeList(wordlist);
    return 0;
}

/*
* Linked list functions
* Code originally from https://gist.github.com/mexuaz/a0014cf8836900ab36e86ce6fe8dd45a
* Repository owner anonymous.
* Heavily adapted to suit this assignment.
*/

node* create (byte* word) {
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Memory allocation failure");
        return NULL;
    }
    newNode->word = word;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

node* next (node* ref) {
    return ref->next;
}

node* prev (node* ref) {
    return ref->prev;
}

node* first (node* ref) {
    while (ref->prev) ref = prev(ref);
    return ref;
}

void freeList (node* ref) {
    if (ref == NULL) return;
    ref = first(ref);
    while (ref->next) {
        ref = ref->next;
        free(ref->prev->word);
        free(ref->prev);
    }
    free(ref->word);
    free(ref);
}

node* push (node* ref, byte* word) {
    if (ref == NULL) {
        return create((byte*)word);
    } else {
        ref = first(ref);
        node* newNode = create((byte*)word);
        ref->prev = newNode;
        newNode->next = ref;
        return newNode;
    }
}

byte* moveToFront(node* ref, int number) {
    if (ref == NULL || number < 1) return NULL;
    ref = first(ref);
    if (number == 1) {
        return ref->word;
    } else {
        node* temp = ref;
        for (int i=1; i<number; i++){
            if (temp->next == NULL) {
                printf("Encoding error\n");
            } else {
                temp = next(temp);
            }
        }
        node* nx = next(temp);
        node* px = prev(temp);
        if (nx) {
            nx->prev = px;
        }
        px->next = nx;
        ref->prev = temp;
        temp->next = ref;
        temp->prev = NULL;
        return temp->word;
    }
}
