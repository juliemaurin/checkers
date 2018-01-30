 #include "vision.h"

// Main program entry point
int main(int argc, char *argv[]) {
  if (argc != 2 && argc != 3 && argc != 5) {
    cout << "Usage : checkersvision calib <empty_image> <full_image> <size> - To create reference file (of size*size) from empty_image , calibrate the thresh values and create the security mask" << endl;
    cout << "        checkersvision parse <image> - To parse an image matching the last calibration" << endl;
    cout << "       checkersvision videoParse - To parse continuously - Connected version" << endl;
    cout << "       checkersvision stat <folder> - To parse continuously the images in a folder and generate results" << endl;
    return EXIT_FAILURE;
  }

  string mode = argv[1];

  if (mode == "calib") return createReference(argv[2], argv[3], atoi(argv[4]));
  else if (mode == "parse") return imageGetPieces(argv[2]);
  else if (mode == "videoParse") return videoGetPieces();
  else if (mode == "stat") return stat(argv[2]);
  else { 
     cout << "Unknown mode '" << mode << "', see ./checkersvision for usage." << endl;
      return EXIT_FAILURE;
  }

}
