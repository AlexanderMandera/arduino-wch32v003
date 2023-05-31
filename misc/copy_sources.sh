#!/usr/bin/env bash

current_dir=$(dirname -- "$( readlink -f -- "$0"; )")
cp -r $current_dir/../ch32v003fun/ch32v003fun $current_dir/../cores/arduino