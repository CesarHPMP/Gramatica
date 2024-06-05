#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif


char ** find_rule(char *, char , const int );
void process_word(Node *, gramatica , char *);
int check_word(char *, Node *, char *, gramatica, size_t);
int check_word_default(char *, Node *, gramatica);

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
    }

    if (root->token != NULL)
    {
        goto end;
    }

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

    printf("\nLoaded Char is %c\n", matches[0][0]);

    root->token = (char *)malloc(strlen(matches[0]) + 1);
    if (root->token == NULL)
    {
        fprintf(stderr, "Memory allocation failed for root token.\n");
        free(matches);
        return;
    }

    strcpy(root->token, matches[0]);
    printf("Added token to root: %s\n", root->token); // Debug print

    i = 0;
    while (matches[i] != NULL)
    {
        if (!test_rule_product(matches[i], word, gram))
        {
            matches[i] = NULL;
            i++;
            continue;
        }

        for (int j = 0; matches[i][j] != ';'; j++)
        {
            root->token[j] = matches[i][j];
        }
        root->token[strlen(matches[i])] = '\0';
        i++;
    }

    if (root->token != NULL)
    {
        printf("\nSUCCESSFUL PROCESS FOR %s", root->token);
    }
    else
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
        process_word(root->esq, gram, word);
    }
    else
    {
        printf("\nGOING RIGHT\n");
        process_word(root->dir, gram, word);
    }

    free(matches);
}

char ** find_rule(char *rules, char var, const int n)
{// 1 to find rule and 0 to find product
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

        if (*rules == var && !s && n == 1) {
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

    // Set the last pointer in the array to NULL to signify the end
    p[match_count] = NULL;

    // Free any unused memory
    p = realloc(p, (match_count + 1) * sizeof(char *));

    printf("\nSuccess for alloc\n");
    return p;
}

int check_word_default(char *word, Node *tree, gramatica gram)
{
    char *new_word = (char *)malloc(sizeof(word));
    size_t len = 0;
    return check_word(word, tree, new_word, gram, len); // Call the overloaded function
} 

int check_word(char *word, Node *tree, char *new_word, gramatica gram, size_t len)
{
    // Declare variables
    int i;
    char current_char;

    // Check if the tree is NULL
    if (tree == NULL)
        return 0;

    // Check if the token of the current node is not NULL
    if (tree->token != NULL)
    {
        // Loop through the token characters
        i = 0;
        while (tree->token[i] != '\0')
        {
            printf("\nINSIDE TREE TOKEN LOOP\n");
            current_char = tree->token[i];
            // Check if the current character is not a variable
            int holder = testvar(gram, current_char, 2);
            if (holder == 0)
            {
                // Append the current character to the new word
                new_word[len] = current_char;
                new_word[len + 1] = '\0'; // Null-terminate the string
                len++;
            }
            i++;
        }
    }
    else
        return 0;

    // Compare new_word with word
    if (strcmp(new_word, word) == 0)
    {
        printf("Success: The new word matches the original word.\n");
        return 1;
    }
    else
    {
        // Check the length of new_word compared to word
        if (strcmp(new_word, word) < 0)
        {
            if(tree->esq == NULL && tree->dir == NULL)
            {
                printf("Partial Success: The new word is shorter than the original word.\n");
                return 1;
            }
        }
        else
        {
            printf("Failure: The new word does not match the original word.\n");
            return 0; // Return to exit the function in case of failure
        }
    }

    // Continue traversal to the left and right subtrees
    if (tree->esq)
    {
        if(check_word(word, tree->esq, new_word, gram, len) == 0)
            return 0;
        else
            return 1;
    }

    if (tree->dir)
    {
        if(check_word(word, tree->dir, new_word, gram, len) == 0)
            return 0;
        else
            return 1;
    }
}

