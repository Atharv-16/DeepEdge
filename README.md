# Image Resizing with SIMD and OpenMP

This project demonstrates image resizing using custom implementations of nearest neighbor, bilinear, and bicubic interpolation. It also compares the performance with OpenCV's resizing functions. The project utilizes SIMD (AVX2) and OpenMP for parallel processing.

## Prerequisites

- CMake 3.10 or higher
- OpenCV 4.x

## Building the Project

1. Clone the repository:
    ```sh
    git clone https://github.com/Atharv-16/DeepEdge.git
    cd DeepEdge
    ```

2. Install the required dependencies:
    - **Ubuntu**:
      ```sh
      sudo apt update
      sudo apt install -y cmake g++ libopencv-dev
      ```

    - **Windows**:
      1. Install [CMake](https://cmake.org/download/).
      2. Install [OpenCV](https://opencv.org/releases/) and set up OpenCV environment variables as described in the OpenCV installation guide.

    - **macOS**:
      ```sh
      brew install cmake opencv
      ```

3. Remove the existing `build` directory if it exists:
    ```sh
    rm -rf build
    ```

4. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

5. Configure and build the project:
    ```sh
    cmake ..
    make
    ```

## Running the Application

Ensure you have the input image `G178_2-1080.BMP` in the project root directory and build.

```sh
./ImageResizeSIMD
