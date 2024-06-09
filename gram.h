#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct gramatica 
{
    char *E;
    char *N;
    char *P;
    char *S;
} gramatica;

typedef struct Node 
{
    struct Node *esq;
    struct Node *dir;
    char *token;
} Node;

int testvar(gramatica , char , size_t ); 
bool test_rule_product(char *, char *, gramatica );
int check_word(char *, Node *, char *, gramatica, size_t);
int check_word_default(char *, Node *, gramatica);

int testvar(gramatica gram, char c, size_t opt) 
{
    if(c == ':')
        return -1;

    if(opt == 1 || opt == 3) 
    {
        if (gram.S[0] == c) 
            return 0; // Match found
        for(int j = 0; gram.N[j] != '\0'; j++)
         {
            if(gram.N[j] == c)
                return 0; // Match found
        }
    }

    if(opt == 2 || opt == 3) 
    {
        for(int i = 0; gram.E[i] != '\0'; i++) 
        {
            if(gram.E[i] == c)
                return 0; // Match found
        }
    }

    return 1; // No match found
}

bool test_rule_product(char *rule, char *word, gramatica gram) 
{
    char buff[100];
    int i = 0;
    char *temp = (char *)malloc(100 * sizeof(char)); // Allocate memory for temp

    if (temp == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    // Find the beginning of the rule
    while (*rule != ':')
        rule++;

    rule++; // Move past the colon

    // Loop through the rule to copy products into buff
    while (*rule != ';' && *rule != '\0') 
    {
        int holder = testvar(gram, *rule, 2);
        if (holder == 1) {
            printf("\nAlfabeto e %s", gram.E);
            printf(" %c IS NOT FROM ALPHABET\n", *rule);
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
    if (strcmp(temp, word) > 0) 
    {
        printf("\nWORD DOES NOT MATCH\n");
        free(temp); // Free dynamically allocated memory
        return false;
    }

    printf("\nWORD MATCH\n");
    free(temp); // Free dynamically allocated memory
    return true;
}

void print_tree(Node *root)
{
    if (root == NULL) 
    {
        printf("Root node is NULL\n");
        return;
    }

    if (root->token == NULL) 
    {
        printf("Token: (null)\n");
    } else 
    {
        printf("\nToken: %s\n", root->token); // Debug print
    }

    if(root->esq != NULL) 
    {
        printf("Printing left subtree:\n");
        print_tree(root->esq);
    }

    if(root->dir != NULL) 
    {
        printf("Printing right subtree:\n");
        print_tree(root->dir);
    }
    return;
}

void free_tree(Node *root) 
{
    if (root == NULL) 
    {
        return;
    }

    // Free memory for the left subtree
    free_tree(root->esq);

    // Free memory for the right subtree
    free_tree(root->dir);

    // Free memory for the node itself
    free(root);
}


// Check if the word matches the grammar using the tree
int check_word_default(char *word, Node *tree, gramatica gram) 
{
    char *new_word = (char *)malloc(strlen(word) + 1); // Allocate memory for new_word
    if (new_word == NULL) 
    {
        fprintf(stderr, "Memory allocation failed for new_word.\n");
        return 0;
    }
    return check_word(word, tree, new_word, gram, 0); // Call the overloaded function
}

// Recursively check if the word matches the grammar using the tree
int check_word(char *word, Node *tree, char *new_word, gramatica gram, size_t len)
{
    if (tree == NULL)
        return 0;

    if (tree->token != NULL) 
    {
        int i = 0;
        while (tree->token[i] != '\0') 
        {
            printf("\nINSIDE TREE TOKEN LOOP\n");
            char current_char = tree->token[i];
            int holder = testvar(gram, current_char, 2);
            if (holder == 0) 
            {
                new_word[len] = current_char;
                new_word[len + 1] = '\0'; // Null-terminate the string
                len++;
            }
            i++;
        }
    }else 
    {
        return 0;
    }

    if (strcmp(new_word, word) == 0) 
    {
        printf("Success: The new %s word matches the original word.\n", new_word);
        return 1;
    }else 
    {
        if (strlen(new_word) < strlen(word)) 
        {
            if (tree->esq == NULL && tree->dir == NULL) 
            {
                printf("Partial Success: The new word is shorter than the original word.\n");
                return 1;
            }
        }else 
        {
            printf("Failure: The new word: %s does not match the original word.\n", new_word);
            EXIT_FAILURE;
        }
    }

    if (tree->esq) 
    {
        if (check_word(word, tree->esq, new_word, gram, len) == 0)
            return 0;
        else
            return 1;
    }

    if (tree->dir) 
    {
        if (check_word(word, tree->dir, new_word, gram, len) == 0)
            return 0;
        else
            return 1;
    }

    return 0;
}