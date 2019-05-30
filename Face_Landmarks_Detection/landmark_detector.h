#pragma once

#include <opencv2\opencv.hpp>
#include <opencv2\face.hpp>

class Landmark_Detector
{
public:
	Landmark_Detector(const char* face_model_path, const char* landmarks_model_path);
	
	bool detect_landmarks(const cv::Mat& image, std::vector<std::vector<cv::Point2f>> &landmarks);

private:
	cv::CascadeClassifier face_detector;
	cv::Ptr<cv::face::Facemark> facemark;
};
