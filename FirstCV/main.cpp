#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main3( int argc, char** argv )
{
    VideoCapture cap("/Users/sanjeevsingh/Desktop/SideEye/Srihari_HW1.mov"); // read the video file
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    Mat polyMat = Mat::zeros(height, width, CV_8UC1);
    // approxPolyDP(<#InputArray curve#>, <#OutputArray approxCurve#>, <#double epsilon#>, <#bool closed#>)
    std::vector<cv::Point> polygon;
    polygon.push_back(Point(400,236));
    polygon.push_back(Point(640,390));
    polygon.push_back(Point(640,240));
    polygon.push_back(Point(365,145));
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(255), 8, 0);
    //rows = 480 = height, cols = 640 = width;
    
    Rect minRect = boundingRect(polygon);
    
    //minRect.x+minRect.width = 641 ,minRect.y+minRect.height = 391
    
    Mat mask = polyMat(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
   // int whitePx = countNonZero(mask);
    // cout<<whitePx<<","<<whitePx/(float)(mask.rows*mask.cols);
    
    Mat frame;
    String filename;
    for(int i=0, j=0, k=0;;)
    {
        // snprintf(buffer, sizeof(char) * 64, "/Users/sanjeevsingh/Desktop/SideEye/train/file%i.jpg", i);
        //  Mat currGray;
        //  Mat output;
        Mat roi;
        cap >> frame; // get a new frame from camera
        // cvtColor(frame, currGray, CV_BGR2GRAY);
       // roi = frame(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
        // roi.copyTo(output, mask);
        //   s = "train/";s.append(i); s.append(".jpg");
      //  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
        //  imshow("calcHist Demo", histImage );
        imshow("frame", frame);
        std::stringstream sstm;
        char q = waitKey(0);
        if(q=='1'){
            sstm << "/Users/sanjeevsingh/Desktop/SideEye/NoVehicleInNextLane/" << i<<".jpg";
            filename = sstm.str();
            i++;
        }
        else if(q=='2'){
            sstm << "/Users/sanjeevsingh/Desktop/SideEye/VehicleInNextLane/" << j<<".jpg";
            filename = sstm.str();
            j++;
        }
        else if(q=='3'){
            sstm << "/Users/sanjeevsingh/Desktop/SideEye/VehicleInNextNextLane/" << j<<".jpg";
            filename = sstm.str();
            k++;
        }
        else if(q=='q')break;
        else continue;
        imwrite( filename   , roi );
        cout<<i<<","<<j<<","<<k<<endl;
        
    }
    
    return 0;
}