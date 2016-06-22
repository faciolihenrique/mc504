/* VSFS
 * Um projeto para a matéria MC504 - Sistemas Operacionais
 * 1s2016 - Profa. Islene
 * Grupo 6
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>           // Bibliotecas fs
#include <linux/module.h>
#include <linux/vfs.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include <asm/current.h>
#include <asm/uaccess.h>        // Função copy_to_user

//#include "data_operations.h"
//#include "data_operations.c"

#include "vsfs.h"

/* Licença do modulo */
MODULE_LICENSE("GPL");

#define VSFS_MAGIC 0x19496CF8

Vsfs_Skeleton Skeleton;



struct vsfs_data* find_data(struct inode *inode, Vsfs_Skeleton *Skeleton){
    struct vsfs_data *aux;
    int position = inode->i_ino;

    printk("VSFS: [find_data] Fazendo a busca. Inode nb = %d\n", position);

    aux = Skeleton->head[espalha(position)];
    //misto quente
    while(aux != NULL){
        if(aux->inode->i_ino == inode->i_ino){
            printk("VSFS: [find_data] Encontrou arquivo desejado\n");
            return aux;
        }
        aux = aux->next;
    }

    printk("VSFS: [find_data] Não encontrou o arquivo\n");

    return NULL;
}

void insert_data(struct vsfs_data *data, Vsfs_Skeleton *Skeleton){
    struct vsfs_data *aux;
    int position = data->inode->i_ino;

    printk("VSFS: [insert_data] Adicionando nova estrutura\n");

    if(data == NULL){
        printk("VSFS: [insert_data] Erro insert\n");
        return;
    }

    aux = find_data(data->inode, Skeleton);

    if(aux != NULL){
        printk("VSFS: [insert_data] Inode já existente\n");
    }else{
        printk("VSFS: [insert_data] Inserindo inode nb = %d", position);
        aux = Skeleton->head[espalha(position)];
        if(aux == NULL){
            Skeleton->head[espalha(position)] = data;
        }else{
            data->next = aux->next;
            aux->next = data;
        }
        (Skeleton->n_of_files++);
    }
}


/* File operations definitions */
static struct file_operations vsfs_file_ops = {
    .open       = vsfs_open,
    .read       = vsfs_read_file,
    .write      = vsfs_write_file,
};
static const struct inode_operations vsfs_inode_ops = {
	.getattr    = simple_getattr,
};

/* filesystem information */
static struct file_system_type vsfs_type = {
    .owner          = THIS_MODULE,
    .name           = "vsfs",
    .mount          = vsfs_mount,
    .kill_sb        = vsfs_umount,
};


/* Inicialização */

static int __init vsfs_init(void)
{
    int reg, i;

    /* Inicialização do Skeleton */
    for(i = 0; i < SIZE_OF_HEAD_VECTOR; i++){
        Skeleton.head[i] = NULL;
    }
    Skeleton.n_of_files=1;


    reg = register_filesystem(&vsfs_type);

    if (reg == 0 && Skeleton.n_of_files)
        printk("VSFS: carregado no kernel\n");
    else
        printk("VSFS: [Erro] não carregado. REG:%d \n", reg);

    return reg;
}

static void __exit vsfs_exit(void)
{
    int unreg = unregister_filesystem(&vsfs_type);

    if ( likely(unreg == 0))
        printk("VSFS: liberado do kernel\n");
    else
        printk("VSFS: [Erro] VSFS não liberado. UNREG:%d", unreg);
}

/*
 * As operações em arquivos
 */

/* abertura de um arquivo
 */
static int vsfs_open(struct inode *inode, struct file *filp)
{
    printk ("VSFS: [OPEN] Você abriu um arquivo.\n");
    if (inode->i_private)
        filp->private_data = inode->i_private;
    return 0;
}

/* leitura de arquivo
 * cat arquivo
 */
static ssize_t vsfs_read_file(struct file *filp, char *buff,
                                size_t count, loff_t *offset)
{
    Vsfs_Data *vsfs_file;
    int size_of_buffer = count;

    printk ("VSFS: [READ] Operação de leitura sendo feita.\n");

    /* Faz a busca do arquivo que deseja retornar */
    vsfs_file = find_data(filp->f_path.dentry->d_inode, &Skeleton);
    if( vsfs_file == NULL){
        return -EIO;
    }
    /* Operações para análise da area de dados */
    if (vsfs_file->inode->i_size < count)
		size_of_buffer = vsfs_file->inode->i_size;

	if ((*offset + size_of_buffer) >= vsfs_file->inode->i_size)
		size_of_buffer = vsfs_file->inode->i_size - *offset;

    /* Envia para o usuário o buffer */
    if (copy_to_user(buff, vsfs_file->infos + *offset, size_of_buffer))
    		return -EFAULT;

    *offset = *offset + size_of_buffer; // Renvia para o usuário onde paramos o envio

    return size_of_buffer;
}

