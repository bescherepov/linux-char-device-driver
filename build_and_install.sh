#!/bin/bash

# Сборка драйвера
make

# Установка драйвера
sudo insmod scdrv.ko
sudo mknod /dev/scdrv c 102 0
sudo chmod 777 /dev/scdrv

# Проверка загрузки драйвера
lsmod | grep scdrv
