#!/bin/bash

if [ $# -lt 3 ]; then
    echo "Usage: $0 <number of arrays> <length of arrays> <max number of threads>"
    exit 1
fi

gcc main.c -o main
./main $1 $2 $3 &
PID=$!

max=0

# Проверяем количество потоков
while kill -0 $PID 2> /dev/null; do
    # Считываем количество потоков, если файл /proc/$PID/status существует
    if [ -e /proc/$PID/status ]; then
        THREADS=$(grep Threads /proc/$PID/status | awk '{print $2}')
        
        # Выводим информацию, если количество потоков больше максимального
        if [ $THREADS -gt $max ]; then
            max=$THREADS
        fi
    else
        break
    fi
    
    # Ждём
    sleep 1
done

echo $(($max - 1))
