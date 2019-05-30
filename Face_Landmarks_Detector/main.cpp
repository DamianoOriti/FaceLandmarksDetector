#include <cstring>
#include <map>
#include "landmark_detector.h"
#include "utils.h"

static const cv::String c_window_name = "Face Landmarks";

enum Draw_Primitive
{
	Circle,
	Polyline,
};

int main(int argc, char** argv)
{
	// Parse program arguments
	std::map<std::string, std::string> args;
	for (int i = 1; i < argc; i++)
	{
		const char* arg_name;
		const char* arg_value;
		split_main_arg(argv[i], &arg_name, &arg_value);
		args.insert(std::pair<std::string, std::string>(arg_name, arg_value));
	}


	// Create face landmarks detector
	auto it = args.find("--face_model_path");
	if (it == args.end())
	{
		std::cout << "Error: missing face_model_path!" << std::endl;
		return -1;
	}
	const char* face_model_path = it->second.c_str();
	it = args.find("--landmarks_model_path");
	if (it == args.end())
	{
		std::cout << "Error: missing landmarks_model_path!" << std::endl;
		return -1;
	}
	const char* landmarks_model_path = it->second.c_str();
	std::cout << "Log: creating face landmarks detector" << std::endl;
	Landmark_Detector landmark_detector(face_model_path, landmarks_model_path);

	Draw_Primitive draw_primitive = Draw_Primitive::Circle;
	it = args.find("--draw_primitive");
	if (it != args.end())
	{
		if (it->second == "Circle")
		{
			draw_primitive = Draw_Primitive::Circle;
		}
		else if (it->second == "Polyline")
		{
			draw_primitive = Draw_Primitive::Polyline;
		}
	}

	const char* export_path = nullptr;
	it = args.find("--export_path");
	if (it != args.end())
	{
		export_path = it->second.c_str();
	}

	cv::namedWindow(c_window_name);

	cv::Mat image;

	it = args.find("--image");
	if (it != args.end())
	{
		std::cout << "reading image file" << std::endl;
		image = cv::imread(it->second, cv::IMREAD_UNCHANGED);

		std::vector<std::vector<cv::Point2f>> landmarks;
		if (landmark_detector.detect_landmarks(image, landmarks))
		{
			for (size_t face_number = 0; face_number < landmarks.size(); face_number++)
			{
				switch (draw_primitive)
				{
				case Circle:
					draw_landmarks_as_circles(image, landmarks[face_number]);
					break;
				case Polyline:
					draw_landmarks_as_polylines(image, landmarks[face_number]);
					break;
				default:
					break;
				}

				if (export_path != nullptr)
				{
					export_data(landmarks[face_number], 0, face_number, export_path);
				}
			}

			cv::imshow(c_window_name, image);

			cv::waitKey(0);
		}
	}

	it = args.find("--video");
	if (it != args.end())
	{
		std::cout << "reading video file" << std::endl;
		cv::VideoCapture cap(it->second);
		if (!cap.isOpened())
		{
			std::cout << "Error: could not open video file!" << std::endl;
			return -1;
		}

		double fps = cap.get(cv::CAP_PROP_FPS);

		it = args.find("--start_frame");
		int start_frame = it != args.end() ? atoi(it->second.c_str()) : 0;
		std::cout << "start frame set to " << start_frame << std::endl;
		it = args.find("--end_frame");
		int end_frame = it != args.end() ? atoi(it->second.c_str()) : std::numeric_limits<int>().max();
		std::cout << "end frame set to " << end_frame << std::endl;

		cap.set(cv::CAP_PROP_POS_FRAMES, start_frame);

		int frame_number = 0;
		while (cv::waitKey(1) != 27 && cap.read(image) && frame_number < end_frame)
		{
			std::cout << "processing frame " << frame_number << std::endl;

			std::vector<std::vector<cv::Point2f>> landmarks;
			if (landmark_detector.detect_landmarks(image, landmarks))
			{
				for (size_t face_number = 0; face_number < landmarks.size(); face_number++)
				{
					switch (draw_primitive)
					{
					case Circle:
						draw_landmarks_as_circles(image, landmarks[face_number]);
						break;
					case Polyline:
						draw_landmarks_as_polylines(image, landmarks[face_number]);
						break;
					default:
						break;
					}

					if (export_path != nullptr)
					{
						export_data(landmarks[face_number], frame_number, face_number, export_path);
					}
				}
			}

			cv::imshow(c_window_name, image);

			frame_number++;
		}

		cap.release();
	}

	cv::destroyWindow(c_window_name);

	return 0;
}
