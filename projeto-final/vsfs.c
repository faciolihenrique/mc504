/* VSFS
 * Um projeto para a matéria MC504 - Sistemas Operacionais
 * 1s2016 - Profa. Islene
 * Grupo 6
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>           // Bibliotecas estão

#include <asm/uaccess.h>        // Função copy_to_user

#include "vsfs.h"

/* Licença do modulo */
MODULE_LICENSE("GPL");


#define VSFS_MAGIC 0x19496CF8

/* File operations definitions */
static struct file_operations vsfs_file_ops = {
    .open       = vsfs_open,
    .read       = vsfs_read_file,
    .write      = vsfs_write_file,
};

/* vsfs[]
 * Essa é a estrutura que está armazenando os arquivos por enquanto!
 */
struct tree_descr vsfs[] = {
    { NULL, NULL, 0 },
    { "hello.txt", &vsfs_file_ops, S_IWUSR|S_IRUGO },
    { "", NULL, 0},
};


/* filesystem information */
static struct file_system_type vsfs_type = {
    .owner          = THIS_MODULE,
    .name           = "vsfs",
    .mount          = vsfs_mount,
    .kill_sb        = vsfs_umount,
};
/* vsfs_init(void) - a 'main()'
 * Função de inicialização do módulo
 * Como é um filesystem deve chamar a função:
 *      int register_filesystem(&fs_type);
 * com o argumento:
 *      static struct file_system_type fs_type = {
 *          .owner 	    = THIS_MODULE,
 *          .name	    = "lwnfs",
 *          .get_sb	    = lfs_get_super,
 *	        .kill_sb	= kill_litter_super,
 *      };
 */
static int __init vsfs_init(void)
{
    int reg = register_filesystem(&vsfs_type);

    if (likely(reg == 0))
        printk(KERN_INFO "VSFS carregado no kernel\n", reg);
    else
        printk(KERN_ERR "[Erro] VSFS não carregado. REG:%d \n", reg);

    return reg;
}

static void __exit vsfs_exit(void)
{
    int unreg = unregister_filesystem(&vsfs_type);

    if ( likely(unreg == 0))
        printk(KERN_INFO "VSFS liberado do kernel\n");
    else
        printk(KERN_ERR "[Erro] VSFS não liberado. UNREG:%d", unreg);
}


/*
 * As operações em arquivos
 */

/* abertura de um arquivo
 */
static int vsfs_open(struct inode *inode, struct file *filp)
{
     return 0;
}

/* leitura de arquivo
 * cat arquivo
 */
static ssize_t vsfs_read_file(struct file *filp, char *buf,
                                size_t count, loff_t *offset)
{
    const char *s = "Hello World!\n";
    if (*offset != 0)
        return 0;
    if (copy_to_user(buf, s, strlen(s)+1))
        return -EFAULT;
    *offset += count;
    return count;
}

/*escrever um arquivo
 *
 */
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
                                            size_t len, loff_t *off)
{
    printk ("Operação não implementada.\n");
    return -EINVAL;
}


/*
 * SuperBlocks
 */

static struct super_operations const vsfs_super_ops = {
    .put_super = vsfs_put_super,
    //.delete_inode = vsfs_inode_delete,
    //.alloc_inode = vsfs_inode_alloc,
};

static void vsfs_put_super(struct super_block *sb)
{
    pr_debug("aufs super block destroyed\n");
}

/* Completa um superblock
 * a função simple_fill_super() é a função que faz isso mais "facilmente"
 */
static int vsfs_fill_super (struct super_block *sb, void *data, int silent)
{
    return simple_fill_super(sb, VSFS_MAGIC, vsfs);
}
/* Para registrar um superbloco do filesystem
 */
static struct dentry *vsfs_mount(struct file_system_type *fst,
					     int flags, const char *devname, void *data)
{

    struct dentry *mount = mount_bdev(fst, flags, devname, data,
                                                    vsfs_fill_super);

    if ( IS_ERR(mount) )
        printk(KERN_ERR "[Erro] Não foi possível montar VSFS");
    else
        printk(KERN_INFO " VSFS montado em %s\n", devname);

    return mount;
}

static void vsfs_umount(struct super_block *sb)
{
	printk(KERN_INFO "VSFS desmontado\n");
	/* This is just a dummy function as of now. As our filesystem gets matured,
	 * we will do more meaningful operations here */
	return kill_litter_super(sb);
}


module_init(vsfs_init);
module_exit(vsfs_exit);
