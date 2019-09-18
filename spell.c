#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

FILE *open_file(const char *filename)
{
    FILE *fptr;

    if ((fptr = fopen(filename, "r")) == NULL)
    {
        return NULL;
    }

    return fptr;
}

void initialize_hashtable(hashmap_t hashtable[])
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashmap_t cur_hash = malloc(sizeof(struct node));
        cur_hash->next = NULL;
        cur_hash->word[0] = '\0';
    }
}

/**
 * Converts a string to lowercase
 **/
/**
 * Inputs:
 *   str:       A string of characters.
 * 
 * Example:
 *   lower_case(str);
 **/
void remove_punctuation_and_convert_case(char * dest, const char * src) {
    char * src_char = malloc(strlen(src) + 1);

    if (src_char) {
        strcpy(src_char, src);
    }

    while (*src_char) {
        if (ispunct((unsigned char)*src_char)) {
            src_char++;
        } else if (isupper((unsigned char)*src_char)) {
            *dest++ = tolower((unsigned char)*src_char);
            src_char++;
        } else if (src_char == dest) {
            src_char++;
            dest++;
        } else {
            *dest++ = *src_char++;
        }
    }
    *dest = '\0';
}

/**
 * Reads a line from a file of unknown length. Returns the string.
 **/
/**
 * Inputs: 
 *  fp:         A file pointer to the document to check for spelling errors.
 *  size:       The initialize size of the string to allocate in memory.
 * 
 * Returns:
 *  char *:     The line as a pointer to a character string.
 * 
 * Example:
 *  char *line = read_line(fp, 1024)
 **/
char *read_line(FILE* fp, size_t size) {
    char *str;

    int cur_char;

    size_t len = 0;

    str = realloc(NULL, sizeof(char)*size);

    if (!str) {
        return str;
    }

    while (EOF != (cur_char=fgetc(fp)) && cur_char != '\n') {
        str[len++]=cur_char;
        if(len == size) {
            str = realloc(str, sizeof(char)*(size += 16));
            if (!str) {
                return str;
            }
        }
    }
    str[len++] = '\0';

    return realloc(str, sizeof(char)*len);
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
/**
 * Inputs:
 *  dictionary_file:    Path to the words file.
 *  hashtable:          The hash table to be populated.
 *            
 * Returns:
 *  bool:       Whether or not the hashmap successfully populated.
 *
 * Modifies:
 *  hashtable: This hashmap should be filled with words from the file provided.
 *
 * Example:
 *  bool success = load_dictionary("wordlist.txt", hashtable);
 **/
bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[])
{
    FILE *dictionary = open_file(dictionary_file);

    if (dictionary == NULL)
    {
        return false;
    }

    char buffer[LENGTH+1];

    while (fgets(buffer, sizeof(buffer), dictionary) != NULL)
    {
        // Create new empty node to store current dictionary word
        hashmap_t new_node = malloc(sizeof(hashmap_t));
        // Set new_node word = to the current word
        new_node->word[0]= *buffer;
        // Set new_node next to NULL
        new_node->next = NULL;
        // Determine hash_value for current word
        char *word = NULL;
        remove_punctuation_and_convert_case(word, buffer);
        int hash_bucket = hash_function((word));
        // Store new_node in hashtable at value i
        if (hashtable[hash_bucket] == NULL)
        {
            hashtable[hash_bucket] = new_node;
        }
        else
        {
            new_node->next = hashtable[hash_bucket];
            hashtable[hash_bucket] = new_node;
        }
    }
    if (feof(dictionary))
    {
        fclose(dictionary);
        return true;
    }
    else {
        return false;
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *            
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *  
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[]) {
    char *simplified_word = malloc(strlen(word)+1);
    if(simplified_word) {
        remove_punctuation_and_convert_case(simplified_word, word);
    }
    int bucket = hash_function(simplified_word);
    hashmap_t cursor = hashtable[bucket];
    while (cursor != NULL) {
        if (simplified_word == cursor->word) {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}


/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
/**
 * Inputs:
 *  fp:         A file pointer to the document to check for spelling errors.
 *  hashtable:  The hash table used to determine spelling
 *  misspelled: An empty char* array to be populated with misspelled words.
 *              This array will never be greater than 1000 words long.
 *            
 * Returns:
 *  int:        The number of words in the misspelled arary.
 *
 * Modifies:
 *  misspelled: This array will be filled with misspelled words.
 *
 * Example:
 *  int num_misspelled = check_words(text_file, hashtable, misspelled);
 **/
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    int num_misspelled = 0;

    char *line = read_line(fp, BUFSIZ);
    
    while (line != NULL) {
        line = read_line(fp, BUFSIZ);
        char *word = strtok(line, " \t");
        while (word != NULL) {
            if (!check_word(word, hashtable)) {
                num_misspelled++;
                misspelled[num_misspelled] = word;
                if (num_misspelled > MAX_MISSPELLED) {
                    return num_misspelled;
                }
            }
            word = strtok(NULL, " \t");
        }
    }
    return num_misspelled;
}