/* Escrever um arquivo */
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
    size_t len, loff_t *offset)
{

    Vsfs_Data *vsfs_file;

    printk ("VSFS: [WRITE] Escrita sendo feita. Inode nb = %d.\n", filp->f_path.dentry->d_inode->i_ino);

    /* Primeiro procura o arquivo no FS*/
    vsfs_file = find_data(filp->f_path.dentry->d_inode, &Skeleton);

    if (vsfs_file == NULL){
        printk("VSFS: [WRITE] Arquivo não encontrado.\n");
        return -ENOENT;
    }

    printk("VSFS: [WRITE] Arquivo encontrado.\n");

    /* copy_from_user() : veja comentario na funcao de leitura */
    if ( copy_from_user(vsfs_file->infos + *offset, buff, len) ){
        printk("VSFS: [WRITE] Não foi possível copiar do usuario.\n");
        return -EFAULT;
    }

    vsfs_file->inode->i_size = len;
    vsfs_file->inode->i_mtime = CURRENT_TIME;

    return len;
}

int vsfs_unlink_file(struct inode *inode,struct dentry *dentry){
    printk("VSFS: [UNLINK] Erro na operação.\n");
    return 0;
}



/*
 * Funções em Diretórios
 */
 static const struct inode_operations vsfs_dir_inode_ops = {
     .lookup        = simple_lookup,
     .create        = vsfs_create_file,
     .mkdir         = vsfs_create_dir,
     .unlink        = vsfs_unlink_file,
 };

static int vsfs_create_file(struct inode *dir, struct dentry * dentry,
			    umode_t mode, bool excl)
{
    struct inode *inode;
    struct vsfs_data *file = kmalloc(sizeof(*file), GFP_KERNEL);
    struct page *page;

    if(!file){
        printk("VSFS: [CREATE_FILE] Erro na operação.\n");
        return -EAGAIN;
    }


    inode = new_inode(dir->i_sb);
    if (!inode)
        return -ENOMEM;

    inode->i_mode = (mode | S_IFREG | 0644);
    inode->i_uid = current->cred->fsuid;
    inode->i_gid = current->cred->fsgid;
    inode->i_blocks = 0;
    inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
    inode->i_ino = (Skeleton.n_of_files + 1);

    inode->i_op = &vsfs_inode_ops;
    inode->i_fop = &vsfs_file_ops;


    file->inode = inode;
    page = alloc_page(GFP_KERNEL);
    if(!page){
        printk("VSFS: [CREATE] Erro na alocação de página \n.");
        iput(inode);
        kfree(file);
        return -EINVAL;
    }

    file->infos = page_address(page);
    insert_data(file, &Skeleton);
    d_instantiate(dentry, inode);

    dentry->d_inode = inode;

    dget(dentry);

    return 0;
}

static int vsfs_create_dir(struct inode *dir, struct dentry * dentry,
			    umode_t mode)
{
    printk ("VSFS: [MKDIR] Operação não implementada.\n");

    //struct inode *inode;


    return 0;
}



/*
 * Funções em SuperBlocks
 */
static struct super_operations const vsfs_super_ops = {
    .statfs         = simple_statfs,
    .drop_inode     = generic_delete_inode,
};

/* Completa um superblock
 * a função simple_fill_super() é a função que faz isso mais "facilmente"
 */
static int vsfs_fill_super (struct super_block *sb, void *data, int silent)
{
    // Essa é a implementação de simple_fill_super que está em fs/libfs.c
    // Foi modificada apenas para receber a função vsfs_super_ops
    struct inode *root;
    struct dentry *root_dentry;
    struct user_namespace *ns = current_user_ns();

    sb->s_magic = VSFS_MAGIC;

    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;
    sb->s_maxbytes = 4096;

    sb->s_op = &vsfs_super_ops;
    sb->s_time_gran = 1;

    root = new_inode(sb);
    if (!root)
        return -ENOMEM;

    root->i_ino = 1;
    root->i_mode = S_IFDIR | 0755;
    root->i_atime = root->i_mtime = root->i_ctime = CURRENT_TIME;
    root->i_blocks = 0;
    root->i_uid = make_kuid(ns, 0);
    root->i_gid = make_kgid(ns, 0);

    root->i_op = &vsfs_dir_inode_ops;
    root->i_fop = &simple_dir_operations;

    set_nlink(root, 2);
    /* Qual a diferença ente d_make_root e d_alloc_root:?*/
    root_dentry = d_make_root(root);

    if (!root_dentry){
        d_genocide(root_dentry);
        shrink_dcache_parent(root_dentry);
        dput(root_dentry);
        return -ENOMEM;
    }

    sb->s_root = root_dentry;

    return 0;
}

/* Para registrar um superbloco do filesystem
 */
static struct dentry *vsfs_mount(struct file_system_type *fst,
					     int flags, const char *devname, void *data)
{

    struct dentry *mount = mount_bdev(fst, flags, devname, data,
                                                    vsfs_fill_super);


    // Fazer uma lista
    if ( IS_ERR(mount) )
        printk("VSFS:[Erro] Não foi possível montar VSFS");
    else
        printk("VSFS: montado em %s\n", devname);

    return mount;
}

static void vsfs_umount(struct super_block *sb)
{
	printk("VSFS: desmontado\n");
	/* This is just a dummy function as of now. As our filesystem gets matured,
	 * we will do more meaningful operations here */
	return kill_litter_super(sb);
}


module_init(vsfs_init);
module_exit(vsfs_exit);
