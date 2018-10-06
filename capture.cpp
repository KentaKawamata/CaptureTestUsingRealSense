//
// Created by kawa on 9/30/18.
//
#include "capture.hpp"

Capture::Capture() {
    startSenser();
}

Capture::~Capture() {
    pipeline.stop();
}

void Capture::startSenser() {
    // デバイスを見つける
    rs2::context ctx;
    // Get a snapshot of currently connected devices
    auto list = ctx.query_devices();
    if (list.size() == 0)
        std::cout << "No device detected. Is it plugged in?" << std::endl;
    rs2::device dev = list.front();

    // Set Device Config
    rs2::config config;
    config.enable_stream(rs2_stream::RS2_STREAM_COLOR, width_, height_, rs2_format::RS2_FORMAT_BGR8, fps_);
    config.enable_stream(rs2_stream::RS2_STREAM_DEPTH, width_, height_, rs2_format::RS2_FORMAT_Z16, fps_);

    pipeline_profile = pipeline.start(config);
}

void Capture::updateFrame() {
    // センサーからframeを得る
    frameset = pipeline.wait_for_frames();

    color_frame = frameset.get_color_frame();
    depth_frame = frameset.get_depth_frame().apply_filter(color_map);

    // Retrive Frame Size
    width_ = color_frame.as<rs2::video_frame>().get_width();
    height_ = color_frame.as<rs2::video_frame>().get_height();
}

void Capture::draw() {

    color_mat_ = cv::Mat(height_, width_, CV_8UC3, const_cast<void *>(color_frame.get_data()));
    depth_mat_ = cv::Mat(height_, width_, CV_8UC3, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);
}

void Capture::show() {

    cv::imshow("color_test", color_mat_);
    cv::imshow("depth_test", depth_mat_);
}

void Capture::run() {

    while(true){

        updateFrame();
        draw();
        show();

        const int32_t key = cv::waitKey(20);
        if(key==27){
            cv::destroyAllWindows();
            break;
        }
    }
}
