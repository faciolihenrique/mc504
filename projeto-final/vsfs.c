/* Código levemente modificado do hellofs
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "vsfs.h"

#define VSFS_MAGIC 0x19496CF8

static struct file_operations vsfs_file_ops = {
    .open   = vsfs_open,
    .read   = vsfs_read_file,
    .write  = vsfs_write_file,
    .release = vsfs_release,
};

static struct file_system_type vsfs_type = {
    .owner          = THIS_MODULE,
    .name           = "vsfs",
    .mount          = vsfs_get_super,
    .kill_sb        = kill_litter_super,
};

/* Essa é a estrutura da arvore de arquivos do vsfs.
 * Ele é estático por enquanto
 */
struct tree_descr vsfs[] = {
  { NULL, NULL, 0 },
  { "hello.txt", &vsfs_file_ops, S_IWUSR|S_IRUGO },
  { "", NULL, 0},
};



static int __init vsfs_init(void)
{
    int reg = register_filesystem(&vsfs_type);
    printk("VSFS properly loaded on kernel %d \n", reg);
    return reg;
}


static void __exit vsfs_exit(void)
{
    unregister_filesystem(&vsfs_type);
}


/* Quando um arquivo é pedido para abertura
 * ex: vim arquivo
 */
static int vsfs_open(struct inode *inode, struct file *filp)
{
     return 0;
}


/* Quando é pedido para fazer uma leitura no arquivo
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

/* Escrita do arquivo:
 *
 */
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
                                            size_t len, loff_t *off)
{
   printk ("Sorry, this operation isn't supported.\n");
   return -EINVAL;
}

/* Release */
static int vsfs_release(struct inode *inode, struct file *file)
{
   return 0;
}

static int vsfs_fill_super (struct super_block *sb, void *data, int silent)
{
    return simple_fill_super(sb, VSFS_MAGIC, vsfs);
}

static struct dentry *vsfs_get_super(struct file_system_type *fst,
					     int flags, const char *devname, void *data)
{
    return mount_bdev(fst, flags, devname, data, vsfs_fill_super);
}

module_init(vsfs_init);
module_exit(vsfs_exit);

MODULE_LICENSE("GPL");
