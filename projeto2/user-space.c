/* Programa de usuário para testar setkey e getkey */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(){
    char option, str[101];
    int key, r;

    printf("Programa para testar setkey e getkey\\n\\t1)setkey\\n\\t2)setkey\\n");
    scanf("%c",&option);

    if(option=='1'){
        printf("Digite uma chave:\\n");
        scanf("%d", &key);
        printf("Digite uma string:\\n");
        scanf("%s",str);
        r = syscall(378, key, str);
        if(r){
            printf("Elemento inserido:\\n - Chave %d com cadeia: %s\\n",key, str);
        }else{
            printf("Valor não inserido. Erro: %d\\n", r);
        }
    }else if(option == '2'){
        printf("Digite uma chave para procurar\\n");
        scanf("%d", &key);
        r = syscall(379, key, str);
        if(r == 1){
            printf("Elemento encontrado:\\n - Chave %d com cadeia: %s\\n",key, str);
        }else{
            printf("Chave não encontrada\\n");
        }
    }

    return 0;
}
