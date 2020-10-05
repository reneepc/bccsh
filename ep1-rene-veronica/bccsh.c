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

// Mata o processo identificado pelo pid como segundo argumento.
// Converte o número passado no primeiro argumento para o sinal
// que será enviado para o processo. Retorna -1 caso ocorra alguma
// falha.
int kill_process(token* container) {
    if(container->size < 2) {
        printf("Uso: \n");
        printf("\tkill -<sinal> <pid>\n");
    }
    int sinal = -atoi(container->tokens[1]);
    pid_t processo = atoi(container->tokens[2]);
    if(kill(processo, sinal) == -1) {
        perror("[kill() falhou]\n");
        return -1;
    }
}

// Executa um comando passado para o shell, retornando -1 em
// caso de erro em alguma das função fork() ou execve()
int execute(token* container) {
    // Cria lista de argumentos que serão passados para a função execve
    char* args[container->size + 1];
    for(int i = 0; i < container->size; i++) {
        args[i] = container->tokens[i];
    }
    args[container->size] = NULL;

    pid_t pid = fork();
    if(pid == 0) {
        if(execve(container->tokens[0], args, NULL) == -1) {
            perror("[execve() falhou]");
            return -1;
        }
    } else if(pid > 0) {
        waitpid(-1, NULL, 0);
    }
    else {
        perror("[fork() falhou]\n");
        return -1;
    }
}

void cmd(token* container) {
    // Sai do shell
    if(!strcmp(container->tokens[0], "exit")) {
        exit(EXIT_SUCCESS);
    }
    //Cria diretório
    else if(!strcmp(container->tokens[0], "mkdir")) {
        if(mkdir(container->tokens[1], 0777) == -1)
            perror("[Não foi possível criar o diretório.]\n");
    //Mata o processo
    } else if (!strcmp(container->tokens[0], "kill")) {
        if(kill_process(container) == -1)
            perror("[Falha ao matar o processo]\n");
    // Cria link simbólico
    } else if (!strcmp(container->tokens[0], "ln")) {
        if(!strcmp(container->tokens[1], "-s")) {
            if(symlink(container->tokens[2], container->tokens[3]))
                perror("[Não foi possível criar o link simbólico.]\n");
        }
    // Executa uma programa caso este inicie com . ou /
    } else if (container->tokens[0][0] == '.' || container->tokens[0][0] == '/') {
        if(execute(container) == -1) {
            perror("[Falha na execução do comando]\n");
        }
    // Caso nenhum comando seja reconhecido
    } else
        fprintf(stderr, "[Comando %s não reconhecido]\n", container->tokens[0]);
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
