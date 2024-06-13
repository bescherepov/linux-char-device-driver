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
#sudo mknod -m 777 /dev/scdrv0 c 102 0

echo "-----------------"
echo "System logs about SCDRV"
echo "-----------------"
# Проверка загрузки драйвера
dmesg | grep SCDRV
lsmod | grep scdrv

echo "-----------------"
echo "Cleaning \"make\" artifacts"
echo "-----------------"
# Очистка папки от артефактов
make clean
