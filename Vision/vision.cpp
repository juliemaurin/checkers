#include <iostream>
#include <bitset>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// onMouse event callback function
void onMouse(int event, int x, int y, int flags, void* data) {
    if(event == CV_EVENT_LBUTTONDOWN) {
        vector<Point2f>* v = static_cast <vector<Point2f>*>(data);
        v->push_back(Point2f(x, y));
    }
}

Mat transformImage(const Mat &image, const int& size) {
  // Create a copy that will be modified
  Mat input = image.clone();

  // Input Quadilateral
  // The 4 points that denotes the contour of the object
  Point2f input_quad[4];

  // Output Quadilateral
  // 4 points that define the output size
  // Output is a square of size x size pixels
  Point2f output_quad[4] = {Point2f(0, 0), Point2f(size - 1, 0), Point2f(size - 1, size - 1), Point2f(0, size - 1)};

  // Input points vector
  vector<Point2f> input_vect;

  // Create window and callback to mouse clicks
  namedWindow("Image");
  setMouseCallback("Image", onMouse, static_cast<void*>(&input_vect));

  // Wait until we have 4 points
  while(input_vect.size() < 4) {
      // Display image
      imshow("Image", input);

      // Display points live
      if (input_vect.size()) circle(input, input_vect[input_vect.size() - 1], 10, Scalar(255, 0, 0), -1, CV_AA);

      // 50Hz refresh
      waitKey(20);
  }

  // The 4 points that select quadilateral on the input , from top-left in clockwise order
  // These four pts are the sides of the rect box used as input
  // This step is to tranform our vect to an array (ugly)
  cout << "Angle coordinates : " << endl;
  for (vector<Point2f>::size_type i = 0; i < input_vect.size(); ++i) {
      input_quad[i] = input_vect[i];
      // We also display point coordinates in stdout
      cout << "Point " << i + 1 << " : (" << input_quad[i].x << ", " << input_quad[i].y << ")" << endl;
  }

  // Get the Perspective Transform Matrix i.e. lambda
  Mat lambda = getPerspectiveTransform(input_quad, output_quad);
  cout << "Transformation matrix : " << endl;
  cout << lambda << endl;

  // Apply the Perspective Transform just found to the src image
  Mat output;
  warpPerspective(image, output, lambda, Size(size, size));
  return output;
}

int createReference(const string &filename, const string &refname, const int& size) {
  // Load the image
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);

  //Resize image
  resize(image, image, Size(600,600), 0, 0, CV_INTER_LINEAR);
  
  // Output image
  Mat output = transformImage(image, size);

  //Display output
  imshow("Output",output);
  imwrite(refname, output);

  waitKey(0);
  return EXIT_SUCCESS;
}


Mat inverseReference(Mat ref){

  Mat inverse;
  // One Row Size
  int c=(int)ref.rows/8.;
  //Cut
  Mat resultUp=ref(Rect(0,0,ref.cols,c));
  Mat resultDown= ref(Rect(0,c,ref.cols,ref.rows-c));
  //concatinate 
  vconcat(resultDown,resultUp, inverse);
  //imwrite("Up.png",resultUp);
  //  imwrite("down.png",resultDown);
  imwrite("inverse.png",inverse);
  return inverse;
  
}



