# VSFS
Um sistema de arquivo MUITO simples.
Baseado -até agora- em hellofs.

## Como testar?
primeiramente rode o make
    make

#### Inserindo o modulo no kernel
    sudo insmod vsfs.ko

#### Removendo o modulo do kernel
    sudo rmmod vsfs

### Testando o VSFS
Carregue o módulo no kernel.
Crie um arquivo vazio
    touch image
Crie um diretório para montar o fs
    mkdir mnt
Monte o sistema de arquivos:
    sudo mount -t vsfs -o loop image ./mnt

Desmonte o sistema com `sudo umount ./mnt` , remova o módulo do kernel e rode `make clean`
