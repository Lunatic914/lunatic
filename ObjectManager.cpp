#include "ObjectManager.hpp"

////////////////////////////////////////////////////////////////////////////////////
//							objectManager 구현부								  //
////////////////////////////////////////////////////////////////////////////////////
// 오브젝트 관리자 생성자
objectManager::objectManager()
{
}

objectManager::objectManager(std::vector<cv::Rect> inputdata)
{
	std::cout << "Gen Object" << std::endl;
	std::vector<cv::Rect> temp = std::vector<cv::Rect>(inputdata.size());

	int j = 0;
	for (int i = 0; i < inputdata.size(); i++)
	{
		if ((inputdata[i].width <= 25) || (inputdata[i].height <= 25) || (inputdata[i].width >= 600) || (inputdata[i].height >= 600))
		{
			continue;
		}
		temp[j] = inputdata[i];
		j++;
	}
	object_list = std::vector<cv::Rect>(j);
	flags = std::vector<bool>(j);
	for (int i = 0; i < temp.size(); i++)
	{
		object_list[i] = temp[i];
		flags[i] = true;
	}

	return;
}

void objectManager::setObject(std::vector<cv::Rect> inputdata)
{
	//object_list = std::vector<cv::Rect>(inputdata.size());
	std::cout << "Set Object : " << inputdata.size() << std::endl;
	std::vector<cv::Rect> temp = std::vector<cv::Rect>(inputdata.size());

	int j = 0;
	for (int i = 0; i < inputdata.size(); i++)
	{
		if ((inputdata[i].width <= 25) || (inputdata[i].height <= 25) || (inputdata[i].width >= 600) || (inputdata[i].height >= 600))
		{
			continue;
		}
		temp[j] = inputdata[i];
		j++;
	}
	object_list = std::vector<cv::Rect>(j);
	flags = std::vector<bool>(j);
	for (int i = 0; i < j; i++)
	{
		object_list[i] = temp[i];
		std::cout << i << " size : " << object_list[i].area() << std::endl;
	}

	return;
}

cv::Mat objectManager::drawObject(cv::Mat src)
{
	for (int i = 0; i < this->object_list.size(); i++)
	{
		cv::Scalar color = cv::Scalar(0, 0, 255);
		if (flags[i])
		{
			color = cv::Scalar(0, 255, 0);
		}
		
		cv::rectangle(src, object_list[i].tl(), object_list[i].br(), color, 2, 8, 0);
	}
	return src.clone();
}

void objectManager::setObject(cv::Rect rect ,int index, int flag)
{
	if (flag == 1)
	{
		this->object_list[index] = rect;
	}
	if (flag == 0)
	{
		this->flags[index] = false;
	}
}

std::vector<cv::Rect>& objectManager::getObjectList()
{
	return object_list;
}

std::vector<bool>& objectManager::getFlagList()
{
	return this->flags;
}
