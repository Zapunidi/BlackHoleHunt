#Download base image ubuntu 18.04
FROM ubuntu:20.04 as base

# LABEL about the custom image
LABEL maintainer="zapunidy@gmail.com"
LABEL version="0.1"
LABEL description="raylib for web builder"

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive

### Stage 1 - add packages download dependencies ###

# Update Ubuntu Software repository
RUN apt update && \
	apt install -y python3 git

# Download raylib
RUN git clone https://github.com/raysan5/raylib.git
# Download emscripten
RUN git clone https://github.com/emscripten-core/emsdk.git

### Stage 3 --- copy everything inside and run ###

### Stage 2 --- copy everything inside and run ###

COPY . .
CMD ["bash",""]
