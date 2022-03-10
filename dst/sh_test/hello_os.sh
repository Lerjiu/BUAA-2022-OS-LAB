#!/bin/bash
touch hello_os.c
sed -n '8p' file >> hello_os.c
sed -n '32p' file >> hello_os.c
sed -n '128p' file >> hello_os.c
sed -n '512p' file >> hello_os.c
sed -n '1024p' file >> hello_os.c




