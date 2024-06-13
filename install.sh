#!/bin/bash
echo "-----------------"
echo "Building SCDRV..."
echo "-----------------"
# Сборка драйвера
make

read -p "Enter driver buffer size in bytes (e.g. 1024): " input

echo "-----------------"
echo "Installing SCDRV..."
echo "-----------------"
# Установка драйвера
sudo rmmod scdrv.ko
sudo insmod scdrv.ko bufsize="$input"
sudo mknod /dev/scdrv c 102 0
sudo chmod 777 /dev/scdrv

echo "-----------------"
echo "System logs about SCDRV"
echo "-----------------"
# Проверка загрузки драйвера
dmesg | grep scdrv
lsmod | grep scdrv

echo "-----------------"
echo "Cleaning \"make\" artifacts"
echo "-----------------"
# Очистка папки от артефактов
make clean
