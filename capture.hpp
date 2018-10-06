//
// Created by kawa on 10/6/18.
//
#ifndef CAPTURE_H
#define CAPTURE_H

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

class Capture{

private:

    //Buffer関係
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;

    rs2::frame color_frame;
    rs2::frame depth_frame;
    rs2::colorizer color_map;

    cv::Mat color_mat_;
    cv::Mat depth_mat_;
    int width_ = 640;
    int height_ = 480;
    int fps_ = 30;

public:

    Capture();
    ~Capture();
    inline void startSenser();
    void run();
    void updateFrame();
    void draw();
    void show();

};

#endif //CAPTURE_H
