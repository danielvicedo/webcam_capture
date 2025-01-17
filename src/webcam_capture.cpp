
//opencv
#include "opencv2/opencv.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//std
#include <iostream>
#include <cstdlib>

//main
int main(int argc, char *argv[])
{
    cv::VideoCapture camera; //OpenCV video capture object
    cv::Mat image; //OpenCV image object
    cv::Mat temporary; //OpenCV temporary image for color modification

	int cam_id; //camera id . Associated to device number in /dev/videoX
	int user_key; //user pressed key to quit
	cv::Vec3b pixel_intensity; //pixel RGB intensity
  cv::Vec4b Rect; //pixel RGB intensity

	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;
			break;
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break;
		default:
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl;
			std::cout << "EXIT program." << std::endl;
			break;
	}

	//advertising to the user
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) )
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //capture loop. Out of user press a key
    while(1)
	{
		//Read image and check it. Blocking call up to a new image arrives from camera.
        if(!camera.read(image))
		{
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }

		// get intensity of the central pixel. Ordered as BGR. Rows = 480, Cols = 640
		pixel_intensity = image.at<cv::Vec3b>(image.rows/2, image.cols/2);
		std::cout << "RGB: " 	<< (unsigned int)pixel_intensity[2] << ","
								<< (unsigned int)pixel_intensity[1] << ","
								<< (unsigned int)pixel_intensity[0] << std::endl;


		// manipulate the central pixel value. Set it as blue
		pixel_intensity[0] = 255;
		pixel_intensity[1] = 0;
		pixel_intensity[2] = 0;
		image.at<cv::Vec3b>(image.rows/2, image.cols/2) = pixel_intensity;

    // Create a rectangle
      // declare variables
      int x = image.cols/2-100;
      int y = image.rows/2-100;
      int width = 200;
      int height = 200;

      cv::Rect rect(x, y, width, height);
      // Point 1 is the upper left corner of the rectangle
      cv::Point pt1(x, y);
      // Point 2 is the bottom right corner of the rectangle
      cv::Point pt2(x + width, y + height);
      // Calls the image, both points and the color of the rectangle
      cv::rectangle(image, pt1, pt2, cv::Scalar(0, 255, 0));

    // Greyscale the inside of the rectangle
      // use a temporary image to convert to Greyscale
      cvtColor(image, temporary, CV_BGR2GRAY);
      cvtColor(temporary, temporary, CV_GRAY2BGR);
      //put mask temporary(rect) in front of image to see both at the same time
      temporary(rect).copyTo(image(rect));
      temporary = image;

        //show image in a window
        cv::imshow("Output Window", image);

		//Waits 30 millisecond to check if 'q' key has been pressed. If so, breaks the loop. Otherwise continues.
    	if( (unsigned char)(cv::waitKey(30) & 0xff) == 'q' ) break;
    }
}
