#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  
#include <vector>
#include <queue>

#include "BMPReader.h"
#include "Logger.h"

namespace{
	void hideConsole(){
		ShowWindow( GetConsoleWindow(), SW_HIDE );	
	}

	struct Point{
		int m_w,m_h;
		Point(int x,int y):m_w(x),m_h(y){}
	};
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

	reader.edgeDetection(30);

	std::queue<Point> point_q;
	for(int h=0;h<reader.height();h++){
		for(int w=0;w<reader.width();w++){
			//logInfo("Is edge["<<h<<","<<w<<"]:"<<reader.isEdge(w,h));
			if(reader.isEdge(w,h)){
				point_q.push(Point(w,h));
			}
		}
	}

	////////////////////////////////////
	// DRAW
	////////////////////////////////////
	for(int i=5;i>0;--i){
		std::cout<<"Drawing starts in ["<<i<<"] seconds"<<std::endl;
		Sleep(1000);
	}

	while(!point_q.empty()){
		SetCursorPos(point_q.front().m_w+300, point_q.front().m_h+300);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		point_q.pop();
		Sleep(3);
	}


	system("PAUSE");
}