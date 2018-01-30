#include "vision.h"

static Mat mask;

// onMouse event callback function
void onMouse(int event, int x, int y, int, void* data) {
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

  
  // Open file read mode
  ifstream fichier(fichier_ref.c_str(), ios::in);
  //if file opened with success
  if(fichier) {
    string line;
    //if the file is empty   // get the reference manually
    if(!getline(fichier, line)) {
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
	
	// The 4 points that select quadilateral on the input , from bottom-left in clockwise order
	// These four pts are the sides of the rect box used as input
	// This step is to tranform our vect to an array (ugly)
	// cout << "Angle coordinates : " << endl;
      }
      
      // Open the file in write mode to save the corners pos
      ofstream fwrite(fichier_ref.c_str(), ios::out | ios::trunc);
	  
      if(fwrite)  //if fwrite opened with success
	{ //write the coordinates in the file
	  for (size_t i = 0; i < input_vect.size(); ++i) {
	    input_quad[i] = input_vect[i];
	    // We also display point coordinates in stdout
	    fwrite <<  input_quad[i].x<<" "<<input_quad[i].y << endl;
	    //		cout << "Point " << i + 1 << " : (" << input_quad[i].x << ", " << input_quad[i].y << ")" << endl;
	  }
	  
	  //close fwrite
	  fwrite.close();
	}//end fwrite opened with success
      else 
	cerr << "Erreur à l'ouverture en écriture !" << endl;
    }//end if fichier empty
    //File not empty
    else {
      //return to the file's beginning
      fichier.seekg(0, ios::beg);
      //cout << endl << "On se trouve au " << fichier.tellg() << "ieme octet." << endl;
      //read coordinates from the file
      string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
      try {
	for (size_t i = 0; i <4; ++i) {
	  fichier >> contenu;//to get the first string
	  input_quad[i].x = stoi(contenu, NULL, 0);
	  //cout << "x" << i << "=" << contenu;
	  fichier >> contenu;//to get the first string
	  input_quad[i].y = stoi(contenu, NULL, 0);
	  //  cout << " ,y" << i << "=" << contenu << endl;
	}
      } catch(std::invalid_argument& e) {
	cerr<<"No covert stoi"<<endl;
      }
      
      fichier.close();  // file closed
    }
  }//end if fichier opened
  else
    cerr << "Erreur à l'ouverture en lecture!" << endl;
  
  Point pts[4];
  mask=image.clone();
  for (size_t i = 0; i <4 ; ++i)
    pts[i]=input_quad[i];
  
  //Security mask : Top-Right ______begin
  pts[1].y=0; pts[1].x=(int)input_quad[1].x-30;
  pts[2].y=0;
  //Right
  pts[2].x=image.cols;
  pts[3].x=image.cols; pts[3].y=(int)input_quad[3].y+30;
  //Make the mask larger 
  pts[0].x=max(0,(int)(input_quad[0].x-30));
  pts[0].y=min(image.rows,(int)(input_quad[0].y+30));
  //Security mask : Top-Right ______end
  

  
  //Security mask : Top-Left  ______begin
  /*
    pts[1].y=0; 
    pts[2].y=0; pts[2].x=input_quad[2].x+30;
    //Left
    pts[1].x=0;  pts[0].x=0; 
  */
  //Security mask : Top-Left  ______end

  
  //create security mask
  fillConvexPoly(mask, pts,4,Scalar(255,255,255) );
  
  // Get the Perspective Transform Matrix i.e. lambda
  Mat lambda = getPerspectiveTransform(input_quad, output_quad);
  
  // Apply the Perspective Transform just found to the src image
  Mat output;
  warpPerspective(image, output, lambda, Size(size, size));
  return output;
}

int createReference(const string &filename, const string &refname, const int& size) {
  // Load the image
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
  if (image.empty()){
    cerr<<"Empty Reference" <<endl;
    return 1;
  }
 
  ofstream f(fichier_ref, ios::out|ios::trunc);//empty the file if it does exist & create it if it doesn't 
  f.close(); //close the file
  //Resize image
  resize(image, image, Size(size,size), 0, 0, CV_INTER_LINEAR);
    
  // Output image
  Mat output = transformImage(image, size);

  //!! The security area should be clear in the first image !!
  //Display the security reference
  imshow("Security Mask",mask);
  imwrite ("security_mask.jpg",mask);
  waitKey(0);
  
  //Display output
  imshow("Reference",output);
  imwrite("Reference.jpg", output);
  
  waitKey(0);
  return EXIT_SUCCESS;
}



