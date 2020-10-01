#include "simulador.h"
#define _GNU_SOURCE
#include <sched.h>

pthread_t threads[THREAD_MAX];
pthread_mutex_t mutex;
int sec = 1;
int verbose = 0;
int mudanca_de_contexto = -1; // mudanças de contexto FCFS = n-1 numero de processos?
FILE* exit_file;

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

int read_file(char* path, proc *processes) {
    FILE* proc_file = fopen(path, "r");
    char name[PROC_NAME_MAX];
    int t0;
    int dt;
    int deadline;
    int i = 0;

    if(proc_file == NULL) {
        fprintf(stderr, "[O arquivo %s não foi encontrado]\n", path);
        exit(EXIT_FAILURE);
    }
    while(fscanf(proc_file, "%s %d %d %d\n", name, &t0, &dt, &deadline) != EOF) {
        proc p = new_proc(name, t0, dt, deadline);
        processes[i] = p;
        i++;
        if (verbose)    printf("[Novo processo\t%s %d %d %d]\n", name, t0, dt, deadline);
    }
    fclose(proc_file);
    
    return i;
}


void *ThreadFCFS (void *p) {
    proc *processes = (proc *) p;
    while(1){
        if(processes->t0 <= sec) {
            pthread_mutex_lock(&mutex);
            if (verbose)    
                printf("[CPU em uso\t%s]\n", processes->name); //erro com sched_getcpu()
            sleep(processes->dt);
            sec += processes->dt;
            mudanca_de_contexto++;
            pthread_mutex_unlock(&mutex);
            if (verbose) {
                printf("[CPU liberada\t%s]\n", processes->name); //erro com sched_getcpu()
                printf("[Finalização\t%s]\n", processes->name);
            }
            fprintf(exit_file, "%s %d %d\n", processes->name, sec, sec - processes->t0);

            return NULL;
        }
        else{
            sleep(1);
        }
    }
    return NULL;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigtimedwait.html
// https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigwait.html
// https://www.gnu.org/software/libc/manual/html_node/Miscellaneous-Signals.html
void first_come_first_served(proc *processes, int process_count, pthread_t *threads){
    int i;

    printf("\n--------------------------------------------------\n");
    printf("[First come first served]\n");
    printf("--------------------------------------------------\n\n");
    for (i = 0; i < process_count; i++){
        if(pthread_create(&threads[i], NULL, ThreadFCFS, (void *)&processes[i])){
            printf("[Erro ao criar a thread %d.]", i);
            exit(1);
        }
    }

}


void shortest_remaining_time_next(){

}

void round_robin(){

}

int main(int argc, char** argv) {
    int escalonador, process_count;
    pthread_t threads[THREAD_MAX];
    proc processes[THREAD_MAX];
    pthread_mutex_init(&mutex, NULL);
    escalonador = atoi(argv[1]);
    int i;

    if (argc > 4)
        verbose += 1;

    exit_file = fopen(argv[3], "w");

    if(exit_file == NULL) {
        printf(stderr, "[O arquivo %s não pôde ser criado]\n", exit);
        exit(EXIT_FAILURE);
    }

    process_count = read_file(argv[2], &processes);

    if (escalonador == 1)
        first_come_first_served(processes, process_count, threads);
    else if(escalonador == 2)
        shortest_remaining_time_next();
    else if(escalonador == 3)
        round_robin();
    else
        printf("[Não existe escalonador identificado pelo número %i.]\n", escalonador);

    for (i = 0; i < process_count; i++) {
        pthread_join(threads[i], NULL);
    }

    fprintf(exit_file, "%d\n", mudanca_de_contexto);

    pthread_mutex_destroy(&mutex);
    return 0;
}

