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

class ObjTrack
{
private:
	cv::Mat img;			// 사용할 영상
	cv::Mat ori;			// 원본 영상
	int threshold = 100;	// 트레숄드 값

	std::vector<std::vector<cv::Point>> contours;	// 윤곽선
	std::vector<cv::Vec4i> hierarchy;				// 하르라키

	std::vector<std::vector<cv::Point>> contours_poly;	// 오브젝트 폴리곤
	std::vector<cv::Rect> boundRect;					// 오브젝트 사각형

public:
	//생성자
	ObjTrack::ObjTrack();	// 기본 생성자
	ObjTrack::ObjTrack(cv::Mat src, int threshold = 100);	// Mat를 입력으로 받는 생성자

	// 변수 설정
	void ObjTrack::setImg(cv::Mat src);			// 영상 설정
	void ObjTrack::setThreshold(int threshold);	// 트레숄드(임계값) 설정

	// 변수 반환
	cv::Mat ObjTrack::getImg();			// 현재 영상 반환
	cv::Mat ObjTrack::getOriginalImg();	// 처리되지 않은 원본 영상 반환
	int ObjTrack::getThreshold();		// 트레숄드(임계값) 반환
	std::vector<cv::Rect> ObjTrack::getObject();	// 검출된 오브젝트 리스트 반환

	// 분할 함수
	void ObjTrack::blur(int kernel);	// 이미지 블러링
	void ObjTrack::colorHistogram();	// 컬러 영상 히스토그램 평활화 (밝기)
	void ObjTrack::sharp();	// 이미지 샤프닝 (에지 강조)
	void ObjTrack::findContours();		// 외곽선 검출
	void ObjTrack::findObject();			// 오브젝트의 위치를 검출

	// 그리기 함수
	cv::Mat ObjTrack::drawContours(cv::Mat src);	// 소스 영상에 외곽선 그리기
	cv::Mat ObjTrack::drawObject(cv::Mat src);		// 소스 영상에 오브젝트 위치 그리기
};
