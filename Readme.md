### Windows

Run from CMake CLI, or any IDE/Code Editor that has CMake support(Visual Studio, CLion, Visual Studio Code). Dependencies are bundled/built from source.

### Linux

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

### Run from CMake CLI

```shell
git clone https://github.com/nevemlaci/SDL_CMake_Template.git
cd SDL_CMake_Template
cmake -S . -B build
cmake --build build
```
