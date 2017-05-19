#include "TrackManager.hpp"

TrackManager::TrackManager()
{
}

TrackManager::TrackManager(std::vector<cv::Rect> inputdata)
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
	object_list = std::vector<cv::Rect>(j - 1);
	for (int i = 0; i < temp.size(); i++)
	{
		object_list[i] = temp[i];
	}

	return;
}
                                                                              
void TrackManager::setObject(std::vector<cv::Rect> inputdata)
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
	std::cout << "object size : " << j << std::endl;
	object_list = std::vector<cv::Rect>(j);

	for (int i = 0; i < j; i++)
	{
		object_list[i] = temp[i];
	}

	return;
}

cv::Mat TrackManager::drawObject(cv::Mat src)
{
	for (int i = 0; i < this->object_list.size(); i++)
	{
		cv::Scalar color = cv::Scalar(0, 255, 0);
		//cv::rectangle(src, object_list[i].tl(), object_list[i].br(), color, 2, 8, 0);
		cv::rectangle(src, object_list[i].tl(), object_list[i].br(), color, 3, 8, 0);
	}
	return src.clone();
}

void TrackManager::composeObject(std::vector<cv::Rect>& object, std::vector<bool>& flags)
{
	std::cout << "intail Object number : " << object.size() << "\tCurrent Object number : " << object_list.size() << std::endl;
	for (int i = 0; i < object.size(); i++)
	{
		bool flag = false;
		//if (flags[i] == false)
		for (int j = 0; j < object_list.size(); j++)
		{	
			bool size_flag = false;
			bool x_flag = false;
			bool y_flag = false;
			if (object_list[j].area() * 0.8 <= (double)object[i].area() && (double)object[i].area() <= object_list[j].area() * 1.2)
			{
				size_flag = true;
			}
			if (object_list[j].x - 20 <= object[i].x && object[i].x <= object_list[j].x + 20)
			{
				x_flag = true;
			}
			if (object_list[j].y - 20 <= object[i].y && object[i].y <= object_list[j].y + 20)
			{
				y_flag = true;
			}
			if (size_flag && x_flag && y_flag)
			{
				object[i] = object_list[j];
				flag = true;
			}
			std::cout << "size flag : " << size_flag << " \tx flag : " << x_flag << "\ty flag : " << y_flag << std::endl;
		}
		if (!flag)
		{
			flags[i] = false;
		}
	}
}
