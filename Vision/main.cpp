#include <iostream>
#include <opencv2/opencv.hpp>

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data) {
    if(event == CV_EVENT_LBUTTONDOWN) {
        std::vector<cv::Point2f>* v = (std::vector<cv::Point2f>*)data;
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

  //Load the image
  cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  cv::Mat input = image.clone();
  cv::Mat output;

  // Input Quadilateral
  cv::Point2f inputQuad[4];

  // Output Quadilateral
  cv::Point2f outputQuad[4];
  outputQuad[0] = cv::Point2f(0, 0);
  outputQuad[1] = cv::Point2f(WIDTH - 1, 0);
  outputQuad[2] = cv::Point2f(WIDTH - 1, HEIGHT - 1);
  outputQuad[3] = cv::Point2f(0, HEIGHT - 1);

  // Lambda Matrix
  cv::Mat lambda(2, 4, CV_32FC1);

  // Set the lambda matrix the same type and size as input
  lambda = cv::Mat::zeros(input.size(), input.type());

  std::vector<cv::Point2f> inputVect;
  cv::namedWindow(IMAGE);
  cv::setMouseCallback(IMAGE, onMouse, (void*)&inputVect);

  while(inputVect.size() < 4) {
      imshow(IMAGE, input);
      for (auto p : inputVect) {
          circle(input, p, 10, cv::Scalar(255, 0, 0), -1, CV_AA);
      }
      cv::waitKey(10);
  }

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  for (int i = 0; i < 4; ++i) {
      cv::Point2f p = inputVect[i];
      inputQuad[i] = p;
      std::cout << p.x << " : " << p.y << std::endl;
  }

  // Get the Perspective Transform Matrix i.e. lambda
  lambda = cv::getPerspectiveTransform(inputQuad, outputQuad);
  // Apply the Perspective Transform just found to the src image
  cv::warpPerspective(image, output, lambda, cv::Size(WIDTH, HEIGHT));

  //Display input and output
  cv::imshow("Output",output);

  cv::waitKey(0);
}
