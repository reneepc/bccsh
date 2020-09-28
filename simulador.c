#include "simulador.h"

proc new_proc(char* name, int t0, int dt, int deadline) {
    proc process;
    strcpy(process.name, name);
    process.t0 = t0;
    process.dt = dt;
    process.deadline = deadline;
    return process;
}

void print_proc(proc process) {
    printf("---------------------------------------------\n");
    printf("Nome: %s\n", process.name);
    printf("Início do processo: %d\n", process.t0);
    printf("Tempo de CPU pra execução: %d\n", process.dt);
    printf("Deadline: %d\n", process.deadline);
    printf("---------------------------------------------\n");
}

int read_file(char* path) {
    FILE* proc_file = fopen(path, "r");
    char name[PROC_NAME_MAX];
    int t0;
    int dt;
    int deadline;

    if(proc_file == NULL) {
        fprintf(stderr, "[O arquivo %s não foi encontrado]\n", path);
        exit(EXIT_FAILURE);
    }

    while(fscanf(proc_file, "%s %d %d %d\n", name, &t0, &dt, &deadline) != EOF) {
        proc process = new_proc(name, t0, dt, deadline);
        print_proc(process);
    }

    fclose(proc_file);
}

int main(int argc, char** argv) {

    read_file(argv[1]);
    return 0;
}

