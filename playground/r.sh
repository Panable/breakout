#!/usr/bin/env sh
set -xe

make -j$(nproc) $1 && build/./$1
