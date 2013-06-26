//
//  hsvHist.cpp
//  FirstCV
//
//  Created by Sanjeev Singh on 4/25/13.
//  Copyright (c) 2013 Sanjeev Singh. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream.h>

using namespace std;
using namespace cv;


int maino( int argc, char** argv )
{
    
    //VideoCapture cap("/Users/sanjeevsingh/Desktop/SideEye/Srihari_HW1.mov"); // read the video file
    VideoCapture cap("/Users/sanjeevsingh/Desktop/meng4.mp4");
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    ofstream file("/Users/sanjeevsingh/Desktop/of.txt");
    
    Mat polyMat = Mat::zeros(height, width, CV_8UC1);
    // approxPolyDP(<#InputArray curve#>, <#OutputArray approxCurve#>, <#double epsilon#>, <#bool closed#>)
    std::vector<cv::Point> polygon;
    //    polygon.push_back(Point(400,236));
    //    polygon.push_back(Point(640,390));
    //    polygon.push_back(Point(640,240));
    //    polygon.push_back(Point(365,145));
    //srihari
//    polygon.push_back(Point(411,348));
//    polygon.push_back(Point(552,480));
//    polygon.push_back(Point(640,352));
//    polygon.push_back(Point(400,288));
    //meng4
    polygon.push_back(Point(861,463));
    polygon.push_back(Point(1092,706));
    polygon.push_back(Point(1280,394));
    polygon.push_back(Point(790,267));
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(255), 8, 0);
    //rows = 480 = height, cols = 640 = width;
    
    Rect minRect = boundingRect(polygon);
    
    //minRect.x+minRect.width = 641 ,minRect.y+minRect.height = 391
    
    Mat mask = polyMat(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));


    String filename;
    Mat frame;
    Mat prevgray;
    for(;;)
    {
        //   Mat currGray;
        Mat output;
        Mat roi;
        cap >> frame; // get a new frame from camera
        // cvtColor(frame, currGray, CV_BGR2GRAY);
        roi = frame(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
       // roi = frame(Rect(100, 100, 2, 3)); //startX, startY, cols, rows
        roi.copyTo(output);//, mask);
        
        
        ////////////
        
        Mat gray, flow, cflow;
        Mat demogray;
        
        cvtColor(output, gray, CV_BGR2GRAY);
        cvtColor(gray, demogray, CV_GRAY2BGR);
        int step = 20;//16;
        double scale = 1.5;
        Scalar color = CV_RGB(0, 255, 0);
        
        int countneg = 0;
        int countpos = 0;
        
        if( prevgray.data )
        {
            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
          //  cout<<flow<<endl;
          //  waitKey(0);

            cvtColor(prevgray, cflow, CV_GRAY2BGR);
            for(int y = 0; y < output.rows; y += step)
                for(int x = 0; x < output.cols; x += step)
                {
                    const Point2f& fxy = flow.at<Point2f>(y, x);
                    float dist = sqrt(pow(fxy.x,2) + pow(fxy.y,2));
                  //  if(fxy.x<0 || fxy.y<0)
                    if(fxy.x>=0 && fxy.y>=0 && dist>2){ countpos++;
                 //   cout<< fxy.x<<","<<fxy.y<<endl;
                 //   line(demogray, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                   //      color);
                 //   circle(demogray, Point(x,y), 2, color, -1);
                    }
                    if(fxy.x<0 && fxy.y<0 && dist>10){ countneg++;
                        //   cout<< fxy.x<<","<<fxy.y<<endl;
                 //       line(demogray, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                        //     CV_RGB(255, 0, 0));
                 //       circle(demogray, Point(x,y), 2, CV_RGB(255, 0, 0), -1);
                    }



                }
        }
        float rationeg = (float)countneg*step*step/(output.rows*output.cols);
         float ratiopos = (float)countpos*step*step/(output.rows*output.cols);
        cout<<rationeg<<","<<ratiopos<<endl;
        file<<rationeg*100<<"\t"<<ratiopos*100<<endl;
        
        std::swap(prevgray, gray);
///////

      //  namedWindow("roi", CV_WINDOW_AUTOSIZE );
      //  imshow("roi", demogray );
     //    waitKey(0);
        /// Display
        
        
        // waitKey(0);
        //  imshow("roi", roi);
     //   if(waitKey(30) >= 0) break;
    }
   // file.close();
    return 0;
}