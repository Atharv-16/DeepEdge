#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread> // Include for multithreading

using namespace cv;
using namespace std;
using namespace std::chrono;

// Custom implementation of Nearest Neighbor interpolation
void resizeNearestNeighbor(const Mat& src, Mat& dst) {
    double x_ratio = static_cast<double>(src.cols) / dst.cols;
    double y_ratio = static_cast<double>(src.rows) / dst.rows;

    for (int y = 0; y < dst.rows; ++y) {
        for (int x = 0; x < dst.cols; ++x) {
            int srcX = static_cast<int>(x * x_ratio);
            int srcY = static_cast<int>(y * y_ratio);

            dst.at<Vec3b>(y, x) = src.at<Vec3b>(srcY, srcX);
        }
    }
}

// Custom implementation of Bilinear interpolation
void resizeBilinear(const Mat& src, Mat& dst) {
    double x_ratio = static_cast<double>(src.cols - 1) / dst.cols;
    double y_ratio = static_cast<double>(src.rows - 1) / dst.rows;

    for (int y = 0; y < dst.rows; ++y) {
        for (int x = 0; x < dst.cols; ++x) {
            double gx = x * x_ratio;
            double gy = y * y_ratio;

            int gxi = static_cast<int>(gx);
            int gyi = static_cast<int>(gy);

            double tx = gx - gxi;
            double ty = gy - gyi;

            Vec3b p1 = src.at<Vec3b>(gyi, gxi);
            Vec3b p2 = src.at<Vec3b>(gyi, gxi + 1);
            Vec3b p3 = src.at<Vec3b>(gyi + 1, gxi);
            Vec3b p4 = src.at<Vec3b>(gyi + 1, gxi + 1);

            for (int c = 0; c < 3; ++c) {
                double pixel = (1 - tx) * (1 - ty) * p1[c] +
                               tx * (1 - ty) * p2[c] +
                               (1 - tx) * ty * p3[c] +
                               tx * ty * p4[c];
                dst.at<Vec3b>(y, x)[c] = static_cast<uchar>(pixel);
            }
        }
    }
}

// Custom implementation of Bicubic interpolation
double cubicInterpolate(double p[4], double x) {
    return p[1] + 0.5 * x * (p[2] - p[0] + x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] + x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}
// Custom implementation of Bicubic interpolation for each color channel with boundary handling
void resizeBicubic(const Mat& src, Mat& dst) {
    double x_ratio = static_cast<double>(src.cols - 1) / dst.cols; // Use src.cols - 1 for proper access
    double y_ratio = static_cast<double>(src.rows - 1) / dst.rows;

    for (int y = 0; y < dst.rows; ++y) {
        for (int x = 0; x < dst.cols; ++x) {
            double gx = x * x_ratio;
            double gy = y * y_ratio;

            // Handle edge cases to avoid accessing pixels outside the image boundary
            int gxi = floor(gx); // Use floor for nearest integer neighbor
            int gyi = floor(gy);
            gxi = max(0, min(src.cols - 2, gxi)); // Clamp gxi within valid range
            gyi = max(0, min(src.rows - 2, gyi)); // Clamp gyi within valid range

            double tx = gx - gxi;
            double ty = gy - gyi;

            // Print intermediate values for debugging (uncomment if needed)
            // cout << "gx: " << gx << ", gy: " << gy << ", tx: " << tx << ", ty: " << ty << endl;

            for (int c = 0; c < 3; ++c) {
                double p[4];
                for (int j = 0; j < 4; ++j) {
                    int yl = gyi + j;
                    yl = max(0, min(src.rows - 1, yl)); // Adjust range for potential edge case
                    for (int i = 0; i < 4; ++i) {
                        int xl = gxi + i;
                        xl = max(0, min(src.cols - 1, xl)); // Adjust range for potential edge case
                        p[i] = src.at<Vec3b>(yl, xl)[c];
                    }
                    double value = cubicInterpolate(p, tx);
                    p[j] = value;
                }
                double result = cubicInterpolate(p, ty);
                dst.at<Vec3b>(y, x)[c] = static_cast<uchar>(result);
            }
        }
    }
}


