# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

target_include_directories(app PRIVATE inc/)
target_sources(
    app PRIVATE 
    src/main.c 
    src/sound_gen.c 
    src/music.c 
    src/instruments.c 
    src/song_god_rest_ye.c 
    src/song_holy_night.c
    src/song_xmas.c
    )
