#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static int vsfs_open(struct inode *inode, struct file *filp);
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
                                            size_t len, loff_t *off);
static ssize_t vsfs_read_file(struct file *filp, char *buf,
                                        size_t count, loff_t *offset);
static ssize_t vsfs_write_file(struct file *filp, const char *buff,
                                            size_t len, loff_t *off);

static int vsfs_release(struct inode *inode, struct file *file);
static struct dentry *vsfs_get_super(struct file_system_type *fst,
					     int flags, const char *devname, void *data);
