#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
        hashtable[i] = malloc(sizeof(hashmap_t));
        if (hashtable[i] == NULL)
        {
            break;
        }
        hashtable[i]->word[0] = '\0';
        hashtable[i]->next = NULL;
    }
}

bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[])
{
    initialize_hashtable(hashtable);

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
        int hash_bucket = hash_function(buffer);
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
