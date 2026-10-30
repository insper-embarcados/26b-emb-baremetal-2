#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int LED_R = 16;
const int BTN_R = 15;

volatile int r_ti = 0;
volatile int r_tf = 0;

const int LED_Y = 17;
const int BTN_Y = 11;

volatile int y_ti = 0;
volatile int y_tf = 0;

volatile int timer_r_long = 0;
volatile int timer_y_long = 0;

int64_t alarm_r_callback(alarm_id_t id, void* user_data) {
    timer_r_long = 0;
    gpio_put(LED_R, 0);
    return 0;
}

int64_t alarm_y_callback(alarm_id_t id, void* user_data) {
    timer_y_long = 0;
    gpio_put(LED_Y, 0);
    return 0;
}

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_R) {
        if (events == GPIO_IRQ_EDGE_FALL)
            r_ti = to_ms_since_boot(get_absolute_time());

        if (events == GPIO_IRQ_EDGE_RISE)
            r_tf = to_ms_since_boot(get_absolute_time()) - r_ti;
    }

    if (gpio == BTN_Y) {
        if (events == GPIO_IRQ_EDGE_FALL)
            y_ti = to_ms_since_boot(get_absolute_time());

        if (events == GPIO_IRQ_EDGE_RISE)
            y_tf = to_ms_since_boot(get_absolute_time()) - y_ti;
    }
}


int main() {
    stdio_init_all();

    /*
     * LED
     */
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);

    gpio_init(BTN_R);
    gpio_set_irq_enabled_with_callback(
        BTN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_pull_up(BTN_R);

    gpio_init(LED_Y);
    gpio_set_dir(LED_Y, GPIO_OUT);
    gpio_put(LED_Y, 0);

    gpio_init(BTN_Y);
    gpio_set_irq_enabled_with_callback(
        BTN_Y, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_pull_up(BTN_Y);

    int r_t = 0;
    int y_t = 0;

    while (1) {
        // curto
        if (r_tf > 0 && r_tf <= 600 && r_t != 0) {
            printf("LED R: Curto\n");

            if (timer_r_long == 0) {
                printf("LED R: Acendendo por %d ms\n", r_t);
                gpio_put(LED_R, 1);
                add_alarm_in_ms(r_t, alarm_r_callback, NULL, false);
                timer_r_long = 1;
            }
            r_tf = 0;

        }  // longo
        if (r_tf > 1000) {
            printf("LED R: Longo\n");
            r_t = r_tf;
            printf("LED R: %d ms\n", r_t);
            r_tf = 0;
        }

        if (y_tf > 0 && y_tf <= 600 && y_t != 0) {
            printf("LED Y: Curto\n");

            if (timer_y_long == 0) {
                printf("LED Y: Acendendo por %d ms\n", y_t);
                gpio_put(LED_Y, 1);
                add_alarm_in_ms(y_t, alarm_y_callback, NULL, false);
                timer_y_long = 1;
            }
            y_tf = 0;

        }  // longo
        if (y_tf > 1000) {
            printf("LED Y: Longo\n");
            y_t = y_tf;
            printf("LED Y: %d ms\n", y_t);
            y_tf = 0;
        }
    }
}

