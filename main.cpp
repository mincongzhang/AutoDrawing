#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>  

#include "BMPReader.h"

namespace{
	void hideConsole(){
		ShowWindow( GetConsoleWindow(), SW_HIDE );	
	}
}

int main(){
	std::cout<<"auto drawing"<<std::endl;

	BMPReader reader("C:\\Users\\mizhang\\Desktop\\a.bmp");


	for(int i=10;i>0;--i){
		std::cout<<"Drawing starts in ["<<i<<"] seconds"<<std::endl;
		Sleep(1000);
	}

	int count = 100;

	while(count<500){
		SetCursorPos(count, 200);
		count+=1;
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(100);
	}
	system("PAUSE");
}