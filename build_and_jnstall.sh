#!/bin/bash

# Сборка драйвера
make

# Установка драйвера
sudo insmod my_driver.ko

# Проверка загрузки драйвера
lsmod | grep my_driver
