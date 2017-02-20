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
const int FRAME_WIDTH = 600;
const int FRAME_HEIGHT = 400;

void morphOps(Mat &thresh){
  Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3,3));
  Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8,8));

  erode(thresh, thresh, erodeElement);
  erode(thresh, thresh, erodeElement);

  dilate(thresh, thresh, dilateElement);
  dilate(thresh, thresh, dilateElement);
}

int main(int argc, char* argv[])
{
  cout << "Yo dog what's up" << endl;
  //
  // Start by opening a new window that has the cam as its feed
  //
  Mat cameraFeed;
  Mat black_image = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);
  Mat HSV;
  Mat threshold;

  VideoCapture capture;
  capture.open(0);
  capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
  //
  // Create a daemon to watch the screen as it refreshes
  //
  while(1) {
    //
    // First do normal camera feed
    //
    capture.read(cameraFeed);
    imshow("Normal Camera Feed", cameraFeed);

    //
    // Then do HSV version of the feed
    //
    cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
    imshow("HSV Feed", HSV);

    //
    // Next binary (black and white) feed
    //
    imshow(windowName2,threshold);

    //
    // Next create a black screen
    //
    inRange(HSV, Scalar(H_MIN,S_MIN,V_MIN), Scalar(H_MAX,S_MAX,V_MAX),threshold);
    imshow("Black Screen of Death", black_image);

    //delay 30ms so that screen can refresh.
    //image will not appear without this waitKey() command
    waitKey(30);
  }

  return 0;
}
