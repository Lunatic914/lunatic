#include <iostream>
#include <Windows.h>
#include "Server.hpp"
using namespace std;
using namespace cv;

int main()
{
	VideoCapture camera;
	camera.open(0);
	if (!camera.isOpened())
	{
		printf("Camera not found!\n");
		return -1;
	}
	namedWindow("Camera", CV_WINDOW_AUTOSIZE);
	namedWindow("Histo", CV_WINDOW_AUTOSIZE);
	namedWindow("Map", CV_WINDOW_AUTOSIZE);

	namedWindow("A", CV_WINDOW_AUTOSIZE);
	//namedWindow("B", CV_WINDOW_AUTOSIZE);
	//namedWindow("C", CV_WINDOW_AUTOSIZE);
	//namedWindow("D", CV_WINDOW_AUTOSIZE);
	//namedWindow("E", CV_WINDOW_AUTOSIZE);
	//namedWindow("F", CV_WINDOW_AUTOSIZE);
	//namedWindow("G", CV_WINDOW_AUTOSIZE);
	
	Segmentation img;
	
	
	while (true)
	{
		Mat frame;
		//camera >> frame;
		camera.read(frame);


		if (frame.empty())
		{
			continue;
		}
		cv::flip(frame, frame, 1);
		cv::medianBlur(frame, frame, 5);

		////////////////////////////////
//		Mat histogram = frame.clone();
	
	//	vector<Mat> channels;
	//	cvtColor(histogram, histogram, CV_BGR2YCrCb);
	//	split(histogram, channels);
	//	equalizeHist(channels[0], channels[0]);
	//	merge(channels, histogram);

	//	cvtColor(histogram, histogram, CV_YCrCb2BGR);

		//////////////////
		img.setImage(frame);
		//img.setImage(histogram);
		img.imageBlur(Segmentation::GAUSSIAN);
		img.imageSharpening(1);
		img.watershed();
		img.imagePartition();

		Mat* result = img.getPartitionedImage();

		if (result[0].empty())
			continue;
		resizeWindow("A", result[0].cols, result[0].rows);
		imshow("A", result[0]);

		Mat map = img.getSegmentationMap();

		map = (map * 3 / 4) + (frame / 4);
		imshow("Map", map);

		imshow("Camera", frame);

		//imshow("Histo", histogram);
		if (waitKey(30) != 255)
		{
			break;
		}
		frame.release();
		img.~Segmentation();
		//Sleep(1000);
	}

	return 0;
}
