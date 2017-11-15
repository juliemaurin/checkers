#include <iostream>
#include <bitset>
#include <opencv2/opencv.hpp>

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data) {
    if(event == CV_EVENT_LBUTTONDOWN) {
        std::vector<cv::Point2f>* v = static_cast <std::vector<cv::Point2f>*>(data);
        v->push_back(cv::Point2f(x, y));
    }
}

int createReference(const std::string &filename, const std::string &refname, const int& size) {
  // Load the image
  cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
  // Also make a copy that will be edited to show dots while we select points
  cv::Mat input = image.clone();
  // Output image
  cv::Mat output;

  // Input Quadilateral
  // The 4 points that denotes the contour of the object
  cv::Point2f input_quad[4];

  // Output Quadilateral
  // 4 points that define the output size
  // Output is a square of size x size pixels
  cv::Point2f output_quad[4];
  output_quad[0] = cv::Point2f(0, 0);
  output_quad[1] = cv::Point2f(size - 1, 0);
  output_quad[2] = cv::Point2f(size - 1, size - 1);
  output_quad[3] = cv::Point2f(0, size - 1);

  // Input points vector
  std::vector<cv::Point2f> input_vect;

  // Create window and callback to mouse clicks
  cv::namedWindow(filename);
  cv::setMouseCallback(filename, onMouse, static_cast<void*>(&input_vect));

  // Wait until we have 4 points
  while(input_vect.size() < 4) {
      // Display image
      imshow(filename, input);

      // Display points live
      if (input_vect.size()) circle(input, input_vect[input_vect.size() - 1], 10, cv::Scalar(255, 0, 0), -1, CV_AA);

      // 50Hz refresh
      cv::waitKey(20);
  }

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  // This step is to tranform our vect to an array (ugly)
  std::cout << "Angle coordinates : " << std::endl;
  for (std::vector<cv::Point2f>::size_type i = 0; i < input_vect.size(); ++i) {
      input_quad[i] = input_vect[i];
      // We also display point coordinates in stdout
      std::cout << "Point " << i + 1 << " : (" << input_quad[i].x << ", " << input_quad[i].y << ")" << std::endl;
  }

  // Get the Perspective Transform Matrix i.e. lambda
  cv::Mat lambda = cv::getPerspectiveTransform(input_quad, output_quad);
  std::cout << lambda << std::endl;

  // Apply the Perspective Transform just found to the src image
  cv::warpPerspective(image, output, lambda, cv::Size(size, size));

  //Display output
  cv::imshow("Output",output);
  cv::imwrite(refname, output);

  cv::waitKey(0);
  return EXIT_SUCCESS;
}

int getPieces(const std::string &filename, const std::string &refname) {
  // Load the image
  cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
  // Also make a copy that will be edited to show dots while we select points
  cv::Mat input = image.clone();
  // Output image
  cv::Mat output;
  // Load reference image
  cv::Mat reference = cv::imread(refname, CV_LOAD_IMAGE_COLOR);
  int size = reference.size().height;

  // Input Quadilateral
  // The 4 points that denotes the contour of the object
  cv::Point2f input_quad[4];

  // Output Quadilateral
  // 4 points that define the output size
  // Output is a square of size x size pixels
  cv::Point2f output_quad[4];
  output_quad[0] = cv::Point2f(0, 0);
  output_quad[1] = cv::Point2f(size - 1, 0);
  output_quad[2] = cv::Point2f(size - 1, size - 1);
  output_quad[3] = cv::Point2f(0, size - 1);

  // Input points vector
  std::vector<cv::Point2f> input_vect;

  // Create window and callback to mouse clicks
  cv::namedWindow(filename);
  cv::setMouseCallback(filename, onMouse, static_cast<void*>(&input_vect));

  // Wait until we have 4 points
  while(input_vect.size() < 4) {
      // Display image
      imshow(filename, input);

      // Display points live
      if (input_vect.size()) circle(input, input_vect[input_vect.size() - 1], 10, cv::Scalar(255, 0, 0), -1, CV_AA);

      // 50Hz refresh
      cv::waitKey(20);
  }

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  // This step is to tranform our vect to an array (ugly)
  std::cout << "Angle coordinates : " << std::endl;
  for (std::vector<cv::Point2f>::size_type i = 0; i < input_vect.size(); ++i) {
      input_quad[i] = input_vect[i];
      // We also display point coordinates in stdout
      std::cout << "Point " << i + 1 << " : (" << input_quad[i].x << ", " << input_quad[i].y << ")" << std::endl;
  }

  // Get the Perspective Transform Matrix i.e. lambda
  cv::Mat lambda = cv::getPerspectiveTransform(input_quad, output_quad);
  std::cout << "Transformation matrix : " << std::endl;
  std::cout << lambda << std::endl;

  // Apply the Perspective Transform just found to the src image
  cv::warpPerspective(image, output, lambda, cv::Size(size, size));

  //Display output
  cv::destroyWindow(filename);
  cv::imshow("Output", output);
//  cv::imshow("Reference", reference);

  // Compute difference between image and reference
  cv::Mat difference = output - reference;
//  cv::imshow("Difference", difference);

  // Morphological opening and closing to filter noise
  int morph_size = size / 20;
  cv::Mat kernel = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(morph_size, morph_size)) * 255;
