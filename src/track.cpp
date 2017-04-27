#include <raspicam/raspicam_cv.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

using namespace std;

class track_main {
	public:


	private:
		void react(float x, float y);
		void detectFace(cv::Mat &frame, cv::CascadeClassifier &face_cascade, vector<cv::Rect> &faces);
		

};

void react(float x, float y);
void detectFace(cv::Mat &frame, cv::CascadeClassifier &face_cascade, vector<cv::Rect> &faces);

void detectFace(cv::Mat &frame, cv::CascadeClassifier &face_cascade, vector<cv::Rect> &faces) {
  cv::Mat grayscale;

  // Convert frame to grayscale, normalize the brightness, and increase the contrast
  cv::cvtColor(frame, grayscale, cv::COLOR_BGR2GRAY);
  cv::equalizeHist(grayscale, grayscale);

  // Detect faces
  face_cascade.detectMultiScale(grayscale, faces, 1.4, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));

  for (size_t i = 0; i < faces.size(); i++) {
    cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    cv::ellipse( frame, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar( 255, 255, 255 ), 2, 8, 0 );
	float x,y;
	x=faces[i].x; 
	y=faces[i].y; 
	react(x,y);
  }
}


void react(float x, float y){
	 if(x<100){
              cout << "left" << endl;
             
          } else if (x>500){
              cout << "right" << endl;
              
          } else if(x>100 && x<500){
              cout << "middle" << endl;
              
          }
          //cout << y << endl;

          if(y<300){
              cout << "top_M" << endl;
              
          } else if (y>300){
              cout << "down_M" << endl;
             
          } else {
              cout << "midlle_M" << endl;
              
          }
}

int main(int argc, char **argv) {

  raspicam::RaspiCam_Cv Camera; // Camera Object
  cv::Mat frame;
  cv::CascadeClassifier face_cascade;
  vector<cv::Rect> faces;
  size_t i = 0; // Used to keep track of iterations in the loop

  //New Cascade. src: OpenCV
  string classifier_file = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";

  // Load face cascade
  cout << "Loading face cascade.." << endl;
  if (!face_cascade.load(classifier_file)) {
    cerr << "Error loading face cascade!" << endl;
    return -1;
  }

  // Set camera params
  Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3); // For color

  // Open camera
  cout << "Opening camera..." << endl;
  if (!Camera.open()) {
    cerr << "Error opening camera!" << endl;
    return -1;
  }

  // Start capturing

  for (;;i++) {
    Camera.grab();
    Camera.retrieve(frame);
	
    detectFace(frame, face_cascade, faces);

    // Don t call detectFace on every iteration, it's too expensive
    if (i % 6 == 0) {
    } else {
      for (size_t i = 0; i < faces.size(); i++) {
        cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		cv::ellipse( frame, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar( 255, 255, 255 ), 2, 8, 0 );
		float x,y;
		x=faces[i].x; 
		y=faces[i].y; 
		react(x,y);
		}
    }
  }

  cout << "Stopping camera.." << endl;
  Camera.release();
  return 0;
}

