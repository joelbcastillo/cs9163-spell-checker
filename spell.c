#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


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
char *read_line(FILE *fp, size_t size)
{
    char *str;

    int cur_char;

    size_t len = 0;

    str = realloc(NULL, sizeof(char) * size);

    if (!str)
    {
        return str;
    }

    while (EOF != (cur_char = fgetc(fp)) && cur_char != '\n')
    {
        str[len++] = cur_char;
        if (len == size)
        {
            str = realloc(str, sizeof(char) * (size += 16));
            if (!str)
            {
                return str;
            }
        }
    }
    str[len++] = '\0';

    return realloc(str, sizeof(char) * len);
}

/**
 * Removes punctuation characters from the provided string. Returns the modified string.
 */
/**
 * Inputs:
 *   word:      String to modify
 * 
 * Returns:
 *   char *:    Modified string (without punctuation)
 * 
 * Example:
 *  char * new_word = remove_punctuation("he.llo!");
 **/

char *remove_punctuation_and_uppercase_characters(const char *word)
{
    char *fixed_word = malloc(sizeof(char) * LENGTH+1);

    int i = 0;

    while (word[i] != '\0')
    {
        if (ispunct(word[i]))
        {
            i++;
            continue;
        }
        else if (isupper(word[i]))
        {
            fixed_word[i] = tolower(word[i]);
            i++;
            continue;
        }
        else
        {
            fixed_word[i] = word[i];
            i++;
        }
    }
    return fixed_word;
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
    FILE *dictionary = fopen(dictionary_file, "r");

    if (dictionary == NULL)
    {
        printf("Unable to load dictionary: %s\n", dictionary_file);
        return false;
    }

    char temp_word[LENGTH + 1] = {'\0'};

    int num_words = 0;

    while (fscanf(dictionary, "%s", temp_word) == 1)
    {
        num_words++;
        // Create new empty node to store current dictionary word
        hashmap_t new_node = malloc(sizeof(node));

        // Set new_node next to NULL
        new_node->next = NULL;
        char * word_to_store = NULL;
        
        word_to_store = remove_punctuation_and_uppercase_characters(temp_word);

        // Save current word in new_node->word
        strncpy(new_node->word, word_to_store, strlen(word_to_store));

        // Determine hash value for word
        int hash_bucket = hash_function(word_to_store);

        // Store new node in hashtable in hash_bucket
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
    fclose(dictionary);
    return true;
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
bool check_word(const char *word, hashmap_t hashtable[])
{
    char * basic_word = remove_punctuation_and_uppercase_characters(word);

    int bucket = hash_function(basic_word);
    printf("Bucket: %i\n", bucket);

    node* cursor = hashtable[bucket];

    while (cursor != NULL) {
        printf("%s: %s\n", basic_word, cursor->word);
        if (strcmp(basic_word, cursor->word) == 0) {
            return true;
        } else {
            cursor = cursor -> next;
        }
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
int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[])
{
    int num_misspelled = 0;
    char *line = NULL;
    size_t line_len = 0;

    while (getline(&line, &line_len, fp) != -1)
    {
        char *word = strtok(line, " \t\n");
        while (word != NULL)
        {
            printf("strlen(%s): %zu\n", word, strlen(word));
            if (!check_word(word, hashtable))
            {
                char *misspelled_word = malloc(strlen(word) * sizeof(char));
                strncpy(misspelled_word, word, strlen(word));
                misspelled[num_misspelled] = misspelled_word;
                num_misspelled++;
                if (num_misspelled > MAX_MISSPELLED)
                {
                    return num_misspelled;
                }
            }
            word = strtok(NULL, " \t\n");
        }
    }
    return num_misspelled;
}