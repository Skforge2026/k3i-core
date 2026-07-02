#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SK-Forge UG");
MODULE_DESCRIPTION("K3I Ring-0 Wächter - Symmetrisches Hardware-Veto System (Austariert)");

#define JITTER_THRESHOLD    5000000ULL
#define GPIO_BASE_ADDR      0x3F200000

#define GPIO_INT_MOSFET     4
#define GPIO_EXT_LUFTSPALT  5
#define GPIO_RAM_PURGE      6
#define GPIO_RESET_PIN      7

static void __iomem *gpio_regs;
static struct task_struct *k3i_watchdog_task;

static inline uint64_t read_tsc(void) {
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static int k3i_watchdog_thread(void *data) {
    uint64_t start, end, delta;

    printk(KERN_INFO "K3I: Wächter-Thread gestartet auf isolierter CPU.\n");

    while (!kthread_should_stop()) {
        start = read_tsc();
        udelay(100);
        end = read_tsc();
        delta = end - start;

        if (delta > JITTER_THRESHOLD) {
            printk(KERN_EMERG "K3I-ALARM: Jitter-Anomalie erkannt! Delta: %llu Zyklen\n", delta);

            iowrite32((1 << GPIO_INT_MOSFET), gpio_regs + 0x1C);
            iowrite32((1 << GPIO_EXT_LUFTSPALT), gpio_regs + 0x28);
            iowrite32((1 << GPIO_RAM_PURGE), gpio_regs + 0x1C);
            mdelay(10);
            iowrite32((1 << GPIO_RESET_PIN), gpio_regs + 0x1C);

            break;
        }

        udelay(30);
    }
    return 0;
}

static int __init k3i_init(void) {
    printk(KERN_INFO "K3I: Initialisierung gestartet...\n");

    gpio_regs = ioremap(GPIO_BASE_ADDR, 0xB0);
    if (!gpio_regs) {
        printk(KERN_ERR "K3I: ioremap GPIO fehlgeschlagen\n");
        return -ENOMEM;
    }

    k3i_watchdog_task = kthread_create(k3i_watchdog_thread, NULL, "k3i_watchdog");
    if (IS_ERR(k3i_watchdog_task)) {
        printk(KERN_ERR "K3I: Thread-Erstellung fehlgeschlagen\n");
        iounmap(gpio_regs);
        return PTR_ERR(k3i_watchdog_task);
    }

    kthread_bind(k3i_watchdog_task, 2);
    wake_up_process(k3i_watchdog_task);

    printk(KERN_INFO "K3I: Wächter erfolgreich aktiviert auf Core 2.\n");
    return 0;
}

static void __exit k3i_exit(void) {
    if (k3i_watchdog_task)
        kthread_stop(k3i_watchdog_task);

    if (gpio_regs)
        iounmap(gpio_regs);

    printk(KERN_INFO "K3I: Modul entladen.\n");
}

module_init(k3i_init);
module_exit(k3i_exit);
