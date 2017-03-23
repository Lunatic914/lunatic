#pragma once
#include <opencv/cxcore.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <cstdlib>


class Segmentation
{
private:
	cv::Mat Ori;
	cv::Mat src;
	cv::Mat* result_imgs;
	std::vector<std::vector<cv::Point> > contours;
	cv::Mat markers;
	cv::Point** point;
	
	int subImageCount;

public:
	Segmentation();
	Segmentation(cv::Mat src);	// set Mat
	~Segmentation();
	void initialize();
	cv::Mat getImage();	// return Mat
	void setImage(cv::Mat src);	// set Mat
	void imageHis();
	void imageBlur(int flag, int kernel = 3, float sigma = 1.0);
	void imageSharpening(int flag);
	void watershed();
	void imagePartition();
	cv::Mat getSegmentationMap();
	cv::Mat* getPartitionedImage();

	//flag
	static const int GAUSSIAN = 1;
	static const int MEDIAN = 2;

	//frame
	static int frame;
	static int getFrame() { return Segmentation::frame; }
	static void addFrame() { Segmentation::frame++; }

	//sift
	static void findSift(cv::Mat img1, cv::Mat img2);
};
