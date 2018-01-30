#ifndef VISION_H
#define VISION_H

#include <iostream>
#include <bitset>
#include <opencv2/opencv.hpp>
#include "tcphelper.h"
#include <string>
#include <fstream>
#include <dirent.h> 
#include <sys/stat.h> 
#include <algorithm>
#include <thread>

using namespace cv;
using namespace std;

static string fichier_ref = "../reference.txt"; 

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data);

Mat transformImage(const Mat &image, const int& size);
int createReference(const string &emptyref, const string &fullref, const int& size);
int videoGetPieces(int black_thresh);
int imageGetPieces(const string &filename,int black_thresh);
string getPieces(Mat &image,int black_thresh,int calib); 
int stat(const string &foldername,int black_thresh);
int security(Mat src);

#endif