int getPieces(const string &filename, const string &refname) {
  // Load the image
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
  // Load reference image
  Mat reference = imread(refname, CV_LOAD_IMAGE_COLOR);
  int size = reference.size().height;
  //Resize image to be visible in all cases
   resize(image, image, reference.size(), 0, 0, CV_INTER_LINEAR);

  // Output image
   Mat output = transformImage(image, size);

   //Display output
   destroyWindow(filename);
  imshow("Output", output);
  //  cv::imshow("Reference", reference);
  
  Mat diff_w,diff_b, white ,plus;
  plus=output.clone();
  // Compute difference between image and reference (To detect the White)
   absdiff(output , reference, diff_b);
   diff_w=output-reference;
  
   for(int j=0;j<=5;j++){
    plus=plus+diff_b;
  }

  //Compute inverse reference
  // inv_ref=inverseReference(reference);

  //Compute difference between image and inverse reference (To detect the black)
  //  inv_diff=output-inv_ref;
  
  // threshold(difference,difference, 80, 255,THRESH_BINARY);
  // erode(difference, difference, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size( size / 20, size / 20)));
   

   imshow("Difference", diff_b);
   imshow("plus",plus);
   
   
   
   // Convert BGR to HSV
   //Mat  hsv;
   //  cvtColor(output,hsv ,COLOR_BGR2HSV);
   //green
   
   //  inRange(output, Scalar(0, 50, 0), Scalar(255, 100, 255), white);
   //brown BGR
   //  inRange(hsv, Scalar(0, 50, 0), Scalar(255, 100, 255), white);
   
   // inRange(plus,Scalar(150,130,60), Scalar(220,193,140),white);
   //imshow("black",white);
   
   //Contrast treatement
   Mat contrast_diff1,contrast_ref,contrast_black;
   double alpha=10; //< Simple contrast control 
   int beta=0;  //< Simple brightness control 
   diff_b.convertTo(contrast_diff1, -1, alpha, beta);
   imshow("contrast apres premier difference",contrast_diff1);
   
  
   // Morphological opening and closing to filter noise
   int morph_size = size / 20;
   Mat kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(morph_size, morph_size)) * 255;
   //  std::cout << "Morph kernel : " << std::endl;
   //  std::cout << kernel << std::endl;
   morphologyEx(diff_w, diff_w, MORPH_OPEN, kernel);
   //  cv::imshow("After opening", difference);
   
   morphologyEx(diff_w, diff_w,MORPH_CLOSE, kernel);
   imshow("After closing", diff_w);
   Mat diff_w_gray, diff_b_gray;
   cvtColor( diff_w, diff_w_gray, CV_BGR2GRAY );
   cvtColor( diff_b, diff_b_gray, CV_BGR2GRAY );
   //threshold( diff_w_gray, diff_w_gray,30, 255,THRESH_BINARY_INV  ); 
   threshold( diff_w_gray, diff_w_gray, 20,255,THRESH_BINARY);
   imshow("After thresh",diff_w_gray);
   
   cvtColor(diff_b_gray, diff_b_gray, COLOR_GRAY2BGR);
   imshow("After gray",diff_b_gray);
   Mat difference;
   cvtColor(diff_w_gray, difference, CV_GRAY2BGR );
   
   
   
   // Split image in rectangles to detect presence of cells
   int rect_size = size / 8;
   
  
   // Initialize bitboard
  unsigned long w_pieces = 0;
  
  unsigned long b_pieces = 0;
  
  // Loop by line
  for(int j = 7; j >= 0; --j) {
    // Computing Y coordinate
    int rect_y = rect_size * (7-j);
    // Loop by column
    for (int i = 0; i < 4; ++i) {
      // Computing X coordinate
      int rect_x = 2 * rect_size * i + (j % 2) * rect_size;
      
      // Computing cell index
      int index = 4*j + i;
      unsigned long cell = 1 << index;
      
      // Fetch cell
      Mat roi = difference(Rect(rect_x, rect_y, rect_size, rect_size));
      
      // Fetch cell
      Mat roi_b = diff_b(Rect(rect_x, rect_y, rect_size, rect_size));
      // cv::imshow(std::to_string(num), roi);
      
      // Convert to HSV to get Value
      int threshold_r = 23;
      int threshold_v = 30;
      Scalar mean_bgr = mean(roi);
      cvtColor(roi, roi, COLOR_BGR2HSV);
      Scalar mean_hsv =mean(roi);
      
      //black
      Scalar mean_bgr_b = mean(roi_b);
      cvtColor(roi_b, roi_b, COLOR_BGR2HSV);
      Scalar mean_hsv_b = mean(roi_b);
      
      // If value is above threshold, a piece is present
      cout << index << " : (" << rect_x << ", " << rect_y << ")" << endl;
      // std::cout << "    BGR" << mean_bgr << " HSV" << mean_hsv;
      if (mean_hsv.val[2] > threshold_v) {
	cout << "    BGR" << mean_bgr << " HSV" << mean_hsv;
              cout << "WHITE PIECE FOUND";
              w_pieces |= cell;
      }
	    else if (mean_hsv_b.val[2] > threshold_r) {
	      cout << "    BGR b " << mean_bgr_b << " HSV b" << mean_hsv_b;
              cout << "BLACK PIECE FOUND";
              b_pieces |= cell;
	    }
	    else
	      cout << "    BGR" << mean_bgr_b << " HSV" << mean_hsv_b;
      cout << endl;
    }
  }
  
  cout << "(Black) BOARD : " << bitset<32>(b_pieces) << " (" << b_pieces << ")" << endl;

  cout << "(White) BOARD : " << bitset<32>(w_pieces) << " (" <<w_pieces << ")" << endl;

  waitKey(0);
  return EXIT_SUCCESS;
}

// Main program entry point
int main(int argc, char *argv[]) {
  if (argc != 4 && argc != 5) {
    cout << "Usage : checkersvision create <image> <reference> <size> - To create reference file (of size*size) from image" << endl;
    cout << "        checkersvision parse <image> <reference> - To parse an image in comparison to reference" << endl;
      return EXIT_FAILURE;
  }

  string mode = argv[1];

  if (mode == "create") return createReference(argv[2], argv[3], atoi(argv[4]));
  else if (mode == "parse") return getPieces(argv[2], argv[3]);
  else {
     cout << "Unknown mode '" << mode << "', see ./checkersvision for usage." << endl;
      return EXIT_FAILURE;
  }

}
