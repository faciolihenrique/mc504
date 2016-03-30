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
#include  <string.h>

#define N 6
#define THREAD_EXECUTION 1
#define TIME 1

typedef enum {False = 0, True = 1} Boolean;

pthread_mutex_t imprimindo;

volatile Boolean interesse[N] = {False,False,False,False,False};
volatile int fast_lock = 0;
volatile int slow_lock = 0;

/* Funções para animações*/
/* Função que imprime na tela a animação
 */
void update_screen(int thr_id, char thread_message[]){
    if(thr_id == 1)
        mvprintw(row/2 -1,(col-17)/2  -9,"%s",thread_message);
    if(thr_id == 2)
        mvprintw(row/2,   (col-17)/2  -9,"%s",thread_message);
    if(thr_id == 3)
        mvprintw(row/2 +1,(col-17)/2  -9,"%s",thread_message);
    if(thr_id == 4)
        mvprintw(row/2 +2,(col-17)/2  -9,"%s",thread_message);
    if(thr_id == 5)
        mvprintw(row/2 +3,(col-17)/2  -9,"%s",thread_message);
    refresh();
    return;
}
/* update_thread_message
 * Função que atualiza o o escrito de thread[i] para o nescessário
 */
void update_thread_message(int thr_id, char message[]){
    pthread_mutex_lock(&imprimindo);
    char thread_message[40];

    sprintf(thread_message, "Thread %d ", thr_id);
    strcat(thread_message, message);

    update_screen(thr_id, thread_message);
    pthread_mutex_unlock(&imprimindo);
}

void* f_thread(void *v) {
    int thr_id = *(int*)v + 1;

    for(int c = 0; c < THREAD_EXECUTION; c++){
        inicio:
        update_thread_message(thr_id, BEGIN);
        getch();
        interesse[thr_id] = True;
        update_thread_message(thr_id, BEGIN_INTEREST);
        getch();pthread_mutex_unlock(&imprimindo);
        fast_lock = thr_id;
        update_thread_message(thr_id, ON_FAST_INTEREST);
        getch();
        if(slow_lock != 0){
            interesse[thr_id] = False;
            getch();
            update_thread_message(thr_id, ON_FAST);
            while(slow_lock != 0);
            goto inicio;
        }
        slow_lock = thr_id;
        update_thread_message(thr_id, ON_SLOW_INTEREST);
        getch();
        if(fast_lock != thr_id){
            interesse[thr_id] = False;
            update_thread_message(thr_id, ON_SLOW);
            getch();
            for(int i = 1; i < N; i++){
                while(interesse[i]);
                if(slow_lock != thr_id){
                    while(slow_lock != 0);
                    goto inicio;
                }
            }
        }
        /* REGIÃO CŔITICA */
        update_thread_message(thr_id, CRITICAL_INTEREST);
        getch();
        slow_lock = 0;
        update_thread_message(thr_id, THE_END);
        getch();
        interesse[thr_id] = False;
    }

    return NULL;
}

int main() {
    initscr();
    getmaxyx(stdscr,row,col);

    pthread_t thr[N];
    int id[N], i;

    printw("Seja B o local de inicio de todas as threads\n     F o local do fastlock\n     S o local do Slow Lock\n     C a região crítica, onde duas threads nunca podem estar juntas\n     E o final, onde tem a variavel compartilhada\n\n");
    mvprintw(row/2 -3,(col-17)/2,"%s",upper);
    mvprintw(row/2 -2,(col-17)/2,"%s",up);
    mvprintw(row/2 +4,(col-17)/2,"%s",down);

    /* Inicializa as strings de cada uma das threads */
    for(i = 1; i < N; i++){
        update_thread_message(i, BEGIN);
    }

    for (i = 0; i < N; i++){
        id[i] = i;
        pthread_create(&thr[i], NULL, f_thread, &id[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(thr[i], NULL);



    endwin();

    return 0;
}
