#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valery");
MODULE_DESCRIPTION("Простой модуль Hello World для ядра Linux"); 

static int __init hello_init(void) {
    printk(KERN_INFO "Привет, мир! Модуль загружен.\n");
    return 0; // Возвращаем 0 при успешной загрузке
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Прощай, мир! Модуль выгружен.\n");
}

module_init(hello_init);
module_exit(hello_exit);
