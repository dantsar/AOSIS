# Advanced Operating Systems Independent Study (AOSIS)

This is the repo for the work done from my Independent Study. The goal is to create an Operating System that is heavily based Linux and handles:
* Interrupt Handling
* Memory Management
* Process Scheduling
* Userspace programs
* LibC

---
## Building the Project
This project is meant to be built under Linux with a cross compiler. The projects can be built with the local tools of the system or with docker

### Build Requirements
To build the project without the use of docker, the following tools are required:
* Make
* grub-mkrescue
* xorriso
* nasm
* i686-gcc-elf
* bash

## Building with docker
To build the project and produce an .iso in `build`, simply run:
```bash
make docker
```
The docker image will be pulled, and the project will be built. 

## Running AOSIS
The system requirments to run the project is ```qemu-system-i386``` and can be run with ```make run```

## Resources
Here is a small list of resources that were used throughout the project:
* [OSDev Wiki](https://wiki.osdev.org/Main_Page/) : this is an invaluable resource and the barebones and meaty skeleton examples were extremely helpful
* [MonkOS](https://github.com/beevik/MonkOS): the inspiration and basis for the docker build system
* [The World of Protected Mode](https://web.archive.org/web/20190424213806/http://www.osdever.net/tutorials/view/the-world-of-protected-mode) : referenced to load the GDT
* [Bran's Kernel Implementation](http://www.osdever.net/bkerndev/Docs/intro.htm): an excelent resource that was reference for setting up the GDT and IDT 

