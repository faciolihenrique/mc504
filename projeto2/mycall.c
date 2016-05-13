#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/string.h>

/*estrutura para guardar a key e o endereço na memoria que o usario inseriu, para posterior busca*/

typedef struct Chain{
    struct Chain *next;
    char *cadeia;
    int key;
} Chain;

/*cabeça da lista global, para poder ser acessada pela duas rotinas*/
Chain *head = NULL;

asmlinkage long sys_setkey(int key, char *ch){
    Chain *element, *search = head;

    /*aloca o element na memoria */
    element = kmalloc(sizeof(Chain));
    element->key = key;
    element->cadeia = kmalloc(sizeof(char)*(strlen(ch) +1));
    copy_from_user(ch, element->cadeia, sizeof(char)*(strlen(ch) +1));
    element->next = NULL;

    /*Caso seja o primeiro element*/
    if(head==NULL){
        head = element;
        return 1;
    }

    /*Faz a busca do ultimo termo*/
    while(search->next != NULL);
    aux->next = element;

    return 1;
}

asmlinkage long sys_getkey(int key, char* ch){
    Chain *search = head;

    /*vai procurando ate o final da lista ligada*/
    while (search != NULL) {
        /*copia o endereço do kernel, para o espaco usuario*/
        if (search->key == key) {
            copy_to_user(ch,search->cadeia,sizeof(char)*(strlen(search->cadeia) + 1));
            return 1;
        }
        /*continua searchndo ate chegar no final da lista*/
        search = search->next;
    }
    return 0;
}
