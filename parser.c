#include "parser.h"

token* new_token() {
    token* token_container = (token*)malloc(sizeof(token));
    token_container->tokens = (char**)malloc(sizeof(char)*ARGS_MAX);
    token_container->size = 0;
    token_container->max = ARGS_MAX;
}

token* new_token_max(size_t max) {
    token* token_container = (token*)malloc(sizeof(token));
    token_container->tokens = (char**)malloc(sizeof(char)*max);
    token_container->size = 0;
    token_container->max = max;
}

void destroy_token(token* token_container) {
    for(int i = 0; i < token_container->size; i++) {
        free(token_container->tokens[i]);
    }
    free(token_container->tokens);
    free(token_container);
}

size_t add_token(token* token_container, char* line) {
    if(token_container->size == token_container->max) resize_container(token_container);
    char* token;
    token = strtok(line, " ");
}

void resize_container(token* token_container) {
    token* new_container = new_token_max(token_container->max*2);
    for(int i = 0; i < token_container->size; i++) {
       new_container->tokens[i] = token_container->tokens[i];
    }
    new_container->size = token_container->size;

    destroy_token(token_container);
    token_container = new_container;
}

