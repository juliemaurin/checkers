#ifndef VISION_H
#define VISION_H

#include <iostream>
#include <bitset>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


unsigned long w_pieces ;
  
unsigned long b_pieces ;

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data);

Mat transformImage(const cv::Mat &image, const int& size);
int createReference(const std::string &filename, const std::string &refname, const int& size);


Mat inverseReference(Mat ref);


int getPieces(const std::string &filename, const std::string &refname);

// Main program entry point
int main(int argc, char *argv[]);


#endif