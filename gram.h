#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct gramatica {
    char *E;
    char *N;
    char *P;
    char *S;
} gramatica;

typedef struct Node {
    struct Node *esq;
    struct Node *dir;
    char *token;
} Node;

int testvar(gramatica gram, char c, size_t opt) {
    if(c == ':')
        return -1;

    if(opt == 1 || opt == 3) {
        if (gram.S[0] == c) 
            return 0; // Match found
        for(int j = 0; gram.N[j] != '\0'; j++) {
            if(gram.N[j] == c)
                return 0; // Match found
        }
    }

    if(opt == 2 || opt == 3) {
        for(int i = 0; gram.E[i] != '\0'; i++) {
            if(gram.E[i] == c)
                return 0; // Match found
        }
    }

    return 1; // No match found
}

bool test_rule_product(char *rule, char *word, gramatica gram) {
    char buff[100];
    int i = 0;
    char *temp = (char *)malloc(100 * sizeof(char)); // Allocate memory for temp

    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    // Find the beginning of the rule
    while (*rule != ':')
        rule++;

    rule++; // Move past the colon

    // Loop through the rule to copy products into buff
    while (*rule != ';' && *rule != '\0') {
        int holder = testvar(gram, *rule, 1);
        if (holder == 1) {
            printf("\n%c IS NOT FROM ALPHABET\n", *rule);
            rule++;
            continue; // Skip recording characters that fail the testvar check
        }
        buff[i++] = *rule++;
    }

    buff[i] = '\0'; // Null-terminate the buff string

    // Copy buff into temp
    strncpy(temp, buff, 99);
    temp[99] = '\0'; // Ensure null-termination

    // Compare temp with word
    if (strcmp(temp, word) != 0) {
        printf("\nWORD DOES NOT MATCH\n");
        free(temp); // Free dynamically allocated memory
        return false;
    }

    printf("\nWORD MATCH\n");
    free(temp); // Free dynamically allocated memory
    return true;
}

void print_tree(Node *root) {
    if (root == NULL) {
        printf("Root node is NULL\n");
        return;
    }

    if (root->token == NULL) {
        printf("Token: (null)\n");
    } else {
        printf("\nToken: %s\n", root->token); // Debug print
    }

    if(root->esq != NULL) {
        printf("Printing left subtree:\n");
        print_tree(root->esq);
    }

    if(root->dir != NULL) {
        printf("Printing right subtree:\n");
        print_tree(root->dir);
    }
}

void free_tree(Node *root) {
    if (root == NULL) {
        return;
    }

    // Free memory for the left subtree
    free_tree(root->esq);

    // Free memory for the right subtree
    free_tree(root->dir);

    // Free memory for the node itself
    free(root);
}


