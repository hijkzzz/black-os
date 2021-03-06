#include "clock.h"
#include "idt.h"
#include "common.h"
#include "debug.h"
#include "sched.h"

// 时间片轮转调度
void timer_callback(pt_regs_t *regs)
{
    schedule();
}

void init_timer(uint32_t frequency)
{
    register_interrupt_handler(32, timer_callback);

    // 8253/8254 端口地址是 40h~43h
    uint32_t divisor = (1193182  + frequency / 2) / frequency;

    // 初始化 8253/8254，方式2，0 0 | 1 1 | 0 1 1 | 0
    outb(0x43, 0x36);
    // 先写入低字节，在写入高字节
    outb(0x40, (uint8_t)(divisor % 256));
    outb(0x40, (uint8_t)(divisor / 256));
}
