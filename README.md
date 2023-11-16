# BlackHoleHunt
A chasing game, based on RayLib engine

# Web build

PLATFORM_WEB builds require emscripten SDK, recompiled raylib and environment variables to support integration. It is done via a docker container.
Libraries are cloned from github, then raylib is recompiled and its examples compiled to prove that it works.
Then examples are exported from container to host. Everything is ready to begin building of Black Hole Hunt.

To prepare image capable of building raylib examples run:
```
docker build --tag emscripten .
```
Now you can create new container with console and explore your code inside
```
docker run -it --entrypoint /bin/bash emscripten
```