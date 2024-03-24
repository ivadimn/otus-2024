gcc -Wall -Wextra -Wpedantic -std=c11 `pkg-config --cflags gtk4` main.c file_info.c common.c tv.c `pkg-config --libs gtk4` -o explore
