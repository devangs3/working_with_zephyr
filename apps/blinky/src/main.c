#include <zephyr.h>
#include <device.h>
#include <zephyr/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
LOG_MODULE_REGISTER(blinky, LOG_LEVEL_INF);

void main(){
    LOG_INF("Starting Blinky application");

    const struct device *led_dev = device_get_binding(DT_LABEL(DT_NODELABEL(led0)));
    if (led_dev == NULL) {
        LOG_ERR("Failed to get LED device binding");
        return;
    }

    int ret = gpio_pin_configure(led_dev, DT_GPIO_PIN(DT_NODELABEL(led0), gpios), GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure LED pin");
        return;
    }

    while (1) {
        gpio_pin_toggle(led_dev, DT_GPIO_PIN(DT_NODELABEL(led0), gpios));
        k_msleep(1000);
    }
}