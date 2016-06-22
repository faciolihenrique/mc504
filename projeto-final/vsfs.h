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

#define SIZE_OF_HEAD_VECTOR 64

/** Nosso Sistema de arquivos **/
typedef struct vsfs_data{
    struct vsfs_data *next;
    struct inode *inode;
    void *infos;
}Vsfs_Data;

/* Pensei em fazer um hash - ao invez de ter uma unica cabeça, teriamos um vetor de cabeça :)*/
typedef struct vsfs_skeleton{
    struct vsfs_data* head;
    int n_of_files;
}Vsfs_Skeleton;

/* Funções para gerenciar a estrutura */
int espalha(int k){
    return (k%SIZE_OF_HEAD_VECTOR);
}
struct vsfs_data* find_data(struct inode *inode);
void insert_data(struct vsfs_data *data);
void remove_data(struct vsfs_data *data);

/*
 * Operações em arquivos
 */

/* abertura de um arquivo */
static int vsfs_open(struct inode *inode, struct file *filp);
/* leitura de arquivo  */
static ssize_t vsfs_read_file(struct file *filp, char *buf,
                                        size_t count, loff_t *offset);
/*escrever um arquivo */
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
                                            size_t len, loff_t *off);

/*
 * Diretorios
 */
 static int vsfs_create_file(struct inode *dir, struct dentry * dentry,
 			    umode_t mode, bool excl);
 static int vsfs_create_dir(struct inode *dir, struct dentry * dentry,
 			    umode_t mode);

/*
 * Superblocks
 */
//static void vsfs_put_super(struct super_block *sb);
static int vsfs_fill_super (struct super_block *sb, void *data, int silent);
static struct dentry *vsfs_mount(struct file_system_type *fst,
					     int flags, const char *devname, void *data);
static void vsfs_umount(struct super_block *sb);

/*
 * Inodes
 */
