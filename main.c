#include <stdio.h>
#include "dictionary.h"

int main(int argc, char *argv[])
{
   if (argc != 3) {
      printf("incorrect number of arguments. Expected 2 received %d\n", argc);
      return 1;
   }
   char *filename = argv[0];
   char *dictionary_file = argv[1];

   if (filename == NULL) {
      printf("Filename not valid.");
      return 1;
   }
   FILE *check_fp = fopen(filename, "r");
   if (dictionary_file == NULL) {
      printf("Dictionary filename not valid.");
      return 1;
   }
   hashmap_t *dictionary = NULL;
   bool successful_load_dictionary = load_dictionary(dictionary_file, dictionary);
   if (!successful_load_dictionary) {
      printf("Unable to load dictionary.");
      return 1;
   }
   char *mispelled_words[MAX_MISSPELLED];
   int incorrect_words_cnt = check_words(check_fp, dictionary, mispelled_words);

   printf("misspelled words count: %i", incorrect_words_cnt);
   return 0;
}