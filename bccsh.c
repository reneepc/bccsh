#include "bccsh.h"
#include "parser.h"

// Realiza o update do prompt do terminal através da atualização do login e do diretório atual.
void prompt_update(char* prompt) {
    // Libera memória anteriormente alocada para o prompt.
    memset(prompt, 0, PROMPT_MAX);

    char login[NAME_MAX];
    struct passwd* pwd = getpwuid(getuid());
    strcpy(login, pwd->pw_name);
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    
    strncpy(prompt, "{", PROMPT_MAX);
    strncat(prompt, login, PROMPT_MAX);
    strncat(prompt, "@", PROMPT_MAX);
    strncat(prompt, cwd, PROMPT_MAX);
    strncat(prompt, "} ", PROMPT_MAX);
}

void cmd(token* container) {
    if(!strcmp(container->tokens[0], "exit")) {
        exit(EXIT_SUCCESS);
    }
    else if(!strcmp(container->tokens[0], "mkdir")) {
        mkdir(container->tokens[1], 0777);
    } else if (!strcmp(container->tokens[0], "kill")) {
        if(container->size < 2) {
            printf("\nUso: \n");
            printf("\tkill -<sinal> <pid>\n");
        }
        int sinal = -atoi(container->tokens[1]);
        pid_t processo = atoi(container->tokens[2]);
        kill(processo, sinal);
    } else if (!strcmp(container->tokens[0], "ln")) {
        if(!strcmp(container->tokens[1], "-s")) {
            symlink(container->tokens[2], container->tokens[3]);
        }
    } else
        print_token(container);
}

int main(int argc, char** argv) {

    char* prompt = malloc(sizeof(char)*PROMPT_MAX);
    char* buf;
    prompt_update(prompt);

    while((buf = readline(prompt)) != NULL) {
        if(strcmp(buf, "")) add_history(buf);

        token* token_container = new_token();
        add_token(token_container, buf);
        cmd(token_container);
        destroy_token(token_container);
        free(buf);
        prompt_update(prompt);
    }
    printf("\n");
    free(prompt);
    return 0;
}
