# Use the latest Ubuntu as the base image
FROM ubuntu:latest AS devconfig

# Set environment variables
ENV VCPKG_ROOT=/vcpkg
ENV PATH="$PATH:$VCPKG_ROOT"

# Install dependencies and tools
RUN apt-get update && \
    apt-get install -y \
    bash \
    build-essential \
    cmake \
    curl \
    entr \
    git \
    g++ \
    cmake \
    make \
    ninja-build \
    pkg-config \
    sudo \
    unzip \
    zip \
    python3 \
    autoconf \
    automake \
    autoconf-archive \
    libtool \
    perl 
    #apt-get clean && rm -rf /var/lib/apt/lists/*


    # Install vcpkg
    RUN git clone https://github.com/microsoft/vcpkg.git $VCPKG_ROOT && \
    chmod +x $VCPKG_ROOT/bootstrap-vcpkg.sh && \
    cd $VCPKG_ROOT && \
    ./bootstrap-vcpkg.sh

# Set the working directory
WORKDIR /app

# Copy the CMakeLists.txt and source files into the container
COPY . .

# List files in the /app directory
RUN ls -la /app

# Install dependencies via vcpkg
RUN vcpkg install curl
RUN vcpkg install nlohmann-json
RUN vcpkg install asio
RUN vcpkg install boost

# Build the project
RUN mkdir build && cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake && make

EXPOSE 4729
# Run the application
CMD ["./build/ECHOES_BACKEND"]
