//objectTracking.cpp

//Written by Bryan O'Rourke 2017

#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
//initial min and max HSV filter values.
int H_MIN = 120;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 120;
int V_MAX = 256;
// Window width and height
const int FRAME_WIDTH = 1280;
const int FRAME_HEIGHT = 800;
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
const int MAX_TRAILING_OBJECTS = 10;

int trailing_xs[MAX_TRAILING_OBJECTS];
int trailing_ys[MAX_TRAILING_OBJECTS];
int array_size = MAX_TRAILING_OBJECTS;

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &result){
  Mat temp;
  flip(threshold, temp, 1);
  //these two vectors needed for output of findContours
  vector< vector<Point> > contours;
  vector<Vec4i> hierarchy;
  //find contours of filtered image using openCV findContours function
  findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
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
      if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
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
      putText(result,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
      for (int i = array_size - 1; i > 0; i--) {
        trailing_xs[i] = trailing_xs[i - 1];
        trailing_ys[i] = trailing_ys[i - 1];
      }
      trailing_xs[0] = x;
      trailing_ys[0] = y;
      for (int i = 0; i <= array_size - 1; i++) {
        int circle1x = trailing_xs[i];
        int circle1y = trailing_ys[i];
        circle(result, Point(circle1x, circle1y), 20, Scalar(255, 0, 0), -1);
      }
    }
  }
}

int main(int argc, char* argv[])
{
  cout << "Yo dog what's up" << endl;
  //
  // Start by opening a new window that has the cam as its feed
  //
  Mat cameraFeed;
  VideoCapture capture;
  capture.open(0);
  capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

  Mat black_image;
  Mat HSV;
  Mat threshold;

  int x=0, y=0;
  //
  // Create a daemon to watch the screen as it refreshes
  //
  while(1) {
    //
    // First do normal camera feed
    //
    capture.read(cameraFeed);

    //
    // Next create a black screen
    //
    black_image = Mat::zeros(cameraFeed.rows, cameraFeed.cols, CV_8UC3);

    //
    // Convert feed to HSV for easier color selection
    //
    cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

    //
    // Next binary (black and white) feed
    //
    //
    // First find the HSV values in range of set detection patterns
    // Then erode and dilate to keep only strong matches
    //
    inRange(HSV, Scalar(H_MIN,S_MIN,V_MIN), Scalar(H_MAX,S_MAX,V_MAX), threshold);
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3,3));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8,8));
    erode(threshold, threshold, erodeElement);
    erode(threshold, threshold, erodeElement);
    dilate(threshold, threshold, dilateElement);
    dilate(threshold, threshold, dilateElement);

    //
    // Next we track the filtered object
    //
    trackFilteredObject(x, y, threshold, black_image);
    imshow("Black Screen of Death", black_image);

    //delay 30ms so that screen can refresh.
    //image will not appear without this waitKey() command
    waitKey(30);
  }

  return 0;
}
