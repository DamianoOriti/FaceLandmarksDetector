#include "utils.h"
#include <fstream>

void draw_polylines(cv::Mat &image, const std::vector<cv::Point2f> &landmarks,
	const int start, const int end, bool is_closed)
{
	// Gather all points between the start and end indices
	std::vector <cv::Point2f> points;
	for (int i = start; i <= end; i++)
	{
		points.push_back(cv::Point2f(landmarks[i].x, landmarks[i].y));
	}
	// Draw polylines.
	cv::polylines(image, points, is_closed, c_line_color, 2, 16);
}

void draw_landmarks_as_polylines(cv::Mat &image, const std::vector<cv::Point2f> &landmarks)
{
	// Draw face for the 68-point model.
	if (landmarks.size() == 68)
	{
		draw_polylines(image, landmarks, 0, 16);           // Jaw line
		draw_polylines(image, landmarks, 17, 21);          // Left eyebrow
		draw_polylines(image, landmarks, 22, 26);          // Right eyebrow
		draw_polylines(image, landmarks, 27, 30);          // Nose bridge
		draw_polylines(image, landmarks, 30, 35, true);    // Lower nose
		draw_polylines(image, landmarks, 36, 41, true);    // Left eye
		draw_polylines(image, landmarks, 42, 47, true);    // Right Eye
		draw_polylines(image, landmarks, 48, 59, true);    // Outer lip
		draw_polylines(image, landmarks, 60, 67, true);    // Inner lip
	}
}

void draw_landmarks_as_circles(cv::Mat &image, const std::vector<cv::Point2f> &landmarks)
{
	// Draw face for the 68-point model.
	if (landmarks.size() == 68)
	{
		for (size_t i = 0; i < 68; i++)
		{
			cv::circle(image, landmarks[i], 1.0f, c_line_color);
		}
	}
}

void export_data(const std::vector<cv::Point2f>& landmarks, int frame_number, int face_number, const char* export_path)
{
	char frame_number_c_str[128];
	_itoa_s(frame_number, frame_number_c_str, 10);

	std::string file_path = std::string(export_path) + "\\data_" + std::string(frame_number_c_str);

	std::ofstream data_file(file_path);
	for (size_t i = 0; i < landmarks.size(); i++)
	{
		data_file << landmarks[i].x << " " << landmarks[i].y << std::endl;
	}
	data_file.close();
}

void split_main_arg(char* main_arg, const char** first, const char** second)
{
	*first = main_arg;
	*second = nullptr;

	int length = strlen(main_arg);

	int i = 0;
	while (i < length && main_arg[i] != ':')
	{
		i++;
	}

	if (i == length)
	{
		throw std::exception("Error: invalid argument!");
	}

	main_arg[i] = '\0';

	*second = &main_arg[++i];
}
