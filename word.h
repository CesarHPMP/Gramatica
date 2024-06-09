#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif

// Function prototypes
char **find_rule(char *, char, const int);
void process_word(Node *, gramatica, char *);

// Process a word based on the grammar and add it to the tree
void process_word(Node *root, gramatica gram, char *word) 
{
    if (word == NULL || *word == '\0')
        return;

    if (root == NULL) 
    {
        root = (Node *)malloc(sizeof(Node));

        if (root == NULL) 
        {
            fprintf(stderr, "Memory allocation failed for root node.\n");
            return;
        }

        root->token = NULL;
        root->esq = NULL;
        root->dir = NULL;
    }

    if (root->token != NULL) 
        goto end;
    

    char **matches = find_rule(gram.P, *word, 0);
    if (matches == NULL) 
    {
        fprintf(stderr, "Failed to find rules.\n");
        return;
    }

    int i = 0;

    while (matches[i] != NULL) 
    {
        printf("\nMATCH %c COM ", *word);
        for (int n = 0; matches[i][n] != ';'; n++)
            printf("%c", matches[i][n]);
        i++;
    }

    i = 0;
    while (matches[i] != NULL) 
    {
        if (!test_rule_product(matches[i], word, gram)) 
        {
            matches[i] = NULL;
            i++;
            continue;
        }

        root->token = (char *)malloc(strlen(matches[i]) + 1);
        if (root->token == NULL)
        {
            fprintf(stderr, "Memory allocation failed for root token.\n");
            free(matches);
            return;
        }

        for (int j = 0; matches[i][j] != ';'; j++) 
        {
            root->token[j] = matches[i][j];
        }
        root->token[strlen(matches[i])] = '\0';
        i++;
    }

    printf("Added token to root: %s\n", root->token); // Debug print

    if (root->token != NULL) 
    {
        printf("\nSUCCESSFUL PROCESS FOR %s", root->token);
    }else 
    {
        printf("\nROOT TOKEN NULL\n");
        free(matches);
        return;
    }

    switch (check_word_default(word, root, gram)) 
    {
        case 1:
            printf("\nSUCCESSFUL WORD CREATION\n");
            break;
        case 0:
            printf("\nWORD GONE WRONG");
            break;
        default:
            printf("\nWord is incomplete\n");
    }

end:
    word++;
    if (root->esq == NULL) 
    {
        printf("\nGOING LEFT\n");
        return process_word(root->esq, gram, word);
    } else 
    {
        printf("\nGOING RIGHT\n");
        return process_word(root->dir, gram, word);
    }

    free(matches);
}

// Find a rule in the grammar
char **find_rule(char *rules, char var, const int n) 
{
    bool s = false;
    char **p = NULL;
    int match_count = 0;

    printf("\nEsta em find_rule\n");
    p = (char **)malloc(strlen(rules) * sizeof(char *)); // Allocate memory for array of pointers

    do 
    {
        p[match_count] = (char *)malloc(2 * sizeof(char)); // Allocate memory for a character and null terminator

        switch (*rules) 
        {
            case ':':
                s = true;
                break;
            case ';':
                s = false;
                break;
        }

        if (*rules == var && !s && n == 1) 
        {
            char *temp = rules;

            while (*temp != ';')
                temp--;

            temp++;
            p[match_count] = temp;
            match_count++;
        }
        if (*rules == var && s && n == 0) 
        {
            char *temp = rules;

            while (*temp != ';')
                temp--;

            temp++;
            p[match_count] = temp;
            match_count++;
        }
        rules++;
    } while (*rules != '\0'); // Make sure not to exceed array bounds

    p[match_count] = NULL; // Set the last pointer in the array to NULL to signify the end

    // Free any unused memory
    p = realloc(p, (match_count + 1) * sizeof(char *));

    printf("\nSuccess for alloc\n");
    return p;
}

