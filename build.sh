#!/usr/bin/bash
gcc main.c $(pkg-config --libs --cflags ibus-1.0)