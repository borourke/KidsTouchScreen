//
// Tracks an object on screen.
// TODO - needs major refactoring and cleaning up
//
//initial min and max HSV filter values.
using namespace std;
using namespace cv;

int H_MIN = 120;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 120;
int V_MAX = 256;
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = 800*1280/1.5;
const int MAX_TRAILING_OBJECTS = 10;
int trailing_xs[MAX_TRAILING_OBJECTS];
int trailing_ys[MAX_TRAILING_OBJECTS];
int array_size = MAX_TRAILING_OBJECTS;

class objectTracker {
public:
  objectTracker(Mat);
  void track(int);
private:
  int x, y;
  Mat HSVFrame;
  Mat binaryFrame;
  Mat rawFrame;
  Mat resultFrame;
  void setRawFrame(Mat);
  void setBinaryFrame();
};

objectTracker::objectTracker(Mat rawVideo) {
  setRawFrame(rawVideo);  // Copy video feed to Mat object
  resultFrame = Mat::zeros(rawFrame.rows, rawFrame.cols, CV_8UC3); // Results frame always starts as black screen
  cvtColor(rawFrame, HSVFrame, COLOR_BGR2HSV); // Next convert the rawFrame Mat into an HSV Mat
}

void objectTracker::setBinaryFrame() {
  //
  // First find the HSV values in range of set detection patterns
  // Then erode and dilate to keep only strong matches
  //
  inRange(HSVFrame, Scalar(H_MIN,S_MIN,V_MIN), Scalar(H_MAX,S_MAX,V_MAX), binaryFrame);
  Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3,3));
  Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8,8));
  erode(binaryFrame, binaryFrame, erodeElement);
  erode(binaryFrame, binaryFrame, erodeElement);
  dilate(binaryFrame, binaryFrame, dilateElement);
  dilate(binaryFrame, binaryFrame, dilateElement);
}

void objectTracker::setRawFrame(Mat rawVideo) {
  rawFrame = rawVideo;
}

//
// Shape type can be [0 - circle]
//
void objectTracker::track(int shapeType) {
  setBinaryFrame();
  flip(binaryFrame, binaryFrame, 1);
  //these two vectors needed for output of findContours
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(binaryFrame, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
  //use moments method to find our filtered object
  double refArea = 0;
  bool objectFound = false;
  if (hierarchy.size() > 0) {
    int numObjects = hierarchy.size();
    for (int index = 0; index >= 0; index = hierarchy[index][0]) {
      Moments moment = moments((cv::Mat)contours[index]);
      double area = moment.m00;

      //if the area is less than 20 px by 20px then it is probably just noise
      //if the area is the same as the 3/2 of the image size, probably just a bad filter
      //we only want the object with the largest area so we safe a reference area each
      //iteration and compare it to the area in the next iteration.
      if(area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA && area > refArea){
        x = moment.m10/area;
        y = moment.m01/area;
        refArea = area;
        objectFound = true;
      }
    }
    //
    // Bump trailing xs and ys to bump the oldest value and take in these new values.
    // Then iterate over all the values drawing circles.
    //
    // TODO: Optimize, we only need one loop if you think about it
    //
    if (objectFound) {
      for (int i = array_size - 1; i > 0; i--) {
        trailing_xs[i] = trailing_xs[i - 1];
        trailing_ys[i] = trailing_ys[i - 1];
      }
      trailing_xs[0] = x;
      trailing_ys[0] = y;
      for (int i = 0; i <= array_size - 1; i++) {
        int circle1x = trailing_xs[i];
        int circle1y = trailing_ys[i];
        Circle circle(circle1x, circle1y);
        circle.draw(resultFrame);
      }
    }
  }
  imshow("Black Screen of Death", resultFrame);
}