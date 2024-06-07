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
int check_word(char *, Node *, char *, gramatica, size_t);
int check_word_default(char *, Node *, gramatica);

// Process a word based on the grammar and add it to the tree
void process_word(Node *root, gramatica gram, char *word) {
    if (word == NULL || *word == '\0')
        return;

    if (root == NULL) {
        root = (Node *)malloc(sizeof(Node));
        if (root == NULL) {
            fprintf(stderr, "Memory allocation failed for root node.\n");
            return;
        }
        root->token = NULL;
        root->esq = NULL;
        root->dir = NULL;
    }

    if (root->token != NULL) {
        goto end;
    }

    char **matches = find_rule(gram.P, *word, 0);
    if (matches == NULL) {
        fprintf(stderr, "Failed to find rules.\n");
        return;
    }

    int i = 0;
    while (matches[i] != NULL) {
        printf("\nMATCH %c COM ", *word);
        for (int n = 0; matches[i][n] != ';'; n++)
            printf("%c", matches[i][n]);
        i++;
    }

    printf("\nLoaded Char is %c\n", matches[0][0]);

    root->token = (char *)malloc(strlen(matches[0]) + 1);
    if (root->token == NULL) {
        fprintf(stderr, "Memory allocation failed for root token.\n");
        free(matches);
        return;
    }

    strcpy(root->token, matches[0]);
    printf("Added token to root: %s\n", root->token); // Debug print

    i = 0;
    while (matches[i] != NULL) {
        if (!test_rule_product(matches[i], word, gram)) {
            matches[i] = NULL;
            i++;
            continue;
        }

        for (int j = 0; matches[i][j] != ';'; j++) {
            root->token[j] = matches[i][j];
        }
        root->token[strlen(matches[i])] = '\0';
        i++;
    }

    if (root->token != NULL) {
        printf("\nSUCCESSFUL PROCESS FOR %s", root->token);
    } else {
        printf("\nROOT TOKEN NULL\n");
        free(matches);
        return;
    }

    switch (check_word_default(word, root, gram)) {
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
    if (root->esq == NULL) {
        printf("\nGOING LEFT\n");
        process_word(root->esq, gram, word);
    } else {
        printf("\nGOING RIGHT\n");
        process_word(root->dir, gram, word);
    }

    free(matches);
}

// Find a rule in the grammar
char **find_rule(char *rules, char var, const int n) {
    bool s = false;
    char **p = NULL;
    int match_count = 0;

    printf("\nEsta em find_rule\n");
    p = (char **)malloc(strlen(rules) * sizeof(char *)); // Allocate memory for array of pointers

    do {
        p[match_count] = (char *)malloc(2 * sizeof(char)); // Allocate memory for a character and null terminator

        switch (*rules) {
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
        if (*rules == var && s && n == 0) {
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

// Check if the word matches the grammar using the tree
int check_word_default(char *word, Node *tree, gramatica gram) {
    char *new_word = (char *)malloc(strlen(word) + 1); // Allocate memory for new_word
    if (new_word == NULL) {
        fprintf(stderr, "Memory allocation failed for new_word.\n");
        return 0;
    }
    return check_word(word, tree, new_word, gram, 0); // Call the overloaded function
}

// Recursively check if the word matches the grammar using the tree
int check_word(char *word, Node *tree, char *new_word, gramatica gram, size_t len) {
    if (tree == NULL)
        return 0;

    if (tree->token != NULL) {
        int i = 0;
        while (tree->token[i] != '\0') {
            printf("\nINSIDE TREE TOKEN LOOP\n");
            char current_char = tree->token[i];
            int holder = testvar(gram, current_char, 2);
            if (holder == 0) {
                new_word[len] = current_char;
                new_word[len + 1] = '\0'; // Null-terminate the string
                len++;
            }
            i++;
        }
    } else {
        return 0;
    }

    if (strcmp(new_word, word) == 0) {
        printf("Success: The new word matches the original word.\n");
        return 1;
    } else {
        if (strlen(new_word) < strlen(word)) {
            if (tree->esq == NULL && tree->dir == NULL) {
                printf("Partial Success: The new word is shorter than the original word.\n");
                return 1;
            }
        } else {
            printf("Failure: The new word does not match the original word.\n");
            return 0;
        }
    }

    if (tree->esq) {
        if (check_word(word, tree->esq, new_word, gram, len) == 0)
            return 0;
        else
            return 1;
    }

    if (tree->dir) {
        if (check_word(word, tree->dir, new_word, gram, len) == 0)
            return 0;
        else
            return 1;
    }

    return 0;
}

