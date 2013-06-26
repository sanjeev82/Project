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


int mainhist( int argc, char** argv )
{
    
    VideoCapture cap("/Users/sanjeevsingh/Desktop/SideEye/Srihari_HW1.mov"); // read the video file
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    Mat polyMat = Mat::zeros(height, width, CV_8UC1);
    // approxPolyDP(<#InputArray curve#>, <#OutputArray approxCurve#>, <#double epsilon#>, <#bool closed#>)
    std::vector<cv::Point> polygon;
//    polygon.push_back(Point(400,236));
//    polygon.push_back(Point(640,390));
//    polygon.push_back(Point(640,240));
//    polygon.push_back(Point(365,145));
    polygon.push_back(Point(411,348));
    polygon.push_back(Point(552,480));
    polygon.push_back(Point(640,352));
    polygon.push_back(Point(400,288));
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(255), 8, 0);
    //rows = 480 = height, cols = 640 = width;
    
    Rect minRect = boundingRect(polygon);
    
    //minRect.x+minRect.width = 641 ,minRect.y+minRect.height = 391
    
    Mat mask = polyMat(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
    // int whitePx = countNonZero(mask);
    // cout<<whitePx<<","<<whitePx/(float)(mask.rows*mask.cols);

    String filename;
    Mat frame;
    for(int i=0, j=0, k=0;;)
    {
     //   Mat currGray;
        Mat output;
        Mat roi;
        cap >> frame; // get a new frame from camera
        // cvtColor(frame, currGray, CV_BGR2GRAY);
        roi = frame(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
         roi.copyTo(output, mask);
        
        /// Separate the image in 3 places ( B, G and R )
        vector<Mat> bgr_planes;
        split( output, bgr_planes );
        
        /// Establish the number of bins
        int histSize = 8;
        
        /// Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 } ;
        const float* histRange = { range };
        
        long colorHist[512];
        
        bool uniform = true; bool accumulate = false;
        
        Mat b_hist, g_hist, r_hist;
        
        /// Compute the histograms:
        calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
        
        // Draw the histograms for B, G and R
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );
        
        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
        
        /// Normalize the result to [ 0, histImage.rows ]
      //  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      //  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      //  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        
        /// Draw for each channel
     //   for( int i = 0; i < histSize; i++ )
      //  {
         //   line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
            //     Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
           //      Scalar( 255, 0, 0), 2, 8, 0  );
          //  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
           //      Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
           //      Scalar( 0, 255, 0), 2, 8, 0  );
          //  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
          //       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
          //       Scalar( 0, 0, 255), 2, 8, 0  );
           
    //    }
        long sum = 0;
        int count = 0;
        long max = 0;
        for(int i = 0; i<8; i++){
            sum = 0;
            sum+= cvRound(b_hist.at<float>(i));
            for (int j=0;  j<8; j++) {
                sum+= cvRound(g_hist.at<float>(j));
                for( int k = 0; k<8; k++){
                    sum+= cvRound(r_hist.at<float>(k));
                    colorHist[count] = sum;
                    if(max<sum) max = sum;
                    sum-= cvRound(r_hist.at<float>(k));
                    count++;

                }
                sum-= cvRound(g_hist.at<float>(j));
            }
            sum-= cvRound(b_hist.at<float>(i));
        }
        int histNorm[512];
        for(int i=0; i<512; i++){
            histNorm[i] = cvRound(hist_h*(float)colorHist[i]/(double)max);
          //  histNorm[i] = histNorm[i]*hist_h;
        //    cout<<"("<<i<<")"<<histNorm[i]<<", ";
        }
        
        count = 0;
       // float graphheight = 0;
        
        for(int i = 0; i<8; i++){
            for (int j=0;  j<8; j++) {
                for( int k = 0; k<8; k++){
                //    graphheight = (float)colorHist[count]/(double)max;
                    line( histImage, Point( count, hist_h-5 ) ,
                         Point( count, hist_h - histNorm[count]
                               ),
                         Scalar( 32*i, 32*j, 32*k));

                    count++;
                }
            }
        }
//        for(int i = 0; i< 512; i++ ){
//            cout<<histNorm[i]<<",";
//        }
        namedWindow("colorHist", CV_WINDOW_AUTOSIZE );
        namedWindow("roi", CV_WINDOW_AUTOSIZE );
                imshow("roi", output );
                imshow("colorHist", histImage );
       // waitKey(0);
        /// Display
        

       // waitKey(0);
      //  imshow("roi", roi);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}