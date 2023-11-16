# BlackHoleHunt
A chasing game, based on RayLib engine

# Web build

PLATFORM_WEB builds require emscripten SDK, recompiled raylib and environment variables to support integration. It is done via a docker container.
Libraries are cloned from github, then raylib is recompiled and its examples compiled for web to prove that it works.
Then examples can exported from container to host and be tested. 
To prepare image capable of building raylib examples run:
```
docker build --tag rlexamples --target examples .
```
To extract examples to host run a container
```
containerId=$(docker create rlexamples)
docker cp "$containerId":/root/raylib/examples .
docker rm "$containerId"
```
If examples compilation works and you checked that the work in browser, it is time to begin building of Black Hole Hunt.
```
docker build --tag emscripten --target build .
```
Then export the resulting code similarly
```
containerId=$(docker create emscripten)
docker cp "$containerId":/root/build .
docker rm "$containerId"
```

Alternatively you can create new container with console and explore your code inside
```
docker run -it --entrypoint /bin/bash emscripten
```