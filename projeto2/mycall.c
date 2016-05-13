#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/string.h>

/*estrutura para guardar a chave e o endereço na memoria que o usario inseriu, para posterior busca*/

typedef struct Chain{
    struct Chain *prox;
    char *cadeia;
    int key;
} Chain;

/*cabeça da lista global, para poder ser acessada pela duas rotinas*/
Chain *head = NULL;

asmlinkage long sys_setkey(int key, char *ch){
    Chain *elemento, *percorre = head;

    /*aloca o elemento na memoria */
    elemento = kmalloc(sizeof(Chain));
    elemento->key = key;
    elemento->cadeia = kmalloc(sizeof(char)*(strlen(ch) +1));
    copy_from_user(ch, elemento->cadeia, sizeof(char)*(strlen(ch) +1));
    elemento->prox = NULL;

    /*Caso seja o primeiro elemento*/
    if(head==NULL){
        head = elemento;
        return 1;
    }

    /*Faz a busca do ultimo termo*/
    while(percorre->prox != NULL);
    aux->prox = elemento;

    return 1;
}

asmlinkage long sys_gettkey(int chave, char* ch){
    Chain *percorre = head;

    /*vai procurando ate o final da lista ligada*/
    while (percorre != NULL) {
        /*copia o endereço do kernel, para o espaco usuario*/
        if (percorre->key == chave) {
            copy_to_user(ch,percorre->cadeia,sizeof(char)*(strlen(percorre->cadeia) + 1));
            return 1;
        }
        /*continua percorrendo ate chegar no final da lista*/
        percorre = percorre->prox;
    }
    return 0;
}
