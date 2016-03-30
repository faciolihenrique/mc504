/* Algoritimo Fast Mutual Exclusion para 5 threads que executam 2 vezes cada
 * Guilherme Lucas da Silva
 * Henrique Noronha Facioli
 * Lauro Cruz e Souza
 * Thiago Silva de Farias
 */

#include "fme.h"

#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 6
#define THREAD_EXECUTION 1

typedef enum {False = 0, True = 1} Boolean;

volatile Boolean interesse[N] = {False,False,False,False,False};
volatile int fast_lock = 0;
volatile int slow_lock = 0;
volatile int counter[N]= {0,0,0,0,0,0};

void* f_thread(void *v) {
    int thr_id = *(int*)v + 1;

    for(int c = 0; c < THREAD_EXECUTION; c++){
        inicio:

        printf("Thread %d: DECLARA INTERESSE NA PAREDE\n", thr_id);

        interesse[thr_id] = True;
        fast_lock = thr_id;

        printf("Thread %d: \tEscrevi que fiz o primeiro lock (fastlock)\n", thr_id);
        printf("Thread %d: \tSerá que se alguém ja fez o  segundo (slowlock)?\n", thr_id);

        if(slow_lock != 0){
            printf("Thread %d: \tAlguem ja tinha feito o slow!\n", thr_id);

            interesse[thr_id] = False;

            printf("Thread %d: \tRemovi meu interesse e vou esperar alguem sair do slow\n", thr_id);

            while(slow_lock != 0);

            printf("Thread %d: \tEBA! ALGUEM REMOVEU O SLOW! VOU TENTAR NOVAMENTE!\n", thr_id);

            goto inicio;
        }

        printf("Thread %d: \tNão, ninguém\nThread %d: \tVou tentar fazer o slow!\n", thr_id, thr_id);

        slow_lock = thr_id;

        printf("Thread %d: \tSlow feito! Um passo mais perto para sair desse labirinto!\n", thr_id);

        if(fast_lock != thr_id){

            printf("Thread %d: \tEita, alguem mudou o fast enquanto eu estava dormindo!\n\tVou tirar meu interesse e esperar!\n", thr_id);

            interesse[thr_id] = False;
            for(int i = 1; i < N; i++){
                while(interesse[i]);

                printf("Thread %d: \tAE! AGORA VOU EU!!!!\n", thr_id);

                if(slow_lock != thr_id){
                    printf("Thread %d: \tAHHH, Nao foi dessa vez! Acabei de ver que já escreveram no slow! Voltando...\n", thr_id);
                    while(slow_lock != 0);
                    goto inicio;
                }
            }
        }
        printf("Thread %d: \tEstou vendo a saida!!! Se somente eu imprimir isso estarei salvo!\n", thr_id);

        counter[thr_id]++;

        printf("Thread %d: \t\tAe, eu sai com  sucesso.bjs!\n\n\t___________\t\n\t|1|2|3|4|5|\t\n\t-----------\t\n\t|%d|%d|%d|%d|%d|\t\n\t-----------\t\n\n",thr_id ,counter[1],counter[2],counter[3],counter[4],counter[5]);

        slow_lock = 0;
        interesse[thr_id] = False;
    }
}

int main() {
    pthread_t thr[N];
    int id[N], i;

    printf("Esta é uma apresentação de 5 threads, que estão em um monólogo em threadimensões\n paralelas, em que nenhuma pode ouvir a outra e todas querem escapar da masmorra mas só podem sair uma de cada vez.\nElas descobriram que existe uma parede que atravessa essas várias threadimensions e que elas podem tentar se comunicar atravez dela!.\nMas cuidado! Se tentarem sair juntas elas se perderão na crise das infinitas threadimensions!!!\n\n\n\n");

    for (i = 0; i < N; i++){
        id[i] = i;
        pthread_create(&thr[i], NULL, f_thread, &id[i]);
    }

    for (i = 0; i < N; i++)
        pthread_join(thr[i], NULL);

    return 0;
}
