#include "ObjTrack.hpp"
////////////////////////////////////////////////////////////////////////////////////
//								ObjTrack 구현부									  //
////////////////////////////////////////////////////////////////////////////////////
// 기본 생성자
ObjTrack::ObjTrack()
{
	return;
}

// 사용할 영상을 매개변수로 받는 생성자 (초기화)
ObjTrack::ObjTrack(cv::Mat src, int threshold)
{
	this->img = src;
	this->ori = src.clone();
	this->threshold = threshold;
}

// 사용할 영상 설정
void ObjTrack::setImg(cv::Mat src)
{
	this->img = src;
	this->ori = src.clone();
}

// 사용할 트레숄드(임계값) 설정
void ObjTrack::setThreshold(int threshold)
{
	this->threshold = threshold;
}

// 현재 처리된 영상 반환
cv::Mat ObjTrack::getImg()
{
	return this->img;
}

// 초기 입력받은 원본 영상 반환
cv::Mat ObjTrack::getOriginalImg()
{
	return this->ori;
}

// 사용중인 트레숄드(입계값) 반환
int ObjTrack::getThreshold()
{
	return this->threshold;
}

std::vector<cv::Rect> ObjTrack::getObject()
{
	return this->boundRect;
}

// 이미지 블러링 (영상의 품질을 저하, 노이즈 제거)
void ObjTrack::blur(int kernel)
{
	// 매개변수로 받은 커널의 크기를 기준으로 필터를 설정
	cv::GaussianBlur(img, img, cv::Size(kernel, kernel), 1.5);	// 가우시안 블러링 (영상의 고주파 성분을 제거)
	cv::medianBlur(img, img, kernel);						// 메디안 블러링 (salt & papper 노이즈 제거)
}

// 컬러 영상의 히스토그램 평활화 (밝기를 기준으로, YCrCb)
void ObjTrack::colorHistogram()
{
	std::vector<cv::Mat> channels;			// 영상의 채널을 담을 벡터를 생성
	cv::cvtColor(img, img, CV_BGR2YCrCb);	// 영상을 BGR혁식의 영상을 YCrCb형식으로 변환

	cv::split(img, channels);					// 영상의 각 채널을 분할하여 벡터에 저장
	cv::equalizeHist(channels[0], channels[0]);	// 영상의 Y(휘도)성분을 기준으로 히스토그램 평활화
	cv::merge(channels, img);					// 평활화가 끝난 뒤 각 성분을 다시 하나로 합침

	cv::cvtColor(img, img, CV_YCrCb2BGR);		// YCrCb 형식의 영상을 다시 BGR형식의 영상으로 변환
}

// 영상 샤프닝 (에지 강화)
void ObjTrack::sharp()
{
	// 영상 샤프닝을 위한 커널 생성
	cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -8, 1,
		1, 1, 1);

	cv::Mat imglaplacian;
	cv::Mat sharp = img.clone();	// 원본 이미지를 복사하여 사용	
	cv::filter2D(sharp, imglaplacian, CV_32F, kernel);	// 이미지 샤프닝

	img.convertTo(sharp, CV_32F);
	img = sharp - imglaplacian;
	img.convertTo(img, CV_8UC3);
}

// 영상의 윤곽선을 검출
void ObjTrack::findContours()
{
	cv::Mat img_gray;							// 1채널 영상을 담을 변수
	cv::cvtColor(img, img_gray, CV_BGR2GRAY);	// 컬러 영상을 1채널 회색 영상으로 변환
	cv::Mat threshold_output;					// 윤곽선을 검출하여 임시로 저장할 변수

	int thresh = this->threshold;				// 임계값을 이용하여 윤곽선을 검출
	cv::threshold(img_gray, threshold_output, thresh, 255, cv::THRESH_BINARY);
	cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	contours_poly = std::vector<std::vector<cv::Point> >(contours.size());	// 영상의 윤곽선을 폴리곤으로 받아옴
	boundRect = std::vector<cv::Rect>(contours.size());					// 폴리곤의 위치를 사각형으로 받아옴

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));
	}
}

// 검출된 윤곽선을 사용하여 오브젝트의 위치를 검출
void ObjTrack::findObject()
{
}

// 영상에서 검출된 윤곽선을 소스 영상에 그린뒤 반환
cv::Mat ObjTrack::drawContours(cv::Mat src)
{
	cv::drawContours(src, contours, -1, cv::Scalar(0, 0, 255), 3);	// 빨간색으로 그림
	return src;
}

// 영상에서 검출된 물체들의 위치를 사각형으로 소스 영상에 그린뒤 반환
cv::Mat ObjTrack::drawObject(cv::Mat src)
{
	for (int i = 0; i < this->contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(0, 0, 255);
		//cv::drawContours(ori, contours_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		cv::rectangle(ori, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	}
	return ori.clone();
}
