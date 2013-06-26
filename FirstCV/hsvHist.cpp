//
//  hsvHist.cpp
//  FirstCV
//
//  Created by Sanjeev Singh on 4/25/13.
//  Copyright (c) 2013 Sanjeev Singh. All rights reserved.
//

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


int mainhsv( int argc, char** argv )
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
    
    Mat src, hsv;
    String filename;
    for(int i=0, j=0, k=0;;)
    {
        // snprintf(buffer, sizeof(char) * 64, "/Users/sanjeevsingh/Desktop/SideEye/train/file%i.jpg", i);
        //  Mat currGray;
        //  Mat output;
        Mat roi;
        cap >> src; // get a new frame from camera
    
    cvtColor(src, hsv, CV_BGR2HSV);
    
    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};
    
    calcHist( &hsv, 1, channels, Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    double maxVal=0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    
    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);
    
    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                      Point( (h+1)*scale - 1, (s+1)*scale - 1),
                      Scalar::all(intensity),
                      CV_FILLED );
        }
    
    namedWindow( "Source", 1 );
    imshow( "Source", src );
    
    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );
    waitKey();
    }
    return 0;
}