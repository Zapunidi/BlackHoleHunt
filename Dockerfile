# syntax=docker/dockerfile:1

#Download base image ubuntu 20.04
FROM ubuntu:20.04 as base

# LABEL about the custom image
LABEL maintainer="zapunidy@gmail.com" \
	version="0.7" \
	description="raylib for web builder"

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive

# Set working directory inside container not to spoil the filesystem root
WORKDIR /root

# Bash is required for source command
SHELL ["/bin/bash", "-c"]

### Stage 1 - add packages ###

# Update Ubuntu Software repository and install tools to download libraries (git), make emscripten work with python, and add Makefile support wit build-essential
RUN apt update && \
	apt install -y python3 git build-essential

### Stage 2 --- download and setup emscripten ###

FROM base as emsdk

# Download emscripten
RUN git clone https://github.com/emscripten-core/emsdk.git

RUN emsdk/emsdk install latest && \
	emsdk/emsdk activate latest

ENV EMSDK=/root/emsdk \
	EMSDK_NODE=/root/emsdk/node/16.20.0_64bit/bin/node \
	PATH="${PATH}:/root/emsdk:/root/emsdk/upstream/emscripten:/root/emsdk/node/16.20.0_64bit/bin"

### Stage 3 --- download and compile raylib ###

FROM emsdk as raylib

# Download raylib
RUN git clone https://github.com/Zapunidi/raylib.git

WORKDIR /root/raylib/src

RUN emcc -c rcore.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rshapes.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rtextures.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rtext.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c rmodels.c -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2 && \
	emcc -c utils.c -Os -Wall -DPLATFORM_WEB && \
	emcc -c raudio.c -Os -Wall -DPLATFORM_WEB

# Compile web support version of libraylib.a and place it in raylib/src folder, then remove object files
RUN emar rcs libraylib.a rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o && \
	rm *.o

### Stage 4 --- compile examples for web ###

FROM raylib as examples

WORKDIR /root/raylib/examples

RUN PLATFORM=PLATFORM_WEB make

### Stage 5 --- Copy our code in image to build it ###

FROM raylib as build

WORKDIR /root

COPY . .
RUN rm -rf build && \
	mkdir -p build
RUN emcc -o ./build/bhh.html main.cpp -Os -Wall ./raylib/src/libraylib.a -I. -I./include \
	-L. -L./raylib/src -s USE_GLFW=3 -s ASYNCIFY --shell-file ./shell1_micro.html -DPLATFORM_WEB \
	--preload-file resources/mini1111.xm@resources/mini1111.xm \
	--preload-file resources/icon.png@resources/icon.png \
	--preload-file resources/m13.png@resources/m13.png \
	--preload-file resources/maya.png@resources/maya.png \
	--preload-file resources/eat.wav@resources/eat.wav \
	--preload-file resources/shaders/glsl330/warp.fs@resources/shaders/glsl330/warp.fs

CMD ["echo", "Hello, run me differently"]
