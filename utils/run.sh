#!/bin/bash

gcc first.c -o first -lm
./first
rm first

gcc second.c -o second -lm
./second
rm second