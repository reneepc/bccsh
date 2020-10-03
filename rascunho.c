#include "bccsh.h"
#include "parser.h"

""" 
./ep1 escalonador file_trace file_saida d


//// TRACE 

nome t0 dt deadline

nome        string s/ espacos, 30 chars
t0          instante em segundos que processo chega no sistema
dt          tempo *real* da CPU simulapro pra esse processo
deadline    tempo antes do qual precisa acabar

os tres ultimos sao int > 0.

cada linha eh um processo que deve ser simulado como uma unica thread

thread: loop que realize *qualquer* operacao que consuma tempo


///// ESCALONADORES

- deve finalizar quando os processos acabarem
- sera mais interessante se permitir arquivos de trace com mais de um processo ao mesmo tempo competindo pela CPU

/ / / / / modos

1 first come first served
2 shortest remaining time next
3 round robin


///// SAIDA
arquivo com linhas no formato
nome tf tr 

nome        identificador do processo
tf          instante de tempo em que terminou execucao (segundos)
tr          tf - t0

a ultima linha sera um numero representando quantas mudancas de contexto ocorreram durante a simulacao


/ / / / / se executado com o parametro d

simulador devera exibir diversos eventos, assim que acontecerem, na saida de erro (stderr)

- chegada de novo processo no sistema
FORMATO: new process    (copia linha do trace)

- uso da CPU de um processo 
FORMATO: CPU usage      processo_que_comecou_a_usar_CPU CPU_q_esta_usando

- liberacao da CPU por um processo
FORMATO: CPU freed      processo_que_deixou_de_usar_CPU processo_que_ele_esta_liberando

- finalizacao da execucao do processo 
FORMATO: process done   (copia linha da saida)

- quantidade de mudancas de contexto
FORMATO: context change XXX


""" 