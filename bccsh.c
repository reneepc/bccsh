#include "bccsh.h"
#include "parser.h"

void cmd(int argc, char** argv) {
}

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

int main(int argc, char** argv) {

    char* prompt = malloc(sizeof(char)*PROMPT_MAX);
    char* buf;
    prompt_update(prompt);

    while((buf = readline(prompt)) != NULL) {
        if(strcmp(buf, "")) add_history(buf);

        token* token_container = new_token();
        print_token(token_container);
        destroy_token(token_container);
        free(buf);
        prompt_update(prompt);
    }
    printf("\n");
    free(prompt);
    return 0;
}
