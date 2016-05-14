#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

/* Estrutura para guardar a key e o endereço na memoria que o usario inseriu, para posterior busca*/

typedef struct Chain{
    struct Chain *next;
    char *str;
    int key;
} Chain;

/*cabeça da lista global, para poder ser acessada pela duas rotinas*/
Chain *head = NULL;

asmlinkage long sys_setkey(int key, char *ch){
    Chain *element, *search = head;
    int r;

    /*aloca o elemento na memoria */
    element = (Chain *) kmalloc(sizeof(Chain), __GFP_REPEAT);
    /* Caso o kernel não tenha conseguido alocar alocado */
    if(element == NULL){
        return 0;
    }
    element->key = key;
    element->str = (char *) kmalloc(sizeof(char)*(strlen(ch) +1), __GFP_REPEAT);
    if(element->str == NULL){
        return 0;
    }
    r = copy_from_user(element->str, ch , sizeof(char)*(strlen(ch) +1));
    if(r != 0){
        printk("Erro ao copiar do usuario: %d", r);
    }
    element->next = NULL;

    /*Caso seja o primeiro element*/
    if(head==NULL){
        head = element;
        return 1;
    }

    /*Faz a busca do ultimo termo ou do termo anterior ao que possui valor key*/
    while(search->next != NULL){
        search = search->next;
    }
    search->next = element;

    return 1;
}

asmlinkage long sys_getkey(int key, char* ch){
    Chain *search = head;
    int r;
    /*vai procurando ate o final da lista ligada*/
    while (search != NULL) {
        /*copia o endereço do kernel, para o espaco usuario*/
        if (search->key == key) {
            r = copy_to_user(ch ,search->str,sizeof(char)*(strlen(search->str) + 1));
            if(r != 0){
                printk("Erro ao copiar para o usuário: %d", r);
            }
            return 1;
        }
        /*continua searchndo ate chegar no final da lista*/
        search = search->next;
    }
    return 0;
}
