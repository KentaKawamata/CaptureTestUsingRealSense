//
// Created by kawa on 9/30/18.
//
#include <librealsense2/rs.hpp>
#include <librealsense2/rs_advanced_mode.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdint>

int main(int argc, char *argv[]) {

    cv::Mat color_mat;
    cv::Mat depth_mat;
    int height=480;
    int width=640;
    int fps=30;

    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;

    rs2::frameset frameset;
    // ポイントクラウドとテクスチャマッピングを計算するためのポイントクラウドオブジェクトの宣言
    rs2::pointcloud pc;
    // ポイントオブジェクトを永続化して、フレームがドロップしたときに最後のクラウドを表示できるようにします
    rs2::points points;
    rs2::colorizer color_map;
    rs2::frame color_frame;
    rs2::frame depth_frame;
    //rs2::frameset aligned_frames;
    //rs2::frameset frameset_filtered;

    // デバイスを見つける
    rs2::context ctx;
    auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
    if (list.size() == 0)
        std::cout << "No device detected. Is it plugged in?" << std::endl;
    rs2::device dev = list.front();

    // Set Device Config
    rs2::config config;
    config.enable_stream(rs2_stream::RS2_STREAM_COLOR, width, height, rs2_format::RS2_FORMAT_BGR8, fps);
    config.enable_stream(rs2_stream::RS2_STREAM_DEPTH, width, height, rs2_format::RS2_FORMAT_Z16, fps);

    pipeline_profile = pipeline.start(config);

    //while(!viewer->wasStopped()) {
    while(true) {

        // センサーからframeを得る
        frameset = pipeline.wait_for_frames();

        color_frame = frameset.get_color_frame();
        depth_frame = frameset.get_depth_frame().apply_filter(color_map);

        // Retrive Frame Size
        width = color_frame.as<rs2::video_frame>().get_width();
        height = color_frame.as<rs2::video_frame>().get_height();

        color_mat = cv::Mat(height, width, CV_8UC3, const_cast<void *>(color_frame.get_data()));
        depth_mat = cv::Mat(height, width, CV_8UC3, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);

        cv::imshow("color_test", color_mat);
        cv::imshow("depth_test", depth_mat);

        if (cv::waitKey(20) == 27) {
            cv::destroyAllWindows();
            break;
        }

    }
    pipeline.stop();

    return 0;
}

