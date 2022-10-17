# Computação Paralela - Trabalho Prático 1

O objetivo deste trabalho é a implementação paralela mestre-escravo usando MPI de um algoritmo que identifica números primos em vetor de N inteiros.

## Identificação de números primos

O arquivo de entrada deverá ser lido em carregado para memória no processo mestre. Após o processamento, cada processo escravo deverá retornar para o processo mestre quantos divisores exatos possui cada valor armazenado na sua fatia do vetor. Depois que o último escravo terminar, o processo mestre deverá colocar o número de divisores de cada elemento do arquivo de entrada em um arquivo de saı́da na ordem em que os valores originais estavam no arquivo de entrada. A tomada de tempo das execuções será feita somente no processo mestre após leitura do arquivo de entrada e antes da escrita do arquivo de saı́da, ou seja, não iremos considerar o tempo de entrada/saı́da.

## Resultados

![image](https://docs.google.com/spreadsheets/d/e/2PACX-1vRo4kL-suLGksJ40WtRhE6scp--m2rvNEKkscz1Rg7VVxcfIbhY_vWvDqJQXZvkF5k7S-e6l0z47dg5/pubchart?oid=1603307684&format=image)
