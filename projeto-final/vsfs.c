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

#include "vsfs.h"

/* Licença do modulo */
MODULE_LICENSE("GPL");


#define VSFS_MAGIC 0x19496CF8
//#define PAGE_SIZE 1024
//#define PAGE_SHIFT 10

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
        printk(KERN_INFO "VSFS: carregado no kernel\n", reg);
    else
        printk(KERN_ERR "VSFS: [Erro] não carregado. REG:%d \n", reg);

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
     printk ("VSFS: [OPEN] Operação não implementada.\n");
     return 0;
}

/* leitura de arquivo
 * cat arquivo
 */
static ssize_t vsfs_read_file(struct file *filp, char *buf,
                                size_t count, loff_t *offset)
{
    printk ("VSFS: [READ] Operação não implementada.\n");
    return -EINVAL;
}

/*escrever um arquivo
 *
 */
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
                                            size_t len, loff_t *off)
{
    printk ("VSFS: [WRITE] Operação não implementada.\n");
    return -EINVAL;
}


/*
 * Funções em Diretórios
 */
 static const struct inode_operations vsfs_dir_inode_ops = {
     .lookup        = simple_lookup,
     .create        = vsfs_create_file,
     .mkdir         = vsfs_create_dir,
 };

static int vsfs_create_file(struct inode *dir, struct dentry * dentry,
			    umode_t mode, bool excl)
{
    printk ("VSFS: [CREATE_FILE] Operação não implementada.\n");
    return 0;
}

static int vsfs_create_dir(struct inode *dir, struct dentry * dentry,
			    umode_t mode)
{
    printk ("VSFS: [MKDIR] Operação não implementada.\n");
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
    //return simple_fill_super(sb, VSFS_MAGIC, vsfs);

    // Essa é a implementação de simple_fill_super que está em fs/libfs.c
    // Foi modificada apenas para receber a função vsfs_super_ops
    struct inode *inode;
    struct dentry *root;
    //struct dentry *dentry;

    struct user_namespace *ns = current_user_ns();

    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;
    sb->s_magic = VSFS_MAGIC;
    sb->s_op = &vsfs_super_ops;
    sb->s_time_gran = 1;

    inode = new_inode(sb);
    if (!inode)
        return -ENOMEM;
    /*
     * because the root inode is 1, the files array must not contain an
     * entry at index 1
     */
    inode->i_ino = 1;
    inode->i_mode = S_IFDIR | S_IRUGO | S_IXUGO | S_IWUSR;
    inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

    inode->i_op = &vsfs_dir_inode_ops;
    inode->i_fop = &simple_dir_operations;

    inode->i_uid = make_kuid(ns, 0);
    inode->i_gid = make_kgid(ns, 0);
    set_nlink(inode, 2);

    root = d_make_root(inode);
    if (!root){
        iput(inode);
        return -ENOMEM;
    }

    sb->s_root = root;
    return 0;
/*
out:
    d_genocide(root);
    shrink_dcache_parent(root);
    dput(root);
*/
}

/* Para registrar um superbloco do filesystem
 */
static struct dentry *vsfs_mount(struct file_system_type *fst,
					     int flags, const char *devname, void *data)
{

    struct dentry *mount = mount_bdev(fst, flags, devname, data,
                                                    vsfs_fill_super);

    if ( IS_ERR(mount) )
        printk(KERN_ERR "VSFS:[Erro] Não foi possível montar VSFS");
    else
        printk(KERN_INFO "VSFS: montado em %s\n", devname);

    return mount;
}

static void vsfs_umount(struct super_block *sb)
{
	printk(KERN_INFO "VSFS: desmontado\n");
	/* This is just a dummy function as of now. As our filesystem gets matured,
	 * we will do more meaningful operations here */
	return kill_litter_super(sb);
}


module_init(vsfs_init);
module_exit(vsfs_exit);
