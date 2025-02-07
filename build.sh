#!/bin/bash

if [ ! -d arch/ ] || [ ! -d common/ ] || [ ! -d userspace/ ]; then
  echo "current working directory does not look like sweb root, aborting..."
  exit 1
fi

if [ "$1" = "clean" ]; then
  echo "removing /tmp/sweb"
  rm -r /tmp/sweb
  exit 0
fi

OS_PATH="$(realpath .)"
mkdir -p /tmp/sweb && cd /tmp/sweb
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 "$OS_PATH" || exit 1
make -j|| exit 1
make qemu
