#include<iostream>
#include "Chapter01/Lib/lib.h"
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include "Chapter01/Lib/shared.h"

using namespace std;
int main() {
    cout << "Hello, World!" << endl;
    int32_t a = 10;
    auto b = 20.0F;
    printer(a);
    const auto c = 10;
    constexpr auto e = 10;
    constexpr auto len = 5U;
    uint32_t myarray[len] = {0};
    // Eigen sample: 2x2 matrix multiplication
    Eigen::Matrix2d mat;
    mat << 1, 2,
           3, 4;
    Eigen::Vector2d vec(5, 6);
    Eigen::Vector2d result = mat * vec;
    cout << "Matrix:\n" << mat << endl;
    cout << "Vector: " << vec.transpose() << endl;
    cout << "Result: " << result.transpose() << endl;
    shared_demo::f1();
    //OpenCV: open camera and display feed
    cv::VideoCapture cap;
    cap.open(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open camera" << endl;
        return 1;
    }

    // Set a common resolution and format
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));

    // Warm up: discard initial frames
    cv::Mat frame;
    for (int i = 0; i < 10; i++) {
        cap.read(frame);
    }

    cout << "Camera opened. Press 'q' to quit." << endl;

    int emptyCount = 0;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            emptyCount++;
            cerr << "Warning: Empty frame (" << emptyCount << ")" << endl;
            if (emptyCount > 30) {
                cerr << "Too many empty frames, exiting." << endl;
                break;
            }
            continue;
        }
        emptyCount = 0;
        cv::imshow("Camera", frame);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    // auto my_text4 = std::string{"Jan"};
    // std::cout << my_text4.size() << "\n";
    // my_text4.append(" Schaffranek");
    // std::cout << my_text4.size() << "\n";
    // auto empty_string = std::string{};
    // std::cout << empty_string.size() << "\n";
    // empty_string.append("Hello");
    // std::cout << empty_string.size() << "\n";
    // return 0;
}
