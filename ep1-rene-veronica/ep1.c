#include "ep1.h"
#include <pthread.h>

// Cria vetor global de threads com no máximo 20000 threads
pthread_t threads[THREAD_MAX];
pthread_mutex_t mutex;

// Cria vetores globais para a tentativa de implementacao do Round Robin
pthread_cond_t cond[THREAD_MAX];
pthread_mutex_t locks[THREAD_MAX];

// Cria vetor global para armazenar no maximo 2000 processos
proc processes[THREAD_MAX];

// Contador global de total de processos
int process_count;

// Variável global para a contagem de tempo.
int sec = 1;
int verbose = 0;
int mudanca_de_contexto = -1; // mudanças de contexto FCFS = n-1 numero de processos?
FILE* exit_file; // Arquivo de saída dos escalonadores

// Cria processo.
proc new_proc(char* name, int t0, int dt, int deadline) {
    proc process;
    strcpy(process.name, name);
    process.t0 = t0;
    process.dt = dt;
    process.deadline = deadline;
    return process;
}

// Imprime informações sobre o processo.
//
// Utilizada para Debugging.
void print_proc(proc process) {
    printf("---------------------------------------------\n");
    printf("Nome: %s\n", process.name);
    printf("Início do processo: %d\n", process.t0);
    printf("Tempo de CPU pra execução: %d\n", process.dt);
    printf("Deadline: %d\n", process.deadline);
    printf("---------------------------------------------\n");
    
}

// Comparador utilizado para realizar qsort na lista de processos, o qual compara a soma dos atributos dt e t0 para
// determinar qual processo será selecionado no Shortest Remaining Time Next.
int comp_proc(void* p1, void* p2) {
    if(((proc*)p1)->dt + ((proc*)p1)->t0 < ((proc*)p2)->dt + ((proc*)p2)->t0) return -1;
    else if(((proc*)p1)->dt + ((proc*)p1)->t0 > ((proc*)p2)->dt + ((proc*)p2)->t0) return 1;
    return 0;
}

// Realiza a leitura do arquivo de processos, retornando a quantidade de processos lida.
//
// Caso o quarto argumento do executável seja "d", a função indica a criação de cada processo.
int read_file(char* path) {
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


// Função de execução das threads no algoritmo First-Come, First-Served, a qual realiza a execução dos processos de
// maneira ordenada, realizando o travamento da seção crítica que vai desde a simulação de execução do processo pela
// função sleep() até a escrita nos arquivos de saída.
//
// Caso a flag "d" seja especificada no quarto argumento do executável a função indicará em stderr quando a CPU entrou
// em uso, foi liberada e finalizada.
void *ThreadFCFS (void *p) {
    proc *processes = (proc *) p;
    while(1){
        if(processes->t0 <= sec) {
            pthread_mutex_lock(&mutex);
            if (verbose)    
                fprintf(stderr, "[CPU em uso\t\t%s, %d]\n", processes->name, sched_getcpu());
            sleep(processes->dt);
            sec += processes->dt;
            mudanca_de_contexto++;
            if (verbose) {
                fprintf(stderr, "[CPU liberada\t\t%s, %d]\n", processes->name, sched_getcpu());
                fprintf(stderr, "[Finalização\t\t%s %d %d]\n", processes->name, sec, sec - processes->t0);
                fprintf(stderr, "[Mud. de Contexto\t%d]\n", mudanca_de_contexto);
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
void first_come_first_served(){
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

// Função que realiza a execução das threads dos processos, a qual espera que o tempo inicial do processo ocorra para
// realizar a simulação.
//
// Caso a flag "d" seja especificada no quarto argumento do executável, será printado o momento de início do processo
// pela CPU, quando a CPU é liberada e o processo é finalizado.
void *ThreadSRTN (void *p) {
    proc *processes = (proc *) p;
    while(1){
        pthread_mutex_lock(&mutex);
        if (verbose)    
            fprintf(stderr, "[CPU em uso\t\t%s, %d]\n", processes->name, sched_getcpu());
        sleep(processes->dt);
        sec += processes->dt;
        mudanca_de_contexto++;
        if (verbose) {
            fprintf(stderr, "[CPU liberada\t\t%s, %d]\n", processes->name, sched_getcpu());
            fprintf(stderr, "[Finalização\t\t%s %d %d]\n", processes->name, sec, sec - processes->t0);
                fprintf(stderr, "[Mud. de Contexto\t%d]\n", mudanca_de_contexto);
        }
        fprintf(exit_file, "%s %d %d\n", processes->name, sec, sec - processes->t0);
        pthread_mutex_unlock(&mutex);

        return NULL;
    }
    return NULL;
}

// Recebe a lista de processos ordenada de acordo com a soma dos atributos dt e t0, para que estes sejamexecutados
// conforme seu tempo restante.
void shortest_remaining_time_next(){
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

// Tentativa de implementar o Round Robin 
void *ThreadRR (void *i) {
    int id = (int) i;
    while(processes[id].dt > 0){

        if(processes[id].t0 <= sec) {
            pthread_mutex_lock(&locks[id]);
            pthread_cond_wait(&cond[id], &locks[id]);
            if (verbose)    
                printf("[CPU em uso\t\t%s, %d]\n", processes[id].name, sched_getcpu());
            sleep(QUANTUM);
            sec += QUANTUM;
            mudanca_de_contexto++;
            processes[id].dt -= QUANTUM;
            if (verbose) {
                printf("[CPU liberada\t\t%s, %d]\n", processes[id].name, sched_getcpu());
                printf(stderr, "[Finalização\t\t%s %d %d]\n", processes->name, sec, sec - processes->t0);
                fprintf(stderr, "[Mud. de Contexto\t%d]\n", mudanca_de_contexto);
            }
            fprintf(exit_file, "%s %d %d\n", processes[id].name, sec, sec - processes->t0);
            pthread_cond_signal(&cond[(id + 1) % process_count]);
            pthread_mutex_unlock(&locks[id]);
        }
        else{
            sleep(QUANTUM);
        }
    }
    return NULL;
}


void round_robin(){
    int i = 0;
    printf("\n--------------------------------------------------\n");
    printf("[Round Robin]\n");
    printf("--------------------------------------------------\n\n");



    pthread_cond_signal(&cond[0]);
    for (i = 0; i < process_count; i++){
        if(pthread_create(&threads[i], NULL, ThreadRR, (void *)i)){
            printf("[Erro ao criar a thread %d.]", i);
            exit(1);
        }
        
    }
    pthread_cond_signal(&cond[0]);   
}

int main(int argc, char** argv) {
    int escalonador;
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

    process_count = read_file(argv[2]);

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

