#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "colorhistogram.h"
#include "histLib.h"

using namespace std;
using namespace cv;

/**
 * @function main
 */
int mainbkp( int argc, char** argv )
{
    VideoCapture cap("/Users/sanjeevsingh/Desktop/SideEye/images/Video_Meng/1.mp4"); // read the video file
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    // Load color image
	ColorHistogram hc;
    CHistLib Histogram;
    
	cv::Mat color;
    
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
    int whitePx = countNonZero(mask);
    int TOTAL = mask.rows*mask.cols;
    
    // cout<<whitePx<<","<<whitePx/(float)(mask.rows*mask.cols);
    
    Mat frame;
    for(;;)
    {
        Mat currGray;
        Mat output;
        Mat roi;
        cap >> frame; // get a new frame from camera
        color = frame;
        color= hc.colorReduce(color,32);
        cv::MatND shist= hc.getHistogram(color);
        // cout<< shist.channels();
        cout<<shist.empty()<<endl;

        
        imshow("reduced", color);
        
        
        
        Mat HistImageBGR;
        
        MatND HistB;
        MatND HistG;
        MatND HistR;
        Histogram.ComputeHistogramBGR(frame, HistB, HistG, HistR);
        Histogram.DrawHistogramBGR(HistB, HistG, HistR, HistImageBGR);
        imshow("bgr",HistImageBGR);
        int c = waitKey(10);
        if( (char)c == 'q' ) { break; }
        else
        continue;
        
        // cvtColor(frame, currGray, CV_BGR2GRAY);
        roi = frame(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
        // roi.copyTo(output, mask);
        
        /// Separate the image in 3 places ( B, G and R )
        vector<Mat> bgr_planes;
        split( roi, bgr_planes );
        
        /// Establish the number of bins
        int histSize = 256;
        
        /// Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 } ;
        const float* histRange = { range };
        
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
        normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
        float sum_r = 0, sum_g = 0, sum_b = 0;
        float mean_r = 0, mean_g = 0, mean_b = 0;
        float var_r = 0, var_g = 0, var_b = 0;
        float stdev_r = 0, stdev_g = 0, stdev_b = 0;
        float z_rg = 0, z_rb = 0, z_bg = 0;
        /// Draw for each channel
        for( int i = 1; i < histSize; i++ )
        {
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                 Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                 Scalar( 255, 0, 0), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                 Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                 Scalar( 0, 255, 0), 2, 8, 0  );
            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                 Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                 Scalar( 0, 0, 255), 2, 8, 0  );
            
            sum_r+= i*r_hist.at<float>(i);
            sum_g+= i*g_hist.at<float>(i);
            sum_b+= i*b_hist.at<float>(i);
            
        }
        mean_r = sum_r/TOTAL;
        mean_g = sum_g/TOTAL;
        mean_b = sum_b/TOTAL;
        
        cout<<"mean: "<<mean_r<<","<<mean_g<<","<<mean_b<<endl;
        
        for( int i = 1; i < histSize; i++ )
        {
            var_r+= pow(i - mean_r, 2)*r_hist.at<float>(i) ;
            var_g+= pow(i - mean_g, 2)*g_hist.at<float>(i) ;
            var_b+= pow(i - mean_b, 2)*b_hist.at<float>(i) ;
        }
        stdev_r = sqrt(var_r/TOTAL);
        stdev_g = sqrt(var_g/TOTAL);
        stdev_b = sqrt(var_b/TOTAL);
        cout<<"stdev: "<<stdev_r<<","<<stdev_g<<","<<stdev_b<<endl;
        
        z_rg = (mean_r - mean_g)/sqrt(pow(stdev_r,2)+pow(stdev_g,2));
        z_rb = (mean_r - mean_b)/sqrt(pow(stdev_r,2)+pow(stdev_b,2));
        z_bg = (mean_b - mean_g)/sqrt(pow(stdev_b,2)+pow(stdev_g,2));
        
        cout<<"Z: "<<z_rg<<","<<z_rb<<","<<z_bg<<endl;
        
        float bg = compareHist( b_hist, g_hist, CV_COMP_CHISQR );
        float br = compareHist( b_hist, r_hist, CV_COMP_CHISQR );
        float rg = compareHist( r_hist, g_hist, CV_COMP_CHISQR );
        
        cout<<"ChiSq: "<<bg<<","<<br<<","<<rg<<endl;
        
        /// Display
        namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
        imshow("calcHist Demo", histImage );
        imshow("roi", roi);
        waitKey(0);
        // if(waitKey(30) >= 0) break;
    }
    
    return 0;
}