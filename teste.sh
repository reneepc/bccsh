#!/usr/bin/bash

if [[ $1 -eq "1" ]] 
then
    modo="fcfs"
elif [[ $1 -eq "2" ]]
then
    modo="srtn"
else
    modo="rr"
fi

echo "5 Processos";
if [[ ! -d saida/${modo}5 ]]
then
    mkdir saida/${modo}5
fi
for i in {1..30}; do
    echo "Execução $i";
    ./ep1 $1 entrada/5-processos saida/${modo}5/comp$2-$i d
done;

echo "50 Processos";
if [[ ! -d saida/${modo}50 ]]
then
    mkdir saida/${modo}50
fi
for i in {1..30}; do
    echo "Execução $i";
    ./ep1 $1 entrada/50-processos saida/${modo}50/comp$2-$i d
done;

echo "500 Processos";
if [[ ! -d saida/${modo}500 ]]
then
    mkdir saida/${modo}500
fi
for i in {1..30}; do
    echo "Execução $i";
    ./ep1 $1 entrada/500-processos saida/${modo}500/comp$2-$i d
done;
