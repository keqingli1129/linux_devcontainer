#include <iostream>

#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap;
    cap.open(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return 1;
    }

    // Set a common resolution and format
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    // Warm up: discard initial frames
    cv::Mat frame;
    for (int i = 0; i < 10; i++) {
        cap.read(frame);
    }

    const cv::Scalar lowerBoundary(15, 200, 90);
    const cv::Scalar upperBoundary(25, 255, 155);
    const cv::Point samplePoint(300, 300);
    cv::Mat mask;
    std::cout << "Camera opened. Press 'q' to quit." << std::endl;

    int emptyCount = 0;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            emptyCount++;
            std::cerr << "Warning: Empty frame (" << emptyCount << ")" << std::endl;
            if (emptyCount > 30) {
                std::cerr << "Too many empty frames, exiting." << std::endl;
                break;
            }
            continue;
        }
        emptyCount = 0;

        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
        cv::inRange(hsv, lowerBoundary, upperBoundary, mask);
        if (mask.at<uchar>(samplePoint) == 255) {
            cv::putText(frame, "Yellow", samplePoint + cv::Point(0, 40), 1, 1, cv::Scalar(0, 255, 255));
        }
        cv::circle(frame, samplePoint, 5, cv::Scalar(0), cv::FILLED);
        cv::imshow("Camera", frame);
        //cv::imshow("HSV", hsv);
        //cv::imshow("Mask", mask);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
