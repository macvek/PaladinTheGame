#!/bin/sh
rm application*.out
gcc game.c  -D WITHDEBUG -std=c11 -Wall -o application-debug.out
if [ "$?" -eq 0 ] 
then 
    ./application-debug.out
else 
    echo "Compilation failed"
fi
