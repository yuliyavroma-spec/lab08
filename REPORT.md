## Laboratory work VIII

Данная лабораторная работа посвещена изучению систем автоматизации развёртывания и управления приложениями на примере **Docker**

## Report

```sh
$ export GITHUB_USERNAME=<имя_пользователя>
```

```
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```

```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab07 lab08
$ cd lab08
$ git submodule update --init
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab08
```

```sh
$ cat > Dockerfile <<EOF
FROM ubuntu:18.04
EOF
```

```sh
$ cat >> Dockerfile <<EOF

RUN apt update
RUN apt install -yy gcc g++ cmake
EOF
```

```sh
$ cat >> Dockerfile <<EOF

COPY . print/
WORKDIR print
EOF
```

```sh
$ cat >> Dockerfile <<EOF

RUN cmake -H. -B_build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=_install
RUN cmake --build _build
RUN cmake --build _build --target install
EOF
```

```sh
$ cat >> Dockerfile <<EOF

ENV LOG_PATH /home/logs/log.txt
EOF
```

```sh
$ cat >> Dockerfile <<EOF

VOLUME /home/logs
EOF
```

```sh
$ cat >> Dockerfile <<EOF

WORKDIR _install/bin
EOF
```

```sh
$ cat >> Dockerfile <<EOF

ENTRYPOINT ./demo
EOF
```

```sh
$ docker build -t logger .
[+] Building 8.6s (14/14) FINISHED                               docker:default
 => [internal] load build definition from Dockerfile                       0.0s
 => => transferring dockerfile: 385B                                       0.0s
 => [internal] load metadata for docker.io/library/ubuntu:18.04            1.0s
 => [internal] load .dockerignore                                          0.0s
 => => transferring context: 2B                                            0.0s
 => [1/9] FROM docker.io/library/ubuntu:18.04@sha256:152dc042452c496007f0  0.1s
 => => resolve docker.io/library/ubuntu:18.04@sha256:152dc042452c496007f0  0.1s
 => [internal] load build context                                          0.1s
 => => transferring context: 73.54kB                                       0.1s
 => CACHED [2/9] RUN apt update                                            0.0s
 => CACHED [3/9] RUN apt install -yy gcc g++ cmake                         0.0s
 => [4/9] COPY . print/                                                    0.4s
 => [5/9] WORKDIR print                                                    0.1s
 => [6/9] RUN cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTAL  3.0s
 => [7/9] RUN cmake --build build                                          1.3s
 => [8/9] RUN cmake --build build --target install                         0.7s 
 => [9/9] WORKDIR /print/install/bin                                       0.1s 
 => exporting to image                                                     1.5s 
 => => exporting layers                                                    0.8s 
 => => exporting manifest sha256:4770efd34d5c0cc21a39c0a45ce911211bfc4014  0.0s 
 => => exporting config sha256:7b2e3eb926cf61ecc17b3f2393aa7dda11e885f0f4  0.0s
 => => exporting attestation manifest sha256:8e6fc572500321a8678359e8a187  0.1s
 => => exporting manifest list sha256:8f479933b41fc79985a020ae8dbfeacb951  0.0s
 => => naming to docker.io/library/logger:latest                           0.0s
 => => unpacking to docker.io/library/logger:latest                        0.5s

```

```sh
$ docker images
                                                           i Info →   U  In Use
IMAGE           ID             DISK USAGE   CONTENT SIZE   EXTRA
logger:latest   8f479933b41f        517MB          148MB       
```

```sh
$ mkdir logs
$ docker run -it -v "$(pwd)/logs/:/home/logs/" logger
text1
text2
text3
<C-D>
```

```sh
$ docker inspect logger
```

```sh
$ cat logs/log.txt
text1
text2
text3
```

```sh
$ gsed -i 's/lab07/lab08/g' README.md
```

```sh
$ vim .travis.yml
/lang<CR>o
services:
- docker<ESC>
jVGdo
script:
- docker build -t logger .<ESC>
:wq
```

```sh
$ git add Dockerfile CMakeLists.txt main.cpp README.md
$ git commit -m "Successful Docker build"
[master e4fd403] Successful Docker build
 3 files changed, 31 insertions(+), 19 deletions(-)
 create mode 100644 Dockerfile
$ git push origin master
```
```sh
$ mkdir -p .github/workflows
$ cat > .github/workflows/docker.yml <<'EOF'
> name: Docker Build

on:
  push:
    branches: [ master, main ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Build Docker image
        run: docker build -t logger .
      
      - name: Test container
        run: |
          mkdir -p logs
          echo -e "test1\ntest2\ntest3" | docker run -i -v $(pwd)/logs:/home/logs/ logger
          cat logs/log.txt
EOF
```
```sh
$ git add .github/workflows/docker.yml
$ git commit -m "Add GitHub Actions workflow"
[master 9a71e91] Add GitHub Actions workflow
 1 file changed, 20 insertions(+)
 create mode 100644 .github/workflows/docker.yml
$ git push origin master
```
