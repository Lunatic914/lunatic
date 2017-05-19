/*
*	�ۼ��� : Kim June-Pyo
*	��  ¥ : 2017.05.02
*	��  �� : �������� �ý���
*   �̸��� : luna914@naver.com
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
	cv::VideoCapture camera;	// ����� ī�޶� ����ϱ����� ��ü ����
	camera.open(0);				// ����� 0��ī�޶� (��ž�� ��� ��ķ) ���

	if (!camera.isOpened())		// ī�޶� ���������� ������ ������� ���� ó��
	{
		printf("Camera not found!\n");
		return -1;
	}
	cv::namedWindow("Camera", CV_WINDOW_AUTOSIZE);	// ī�޶��� ������ �ǽð����� ����� ������ ����
													//cv::namedWindow("Object", CV_WINDOW_AUTOSIZE);
	cv::Mat frame;			// ī�޶󿡼� �޾ƿ� ������ ���� ����
	cv::Mat back_frame;

	ObjTrack tracker;
	objectManager manager;
	TrackManager trackManager;
	camera.read(frame);

	std::cout << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << std::endl;

	while (true)
	{
		camera.read(frame);		// ī�޶󿡼� ���� ������ �޾� frame ������ ����

		if (frame.empty())		// ī�޶� ������ �۵� �� ���� ó��
		{
			continue;
		}

		cv::flip(frame, frame, 1);	// ī�޶󿡼� �޾ƿ� ������ ������Ŵ
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
	cv::VideoCapture camera;	// ����� ī�޶� ����ϱ����� ��ü ����
	camera.open(0);				// ����� 0��ī�޶� (��ž�� ��� ��ķ) ���

	if (!camera.isOpened())		// ī�޶� ���������� ������ ������� ���� ó��
	{
		printf("Camera not found!\n");
		return -1;
	}
	cv::namedWindow("Camera", CV_WINDOW_AUTOSIZE);	// ī�޶��� ������ �ǽð����� ����� ������ ����
	//cv::namedWindow("Object", CV_WINDOW_AUTOSIZE);
	cv::Mat frame;			// ī�޶󿡼� �޾ƿ� ������ ���� ����
	cv::Mat back_frame;

	ObjTrack tracker;
	objectManager manager;
	TrackManager trackManager;
	//Sleep(1000);
	camera.read(frame);

	while (true)
	{
		back_frame = frame.clone();
		camera.read(frame);		// ī�޶󿡼� ���� ������ �޾� frame ������ ����

		if (frame.empty())		// ī�޶� ������ �۵� �� ���� ó��
		{
			continue;
		}
		double com = compareImg(frame, back_frame);
		if (com <= 0.95)
		{
			continue;
		}

		cv::flip(frame, frame, 1);	// ī�޶󿡼� �޾ƿ� ������ ������Ŵ
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
		camera.read(frame);		// ī�޶󿡼� ���� ������ �޾� frame ������ ����

		if (frame.empty())		// ī�޶� ������ �۵� �� ���� ó��
		{
			continue;
		}
		double com = compareImg(frame, back_frame);
		if (com <= 0.95)
		{
			continue;
		}

		cv::flip(frame, frame, 1);	// ī�޶󿡼� �޾ƿ� ������ ������Ŵ
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
