#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "grams.h"
#include "word.h"
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif

int main(void)
{
    Node *palavra;
    palavra = (Node *)malloc(sizeof(Node)); // Allocate memory for the root node
    
    if (palavra == NULL)
    {
        printf("Memory allocation failed for root node.\n");
        return -1;
    }
    
    palavra->esq = NULL;
    palavra->dir = NULL;
    palavra->token = NULL;

    gramatica gram;
    char buff[100];
    char *word = (char *)malloc(sizeof(buff)); // Initialize an empty character array to store the word

    gram_set(&gram); // Setting up the grammar

    printf("Digite a palavra: ");
    fgets(buff, sizeof(buff), stdin);
    buff[strlen(buff) - 1] = '\0'; // Removing the newline character
    word = strdup(buff);
    
    process_word(palavra, gram, word); // Generating the word
    
    printf("\nTree is built\n");

    print_tree(palavra);
    printf("Generated word: %s\n", word); // Printing the generated word
    // Free allocated memory for the parse tree
    free_tree(palavra);

    return 0;
}

