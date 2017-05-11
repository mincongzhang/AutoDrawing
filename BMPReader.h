#ifndef BMP_READER_H_INCLUDED
#define BMP_READER_H_INCLUDED

#pragma warning(disable:4996)
//Disable fopen warning

#include <stdio.h>
#include <string>

#include "Logger.h"

class BMPReader{
private:
	unsigned char* m_data_gray_scale;
	int m_width;
	int m_height;

public:
	BMPReader(const std::string & filename){
		FILE* f = fopen(filename.c_str(), "rb");
		if(!f){
			logInfo("WARNING! Failed to open file ["<<filename<<"]");			
		} else {
			logInfo("Successfilly opened file ["<<filename<<"]");		
		}
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

		// extract image m_height and m_width from header
		m_width = *(int*)&info[18];
		m_height = *(int*)&info[22];
		logInfo("Loaded width ["<<m_width<<"]");	
		logInfo("Loaded height ["<<m_height<<"]");	

		int size = 3 * m_width * m_height;
		unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
		fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
		fclose(f);
		unsigned char* m_data_gray_scale = new unsigned char[m_width*m_height];

		for(int i = 0; i < size; i += 3){
			int pixel_pos = i/3;
			logInfo("loading pixel ["<<pixel_pos<<"] = ["<<data[i]-'0'<<"," <<data[i+1]-'0'<<","<<data[i+2]-'0'<<"], total pixels ["<<m_width*m_height<<"]");
			Sleep(10);
			m_data_gray_scale[pixel_pos] = (data[i]+data[i+1]+data[i+2])/3; //the avg of the rgb is the grayscale value
		}

		delete [] data;  	
	}

	int height(){ return m_height; }
	int width(){ return m_width; }
	int getPixel(unsigned int width,unsigned int height){
		return 1;
	}

	~BMPReader(){
		delete [] m_data_gray_scale;
	}
};

#endif /* BMP_READER_H_INCLUDED */