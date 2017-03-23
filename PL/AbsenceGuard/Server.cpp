#include "Server.hpp"
#include <stdlib.h>

int Segmentation::frame = 0;

Segmentation::Segmentation()
{
	this->subImageCount = 0;
	this->result_imgs = nullptr;
	
}

Segmentation::Segmentation(cv::Mat src) :src(src)
{
	this->subImageCount = 0;
	this->Ori = this->src.clone();
	this->result_imgs = nullptr;
}

Segmentation::~Segmentation()
{
	/*
	for (int i = 0; i < subImageCount; i++)
	{
		result_imgs[i].release();
	}
	*/
	for (int i = 0; i < contours.size(); i++)
	{

		delete point[i];
	}
	delete point;
	/*
	if (result_imgs != nullptr)
	{
		delete result_imgs;
	}
	*/
	//this->contours.clear();

	
}

void Segmentation::initialize()
{
	/*
	for (int i = 0; i < subImageCount; i++)
	{
		result_imgs[i].release();
	}
	*/

	for (int i = 0; i < contours.size(); i++)
	{

		delete point[i];
	}
	delete point;
	/*
	if (result_imgs != nullptr)
	{
		delete result_imgs;
	}
	*/
	//this->contours.clear();
}

cv::Mat Segmentation::getImage()
{
	return this->src.clone();
}

void Segmentation::setImage(cv::Mat src)
{
	this->Ori = src.clone();
	this->src = src;
}

void Segmentation::imageHis()
{
	cv::equalizeHist(this->src, this->src);
}

void Segmentation::imageBlur(int flag, int kernel, float sigma)
{
	if (flag & Segmentation::GAUSSIAN)
	{
		cv::GaussianBlur(this->src, this->src, cv::Size(kernel, kernel), sigma);
	}
	if (flag & Segmentation::MEDIAN)
	{
		cv::medianBlur(this->src, this->src, kernel);
	}
}

void Segmentation::imageSharpening(int flag)
{
	cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1);

	cv::Mat imglaplacian;
	cv::Mat sharp = this->src; // copy source image to another temporary one
	cv::filter2D(sharp, imglaplacian, CV_32F, kernel);

	this->src.convertTo(sharp, CV_32F);
	this->src = sharp - imglaplacian;
	this->src.convertTo(this->src, CV_8UC3);
}

void Segmentation::watershed()
{
	//contours.clear();
	//get binary image
	cv::Mat bw;
	cv::cvtColor(this->src, bw, CV_BGR2GRAY);
	cv::threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	// perform the distance transform algorithm	
	cv::Mat dist;
	distanceTransform(bw, dist, CV_DIST_L2, 3);

	// normalize the distance image for range = {0.0, 1.0}
	// so we can visualize and threshold it
	cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);

	// threshold to obtain the peaks
	// this will be the markers for the foreground objects
	cv::threshold(dist, dist, .4, 1., CV_THRESH_BINARY); 

	// dilate a bit the dist image
	cv::Mat kernel = cv::Mat::ones(3, 3, CV_8UC1);
	cv::dilate(dist, dist, kernel);

	// create the cv_8u version of the distance image
	// it is needed for findcontours()
	cv::Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);

	// find total markers
	
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// create the marker image for the watershed algorithm
	markers = cv::Mat::zeros(dist.size(), CV_32SC1);
	// draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers, contours, static_cast<int>(i), cv::Scalar::all(static_cast<int>(i) + 1), -1);

	circle(markers, cv::Point(5, 5), 3, CV_RGB(255, 255, 255), -1);

	cv::watershed(this->src, markers);
	cv::Mat mark = cv::Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);
}

void Segmentation::imagePartition()
{	
	Segmentation::addFrame();
	std::cout << "Frame : " << Segmentation::getFrame() << std::endl;
	result_imgs = new cv::Mat[contours.size()];
	point = new cv::Point*[contours.size()];
	for (int i = 0; i < contours.size(); i++)
	{
		point[i] = new cv::Point[2];
		point[i][0] = cv::Point(0, 0);	// MAX
		point[i][1] = cv::Point(this->src.rows, this->src.cols); // MIN
	}

	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
			{
				if (point[index - 1][0].y < i)
				{
					point[index - 1][0].y = i;
				}
				if (point[index - 1][0].x < j)
				{
					point[index - 1][0].x = j;
				}
				if (point[index - 1][1].y > i)
				{
					point[index - 1][1].y = i;
				}
				if (point[index - 1][1].x > j)
				{
					point[index - 1][1].x = j;
				}
			}
		}
	}
	for (int k = 0; k < contours.size(); k++)
	{
		std::cout << k << std::endl;
		std::cout << "p1: x = " << point[k][0].x << "\ty = " << point[k][0].y << std::endl;
		std::cout << "p2: x = " << point[k][1].x << "\ty = " << point[k][1].y << std::endl;
		int x = point[k][0].x - point[k][1].x;
		int y = point[k][0].y - point[k][1].y;
		if (x == 0 || y == 0)
		{
			continue;
		}
		else
		{
			std::cout << "x = " << x << "\ty = " << y << std::endl;
			result_imgs[k] = this->Ori(cv::Rect(point[k][0], point[k][1])).clone();
			this->subImageCount++;
		}
	}
}

cv::Mat Segmentation::getSegmentationMap()
{
	std::vector<cv::Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int b = cv::theRNG().uniform(0, 255);
		int g = cv::theRNG().uniform(0, 255);
		int r = cv::theRNG().uniform(0, 255);
		colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// create the result image
	cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);

	// fill labeled objects with random colors
	
	
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
				dst.at<cv::Vec3b>(i, j) = colors[index - 1];
			else
				Ori.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
		}
	}
	return dst.clone();


	
}

cv::Mat * Segmentation::getPartitionedImage()
{
	return result_imgs;
}

void Segmentation::findSift(cv::Mat img1, cv::Mat img2)
{
	if (!img1.data || !img2.data)
	{
		printf("sift error\n");
		return;
	}

	int minHessian = 400;

	//cv::FeatureDetector detector(minHessian);
	//cv::SurfFeatureDetector detector(minHessian);
	cv::FeatureDetector detector;

	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	
	detector.detect(img1, keypoints_1);
	detector.detect(img2, keypoints_2);

	//cv::SurfDescriptorExtractor extractor;
	cv::DescriptorExtractor extractor;

	cv::Mat descriptors_1, descriptors_2;

	extractor.compute(img1, keypoints_1, descriptors_1);
	extractor.compute(img2, keypoints_2, descriptors_2);

	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	std::vector< cv::DMatch > good_matches;

	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= std::max(2 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}
	cv::Mat img_matches;
	drawMatches(img1, keypoints_1, img2, keypoints_2, good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
		std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Show detected matches
	imshow("Good Matches", img_matches);
	imwrite("C:\\Users\\Jp\\Desktop\\Test\\sub3\\match_result2.jpg", img_matches);

	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}
}
