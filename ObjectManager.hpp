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

class objectManager
{
public:
	std::vector<cv::Rect> object_list;	// 오브젝트 관리
	std::vector<bool> flags;

public:
	objectManager::objectManager();
	objectManager::objectManager(std::vector<cv::Rect> inputdata);
	void objectManager::setObject(std::vector<cv::Rect> inputdata);
	cv::Mat objectManager::drawObject(cv::Mat src);
	void objectManager::setObject(cv::Rect rect, int index, int flag);

	std::vector<cv::Rect>& objectManager::getObjectList();
	std::vector<bool>& objectManager::getFlagList();
};
