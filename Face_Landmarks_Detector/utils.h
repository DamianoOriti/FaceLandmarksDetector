#pragma once

#include <opencv2/opencv.hpp>

const cv::Scalar c_line_color = cv::Scalar(255, 200, 0);

void draw_polylines(cv::Mat &image, const std::vector<cv::Point2f> &landmarks,
	const int start, const int end, bool is_closed = false);

void draw_landmarks_as_polylines(cv::Mat &image, const std::vector<cv::Point2f> &landmarks);

void draw_landmarks_as_circles(cv::Mat &image, const std::vector<cv::Point2f> &landmarks);

void export_data(const std::vector<cv::Point2f> &landmarks, int frame_number, int face_number, const char* export_path);

void split_main_arg(char* main_arg, const char** first, const char** second);
