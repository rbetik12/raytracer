# Cool raytracer
Repo description is pretty enough for understanding that project purpose. 👀 


## Features:
- ~~Single threaded~~ Multithreaded
- ~~Weird support for different output images resolution (I almost always get SIGFAULT)~~ Supports different rendering resolutions
- Hardcoded scene (4 spheres and 2 light objects)
- No input system (Cannot control raytracer without recompiling)

## Scene example(the only possible variant now, if you don't want to hardcode all scene description)
![Scene photo](https://i.imgur.com/dJ907o6.png)

## Build procedure
First, you need to install Cmake build system. It should be pretty straightforward for Linux users and, I suppose, MacOS users. Windows users...well, idk...never used cmake on windows.

```sh
git clone --recursive https://github.com/rbetik12/raytracer.git
cd raytracer
mkdir build
cd build
cmake ..
make
```

## Plans
![Cool meme about my repos](https://i.imgur.com/79LMNek.jpg)

First thing first, I need to "fix" all the features above, lmao. And I really want to try implementing this with OpenGL and shaders, cause realtime raytracing rules.

