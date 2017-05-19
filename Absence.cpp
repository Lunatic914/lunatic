/*
*	작성자 : Kim June-Pyo
*	날  짜 : 2017.05.02
*	설  명 : 도난방지 시스템
*   이메일 : luna914@naver.com
*/
#include <iostream>
#include <Windows.h>
#include "ObjTrack.hpp"
#include "ObjectManager.hpp"
#include "TrackManager.hpp"

double compareImg(cv::Mat img1, cv::Mat img2);
/*
int main()
{
	cv::VideoCapture camera;	// 장비의 카메라를 사용하기위한 객체 생성
	camera.open(0);				// 장비의 0번카메라 (랩탑의 경우 웹캠) 사용

	if (!camera.isOpened())		// 카메라가 정상적으로 열리지 않은경우 예외 처리
	{
		printf("Camera not found!\n");
		return -1;
	}
	cv::namedWindow("Camera", CV_WINDOW_AUTOSIZE);	// 카메라의 영상을 실시간으로 출력할 윈도우 생성
													//cv::namedWindow("Object", CV_WINDOW_AUTOSIZE);
	cv::Mat frame;			// 카메라에서 받아올 영상을 담을 변수
	cv::Mat back_frame;

	ObjTrack tracker;
	objectManager manager;
	TrackManager trackManager;
	camera.read(frame);

	std::cout << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << std::endl;

	while (true)
	{
		camera.read(frame);		// 카메라에서 현재 영상을 받아 frame 변수에 저장

		if (frame.empty())		// 카메라가 비정상 작동 시 예외 처리
		{
			continue;
		}

		cv::flip(frame, frame, 1);	// 카메라에서 받아온 영상을 반전시킴
		tracker = ObjTrack();
		tracker.setImg(frame.clone());

		tracker.blur(3);
		tracker.colorHistogram();
		tracker.sharp();
		tracker.blur(3);
		tracker.findContours();
		tracker.findObject();

		manager.setObject(tracker.getObject());
		back_frame = frame.clone();
		manager.drawObject(frame);

		cv::imshow("Camera", frame);

		if (cv::waitKey(30) == 27)
		{
			break;
		}
	}
}
*/


int main()
{
	cv::VideoCapture camera;	// 장비의 카메라를 사용하기위한 객체 생성
	camera.open(0);				// 장비의 0번카메라 (랩탑의 경우 웹캠) 사용

	if (!camera.isOpened())		// 카메라가 정상적으로 열리지 않은경우 예외 처리
	{
		printf("Camera not found!\n");
		return -1;
	}
	cv::namedWindow("Camera", CV_WINDOW_AUTOSIZE);	// 카메라의 영상을 실시간으로 출력할 윈도우 생성
	//cv::namedWindow("Object", CV_WINDOW_AUTOSIZE);
	cv::Mat frame;			// 카메라에서 받아올 영상을 담을 변수
	cv::Mat back_frame;

	ObjTrack tracker;
	objectManager manager;
	TrackManager trackManager;
	//Sleep(1000);
	camera.read(frame);

	while (true)
	{
		back_frame = frame.clone();
		camera.read(frame);		// 카메라에서 현재 영상을 받아 frame 변수에 저장

		if (frame.empty())		// 카메라가 비정상 작동 시 예외 처리
		{
			continue;
		}
		double com = compareImg(frame, back_frame);
		if (com <= 0.95)
		{
			continue;
		}

		cv::flip(frame, frame, 1);	// 카메라에서 받아온 영상을 반전시킴
		tracker = ObjTrack();
		tracker.setImg(frame.clone());

		tracker.blur(3);
		tracker.colorHistogram();
		tracker.sharp();
		tracker.blur(3);
		tracker.findContours();
		tracker.findObject();


		manager.setObject(tracker.getObject());
		manager.drawObject(frame);
		cv::imshow("Camera", frame);
		if (cv::waitKey(30) == 27)
		{
			break;
		}
	}
	while (true)
	{
		back_frame = frame.clone();
		camera.read(frame);		// 카메라에서 현재 영상을 받아 frame 변수에 저장

		if (frame.empty())		// 카메라가 비정상 작동 시 예외 처리
		{
			continue;
		}
		double com = compareImg(frame, back_frame);
		if (com <= 0.95)
		{
			continue;
		}

		cv::flip(frame, frame, 1);	// 카메라에서 받아온 영상을 반전시킴
		tracker = ObjTrack();
		tracker.setImg(frame.clone());

		tracker.blur(3);
		tracker.colorHistogram();
		tracker.sharp();
		tracker.blur(3);
		tracker.findContours();
		tracker.findObject();

		trackManager.setObject(tracker.getObject());

		trackManager.composeObject(manager.getObjectList(), manager.getFlagList());
		cv::Mat img = frame.clone();
		manager.drawObject(img);
		trackManager.drawObject(img);
		imshow("Camera", img);
		if (cv::waitKey(30) == 27)
		{
			break;
		}
	}
}


double compareImg(cv::Mat img1, cv::Mat img2)
{
	cv::Mat img1_gray, img2_gray;
	cv::cvtColor(img1, img1_gray, CV_BGR2GRAY);
	cv::cvtColor(img2, img2_gray, CV_BGR2GRAY);

	int histSize = 256;
	float range[] = { 0, 255 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;
	cv::Mat a1_hist, a2_hist;

	cv::calcHist(&img1_gray, 1, 0, cv::Mat(), a1_hist, 1, &histSize, &histRange, uniform, accumulate);
	cv::calcHist(&img2_gray, 1, 0, cv::Mat(), a2_hist, 1, &histSize, &histRange, uniform, accumulate);

	double compar_c = cv::compareHist(a1_hist, a2_hist, CV_COMP_CORREL);
	std::cout << "compare(CV_COMP_CORREL): " << compar_c << "\n";

	return compar_c;
}
