# CameraSimulator

This project simulates the behavior of an image sensor, including capturing light, adding noise, applying a color filter array (CFA), simulating optical diffraction, and demosaicing the image.

## Directory Structure

```
ImageSensorSimulation/
├── CMakeLists.txt
├── conanfile.txt
├── include/
│ └── ImageSensor.h
├── src/
│ └── ImageSensor.cpp
├── app/
│ └── main.cpp
└── build/ (created during the build process)
```

## Requirements

- CMake 3.10 or higher
- Conan package manager
- A C++ compiler (e.g., g++, clang++)

## Setup Instructions

### 1. Install Conan

Install Conan using pip:
```sh
pip install conan
```
If you do not already have a Conan profile configured, you can run:
```sh
conan profile detect --force
```

### 2. Build Dependencies

Create a `build` directory in the top-level directory. Change directory to this `build` directory and build the missing dependencies.

```
mkdir build
cd build
conan install --build missing ..
```

Configure your build environment by typing

```
cmake ..
```

Build the project with your preferred compiler

```
make
```
OR

```
ninja
```

# REQUIREMENTS #
* C++ compiler that supports C++17 dialect/ISO standard

# DEPENDENCIES #
* OpenCV 4