//  std::cout << "Morph kernel : " << std::endl;
//  std::cout << kernel << std::endl;
  cv::morphologyEx(difference, difference, cv::MORPH_OPEN, kernel);
//  cv::imshow("After opening", difference);

  cv::morphologyEx(difference, difference, cv::MORPH_CLOSE, kernel);
  cv::imshow("After closing", difference);

  // Split image in rectangles to detect presence of cells
  int rect_size = size / 8;

  // Initialize bitboard
  unsigned long pieces = 0;

  // Loop by line
  for(int j = 7; j >= 0; --j) {
      // Computing Y coordinate
      int rect_y = rect_size * (7 - j);

      // Loop by column
      for (int i = 0; i < 4; ++i) {
          // Computing X coordinate
          int rect_x = 2 * rect_size * i + (j % 2) * rect_size;

          // Computing cell index
          int index = 4*j + i;
          unsigned long cell = 1 << index;

          // Fetch cell
          cv::Mat roi = difference(cv::Rect(rect_x, rect_y, rect_size, rect_size));
//          cv::imshow(std::to_string(num), roi);

          // Convert to HSV to get Value
          int threshold_r = 0;
          int threshold_v = 10;
          cv::Scalar mean_bgr = cv::mean(roi);
          cv::cvtColor(roi, roi, cv::COLOR_BGR2HSV);
          cv::Scalar mean_hsv = cv::mean(roi);

          // If value is above threshold, a piece is present
          std::cout << index << " : (" << rect_x << ", " << rect_y << ")" << std::endl;
          std::cout << "    BGR" << mean_bgr << " HSV" << mean_hsv;
          if (mean_hsv.val[2] > threshold_v) {
              std::cout << " PIECE FOUND";
              pieces |= cell;
          }
          std::cout << std::endl;
      }
  }

  std::cout << "BOARD : " << std::bitset<32>(pieces) << " (" << pieces << ")" << std::endl;

  cv::waitKey(0);
  return EXIT_SUCCESS;
}

// Main program entry point
int main(int argc, char *argv[]) {
  if (argc != 4 && argc != 5) {
      std::cout << "Usage : checkersvision create <image> <reference> <size> - To create reference file (of size*size) from image" << std::endl;
      std::cout << "        checkersvision parse <image> <reference> - To parse an image in comparison to reference" << std::endl;
      return EXIT_FAILURE;
  }

  std::string mode = argv[1];

  if (mode == "create") return createReference(argv[2], argv[3], atoi(argv[4]));
  else if (mode == "parse") return getPieces(argv[2], argv[3]);
  else {
      std::cout << "Unknown mode '" << mode << "', see ./checkersvision for usage." << std::endl;
      return EXIT_FAILURE;
  }

}
