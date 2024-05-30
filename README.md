# linux-symbol-driver
Символьный драйвер, позволяющий передавать информацию двум независимым процессам по принципу кольцевого буфера

## Файлы исходного кода
`scdrv.c` - основной файл, содержащий программу. В нём функция main с исполнением mod_exec, находящимся в module.c

`module.c` - исходный код функций, связанных с инициализацией, закрытием модуля. В нём в частности работа со структурой scdrv, которая находится в driver.h

`driver.h` - заголовочный файл, содержащий тело структуры scdrv и функции для неё. scdrv - структура, соответствующая драйверу. 

`driver.c` - реализация driver.h

`buffer.c` - код, отвечающий за основную структуру данных драйвера - кольцевого буфера "ringbuffer" - и функции работы с ним

`libs.h` - заголовочный файл со всеми используемыми в коде библиотеками

## Сборка и установка
`# sudo ./build_and_install.sh`

## TODO
Применение функционала обеспечения получения информации о времени последней операции чтения и записи в буфер, а так же идентификаторов и владельцев процессов совершивших эту операцию;
Переключение режима работы вызовов read\write из блокирующих \ не блокирующих и обратно (логика прописана, осталось реализовать активацию переключения);
Обеспечение возможности задавать размер кольцевого буфера через механизм параметров модулей ядра (осталось реализовать ввод с параметра);
Тестирование (на основной машине возникли проблемы с нахождением некоторых нужных библиотек, хотя расчётно сборка должна выполняться);
Оформление документации;
Сборка вне дерева исходных кодов ядра при помощи Makefile (на данный момент используются исходный код ядра на машине). 

## Задача
Реализовать символьный драйвер предлагающий возможность передавать информацию двум независимым процессам 
через файл символьного устройства: 
- взаимодействие должно осуществляться по принципу кольцевого буфера;  т.е. один процесс может писать в файл символьного устройства, а другой из него читать. 
- обеспечить возможность задавать размер кольцевого буфера через механизм параметров модулей ядра; 
- по умолчанию вызовы чтения и записи должны быть блокирующими; 
- обеспечить поддержку нескольких вызовов IOCTL: 
-- переключение режима работы вызовов read\write из блокирующих \ не блокирующих и обратно;
-- обеспечить возможность получения информации о времени последней операции чтения и записи в буфер, а так же идентификаторов и владельцев процессов совершивших эту операцию;

- драйвер должен собираться вне дерева исходных кодов ядра при помощи Makefile; 
- исходные коды драйвера должны находиться под системой контроля версий git и содержать историю коммитов; 
- результат передаётся в виде ссылки на репозиторий размещённый на github или в виде архива содержащего репозиторий с исходными кодами.
