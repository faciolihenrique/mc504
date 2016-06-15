#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


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
 * Superblocks
 */
static void vsfs_put_super(struct super_block *sb);
static int vsfs_fill_super (struct super_block *sb, void *data, int silent);
static struct dentry *vsfs_mount(struct file_system_type *fst,
					     int flags, const char *devname, void *data);
static void vsfs_umount(struct super_block *sb);

/*
 * Inodes
 */
