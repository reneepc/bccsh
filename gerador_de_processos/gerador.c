#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF 100

void usage() {
    printf("Uso:\n");
    printf("./gerador <número-de-processos> [semente]\n");
    exit(0);
}

int main(int argc, char** argv) {
    if(argc < 2)
        usage();

    int num_processes = atoi(argv[1]);
    char file_name[MAX_BUF];
    snprintf(file_name, MAX_BUF, "entrada/%d-processos", num_processes);
    FILE* processes = fopen(file_name, "w");

    char line[MAX_BUF];
    int seed = 0;
    if(argc == 3) seed = atoi(argv[2]);
    srand(seed);
    int t0 = 1;
    int dt;
    for(int i = 0; i < num_processes; i++) {
        // Gera um intervalo dt entre 1 e 9;
        dt = ((rand() % 9) + 1);
        snprintf(line, MAX_BUF, "Processo%d %d %d %d", i, t0, dt, t0+dt+(rand()%15)); 
        // Gera uma adição em t0 entre 0 e 3.
        t0 += (rand() % 4);
        if(i != 0) fprintf(processes, "\n");
        fprintf(processes, "%s", line);
    }
    return 0;
}

