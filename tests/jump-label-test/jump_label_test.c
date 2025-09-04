#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jump_label.h>
#include <linux/ktime.h>
#include <linux/math64.h>

#define TEST_ITERATIONS 10000000

/* Define two static jump labels */
static DEFINE_STATIC_KEY_FALSE(test_key_false);
static DEFINE_STATIC_KEY_TRUE(test_key_true);

/* Normal conditional branch test function */
static noinline void normal_branch_test(bool condition)
{
    if (unlikely(condition)) {
        /* Rarely executed branch */
        asm volatile("" ::: "memory");
    } else {
        /* Frequently executed branch */
        asm volatile("" ::: "memory");
    }
}

/* Jump label test function (false case) */
static noinline void jump_label_test_false(void)
{
    if (static_branch_unlikely(&test_key_false)) {
        /* Rarely executed branch */
        asm volatile("" ::: "memory");
    } else {
        /* Frequently executed branch */
        asm volatile("" ::: "memory");
    }
}

/* Jump label test function (true case) */
static noinline void jump_label_test_true(void)
{
    if (static_branch_unlikely(&test_key_true)) {
        /* Rarely executed branch */
        asm volatile("" ::: "memory");
    } else {
        /* Frequently executed branch */
        asm volatile("" ::: "memory");
    }
}

/* Measure function execution time */
static u32 measure_time(void (*func)(void))
{
    ktime_t start, end;
    u32 total_time = 0;
    int i;
    
    start = ktime_get_ns();
    for (i = 0; i < TEST_ITERATIONS; i++) {
        func();
    }
    end = ktime_get_ns();
    
    total_time = end - start;
    return total_time;
}

/* Wrapper function for measuring normal branch */
static void measure_normal_branch(void)
{
    bool condition = false;
    normal_branch_test(condition);
}

/* Module initialization function */
static int __init jump_label_test_init(void)
{
    u32 normal_time, jump_false_time, jump_true_time;
    
    printk(KERN_INFO "Jump label performance test module loaded\n");
    
    /* Test normal conditional branch */
    normal_time = measure_time(measure_normal_branch);
    
    /* Test jump label (initial false) */
    jump_false_time = measure_time(jump_label_test_false);
    
    /* Test jump label (initial true) */
    jump_true_time = measure_time(jump_label_test_true);
    
    printk(KERN_INFO "Normal branch time: %d ns (%d ns per iteration)\n",
           normal_time, normal_time / TEST_ITERATIONS);
    printk(KERN_INFO "Jump label (false) time: %d ns (%d ns per iteration)\n",
           jump_false_time, jump_false_time / TEST_ITERATIONS);
    printk(KERN_INFO "Jump label (true) time: %d ns (%d ns per iteration)\n",
           jump_true_time, jump_true_time / TEST_ITERATIONS);
    
    return 0;
}

/* Module exit function */
static void __exit jump_label_test_exit(void)
{
    printk(KERN_INFO "Jump label test module unloaded\n");
}

module_init(jump_label_test_init);
module_exit(jump_label_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Miao");
MODULE_DESCRIPTION("Jump Label Performance Test Module");
