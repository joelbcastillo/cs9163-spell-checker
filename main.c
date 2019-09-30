#include <stdio.h>
#include "dictionary.h"

int main(int argc, char *argv[])
{
   if (argc != 3) {
      printf("incorrect number of arguments. Expected 2 received %d\n", argc);
      return 1;
   }
   char *filename = argv[1];
   char *dictionary_file = argv[2];

   if (filename == NULL) {
      printf("Filename not valid.\n");
      return 1;
   }
   if (dictionary_file == NULL) {
      printf("Dictionary filename not valid.\n");
      return 1;
   }
   hashmap_t dictionary[HASH_SIZE];
   bool successful_load_dictionary = load_dictionary(dictionary_file, dictionary);
   if (!successful_load_dictionary) {
      printf("Unable to load dictionary.\n");
      return 1;
   }
   FILE *check_fp = fopen(filename, "r");

   if (check_fp == NULL) {
      printf("Unable to open filename.\n");
   }
   char *mispelled_words[MAX_MISSPELLED];
   int incorrect_words_cnt = check_words(check_fp, dictionary, mispelled_words);

   printf("misspelled words count: %i\n", incorrect_words_cnt);
   printf("misspelled_words:\n");

   // // Loop through mispelled_words array and printf
   for(int i = 0; i < incorrect_words_cnt; i++) {
      printf("\t%s\n", mispelled_words[i]);
   }
   return 0;
}