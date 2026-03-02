#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(blinky_with_thread, LOG_LEVEL_INF);

#define STACK_SIZE 1024
#define PRIORITY 7

// Get LED devices using the same pattern as ch2_blinky
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

// Define separate stack areas for each thread
K_THREAD_STACK_DEFINE(led0_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(led1_stack, STACK_SIZE);

struct k_thread led0_thread_data, led1_thread_data;

// This function will be called for the LED0 thread
void led0_thread_entry(void *p1, void *p2, void *p3) {
    const struct gpio_dt_spec led_dev0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
    if (&led_dev0 == NULL || !device_is_ready(&led_dev0)) {
        LOG_ERR("Failed to get LED0 device");
        return;
    }

    int ret = gpio_pin_configure_dt(&led_dev0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure LED0 pin");
        return;
    }

    while (1) {
        gpio_pin_toggle_dt(&led_dev0);
        k_msleep(1000);
    }
}

// This function will be called for the LED1 thread
void led1_thread_entry(void *p1, void *p2, void *p3) {
    const struct gpio_dt_spec led_dev1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
    if (&led_dev1 == NULL || !device_is_ready(&led_dev1)) {
        LOG_ERR("Failed to get LED1 device");
        return;
    }

    int ret = gpio_pin_configure_dt(&led_dev1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure LED1 pin");
        return;
    }

    while (1) {
        gpio_pin_toggle_dt(&led_dev1);
        k_msleep(3300);
    }
}

int main(void) {
    LOG_INF("Starting Blinky with Threads example");

    // Create a thread for LED0
    k_thread_create(&led0_thread_data, led0_stack, STACK_SIZE,
                    led0_thread_entry, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    // Create a thread for LED1
    k_thread_create(&led1_thread_data, led1_stack, STACK_SIZE,
                    led1_thread_entry, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    while (1) {
        k_msleep(10000);
    }

    return 0;
}