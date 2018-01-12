#ifndef VISION_H
#define VISION_H

#include <iostream>
#include <bitset>
#include <opencv2/opencv.hpp>
#include "../Game/tcphelper.h"

using namespace cv;
using namespace std;

static unsigned long w_pieces;
static unsigned long b_pieces;

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data);

Mat transformImage(const cv::Mat &image, const int& size);
int createReference(const std::string &filename, const std::string &refname, const int& size);
int connectedGetPieces(const std::string &filename, const std::string &refname);
string getPieces(const std::string &filename, const std::string &refname); 

#endif
