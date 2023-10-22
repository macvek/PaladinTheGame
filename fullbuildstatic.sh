#!/bin/sh
rm application*.out
gcc game.c -D WITHDEBUG -std=c11 -static -static-libgcc -Wall -o application-debug.out
gcc game.c -Os -std=c11 -static -static-libgcc -Wall -o application-release.out
