#!/bin/bash

src_files=$(find src/ -name "*.cpp")

compile_on_linux () {
    allegro_addons="allegro-5 allegro_image-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5"
    g++ -iquote include/ ${src_files} -o iceberg $(pkg-config ${allegro_addons} --libs --cflags)
}

compile_on_mac () {
    allegro_addons="allegro-5 allegro_main-5 allegro_image-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5"
    g++ -iquote include/ ${src_files} -o iceberg $(pkg-config ${allegro_addons} --libs --cflags)
}

compile_on_windows () {
    allegro_addons="-lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_dialog"
    g++ -iquote include/ ${src_files} -o iceberg.exe ${allegro_addons}
}

if [ $# -eq 1 ]
then
    os=$1
    if [[ $os = "linux" || $os = "Linux" ]];
    then
        compile_on_linux
    elif [[ $os = "mac" || $os = "Mac" ]];
    then
        compile_on_mac
    elif [[ $os = "windows" || $os = "Windows" ]];
    then
        compile_on_windows
    else
        echo "Usage: ./setup.sh [linux|mac|windows]"
    fi
else
    echo "Usage: ./setup.sh [linux|mac|windows]"
fi
