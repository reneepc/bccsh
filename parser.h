#ifndef _PARSER_
#define _PARSER_

#include "util.h"

#define ARGS_MAX 10

typedef struct token_t {
    char** tokens;
    size_t size;
    size_t max;
} token;

token* new_token();

token* new_token_max(size_t max);

void destroy_token(token* token_container);

size_t add_token(token* token_container, char* line);

void resize_container(token* token_container);

token* parse(char* line);

#endif
