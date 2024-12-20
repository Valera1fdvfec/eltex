#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_FILE_NAME "proc_example" //так будет называться файл в /proc
#define BUFFER_SIZE 128

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valery_Istratenko");
MODULE_DESCRIPTION("Модуль для обмена данными между ядром и userspace через /proc");

static char *kernel_buffer;
static size_t buffer_len;

// Обработчик чтения из файла /proc
static ssize_t proc_read(struct file *file, char __user *user_buffer, size_t count, loff_t *pos) {
    if (*pos > 0 || count < buffer_len) {
        return 0;
    }
    if (copy_to_user(user_buffer, kernel_buffer, buffer_len)) {
        return -EFAULT;
    }
    *pos = buffer_len;
    return buffer_len;
}

// Обработчик записи в файл /proc
static ssize_t proc_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *pos) {
    if (count > BUFFER_SIZE) {
        return -EINVAL;
    }
    if (copy_from_user(kernel_buffer, user_buffer, count)) {
        return -EFAULT;
    }
    buffer_len = count;
    kernel_buffer[count] = '\0';
    return count;
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

// Функция инициализации модуля
static int __init proc_module_init(void) {
    kernel_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        pr_err("Failed to allocate memory for buffer\n");
        return -ENOMEM;
    }
    if (!proc_create(PROC_FILE_NAME, 0666, NULL, &proc_file_ops)) {
        pr_err("Failed to create /proc/%s\n", PROC_FILE_NAME);
        kfree(kernel_buffer);
        return -ENOMEM;
    }
    pr_info("/proc/%s created\n", PROC_FILE_NAME);
    return 0;
}

// Функция очистки модуля
static void __exit proc_module_exit(void) {
    remove_proc_entry(PROC_FILE_NAME, NULL);
    kfree(kernel_buffer);
    pr_info("/proc/%s removed\n", PROC_FILE_NAME);
}

module_init(proc_module_init);
module_exit(proc_module_exit);
