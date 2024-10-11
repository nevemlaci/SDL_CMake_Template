### Windowson

Csak futtasd CMake CLI-ből, vagy valamilyen CMake supportal rendelkező
IDE(pl CLion) segítségével.

### Linuxon

#### APT:
```shell
sudo apt install libsdl2-dev
sudo apt install libsdl2-image-dev
sudo apt install libsdl2-ttf-dev
sudo apt install libsdl2-mixer-dev
```

#### Pacman:

```shell
sudo pacman -S sdl2
sudo pacman -S sdl2_image
sudo pacman -S sdl2_ttf
sudo pacman -S sdl2_mixer
```

### CMake CLI

```shell
git clone https://github.com/nevemlaci/SDL_CMake_Template.git
cd SDL_CMake_Template
cmake -S . -B build
cmake --build build
```
