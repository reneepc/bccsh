#include "parser.h"

// Inicializa um container de palavras (tokens), tendo como capacidade máxima o valor ARGS_MAX = 10.
token* new_token() {
    token* token_container = (token*)malloc(sizeof(token));
    token_container->tokens = (char**)malloc(sizeof(char*)*ARGS_MAX);
    for(int i = 0; i < ARGS_MAX; i++) {
        token_container->tokens[i] = (char*)malloc(sizeof(char)*NAME_MAX);
    }
    token_container->size = 0;
    token_container->max = ARGS_MAX;
}

// Inicializa um container de palavras tendo como capacidade máxima o argumento max.
token* new_token_max(size_t max) {
    token* token_container = (token*)malloc(sizeof(token));
    token_container->tokens = (char**)malloc(sizeof(char*)*max);
    for(int i = 0; i < max; i++) {
        token_container->tokens[i] = (char*)malloc(sizeof(char)*NAME_MAX);
    }
    token_container->size = 0;
    token_container->max = max;
    return token_container;
}

// Libera a memória de cada string armazenada e do container
void destroy_token(token* token_container) {
    for(int i = 0; i < token_container->size; i++) {
        free(token_container->tokens[i]);
    }
    free(token_container->tokens);
    free(token_container);
}


// Adiciona todas as palavras pertencentes à string line, as quais são armazenadas individualmente. A estrutura também
// realiza a contagem desta palavras pelo atributo size.
size_t add_token(token* token_container, char* line) {
    char delim[] = " ";
    char* token;
    token = strtok(line, delim);

    while(token != NULL) {
        if(token_container->size == token_container->max) resize_container(token_container);
        strcpy(token_container->tokens[token_container->size++], token);
        token = strtok(NULL, delim);
    }
    return token_container->size;
}

// Cria um novo container com o dobro de capacidade que o container anterior, copiando todas as strings do anterior  para o novo e liberando a memória associada com o antigo.
void resize_container(token* token_container) {
    token* new_container = new_token_max(token_container->max*2);
    for(int i = 0; i < token_container->size; i++) {
       strcpy(new_container->tokens[i], token_container->tokens[i]);
    }
    new_container->size = token_container->size;

    destroy_token(token_container);
    token_container = new_container;
}

// Imprime todos os elementos do container
void print_token(token* container) {
    for(int i = 0; i < container->size; i++) {
        fprintf(stdout, "%s ", container->tokens[i]);
    }
    fprintf(stdout, "\n");
}
