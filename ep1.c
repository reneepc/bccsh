#include "ep1.h"
#include <pthread.h>

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

int comp_proc(void* p1, void* p2) {
    if(((proc*)p1)->dt + ((proc*)p1)->t0 < ((proc*)p2)->dt + ((proc*)p2)->t0) return -1;
    else if(((proc*)p1)->dt + ((proc*)p1)->t0 > ((proc*)p2)->dt + ((proc*)p2)->t0) return 1;
    return 0;
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
        if (verbose)    
            fprintf(stderr, "[Novo processo\t%s %d %d %d]\n", name, t0, dt, deadline);
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
                fprintf(stderr, "[CPU em uso\t%s, %d]\n", processes->name, sched_getcpu());
            sleep(processes->dt);
            sec += processes->dt;
            mudanca_de_contexto++;
            if (verbose) {
                fprintf(stderr, "[CPU liberada\t%s, %d]\n", processes->name, sched_getcpu());
                fprintf(stderr, "[Finalização\t%s]\n", processes->name);
            }
            fprintf(exit_file, "%s %d %d\n", processes->name, sec, sec - processes->t0);
            pthread_mutex_unlock(&mutex);

            return NULL;
        } else {
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
            fprintf(stderr, "[Erro ao criar a thread %d.]", i);
            exit(1);
        }
        pthread_join(threads[i], NULL);
    }
    

}

void *ThreadSRTN (void *p) {
    proc *processes = (proc *) p;
    while(1){
        if(processes->t0 < sec) sleep(sec - processes->t0);
        pthread_mutex_lock(&mutex);
        if (verbose)    
            fprintf(stderr, "[CPU em uso\t%s, %d]\n", processes->name, sched_getcpu());
        sleep(processes->dt);
        sec += processes->dt;
        mudanca_de_contexto++;
        if (verbose) {
            fprintf(stderr, "[CPU liberada\t%s, %d]\n", processes->name, sched_getcpu());
            fprintf(stderr, "[Finalização\t%s]\n", processes->name);
        }
        fprintf(exit_file, "%s %d %d\n", processes->name, sec, sec - processes->t0);
        pthread_mutex_unlock(&mutex);

        return NULL;
    }
    return NULL;
}

void shortest_remaining_time_next(proc *processes, int process_count, pthread_t *threads){
    qsort(processes, process_count, sizeof(proc), comp_proc);
    
    printf("\n--------------------------------------------------\n");
    printf("[Shortest Remaining Time Next]\n");
    printf("--------------------------------------------------\n\n");

    for (int i = 0; i < process_count; i++){
        if(pthread_create(&threads[i], NULL, ThreadSRTN, (void *)&processes[i])){
            fprintf(stderr, "[Erro ao criar a thread %d.]", i);
            exit(1);
        }
        pthread_join(threads[i], NULL);
    }
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

    if (!strcmp(argv[4], "d"))
        verbose += 1;

    exit_file = fopen(argv[3], "w");

    if(exit_file == NULL) {
        fprintf(stderr, "[O arquivo %s não pôde ser criado]\n", (char*)exit);
        exit(EXIT_FAILURE);
    }

    process_count = read_file(argv[2], &processes);

    if (escalonador == 1)
        first_come_first_served(processes, process_count, threads);
    else if(escalonador == 2) {
        shortest_remaining_time_next(processes, process_count, threads);
    } else if(escalonador == 3)
        round_robin();
    else
        fprintf(stderr, "[Não existe escalonador identificado pelo número %i.]\n", escalonador);


    fprintf(exit_file, "%d\n", mudanca_de_contexto);

    pthread_mutex_destroy(&mutex);
    return 0;
}