// Function to measure time taken for resizing with multi-threading
double measureTimeMultiThread(void (*func)(const Mat&, Mat&), const Mat& src, Mat& dst, int iterations, int numThreads) {
    vector<thread> threads(numThreads);
    auto start = high_resolution_clock::now();
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = thread(func, cref(src), ref(dst));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    auto end = high_resolution_clock::now();
    duration<double, milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    // Load the input image
    Mat image = imread("./G178_2 -1080.BMP");

    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return -1;
    }

    // Define output size for resizing
    Size newSize(image.cols / 2, image.rows / 2);

    // Variables to store resized images
    Mat resizedNearest(newSize, image.type());
    Mat resizedBilinear(newSize, image.type());
    Mat resizedBicubic(newSize, image.type());

    int iterations = 1000;
    int numThreads = 4; // Number of threads for parallel execution

    // Measure time for custom implementations with multi-threading
    double timeNearest = measureTimeMultiThread(resizeNearestNeighbor, image, resizedNearest, iterations, numThreads);
    cout << "Time taken for " << iterations << " iterations using Custom Nearest Neighbor (Multi-threaded): " << timeNearest << " ms" << endl;

    double timeBilinear = measureTimeMultiThread(resizeBilinear, image, resizedBilinear, iterations, numThreads);
    cout << "Time taken for " << iterations << " iterations using Custom Bilinear (Multi-threaded): " << timeBilinear << " ms" << endl;

    double timeBicubic = measureTimeMultiThread(resizeBicubic, image, resizedBicubic, iterations, numThreads);
    cout << "Time taken for " << iterations << " iterations using Custom Bicubic (Multi-threaded): " << timeBicubic << " ms" << endl;

    // Measure time for OpenCV implementations
    Mat resizedOpenCVNearest, resizedOpenCVBilinear, resizedOpenCVBicubic;
    auto startOpenCV = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        resize(image, resizedOpenCVNearest, newSize, 0, 0, INTER_NEAREST);
    }
    auto endOpenCV = high_resolution_clock::now();
    duration<double, milli> elapsedOpenCV = endOpenCV - startOpenCV;
    double timeOpenCVNearest = elapsedOpenCV.count();
    cout << "Time taken for " << iterations << " iterations using OpenCV Nearest Neighbor: " << timeOpenCVNearest << " ms" << endl;

    startOpenCV = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        resize(image, resizedOpenCVBilinear, newSize, 0, 0, INTER_LINEAR);
    }
    endOpenCV = high_resolution_clock::now();
    elapsedOpenCV = endOpenCV - startOpenCV;
    double timeOpenCVBilinear = elapsedOpenCV.count();
    cout << "Time taken for " << iterations << " iterations using OpenCV Bilinear: " << timeOpenCVBilinear << " ms" << endl;

    startOpenCV = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        resize(image, resizedOpenCVBicubic, newSize, 0, 0, INTER_CUBIC);
    }
    endOpenCV = high_resolution_clock::now();
    elapsedOpenCV = endOpenCV - startOpenCV;
    double timeOpenCVBicubic = elapsedOpenCV.count();
    cout << "Time taken for " << iterations << " iterations using OpenCV Bicubic: " << timeOpenCVBicubic << " ms" << endl;

    // Display resized images (for verification purposes)
    imshow("Resized Nearest", resizedNearest);
    imshow("Resized Bilinear", resizedBilinear);
    imshow("Resized Bicubic", resizedBicubic);
    imshow("Resized OpenCV Nearest", resizedOpenCVNearest);
    imshow("Resized OpenCV Bilinear", resizedOpenCVBilinear);
    imshow("Resized OpenCV Bicubic", resizedOpenCVBicubic);
    waitKey(0);

    return 0;
}
