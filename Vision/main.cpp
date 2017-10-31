#include <iostream>
#include <opencv2/opencv.hpp>

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data) {
    if(event == CV_EVENT_LBUTTONDOWN) {
        std::vector<cv::Point2f>* v = static_cast <std::vector<cv::Point2f>*>(data);
        v->push_back(cv::Point2f(x, y));
    }
}

// Main program entry point
int main(int argc, char *argv[]) {
  if (argc != 4) {
      std::cout << "Usage : checkersvision <image> <width> <height>" << std::endl;
      return EXIT_FAILURE;
  }

  const int WIDTH = atoi(argv[2]);
  const int HEIGHT = atoi(argv[3]);
  const std::string IMAGE = argv[1];

  // Load the image
  cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  // Also make a copy that will be edited to show dots while we select points
  cv::Mat input = image.clone();
  // Output image
  cv::Mat output;

  // Input Quadilateral
  // The 4 points that denotes the contour of the object
  cv::Point2f inputQuad[4];

  // Output Quadilateral
  // 4 points that define the output size
  cv::Point2f outputQuad[4];
  outputQuad[0] = cv::Point2f(0, 0);
  outputQuad[1] = cv::Point2f(WIDTH - 1, 0);
  outputQuad[2] = cv::Point2f(WIDTH - 1, HEIGHT - 1);
  outputQuad[3] = cv::Point2f(0, HEIGHT - 1);

  // Input points vector
  std::vector<cv::Point2f> inputVect;

  // Create window and callback to mouse clicks
  cv::namedWindow(IMAGE);
  cv::setMouseCallback(IMAGE, onMouse, static_cast<void*>(&inputVect));

  // Wait until we have 4 points
  while(inputVect.size() < 4) {
      // Display image
      imshow(IMAGE, input);

      // Display points live
      for (auto p : inputVect) circle(input, p, 10, cv::Scalar(255, 0, 0), -1, CV_AA);

      // 50Hz refresh
      cv::waitKey(20);
  }

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  // This step is to tranform our vect to an array (ugly)
  for (int i = 0; i < 4; ++i) {
      cv::Point2f p = inputVect[i];
      inputQuad[i] = p;
      // We also display point coordinates in stdout
      std::cout << p.x << " : " << p.y << std::endl;
  }

  // Get the Perspective Transform Matrix i.e. lambda
  cv::Mat lambda = cv::getPerspectiveTransform(inputQuad, outputQuad);
  std::cout << lambda << std::endl;

  // Apply the Perspective Transform just found to the src image
  cv::warpPerspective(image, output, lambda, cv::Size(WIDTH, HEIGHT));

  //Display output
  cv::imshow("Output",output);

  cv::waitKey(0);
}
