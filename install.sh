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
sudo rmmod scdrvmod.ko
sudo insmod scdrvmod.ko bufsize="$input"
sudo chmod 666 /dev/scdrv*

echo "-----------------"
echo "System logs about SCDRV"
echo "-----------------"
# Проверка загрузки драйвера
dmesg | tail | grep SCDRV
lsmod | grep scdrv

echo "-----------------"
echo "Cleaning \"make\" artifacts"
echo "-----------------"
# Очистка папки от артефактов
make clean
