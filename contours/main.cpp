#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD 100
using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\5.jpg");
	Mat ori = src.clone();
	Mat src_gray;
	Mat histogram = src.clone();

	namedWindow("A", CV_WINDOW_AUTOSIZE);
	imshow("A", src);
	waitKey(0);

	GaussianBlur(src, src, Size(3,3), 1.5);
	medianBlur(src, src, 3);
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\2.firstblur.jpg", src);

	vector<Mat> channels;
	cvtColor(histogram, histogram, CV_BGR2YCrCb);

	split(histogram, channels);
	equalizeHist(channels[0], channels[0]);
	merge(channels, histogram);


	cvtColor(histogram, histogram, CV_YCrCb2BGR);

	src = histogram.clone();
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\3.hitogram.jpg", src);
	cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1);

	cv::Mat imglaplacian;
	cv::Mat sharp = src; // copy source image to another temporary one
	cv::filter2D(sharp, imglaplacian, CV_32F, kernel);

	src.convertTo(sharp, CV_32F);
	src = sharp - imglaplacian;
	src.convertTo(src, CV_8UC3);
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\4.Sharp.jpg", src);
	/// Convert image to gray and blur it
	blur(src, src, Size(15, 15));
	medianBlur(src, src, 5);
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\5.blur.jpg", src);
	cvtColor(src, src_gray, CV_BGR2GRAY);

	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\6.gray.jpg", src_gray);
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	int thresh = THRESHOLD;
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	/// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	Mat temp = ori.clone();
	drawContours(temp, contours, -1, Scalar(0, 0, 255), 3);
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\7.contours.jpg", temp);
	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	/// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);
		if ((boundRect[i].width <= 100) || (boundRect[i].height <= 100))
		{
			continue;
		}
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		rectangle(ori, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", ori);
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\8.drawing.jpg", ori);
	imwrite("C:\\Users\\luna9\\OneDrive\\문서\\Lab\\SampleImage\\9.map.jpg", drawing);
}
