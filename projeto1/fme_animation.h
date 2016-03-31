#include <pthread.h>

/* Declaracao do tipo booleano*/
typedef enum {False = 0, True = 1} Boolean;

/* Definicoes de mascaras que serao aplicadas */
#define HOWTO "\n\t- B eh o local de inicio de todas as threads\n\t- F eh o local do fastlock\n\t- S eh o local do slowlock\n\t- C eh a regiao critica, onde duas threads nunca podem\n\testar juntas\n\t- E eh o final, onde tem a variavel compartilhada\n\t- Quando a thread esta em 'O' ela esta interessada,\n\tquando esta em 'o' nao tem interesse\n"
#define UPPER     "#--------------------#\n"
#define UP        "#_B_|_F_|_S_|_C_|_E_|#\n"
#define BEGIN     ": o |   |   | x |   |#\n"
#define ON_FAST   ":   | o |   | x |   |#\n"
#define ON_SLOW   ":   |   | o | x |   |#\n"
#define CRITICAL  ":   |   |   | o |   |#\n"
#define THE_END   ":   |   |   |   | o |#\n"
#define DOWN      "#--------------------#\n"
#define BEGIN_INTEREST     ": O |   |   | x |   |#\n"
#define ON_FAST_INTEREST   ":   | O |   | x |   |#\n"
#define ON_SLOW_INTEREST   ":   |   | O | x |   |#\n"
#define CRITICAL_INTEREST  ":   |   |   | O |   |#\n"
#define THE_END_INTEREST   ":   |   |   |   | O |#\n"

/* Definições de constantes */
#define N 6
#define THREAD_EXECUTION 1      /* Num de vezes que cada thread eh executada*/
#define TIME 700000             /* Para usar algum sleep() */


/* Funcoes para animações*/
/* Funcao que imprime na tela a animacao
 */
void update_screen(int thr_id, char thread_message[]);

/* update_thread_message
 * Funcao que atualiza o o escrito de thread[i] para o nescessario
 */
void update_thread_message(int thr_id, char message[]);

/* print_to_a_file
 * Funcao que imprime em um arquivo o passo a passo da animacao */
void print_to_a_file();
