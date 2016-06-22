# VSFS
Um sistema de arquivo MUITO simples.
Implementado somente as operações de criar um arquivo, ler arquivo e gravar arquivo.

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


#### Referencias

1. https://ext4.wiki.kernel.org/index.php/Ext4_Disk_Layout
2. https://www.win.tue.nl/~aeb/linux/lk/lk-8.html
3. https://github.com/psankar/simplefs/commits/master?page=3
4. http://kukuruku.co/hub/nix/writing-a-file-system-in-linux-kernel
5. http://www.tldp.org/LDP/khg/HyperNews/get/fs/vfstour.html
6. https://accelazh.github.io/filesystem/Writing-a-Kernel-Filesystem
7. http://www.makelinux.net/ldd3/chp-3-sect-3
8. https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/11_FileSystemImplementation.html
9. https://lwn.net/Articles/57369/

e principalmente

10. http://lxr.free-electrons.com/ident?i=.
11. http://www.makelinux.net/ldd3/chp-11-sect-5
