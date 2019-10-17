#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_SIZE 20
#define MAX_WORDS 120
#define MAX_LINES 100
#define MAX_LINE_LENGTH 80

char words [MAX_WORDS] [MAX_WORD_SIZE];
int word_count = 128;

/*
 * Print words array, used for testing
 *
void print_words (){
int i;
  for (i = 0; strcmp(words[i], "") != 0; i++){
    printf("%s..", words[i]);
  }
  printf("\n\n\n");
}
*/

/* Searches to see if current word from file already exists in array */
int find_word(char* word) {
  int i = 0;
  for (i = 0; strlen(words[i]); i++){
    if (strcmp(words[i], word) == 0){
      return i;
    }
  }
  return -1;
}

/* Encodes position of existing word to output file
 * moves word to end of array
 */
void move_to_front(int word_position, int i, FILE* outfile){
  int m = i - word_position;
  char c = m + 128;
  fwrite(&c,1,1,outfile);
  char temp[21];
  strncpy(temp, words[word_position], strlen(words[word_position]));
  temp[strlen(words[word_position])] = '\0';
  int j;
  for(j = word_position + 1; j < i; j++){
    strncpy(words[j-1],words[j],strlen(words[j]));
    words[j-1][strlen(words[j])] = '\0';
  }
  strncpy(words[i-1],temp,strlen(temp));
  words[i-1][strlen(temp)] = '\0';
}


/* Writes code and word to file */
void write_word(char* word, FILE* outfile){
  word_count = word_count+1;

  char c = word_count;
  fwrite(&c,1,1,outfile);
  fwrite(word,1, strlen(word),outfile);
}


int main(int argv, char** args) {

  /* Initialize word storage to empty strings */
  int i = 0;
  for (i = 0; i < MAX_WORDS; i++){
    strcpy(words[i], "");
  }

  /* Ensure proper number of arguments */
  if (argv != 2) {
    printf("This program takes exactly one additional argument\n");
    exit(1);
  }

  /* Determining file names for input and output */
  char* input_file = args[1];
  int input_file_len = strlen(input_file);
  char output_file[input_file_len+1];
  output_file[input_file_len] = '\0';
  strncpy(output_file, input_file, input_file_len);
  output_file[input_file_len - 3] = 'm';
  output_file[input_file_len - 2] = 't';
  output_file[input_file_len - 1] = 'f';

  /* Open file */
  FILE* infile = fopen(input_file, "r");
  if (infile == NULL) {
    printf("Could not locate file: %s\n", input_file);
    exit(1);
  }

  /* Creating outfile */
  FILE* outfile = fopen(output_file, "w");
  if (outfile == NULL) {
    printf("Error creating file: %s\n", output_file);
  }
  char magic[4] = {0xba, 0x5e, 0xba, 0x11};
  fwrite(magic,1,4,outfile);

  /* Read input from file */
  char line[80];
  char* word;
  i = 0;
  int word_position = -1;

  while (fgets(line, 80, infile) != NULL){

    /* Tokenize line */
    word = strtok(line, "\n ");
    while (word != NULL) {
      word_position = find_word(word);
      if (word_position == -1){
        /* We have found a new word */
        write_word(word,outfile);
        strcpy(words[i], word);
        i++;
      } else {
        /* Word already exists in file */
        move_to_front(word_position, i, outfile);
      }
      word = strtok(NULL, "\n ");
    } /* end of inner while loop */
    char c = '\n';
    fwrite (&c,1,1,outfile);
  } /* end of outer while loop */

  fclose(infile);
  fclose(outfile);

  return 0;
}
