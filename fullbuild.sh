#!/bin/sh
rm application*.out
gcc game.c -D WITHDEBUG -std=c11 -Wall -o application-debug.out
gcc game.c -Os -std=c11 -Wall -o application-release.out
