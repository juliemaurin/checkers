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


using namespace cv;
using namespace std;

static string fichier_ref = "../reference.txt"; 

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data);

Mat transformImage(const Mat &image, const int& size);
int createReference(const string &emptyref, const string &fullref, const int& size);
int videoGetPieces();
int imageGetPieces(const string &filename);
string getPieces(Mat &image,int calib); 
int stat(const string &foldername);
int security(Mat src);

#endif
