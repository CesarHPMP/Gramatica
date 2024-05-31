#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif

int gram_set(gramatica *gram) 
{
    int capacity = 100; // Initial capacity, adjust as needed
    char buffer[100];// Buffer for reading a line
    int i;

    // Prompt and read alphabet (E)
    printf("Digite o alfabeto (E): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0'; // Removing the newline character
    gram->E = strdup(buffer);

    // Prompt and read variables (N)
    printf("Digite as variáveis (N): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0'; // Removing the newline character
    gram->N = strdup(buffer);

    // Allocate memory for S
    gram->S = (char *)malloc(2 * sizeof(char)); // Allocate memory for S
    if (gram->S == NULL) {
        printf("Memory allocation failed for S.\n");
        return -1;
    }

    // Prompt and read initial symbol (S)
    printf("Digite o símbolo inicial (S): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%c", &gram->S[0]);
    gram->S[1] = '\0'; // Null-terminate the string

    // Allocate memory for P
    gram->P = (char *)malloc(capacity * sizeof(char)); // Allocate memory for P
    if (gram->P == NULL) {
        printf("Memory allocation failed for P.\n");
        return -1;
    }

    printf("Digite as regras de produção (P)\n");

    do
    {
        printf("Digite 'var:branch'[apenas enter para sair]");
        fgets(buffer, sizeof(buffer), stdin);
        
        if(buffer[0] == '\n')
            break;
        
        i = 0;
        while (buffer[i] != '\n' && buffer[i] != '\0') 
        {
            if (testvar(*gram, buffer[i], 3)) {
                exit(testvar(*gram, buffer[i], 3));
            }
            i++;
        }
        if (buffer[i] == '\0') {
            printf("Missing newline character in input.\n");
            return -1;
        }
        buffer[i] = '\0'; // Null-terminate the string
        strcat(gram->P, buffer);      
        gram->P[strlen(gram->P)] = ';'; // Add semicolon to indicate the end of a production rule
    
    } while(1);
    return 0;
}


void gram_feed(Node *root, char *production_rule) {
    char buff[100];
    int counter = 0;

    // Skip leading whitespace characters
    while (*production_rule == ' ' || *production_rule == '\t') {
        production_rule++;
    }

    // Check if the production rule string is empty
    if (*production_rule == '\0') {
        printf("\nNo production rules found.\n");
        return;
    }

    // Allocate memory for a new node
    Node *new_node;
    new_node = (Node *)malloc(sizeof(Node *));
    if (new_node == NULL) {
        printf("Memory allocation failed for root node.\n");
        return;
    }

    // Initialize the new node
    new_node->esq = NULL;
    new_node->dir = NULL;

    // Copy the production rule into buff
    while (*production_rule != ';') {
        buff[counter++] = *production_rule;
        production_rule++;
    }
    buff[counter] = '\0'; // Null-terminate the string

    // Allocate memory for the token and copy the production rule
    new_node->token = malloc(strlen(buff) + 1); // +1 for the null terminator
    if (new_node->token == NULL) {
        printf("Memory allocation failed for token.\n");
        free(new_node); // Free the new node memory
        return;
    }
    strcpy(new_node->token, buff); // Copy the content of buff into token
     
    printf("\nNew node token-> %s", new_node->token);

    // Move to the end of the production rule
    while (*production_rule != ';' && *production_rule != '\0') {
        production_rule++;
    }

    // Check for end of string
    if (*production_rule == '\0') {
        // If there are no more production rules, exit the function
        return;
    }

    // Null-terminate the production rule and move to the next rule
    *production_rule = '\0';
    production_rule++;

    // Append the new node to the root of the parse tree
    if (root->esq == NULL) {
        printf("\ngoing left");
        root->esq = new_node;
        gram_feed(root, production_rule);
    } else if (root->dir == NULL) {
        printf("\ngoing right");
        root->dir = new_node;
        gram_feed(new_node, production_rule);
    } else {
        printf("Error: Both esq and dir nodes are occupied.\n");
        free(new_node->token); // Free the token memory
        free(new_node); // Free the new node memory
        return;
    }
}
