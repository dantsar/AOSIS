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
To build the project and produce a .iso in `build`, simply run:
```
make docker
```
The docker image will be pulled, and the project will be built. 

## Resources
Here is a small list of resources that were used throughout the project:
* [OSDev Wiki](https://wiki.osdev.org/Main_Page/) : this is an invaluable resource and the barebones and meaty skeleton examples were extremely helpful
* [MonkOS](https://github.com/beevik/MonkOS)
