#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "util.h"

#define PROC_NAME_MAX 31
#define GET_MAX 80
#define NUM_MAX 12
#define THREAD_MAX 20000
#define QUANTUM 1000000000

typedef struct proc_t {
    char name[PROC_NAME_MAX]; // Nome do processo
    int t0;       // Instante de tempo inicial do processo
    int dt;       // Tempo real da CPU para ser dedicado a simulação
    int deadline; // Instante de tempo em que o processo deve terminar
} proc;

proc new_proc(char* name, int t0, int dt, int deadline);

void print_proc(proc process);

int read_file(char* path, proc *processes);

int comp_dt(void* p1, void* p2);

void first_come_first_served(proc *processes, int process_count, pthread_t *threads);

void shortest_remaining_time_next(proc *processes, int process_count, pthread_t *threads);

void round_robin();

#endif
