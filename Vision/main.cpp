 #include "vision.h"

// Main program entry point
int main(int argc, char *argv[]) {
  if (argc != 2 && argc != 3 && argc != 4 && argc != 5) {
    cout << "Usage : checkersvision calib <empty_image> <full_image> <size> - To create reference file (of size*size) from empty_image , compute the best thresh value and create a security mask" << endl;
    cerr<<endl;
    cout << "        checkersvision parse <image> <black_thresh>- To parse an image matching the last calibration and thresh" << endl;
    cerr<<endl;
    cout << "       checkersvision videoParse <black_thresh>- To parse continuously via video - Connected version" << endl;
    cerr<<endl;
    cout << "       checkersvision stat <folder> <black_thresh>- To parse continuously the images in a folder with respect to the calibration thresh then generate results" << endl;
    return EXIT_FAILURE;
  }

  string mode = argv[1];

  if (mode == "calib") return createReference(argv[2], argv[3], atoi(argv[4]));
  else if (mode == "videoCalib") return videoCalib();
  else if (mode == "parse") return imageGetPieces(argv[2] , atoi(argv[3]));
  else if (mode == "videoParse") return videoGetPieces(atoi(argv[2]));
  else if (mode == "stat") return stat(argv[2], atoi(argv[3]));
  else { 
     cout << "Unknown mode '" << mode << "', see ./checkersvision for usage." << endl;
      return EXIT_FAILURE;
  }

}
