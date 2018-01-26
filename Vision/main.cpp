 #include "vision.h"

// Main program entry point
int main(int argc, char *argv[]) {
  if (argc != 3 && argc != 4 && argc != 5) {
    cout << "Usage : checkersvision create <image> <reference> <size> - To create reference file (of size*size) from image" << endl;
    cout << "        checkersvision parse <image> <reference> - To parse an image in comparison to reference" << endl;
    cout << "       checkersvision videoParse <reference> - To parse continuously in comparison to reference" << endl;
    cout << "       checkersvision stat <folder> <reference> - To parse continuously the images in a folder in comparison to reference" << endl;
    return EXIT_FAILURE;
  }

  string mode = argv[1];

  if (mode == "create") return createReference(argv[2], argv[3], atoi(argv[4]));
  else if (mode == "parse") return imageGetPieces(argv[2], argv[3]);
  else if (mode == "videoParse") return videoGetPieces(argv[2]);
  else if (mode == "stat") return stat(argv[2], argv[3]);
  else { 
     cout << "Unknown mode '" << mode << "', see ./checkersvision for usage." << endl;
      return EXIT_FAILURE;
  }

}