string getPieces(Mat &image) {

  // Load reference image
  Mat reference = imread("Reference.jpg", CV_LOAD_IMAGE_COLOR);
  if (reference.empty()){
    cerr<<"Empty reference" <<endl;
    return 0;
  }

  int size = reference.size().height;
  //Resize image to be visible in all cases
  resize(image, image, reference.size(), 0, 0, CV_INTER_LINEAR);

  // Output image
  Mat output = transformImage(image, size);
  
  //Display output
  //imshow("Output", output);
  //imshow("Reference", reference);
  
  Mat diff_w,diff_b, white ;
  
  // Compute difference between image and reference
  absdiff(output , reference, diff_b);
  diff_w = output - reference;
  
  //imshow("Black Difference", diff_b);
   
  //Contrast treatement
  Mat contrast_diff1,contrast_ref,contrast_black;
  double alpha = 10; //< Simple contrast control
  int beta = 0;  //< Simple brightness control
  diff_b.convertTo(contrast_diff1, -1, alpha, beta);
  //imshow("contrast apres premier difference",contrast_diff1);
  
  // Morphological opening and closing to filter noise
  int morph_size = size / 20;
  Mat kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(morph_size, morph_size)) * 255;
  morphologyEx(diff_w, diff_w, MORPH_OPEN, kernel);
  //  cv::imshow("After opening", difference);
  morphologyEx(diff_w, diff_w,MORPH_CLOSE, kernel);
  //imshow("After closing", diff_w);
  
  Mat diff_w_gray, diff_b_gray;
  cvtColor( diff_w, diff_w_gray, CV_BGR2GRAY );
  // cvtColor( diff_b, diff_b_gray, CV_BGR2GRAY );
  //threshold( diff_w_gray, diff_w_gray,30, 255,THRESH_BINARY_INV  );
  threshold( diff_w_gray, diff_w_gray, 20,255,THRESH_BINARY);
  //imshow("After thresh",diff_w_gray);

  // threshold( diff_b_gray, diff_b_gray, 15,255,THRESH_BINARY);
  // imshow("After thresh",diff_b_gray);
  //waitKey(0);
  //cvtColor(diff_b_gray, diff_b_gray, COLOR_GRAY2BGR);
  //imshow("After gray",diff_b_gray);
  Mat difference;
  cvtColor(diff_w_gray, difference, CV_GRAY2BGR );

  // Split image in rectangles to detect presence of cells
  int rect_size = size / 8;

  // Initialize bitboard
  uint32_t w_pieces = 0;
  uint32_t b_pieces = 0;
  int doubt_count =0;
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
      uint32_t cell = 1 << index; 
      // Fetch cell
      Mat roi = difference(Rect(rect_x, rect_y, rect_size, rect_size));
      
      // Fetch cell
      Mat roi_b = diff_b(Rect(rect_x, rect_y, rect_size, rect_size));
      // cv::imshow(std::to_string(num), roi);
      
      // Convert to HSV to get Value
      int threshold_r = 15;
      int threshold_v = 10;
      Scalar mean_bgr = mean(roi);
      cvtColor(roi, roi, COLOR_BGR2HSV);
      Scalar mean_hsv =mean(roi);
      
      //black
      Scalar mean_bgr_b = mean(roi_b);
      cvtColor(roi_b, roi_b, COLOR_BGR2HSV);
      Scalar mean_hsv_b = mean(roi_b);
      string doubt;
      
      // If value is above threshold, a piece is present
        cout << index << " : (" << rect_x << ", " << rect_y << ")" << endl;
      // std::cout << "    BGR" << mean_bgr << " HSV" << mean_hsv;
      if (mean_hsv.val[2] > threshold_v) {
	doubt="";
	if (abs(mean_hsv.val[2]- threshold_v)<5)
	  { doubt="With doupt";
	    doubt_count++;
	  }
	cout << "    BGR" << mean_bgr << " HSV" << mean_hsv;
	cout << "WHITE PIECE FOUND  "<< doubt;
        w_pieces |= cell;
      }
      else
	if (mean_hsv_b.val[2] > threshold_r) {
	  doubt="";
	  if (abs(mean_hsv_b.val[2]- threshold_r)<5)
	    { doubt="With doupt";
	      doubt_count++;
	    }
	  cout << "    BGR b " << mean_bgr_b << " HSV b" << mean_hsv_b;
	  cout << "BLACK PIECE FOUND  " << doubt;
	  b_pieces |= cell;
	} else
	  cout << "    BGR" << mean_bgr_b << " HSV" << mean_hsv_b;
	   cout << endl;
	}
  }
  
  cout << "(Black) BOARD : " << bitset<32>(b_pieces) << " (" << b_pieces << ")" << endl;
  cout << "(White) BOARD : " << bitset<32>(w_pieces) << " (" << w_pieces << ")" << endl;
  string pieces= to_string(w_pieces)+"+"+ to_string(b_pieces);
  cerr<<" white + Black (sent to AI) : "<<pieces<<" With "<<doubt_count<<" doubts"<<endl;
  
  return pieces;
}


