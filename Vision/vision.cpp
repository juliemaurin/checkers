#include "vision.h"

static Mat mask;
static string pieces_valid;
static bool is_robot_moving = false;
static int vision_timer = 0;

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
  int margin=30;
  Point pts[4];
  mask=image.clone();
  for (size_t i = 0; i <4 ; ++i)
    pts[i]=input_quad[i];
  
  //Security mask : Top-Right ______begin
  pts[1].y=0; pts[1].x=(int)input_quad[1].x-margin;
  pts[2].y=0;
  //Right
  pts[2].x=image.cols;
  pts[3].x=image.cols; pts[3].y=(int)input_quad[3].y+margin;
  //Make the mask larger 
  pts[0].x=max(0,(int)(input_quad[0].x-margin));
  pts[0].y=min(image.rows,(int)(input_quad[0].y+margin));
  //Security mask : Top-Right ______end
  

  
  //Security mask : Top-Left  ______begin
  /*
    pts[1].y=0; 
    pts[2].y=0; pts[2].x=input_quad[2].x+margin;
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

int createReference(const string &emptyname, const string &fullname, const int& size) {
  // Load the image
  Mat image = imread(emptyname, CV_LOAD_IMAGE_COLOR);
  Mat full = imread(fullname, CV_LOAD_IMAGE_COLOR);
   
  if ((image.empty())||(full.empty())){
    cerr<<"Empty References" <<endl;
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
  string thresh = getPieces(full,0,1);
  cerr<<"Best thresh value = "<<thresh<<endl;
  waitKey(0);
  return EXIT_SUCCESS;
}



string getPieces(Mat &image, int black_thresh , int calib) {

  
  // Load reference image
  Mat reference = imread("Reference.jpg", CV_LOAD_IMAGE_COLOR);
  if (reference.empty()){
      throw std::runtime_error("getPieces : Empty reference");
  }
  int threshold_r, threshold_v;
  int size = reference.size().height;
  //Resize image to be visible in all cases
  resize(image, image, reference.size(), 0, 0, CV_INTER_LINEAR);

  // Output image
  Mat output = transformImage(image, size);
  
  //Display output
    imshow("Output", output);
  //imshow("Reference", reference);
  //waitKey(0);


  Mat diff_w,diff_b ;
  // Compute difference between image and reference
  absdiff(output , reference, diff_b);
  diff_w = output - reference;
  
  //Contrast treatement
  Mat contrast_diff1,contrast_ref,contrast_black;
  double alpha = 10; //< Simple contrast control
  int beta = 0;  //< Simple brightness control
  diff_b.convertTo(contrast_diff1, -1, alpha, beta);

  imshow("Contrast", contrast_diff1);
  waitKey(1);

  // Morphological opening and closing to filter noise
  int morph_size = size / 20;
  Mat kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(morph_size, morph_size)) * 255;
  
  //black morph
  morphologyEx(contrast_diff1,contrast_diff1,MORPH_CLOSE, kernel);
  morphologyEx(contrast_diff1,contrast_diff1,MORPH_OPEN, kernel);

  //White  morph
  morphologyEx(diff_w, diff_w, MORPH_OPEN, kernel);
  morphologyEx(diff_w, diff_w,MORPH_CLOSE, kernel);


  //convert to black difference to hsv
  Mat img_hsv;
  cvtColor(contrast_diff1, img_hsv, CV_BGR2HSV);

  imshow("Image HSV", img_hsv);
  waitKey(1);

  //convert to white difference to gray then threshold
  Mat diff_w_gray;
  cvtColor( diff_w, diff_w_gray, CV_BGR2GRAY );
  threshold( diff_w_gray, diff_w_gray, 20,255,THRESH_BINARY);

  imshow("Thresholded image", diff_w_gray);
  waitKey(1);

  //return white difference to BGR
  Mat difference;
  cvtColor(diff_w_gray, difference, CV_GRAY2BGR );


  // Split image in rectangles to detect presence of cells
  //Rectangle size
  int rect_size = size / 8;
  
  // Initialize bitboard
  uint32_t w_pieces = 0;
  uint32_t b_pieces = 0;
  //number of pieces detected with a doubt
  int doubt_count =0;

  //min & max to compute the appropriate thres_value for the black detection
  int min=255;
  int max=0;
  
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
      // Fetch cell : white
      Mat roi = difference(Rect(rect_x, rect_y, rect_size, rect_size));
      
      // Fetch cell : black
      Mat roi_b = img_hsv(Rect(rect_x, rect_y, rect_size, rect_size));
      

      int white_thresh=10;

      // Convert to HSV for
      cvtColor(roi, roi, COLOR_BGR2HSV);

      //white
      Scalar mean_w =mean(roi);
      
      //black
      Scalar mean_b = mean(roi_b);

      //min black pieces
      if((index<12)&&(index>=0)){
	if(min>mean_b.val[2])
	  min=mean_b.val[2];
       }


      //max empty boxes
      if((index<20)&&(index>=12)){
	if(max<mean_b.val[2])
	  max=mean_b.val[2];
       }
      
      string doubt;
      int doubt_margin=8;
      
      // If value is above threshold, a piece is present
      cout << index << " : (" << rect_x << ", " << rect_y << ")" << endl;
      //Detect a white piece
      if (mean_w.val[2] > white_thresh) {
	doubt="";
	if (abs(mean_w.val[2] - white_thresh) < doubt_margin){
	  doubt="With doupt";
	  doubt_count++;
	}
	cout << "    mean_White" << mean_w;
	cout << "WHITE PIECE FOUND  "<< doubt;
	w_pieces |= cell;
      }
      else  //Detect a white piece
	if (mean_b.val[2] > black_thresh) {
	  doubt="";
	  if (abs(mean_b.val[2] - black_thresh) < doubt_margin){
	    doubt="With doupt";
	    doubt_count++;
	  } 
	  cout << "    mean Black " << mean_b ;
	  cout << "BLACK PIECE FOUND  " << doubt;
	  b_pieces |= cell;
	} else   //Detect an empty boxe
	  cout << "   mean Empty" << mean_b;
      cout << endl;
    }
  }
  
  
 
  //Done in calib mode
  if(calib){
  int thresh = max+ (min-max)*3/4;
  return to_string(thresh);
  }
  //Done in the parse modes
 else{
   cout << "(Black) BOARD : " << bitset<32>(b_pieces) << " (" << b_pieces << ")" << endl;
   cout << "(White) BOARD : " << bitset<32>(w_pieces) << " (" << w_pieces << ")" << endl;
   string pieces= to_string(w_pieces)+"+"+ to_string(b_pieces);
   cerr<<" white + Black (sent to AI) : "<<pieces<<" With "<<doubt_count<<" doubts"<<endl;
   
   return pieces; 
 }

}
 
 
//Compare an image to the security mask
int security(Mat src){
  int warning=0;
  Scalar m;
  double maxValue = 255;
  Mat thres,diff;


  //Calibration params 
  int security_coef=5;
  int morph_size = 10;
  double thresh =30;



  //load security reference
  Mat security_ref=imread("security_mask.jpg");
  //check if it was found successfully
  if(security_ref.empty()){
    cerr<<"Empty Security reference"<<endl;
    return 1 ;
  }

  //convert to gray
  cvtColor(security_ref,security_ref,CV_BGR2GRAY);
  cvtColor(mask,mask,CV_BGR2GRAY);
  // difference
  absdiff(mask,security_ref, diff);
  // Binary Threshold
  threshold(diff,thres, thresh, maxValue, THRESH_BINARY);

  // Morphological opening and closing to filter noise
 
  Mat kernel = getStructuringElement(CV_SHAPE_ELLIPSE, Size(morph_size, morph_size)) * 255;
  
  //  imshow("difference", mask);
  // waitKey(0);
  //imshow("difference",diff);
  //waitKey(0);
  // imshow("difference",thres);
  //waitKey(0);
  
  // morph
  morphologyEx(thres,thres,MORPH_OPEN, kernel);
  // imshow("difference",thres);
  //waitKey(0);

  morphologyEx(thres,thres,MORPH_CLOSE, kernel);
  //imshow("difference",thres);
  //waitKey(0);
  
  m = mean (thres);
  cerr<<"security mean"<<m<<endl;
  if(m[0]>security_coef){
    warning=1;
    imshow ("Board",thres);
    waitKey(1);
    cerr<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    cerr<<"An object is crossing the security Area "<<m[0]<<endl;
    cerr<<endl;
  }
  return warning;
}

int imageGetPieces(const string &filename, int black_thresh) {
  Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
  string pieces= getPieces(image, black_thresh,0);
  waitKey(0);
  return EXIT_SUCCESS;
}

int videoCalib() {
    VideoCapture cap(1);

    if(!cap.isOpened())
      throw std::runtime_error("VideoCapture : could not open camera");

    std::cout << "Please press a key then enter when you have finished clearing the board (1/2)" << std::endl;
    std::string c;
    std::cin >> c;
    std::cin.clear();

    Mat empty;
    cap >> empty;
    imshow("Empty", empty);
    imwrite("calib_empty.jpg", empty);

    cap.release();

    std::cout << "Please press a key then enter when you have finished filling the board (2/2)" << std::endl;
    std::cin >> c;
    std::cin.clear();

    cap.open(1);

    Mat full;
    cap >> full;
    imshow("Full", full);
    imwrite("calib_full.jpg", full);

    cap.release();

    waitKey(0);

    return createReference("calib_empty.jpg", "calib_full.jpg", 512);
}

int stat(const string &directory, int black_thresh){
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
  
  for(size_t i = 0; i < filenames.size(); ++i){
    Mat src = imread(directory + filenames[i]);
    if(!src.data) { //Protect against no file
      cerr << directory + filenames[i] << ", file #" << i << ", is not an image" << endl;
      continue;
    }
    string pieces= getPieces(src,black_thresh,0);
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

    imshow("Board", src);
    waitKey(50);

    int warning = security(src); // 1:an object was detected; 0 : clear
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
            if (data.front() == 'M') {
                is_robot_moving = true;
                soc.send("ACK Moving");
            } else if (data.front() == 'E') {
                is_robot_moving = false;
                soc.send("ACK End");
            } else if (data.front() == 'R') {
                std::cout << "Sending pieces data" << std::endl;
                soc.send(pieces_valid);
                cout << "The message : "<< pieces_valid <<" was sent to the Game"<<endl;
            } else if (data.front() == 'T') {
                while(vision_timer < 50);
                vision_timer = 0;
                soc.send("ACK PLAYER");
            } else {
                cout << "Unknown message : " << data << endl;
            }
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

int videoGetPieces(int black_thresh) {
    Mat image;
    VideoCapture cap(1); // open the default camera

    if(!cap.isOpened())
      throw std::runtime_error("VideoCapture : could not open camera");
  
    string requestForPieces = "1";

    thread st(socket_thread);

    int warning = 0;

    while(1) {
        // Get a new frame
        Mat image;
        cap >> image; // get a new frame from camera
        string pieces = getPieces(image, black_thresh, 0);

        int old_warn = warning;
        warning = security(image);

        if (warning || (!warning && old_warn)) {
            vision_timer = 0;
        } else if (!warning && !old_warn && !is_robot_moving) {
            vision_timer++;
        }

        std::cout << "T = " << vision_timer << std::endl;

        if (warning && is_robot_moving) {
            std::cout << "SECURITY ERROR" << std::endl;
            system("python emergency.py");
            throw std::runtime_error("SECURITY ERROR");
          } else {
            pieces_valid = pieces;
          }

        //show captured frame from the video
        imshow("Video feed", image);
        waitKey(1);
    } 
    return EXIT_SUCCESS;
}
