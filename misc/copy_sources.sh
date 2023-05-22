#!/usr/bin/env bash

current_dir=$(dirname -- "$( readlink -f -- "$0"; )")
echo $current_dir

cp -ar $current_dir/../ch32v003fun/ch32v003fun $current_dir/../cores/arduino/ch32v003fun