//Compare an image to the security mask
int security(Mat src){
  int security_coef, warning=0;
  Scalar m;
  Mat diff;
  Mat security_ref=imread("security_mask.jpg");
  if(security_ref.empty()){
    cerr<<"Empty Security reference"<<endl;
    return 1 ;
  }
  cvtColor(security_ref,security_ref,CV_BGR2GRAY);
  cvtColor(mask,mask,CV_BGR2GRAY);
  imshow("Board", src);
  waitKey(50);
  diff=abs(mask-security_ref);
  
  //security_coef=1; //log9
  security_coef=7; //log7
  
  m = mean (diff);
  if(m[0]>security_coef){
    warning=1;
    imshow ("Board",diff);
    waitKey(50);
    cerr<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    cerr<<"An object is crossing the security Area "<<m[0]<<endl;
    cerr<<endl;
  }
  return warning;
}

int imageGetPieces(const string &filename) {
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
  string pieces= getPieces(image);
  waitKey(0);
  return EXIT_SUCCESS;
}

int stat(const string &directory,const string &refname){
  string result = "None";
  int error = 0;
  int success = 0;
  
  cout << "Reading in directory " <<directory << endl;
  // GetFilesInDirectory
  vector<string> filenames;
  DIR *dir;
  class dirent *ent;
  class stat st;
  
  dir = opendir(directory.c_str());
  while ((ent = readdir(dir)) != NULL) {
    const string file_name = ent->d_name;
    const string full_file_name = directory + "/" + file_name;
    
    if (file_name[0] == '.')
      continue;
    
    if (stat(full_file_name.c_str(), &st) == -1)
      continue;
    
    const bool is_directory = (st.st_mode & S_IFDIR) != 0;
    
    if (is_directory)
      continue;
    
    // filenames.push_back(full_file_name); // returns full path
    filenames.push_back(file_name); // returns just filename
  }
  closedir(dir);
  
  int num_files=filenames.size(); // how many we found
  //cout << "Number of files = " << num_files << endl;
  
  for(size_t i = 0; i < filenames.size(); ++i){
    Mat src = imread(directory + filenames[i]);
    if(!src.data) { //Protect against no file
      cerr << directory + filenames[i] << ", file #" << i << ", is not an image" << endl;
      continue;
    }
    string pieces= getPieces(src);
    string p_name=filenames[i];
    //Remove the file extension
    if (p_name.size () > 0)  p_name.resize (p_name.size () - 4);
    if(pieces.compare(p_name)==0){      
      result=" Success";
      success++; 
    }
    else{
      result=" Error";
      error++;
    }
    cerr<<"Pieces="<<pieces<<endl;
    cerr<<"Name  ="<<p_name<<endl;
    cerr<<"Result = " << result << endl;
    cerr<<endl;
    //control security area
    int warning=security(src); // 1:an object was detected; 0 : clear 
  }
  cerr<<endl;
  cerr << "Total number of test images   : "<<num_files<<endl;
  cerr<< "   Number of successful tests : "<<success<<endl;
  cerr<< "   Number of failed tests     : "<<error<<endl;
}

void socket_thread() {
  cout << "Starting socket thread" << endl;
  TCPHelper soc = TCPHelper("127.0.0.1", "20000");

  while(1) {
      cout << "Waiting for connection" << endl;
      soc.connect();

      while(1) {
        std::cout << "Waiting for incoming data" << std::endl;
        string data = soc.receive();

        // If we receive data
        if(data.length() > 0) {

            // CAPTURE LAST PROCESSED DATA
            string pieces = "305419896+4294967295"; // For testing purposes

            std::cout << "Sending pieces data" << std::endl;
            soc.send(pieces);
            cout << "The message : "<< pieces <<" was sent to the Game"<<endl;
        } else {
          cout << "Nothing else to read" << endl;
            break;
        }
      }

      std::cout << "Disconnecting client" << std::endl;
      soc.disconnect();
  }

  soc.closeSocket();
}

int videoGetPieces(const string &refname) {
    Mat image;
    VideoCapture cap(0); // open the default camera

    if(!cap.isOpened())
      throw std::runtime_error("VideoCapture : could not open camera");
  
    string requestForPieces = "1";
    thread st(socket_thread);

    while(1) {
        Mat image;
        cap >> image; // get a new frame from camera
        string pieces = getPieces(image, refname);

        //show captured frame from the video
        imshow("board", image);
        waitKey(1);

        std::cout << "Disconnecting client" << std::endl;
    } 
    return EXIT_SUCCESS;
}
