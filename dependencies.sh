#!/bin/bash

# Update package lists
sudo apt update

# Install C++ development tools
sudo apt install g++ cmake

# Install Matplot++ system-wide
sudo make install

# Install JPEG and TIFF libraries
sudo apt install libjpeg-dev libtiff5-dev

# Install BLAS and LAPACK libraries
sudo apt install libblas-dev liblapack-dev

# Install FFTW library
sudo apt install libfftw3-dev

# Clone the Matplot++ repository
git clone https://github.com/alandefreitas/matplotplusplus.git

# Navigate to the Matplot++ directory
cd matplotplusplus

# Create a build directory
mkdir build

# Enter the build directory
cd build

# Configure and build Matplot++
cmake .. -DCMAKE_CXX_FLAGS="-O2" -DMATPLOTPP_BUILD_EXAMPLES=OFF -DMATPLOTPP_BUILD_TESTS=OFF
sudo cmake --build .
sudo cmake --install .

# Install Matplot++ system-wide
sudo make install
