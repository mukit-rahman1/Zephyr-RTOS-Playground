//esp32 build cmd: west build -b esp32s3_devkitc/esp32s3/procpu -p always
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


#define LED_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static const struct device *get_ds18b20_device(void)
{
    const struct device *dev = DEVICE_DT_GET_ANY(maxim_ds18b20);

    if (dev == NULL) {
        printk("Error: no DS18B20 node found (status okay?)\n");
        return NULL;
    }
    if (!device_is_ready(dev)) {
        printk("Error: DS18B20 device not ready: %s\n", dev->name);
        return NULL;
    }
    return dev;
}

static struct gpio_callback btn_cb;
static volatile bool paused;

static void button_pressed_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    paused = !paused;
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int main() {

    int ret;
    bool led_state = true;
    const struct device *ds = get_ds18b20_device();


    if (!gpio_is_ready_dt(&sw0)){
        return 0;
    }
    if (!gpio_is_ready_dt(&led)){
        return 0;
    }
    
    ret = gpio_pin_configure_dt(&sw0, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

    //configure interrupt
    gpio_pin_interrupt_configure_dt(&sw0, GPIO_INT_EDGE_TO_ACTIVE);

    //add callback
    gpio_init_callback(&btn_cb, button_pressed_isr, BIT(sw0.pin));
    gpio_add_callback(sw0.port, &btn_cb);

    // print onces in boot
    printk("--- SYSTEM BOOTING ---\n");

    while (true) {
        printk("Hello\n");

		if(!paused) gpio_pin_toggle_dt(&led);

		led_state = !led_state;
		printk("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(1000);


       if (ds) {
            if (sensor_sample_fetch(ds) == 0) {
                struct sensor_value t;
                int res = sensor_sample_fetch(ds);
                if (res == 0) {
                    printk("Temp: %d.%06d C\n", t.val1, t.val2);
                } else {
                    printk("Temp read failed (channel_get)\n");
                }
            } else {
                printk("Temp read failed (sample_fetch)\n");
            }
        }

    }
    return 0;
}