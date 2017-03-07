//
// Desribes a generic circle that can be drawn onto an OpenCV frame
//

using namespace std;
using namespace cv;

const int DEFAULT_CIRCLE_RADIUS = 20;

class Circle {
public:
  Circle(int, int, int, Scalar, int);
  void draw(Mat);
  void setX(int);
  void setY(int);
  void setR(int);
  void setFill(int);
  void setRgb(Scalar);
private:
  int x, y, radius, fill;
  Scalar rgb;
};

Circle::Circle(int x, int y, int r = DEFAULT_CIRCLE_RADIUS, Scalar rgb = Scalar(255, 0, 0), int fill = -1) {
  setX(x);
  setY(y);
  setR(r);
  setRgb(rgb);
  setFill(fill);
};

void Circle::draw(Mat frame) {
  circle(frame, Point(x, y), radius, rgb, fill);
}

void Circle::setX(int cx) {
  x = cx;
}

void Circle::setY(int cy) {
  y = cy;
}

void Circle::setR(int r) {
  radius = r;
}

void Circle::setRgb(Scalar color) {
  rgb = color;
}

void Circle::setFill(int f) {
  fill = f;
}