# Image Resizing with OpenCV

This project demonstrates image resizing using OpenCV's `cv::resize` function and a custom resizing function. The project includes timing measurements for three interpolation methods: INTER_NEAREST, INTER_LINEAR, and INTER_CUBIC.

## Prerequisites

- CMake 3.10 or higher
- OpenCV 4.x

## Building the Project

1. Clone the repository:
    ```sh
    git clone https://github.com/Atharv-16/DeepEdge.git
    cd DeepEdge/assignment
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
Ensure you have the input image `G178_2-1080.BMP` in the `build` directory.

```sh
./ImageResizeSIMD
```

# Results

![time](https://github.com/Atharv-16/DeepEdge/assets/119600601/5b0d6956-30d2-4999-a090-4d08184c2189)
![cubic](https://github.com/Atharv-16/DeepEdge/assets/119600601/e95e13c3-7b62-458f-a68a-a46f42e0adc8)
