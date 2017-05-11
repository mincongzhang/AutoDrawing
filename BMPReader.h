#ifndef BMP_READER_H_INCLUDED
#define BMP_READER_H_INCLUDED

#pragma warning(disable:4996)
//Disable fopen warning

#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Logger.h"

//REF: http://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file

class BMPReader{
private:
	std::vector< std::vector<int> > m_pixels;
	std::vector< std::vector<bool> > m_edge_pixels;
	int m_width;
	int m_height;

public:

	void edgeDetection(unsigned int thresh){
		for(int h=1;h<m_height-1;h++){
			for(int w=1;w<m_width-1;w++){
				int cur_pix = m_pixels[w][h]*4;
				int surroundings = m_pixels[w-1][h]+m_pixels[w][h-1]+m_pixels[w+1][h]+m_pixels[w][h+1];
				if(std::abs(cur_pix-surroundings)>thresh){
					m_edge_pixels[w][h] = true;
				}
			}
		}
	}

	BMPReader(const std::string & filename){
		//OPEN FILE
		FILE* f = fopen(filename.c_str(), "rb");
		if(!f){
			logInfo("WARNING! Failed to open file ["<<filename<<"]");			
		} else {
			logInfo("Successfilly opened file ["<<filename<<"]");		
		}

		// extract image m_height and m_width from header
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
		m_width = *(int*)&info[18];
		m_height = *(int*)&info[22];
		logInfo("Loaded width ["<<m_width<<"]");	
		logInfo("Loaded height ["<<m_height<<"]");

		// INIT m_pixels and m_edge_pixels
		std::vector<int> height_vector(m_height,0);
		std::vector< std::vector<int> > pixels(m_width,height_vector);
		m_pixels = pixels;
		std::vector<bool> height_bool_vector(m_height,false);
		std::vector< std::vector<bool> > edge_pixels(m_width,height_bool_vector);
		m_edge_pixels = edge_pixels;


		//GET PIXEL
		int row_padded = (m_width*3 + 3) & (~3);
		unsigned char* data = new unsigned char[row_padded];
		unsigned char tmp;
		for(int h = 0; h < m_height; h++){
			fread(data, sizeof(unsigned char), row_padded, f);
			for(int w = 0; w < m_width*3; w += 3){
				// Convert (B, G, R) to (R, G, B)
				tmp = data[w];
				data[w] = data[w+2];
				data[w+2] = tmp;

				int cur_width = w/3;
				int cur_height = (m_height-h) - 1;
				m_pixels[cur_width][cur_height] = ((int)data[w]+(int)data[w+1]+(int)data[w+2])/3;
				//logInfo("[Width,Height]:["<<cur_width<<","<<cur_height<<"],RGB:"<<(int)data[w]<<","<<(int)data[w+1]<<","<<(int)data[w+2]
				//<<" GREY:"<<m_pixels[cur_width][cur_height]);
			}
		}

		fclose(f);

		logInfo("["<<filename<<"] Loaded. ");
		//system("PAUSE");
		delete [] data;  	
	}

	int height(){ return m_height; }
	int width(){ return m_width; }
	int getPixel(unsigned int width,unsigned int height){
		return m_pixels[width][height];
	}

	bool isEdge(unsigned int width,unsigned int height){
		return m_edge_pixels[width][height];
	}
};

#endif /* BMP_READER_H_INCLUDED */