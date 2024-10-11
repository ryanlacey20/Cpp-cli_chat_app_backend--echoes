# Use Alpine as the base image

FROM alpine:edge AS devconfig
# Set environment variables
ENV VCPKG_ROOT=/vcpkg
ENV PATH="$PATH:$VCPKG_ROOT"

RUN apk add --no-cache cmake --repository=http://dl-cdn.alpinelinux.org/alpine/edge/main

#vcpkg needs at least cmake version 3.27.1, which is not yet in regular alpine  RUN apk add --no-cache cmake
RUN apk add linux-headers
RUN apk add perl


RUN apk update && \
    apk add --no-cache \
      bash \
      build-base \
      curl \
      git \
      g++ \ 
      cmake \
      make \
      ninja-build \
      pkgconfig \
      sudo \
      unzip \
      zip \
      python3 \
      unixodbc \
      unixodbc-dev \
      autoconf \
      automake \
      autoconf-archive \
      libtool

RUN ln -s /usr/lib/ninja-build/bin/ninja /usr/bin/ninja
# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git $VCPKG_ROOT && \
    chmod +x $VCPKG_ROOT/bootstrap-vcpkg.sh && \
    cd $VCPKG_ROOT && \
    ./bootstrap-vcpkg.sh


# Set the working directory
WORKDIR /app

# Copy the CMakeLists.txt and source files into the container
COPY . .

# List files in the /app dir
RUN ls -la /app

# Install dependencies via vcpkg
RUN vcpkg install unixodbc
RUN vcpkg install curl
RUN vcpkg install nlohmann-json
RUN vcpkg install asio
RUN vcpkg install boost



# Build  proj
RUN mkdir build && cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake && make

# run  app
CMD ["./build/ECHOES_BACKEND"]
