//objectTracking.cpp

//Written by Bryan O'Rourke 2017
#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include "shapes/circle.hpp"
#include "objectTracker.hpp"

using namespace std;
using namespace cv;

// Window width and height
const int FRAME_WIDTH = 1280;
const int FRAME_HEIGHT = 800;

int main(int argc, char* argv[])
{
  cout << "Yo dog what's up" << endl;
  //
  // Start by opening a new window that has the cam as its feed
  //
  VideoCapture capture;
  capture.open(0);
  capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

  Mat cameraFeed;
  //Mat displayFeed;

  //
  // Create a daemon to watch the screen as it refreshes
  //
  while(1) {    
    //
    // Detect and track the object
    //
    capture.read(cameraFeed);
    objectTracker tracker(cameraFeed);
    tracker.track(0);
    //imshow("Black Screen of Death", displayFeed);

    //delay 30ms so that screen can refresh.
    //image will not appear without this waitKey() command
    waitKey(5);
  }

  return 0;
}
