#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int major;
static atomic_t already_open = ATOMIC_INIT(0);
static char msg[BUF_LEN];
static struct class *cls;

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);
    if (major < 0) {
        pr_alert("Регистрация символьного устройства не удалась с кодом %d\n", major);
        return major;
    }

    pr_info("Назначен основной номер %d\n", major);

    // Создание класса устройства
    cls = class_create(DEVICE_NAME); // Передаем только имя класса
    if (IS_ERR(cls)) { // Проверка ошибки создания класса
        pr_alert("Ошибка при создании класса устройства\n");
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(cls);
    }

    // Создание устройства
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    pr_info("Устройство создано в /dev/%s\n", DEVICE_NAME);
    return SUCCESS;
}

static void __exit chardev_exit(void) {
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file) {
    static int counter = 0;

    if (atomic_cmpxchg(&already_open, 0, 1))
        return -EBUSY;

    sprintf(msg, "Я уже сказал вам %d раз Привет, мир!\n", counter++);
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file) {
    atomic_set(&already_open, 0);
    module_put(THIS_MODULE);
    return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer, 
                           size_t length, loff_t *offset) {
    int bytes_read = 0;
    const char *msg_ptr = msg;

    if (!*(msg_ptr + *offset)) { // Проверяем, достигли ли конца сообщения
        *offset = 0; // Сбрасываем смещение
        return 0; // EOF
    }

    msg_ptr += *offset;

    while (length && *msg_ptr) {
        if (put_user(*(msg_ptr++), buffer++)) {
            return -EFAULT; // Ошибка копирования в пространство пользователя
        }
        length--;
        bytes_read++;
    }

    *offset += bytes_read; // Обновляем смещение
    return bytes_read; // Возвращаем количество прочитанных байтов
}

static ssize_t device_write(struct file *filp, const char __user *buff, 
                            size_t len, loff_t *off) {
    size_t bytes_to_write = (len < BUF_LEN - 1) ? len : BUF_LEN - 1;

    if (copy_from_user(msg, buff, bytes_to_write)) {
        return -EFAULT; // Ошибка копирования из пользовательского пространства
    }

    msg[bytes_to_write] = '\0'; // Завершаем строку
    pr_info("Записано сообщение: %s\n", msg);
    return bytes_to_write; // Возвращаем количество записанных байтов
}

module_init(chardev_init);
module_exit(chardev_exit);
MODULE_LICENSE("GPL");
