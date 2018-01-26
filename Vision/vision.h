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
static unsigned long w_pieces;
static unsigned long b_pieces;

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data);

Mat transformImage(const Mat &image, const int& size);
int createReference(const string &filename, const string &refname, const int& size);
int videoGetPieces(const string &refname);
int imageGetPieces(const string &filename,const string &refname);
string getPieces(Mat &image, const string &refname); 
int stat(const string &foldername,const string &refname);

#endif
