#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  
#include <vector>
#include <queue>
#include <unordered_map>
#include <sstream>

#include "BMPReader.h"
#include "Logger.h"

namespace{
	void hideConsole(){
		ShowWindow( GetConsoleWindow(), SW_HIDE );	
	}

	struct Point{
		int m_w,m_h;
		Point(int x,int y):m_w(x),m_h(y){}
		std::string toString(){
			std::stringstream ss;
			ss<<m_w<<","<<m_h;
			return ss.str();
		}
	};

	std::vector<Point> neighbourSort(const std::vector<Point> & point_array){
		logInfo("Sorting points with neighbours");
		std::vector<Point> sorted;
		if(point_array.empty()) {
			logInfo("WARNING! point_array is empty!");
			return sorted; 
		};

		std::unordered_map<std::string,Point> hash;
		for(int i=0;i<point_array.size();++i){
			Point p = point_array[i];
			std::string key = p.toString();
			hash.insert(std::make_pair(key,p));
		}

		if(hash.empty()){ 
			logInfo("WARNING! point_array is empty!");
			return sorted; 
		}

		//Find nearest neighbour
		Point p = point_array[0];
		sorted.push_back(p);
		hash.erase(p.toString());

		while(!hash.empty()){
			long min_len = LONG_MAX;
			Point neighbour_point(0,0);
			for(std::unordered_map<std::string,Point>::const_iterator it=hash.begin();it!=hash.end();++it){
				Point tmp_p = it->second;
				long length = (tmp_p.m_w-p.m_w)*(tmp_p.m_w-p.m_w) + (tmp_p.m_h-p.m_h)*(tmp_p.m_h-p.m_h);
				if(length<min_len){
					min_len = length;
					neighbour_point = tmp_p;
					p = neighbour_point;
				}
			}

			sorted.push_back(neighbour_point);
			hash.erase(neighbour_point.toString());
		}

		logInfo("Sorting points with neighbours done");
		return sorted;
	}
}

int main(){

	std::cout<<"Auto Drawing"<<std::endl;

	//////////////////////////////////////
	// EDGE DETECTION
	//////////////////////////////////////
	BMPReader reader("C:\\Users\\mizhang\\Desktop\\b.bmp");
	for(int h=0;h<reader.height();h++){
		for(int w=0;w<reader.width();w++){
			//logInfo("Loading pixel in greyscale["<<h<<","<<w<<"]:"<<reader.getPixel(w,h));
		}
	}

	reader.edgeDetection(70);

	std::vector<Point> point_array;
	for(int h=0;h<reader.height();h++){
		for(int w=0;w<reader.width();w++){
			//logInfo("Is edge["<<h<<","<<w<<"]:"<<reader.isEdge(w,h));
			if(reader.isEdge(w,h)){
				point_array.push_back(Point(w,h));
			}
		}
	}

	std::vector<Point> sorted_points = neighbourSort(point_array);

	////////////////////////////////////
	// DRAW
	////////////////////////////////////
	for(int i=5;i>0;--i){
		std::cout<<"Drawing starts in ["<<i<<"] second(s)"<<std::endl;
		Sleep(1000);
	}

	hideConsole();

	for(int i=0; i<sorted_points.size();++i){
		SetCursorPos(sorted_points[i].m_w+300, sorted_points[i].m_h+300);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(3);
	}


	system("PAUSE");
}