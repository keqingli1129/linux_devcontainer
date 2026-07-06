#include<iostream>
#include "Chapter01/Lib/lib.h"
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include "Chapter01/Lib/shared.h"
#include <torch/torch.h>

using namespace std;
int main() {
    cout << "Hello, World!" << endl;
    // int32_t a = 10;
    // auto b = 20.0F;
    // printer(a);
    // const auto c = 10;
    // constexpr auto e = 10;
    // constexpr auto len = 5U;
    // uint32_t myarray[len] = {0};
    // Eigen sample: 2x2 matrix multiplication
    // Eigen::Matrix2d mat;
    // mat << 1, 2,
    //        3, 4;
    // Eigen::Vector2d vec(5, 6);
    // Eigen::Vector2d result = mat * vec;
    // cout << "Matrix:\n" << mat << endl;
    // cout << "Vector: " << vec.transpose() << endl;
    // cout << "Result: " << result.transpose() << endl;
    // shared_demo::f1();

    // LibTorch sample: tensor operations
    // cout << "\n--- LibTorch Test ---" << endl;
    // auto tensor_a = torch::ones({2, 3});
    // cout << "ones(2,3):\n" << tensor_a << endl;

    // auto tensor_b = torch::randn({2, 3});
    // cout << "randn(2,3):\n" << tensor_b << endl;

    // auto tensor_sum = tensor_a + tensor_b;
    // cout << "sum:\n" << tensor_sum << endl;

    // Matrix multiply: (2x3) @ (3x2) -> (2x2)
    // auto tensor_mm = torch::mm(tensor_a, tensor_b.t());
    // cout << "matmul:\n" << tensor_mm << endl;

    // Check CUDA availability
    cout << "CUDA available: " << (torch::cuda::is_available() ? "yes" : "no") << endl;
    cout << "--- LibTorch OK ---\n" << endl;

    // OpenCV: open camera and display feed
    cv::VideoCapture cap;
    cap.open(0, cv::CAP_V4L2);
    // cap.open(0);
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
    cv::Scalar lowerBoundary(15, 200, 90);
    cv::Scalar upperBoundary(25, 255, 155);
    cv::Mat mask;
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
        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, lowerBoundary, upperBoundary, mask);
        if (mask.at<uchar>(cv::Point(300, 300)) == 255) {
            cv::putText(frame, "Yellow", cv::Point(300, 340), 1, 1, cv::Scalar(0,255,255));
        }
        cv::imshow("Camera", frame);
        cv::imshow("HSV", hsv);
        circle(frame, cv::Point(300, 300), 5, cv::Scalar(0), cv::FILLED);
        cv::imshow("Mask", mask);
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
    // int x{7};
    // string str{"Let us begin"};
    // cout << "x: " << x << ", str: " << str << endl;
    // int x1 = 7.7;
    // //int y{7.7}; //ilegal narrowing conversion from 'double' to 'int' [-Wnarrowing]
    // vector<int> vec1{1, 2, 3, 4, 5};

    // for (auto val = vec1.begin(); val != vec1.end(); val++) {
    //     cout << *val << " ";
    // }
    // cout << endl;
    return 0;
}
