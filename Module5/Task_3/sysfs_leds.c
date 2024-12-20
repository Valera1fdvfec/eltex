#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

MODULE_DESCRIPTION("Sysfs-controlled keyboard LEDs");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valery Istratenko");

static struct kobject *led_kobj;
static int led_mask = 0;
struct tty_driver *my_driver;

#define CAPS_LED 0x01
#define FN_LED   0x02
#define F7_LED   0x04

// Устанавливает состояние светодиодов клавиатуры
static void set_leds(int mask) {
    if (my_driver && my_driver->ops->ioctl) {
        (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, mask);
    }
}

// Возвращает текущее состояние светодиодов через sysfs
static ssize_t led_mask_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", led_mask);
}

// Устанавливает новое состояние светодиодов через sysfs
static ssize_t led_mask_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    int new_mask;

    if (kstrtoint(buf, 10, &new_mask)) {
        return -EINVAL;
    }

    if (new_mask < 0 || new_mask > 7) {
        return -EINVAL;
    }

    led_mask = new_mask;
    set_leds(led_mask);
    return count;
}

static struct kobj_attribute led_mask_attr = __ATTR(led_mask, 0664, led_mask_show, led_mask_store);

// Инициализирует модуль и создает файл в sysfs
static int __init sysfs_leds_init(void) {
    int retval;
    my_driver = vc_cons[fg_console].d->port.tty->driver;

    led_kobj = kobject_create_and_add("keyboard_leds", kernel_kobj);
    if (!led_kobj) {
        return -ENOMEM;
    }

    retval = sysfs_create_file(led_kobj, &led_mask_attr.attr);
    if (retval) {
        kobject_put(led_kobj);
        return retval;
    }

    pr_info("Sysfs LED module loaded. Control LEDs via /sys/kernel/keyboard_leds/led_mask\n");
    return 0;
}

// Очищает модуль и удаляет файл из sysfs
static void __exit sysfs_leds_exit(void) {
    set_leds(0);
    kobject_put(led_kobj);
    pr_info("Sysfs LED module unloaded.\n");
}

module_init(sysfs_leds_init);
module_exit(sysfs_leds_exit);
