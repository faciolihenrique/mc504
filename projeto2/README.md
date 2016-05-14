# Grupo
 - Nome: Guilherme Lucas da Silva RA: 155618
 - Nome: Henrique Noronha Facioli RA: 157986
 - Nome: Thiago Silva de Farias   RA: 148077
 - Nome: Lauro Cruz e Souza       RA: 156175

## Projeto 2
Este projeto consiste em criar duas chamadas de sistema (syscall's) no kernel do
linux. Foram criadas duas chamadas, a getkey e a setkey.
 - setkey(key, chain) : Essa chamada recebe um inteiro e uma cadeia de caracteres e a armazena em uma estrutura de dados dentro do Kernel. retorna 0 se não conseguiu alocar, e 1 se sim.
 - getkey(key, chain) : essa chama recebe um inteiro e um ponteiro de caracteres, procurando na estrutura armazenada no kernel o inteiro key, se encontrar faz o apontador apontar para a cadeia. retorna 0 se não encontrou, e 0 se encontrou.

## Requerimentos
 - gcc-multilib
 - qemu

## Rodar
Execute:
```
chmod +x execute.sh
./execute
```
siga as instruções que aparecerão no terminal


## Questões
Respondendo a questão feita no enunciado do laboratório, usamos a as funções
copy_from_user e copy_to_user pois armazenamos e recuperamos valores que não
foram alocados no espaço do usuário e sim no espaço de endereço do Kernel. Para
poder realizar tais operações, é necessário o privilégio de sistema, que essas
rotinas tornam simples. Além disso, é bom lembrar que a função que nos permite
realizar a alocação de espaço no kernel é a kmalloc.
