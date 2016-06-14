# VSFS
Um sistema de arquivo MUITO simples.
Baseado -até agora- em hellofs.

## Como testar?
### Requerimentos
No arch, instale com o pacman os pacotes base-devel e linux-headers.
Em debian-like é necessário o build-essentia e linux-headers (não sei exatamente o nome dos pacotes) * no ubuntu tente rodar `sudo apt-get install linux-headers-$(uname -r) build-essential`

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

## Erros
Alguns erros são facilmente corrigidos com um reboot.
