// Implements a dictionary's functionality

#include "dictionary.h"
#include <cs50.h> // imports tolower
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h> // imports strcasecmp

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26; // 26 Letters in the alphbet

// Hash table
node *table[N];

// initialise positive hash value and word count
unsigned int hash_value;
unsigned int word_count;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get the hash value
    hash_value = hash(word);
    // Set pointer to respective linked list via hash value
    node *cursor = table[hash_value];

    // Traverse the linked list
    while (cursor != NULL) // When it is null, you've reached the end of the linked list
    {
        // Use strcasecmp to check word
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        // Now move the cursor to the next code if the word does not match
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash from http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = tolower(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *in_file = fopen(dictionary, "r");
    // Return false if unable to open
    if (in_file == NULL)
    {
        return false;
    }

    // Storage space for word
    char word[LENGTH + 1];

    while (fscanf(in_file, "%s", word) != EOF)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        // Return false if failed to allocate memory
        if (n == NULL)
        {
            return false;
        }

        // Copies word into character array "n->word"
        strcpy(n->word, word);
        // Hash the word to get hash value
        hash_value = hash(word);
        // Set the new pointer
        n->next = table[hash_value];
        // Then set the head to the new pointer
        table[hash_value] = n;
        // Increment word count
        word_count++;
    }

    // Close file
    fclose(in_file);

    // If dict is loaded, return true
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{

    // If there are words, return the amount of words
    if (word_count > 0)
    {
        // Return count
        return word_count;
    }
    // Else
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Run through total buckets set above in 'N'
    for (int i = 0; i < N; i++)
    {
        // Set cursor to this each bucket location, starting with i
        node *cursor = table[i];
        // If cursor is valid, free tmp
        while (cursor)
        {
            // Create temp
            node *tmp = cursor;
            // Move cursor to next node
            cursor = cursor->next;
            // Free up temp
            free(tmp);
        }
        // If we're on the last bucket and cursor is NULL
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }

    return false;
}
