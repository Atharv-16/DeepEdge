# Image Resizing with OpenCV

This project demonstrates image resizing using OpenCV's `cv::resize` function and a custom resizing function. The project includes timing measurements for three interpolation methods: INTER_NEAREST, INTER_LINEAR, and INTER_CUBIC.

## Prerequisites

- CMake 3.10 or higher
- OpenCV 4.x

## Building the Project

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/ImageResizing.git
    cd ImageResizing
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Configure and build the project:
    ```sh
    cmake ..
    make
    ```

## Running the Application

Ensure you have the input image `G178_2-1080.BMP` in the `build` directory.

```sh
./main G178_2-1080.BMP
