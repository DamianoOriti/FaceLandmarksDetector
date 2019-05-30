#include "landmark_detector.h"

Landmark_Detector::Landmark_Detector(const char* face_model_path, const char* landmarks_model_path) :
	face_detector(face_model_path)
{
	this->facemark = cv::face::FacemarkLBF::create();
	this->facemark->loadModel(landmarks_model_path);
}

bool Landmark_Detector::detect_landmarks(const cv::Mat& image, std::vector<std::vector<cv::Point2f>>& landmarks)
{
	std::vector<cv::Rect> faces;
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	this->face_detector.detectMultiScale(gray, faces);

	std::vector<cv::Rect> big_faces;
	for (auto it = faces.begin(); it != faces.end(); it++)
	{
		if (it->area() > (image.rows * image.cols / 8))
		{
			big_faces.push_back(*it);
		}
	}

	return this->facemark->fit(image, big_faces, landmarks);
}
