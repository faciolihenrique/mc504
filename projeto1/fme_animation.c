/* Algoritimo Fast Mutual Exclusion para 5 threads que executam 1 vezes cada
 * Guilherme Lucas da Silva -
 * Henrique Noronha Facioli - 157988
 * Lauro Cruz e Souza       - 156175
 * Thiago Silva de Farias   - 148077
 */

#include "fme_animation.h"

#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Variavez globais */
int row, col;
pthread_mutex_t imprimindo;     /* Mutex ajuda na impressao */

/* Variaveis compartilhadas pelas threads */
volatile Boolean interesse[N] = {False,False,False,False};
volatile int fast_lock = 0;
volatile int slow_lock = 0;

/* Uma thread qualquer*/
void* f_thread(void *v) {
    int thr_id = *(int*)v + 1;

    for(int c = 0; c < THREAD_EXECUTION; c++){
        inicio:

        update_thread_message(thr_id, BEGIN);
        usleep(TIME);

        interesse[thr_id] = True;

        update_thread_message(thr_id, BEGIN_INTEREST);
        usleep(TIME);

        fast_lock = thr_id;

        update_thread_message(thr_id, ON_FAST_INTEREST);
        usleep(TIME);

        if(slow_lock != 0){
            interesse[thr_id] = False;
            update_thread_message(thr_id, ON_FAST);

            while(slow_lock != 0);
            goto inicio;
        }
        slow_lock = thr_id;

        update_thread_message(thr_id, ON_SLOW_INTEREST);
        usleep(TIME);

        if(fast_lock != thr_id){
            interesse[thr_id] = False;

            update_thread_message(thr_id, ON_SLOW);
            usleep(TIME);

            for(int i = 1; i < N; i++){
                while(interesse[i]);
                if(slow_lock != thr_id){
                    while(slow_lock != 0);
                    goto inicio;
                }
            }
        }

        /* ENTROU NA REGIAO CRTICA */
        update_thread_message(thr_id, CRITICAL_INTEREST);
        usleep(TIME);
        slow_lock = 0;
        interesse[thr_id] = False;
        /* SAIU DA REGIAO CRITICA */
        update_thread_message(thr_id, THE_END);
    }

    return NULL;
}

int main() {
    pthread_t thr[N];
    int id[N], i;

    initscr();
    getmaxyx(stdscr,row,col);

    printw(HOWTO);
    mvprintw( row-3, 0, "%s", "\tAPERTE ENTER PARA CONTINUAR");
    mvprintw(row/2 -3,(col-17)/2,"%s",UPPER);
    mvprintw(row/2 -2,(col-17)/2,"%s",UP);
    mvprintw(row/2 +4,(col-17)/2,"%s",DOWN);

    usleep(TIME);

    /* Inicializa as strings de cada uma das threads */
    for(i = 1; i < N; i++){
        update_thread_message(i, BEGIN);
    }
    /* Inicializa todas as threads */
    for (i = 0; i < N; i++){
        id[i] = i;
        pthread_create(&thr[i], NULL, f_thread, &id[i]);
    }
    /* Faz esperar a execucao de todas as threads */
    for (i = 0; i < N; i++){
        pthread_join(thr[i], NULL);
    }
    getch();
    endwin();

    return 0;
}

/* Funcoes para animacoes*/
/* Funcao que imprime na tela a animacao
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
 * Funcao que atualiza o o escrito de thread[i] para o nescessario
 */
void update_thread_message(int thr_id, char message[]){
    pthread_mutex_lock(&imprimindo);
    char thread_message[40];

    sprintf(thread_message, "Thread %d ", thr_id);
    strcat(thread_message, message);

    update_screen(thr_id, thread_message);
    /* Pode colocar um print_to_a_file aqui caso queira que imprima passo
     * a passo
     */
    pthread_mutex_unlock(&imprimindo);
}
