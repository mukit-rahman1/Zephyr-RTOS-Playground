# Zephyr RTOS Playground 

A minimal Zephyr RTOS project for the **ESP32-S3 DevKitC** that:
- Blinks a GPIO LED once per second
- Uses a GPIO button interrupt to **toggle a paused state**
- Temperature logging using DS18B20 sensor

The purpose of this project is to become familiar with Zephyr RTOS and West to be able to develop on the STM32 Satnoggs Communications board in Queen's Space Engineering Team  

---

## Install
After following the getting started guide from <a href="https://docs.zephyrproject.org/latest/develop/getting_started/index.html"> Zephyr RTOS</a>  

Copy this folder into the root folder (should be zephyrproject)  
Plug in ESP32  
west build -b esp32s3_devkitc/esp32s3/procpu -p always
west flash  

---

## Hardware

- Board: **ESP32-S3 DevKitC** (Zephyr target: `esp32s3_devkitc/esp32s3/procpu`)
- LED: GPIO **14** (active high)
- Button: GPIO **8** (pull-up, active low)
- Temperature Sensor: DS18B20 GPIO **12**

> Adjust pins in the overlay if wired differently.

