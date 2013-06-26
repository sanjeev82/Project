

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>

using namespace std;
using namespace cv;



int mainplot( int argc, char** argv )
{
    char* source_window = "Source image";
    
    Point2f srcTri[4];
    Point2f dstTri[4];
    
    //  Mat rot_mat( 2, 3, CV_32FC1 );
    Mat warp_mat( 3, 3, CV_32FC1 );
    Mat warp_dst;
    
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
    int TOTAL = 0 ;
    
    histSize[0] = 256;
    hranges[0] = 0.0;
    hranges[1] = 255.0;
    ranges[0] = hranges;
    channels[0] = 0;
    
    String filename;
    char* path = "/Users/sanjeevsingh/Desktop/o666.png";//063.png";
    ofstream file("/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Data/histo.txt");
    
    std::vector<cv::Point> polygon;
    polygon.push_back(Point(861,463));
    polygon.push_back(Point(1092,706));
    polygon.push_back(Point(1280,394));
    polygon.push_back(Point(790,267));
    
    srcTri[0] = Point2f( 861, 463 );
    srcTri[1] = Point2f( 1092, 706 );
    srcTri[2] = Point2f( 1280, 394);
    srcTri[3] = Point2f( 790, 267 );
    
    dstTri[0] = Point2f( 400, 720 );
    dstTri[1] = Point2f( 1280, 720 );
    dstTri[2] = Point2f( 1280, 352 );
    dstTri[3] = Point2f( 400, 352 );
    
    /// Get the Affine Transform
    //   warp_mat = getAffineTransform( srcTri, dstTri );
    warp_mat = getPerspectiveTransform(srcTri, dstTri);
    // warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    warp_dst = Mat::zeros( 720, 1280, CV_8UC3 );
    
    Rect minRect = boundingRect(polygon);
    
    //Mat polyMat = Mat::zeros(480, 640, CV_8UC1);
    Mat polyMat = Mat::zeros(720, 1280, CV_8UC1);
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(255), 8, 0);
    //rows = 480 = height, cols = 640 = width;
    Mat mask = polyMat(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
    //  TOTAL = mask.rows*mask.cols - 112289;
    // cout<<TOTAL<<endl;
    // Mat mask = polyMat(Rect(minRect.x, minRect.y, 100, 100));
    
    Mat image = Mat::zeros( 720, 1280, CV_8UC3 );
    std::stringstream sstm;
    sstm << path;
    filename = sstm.str();
    Mat gray;
    Mat output;
    image = imread(filename,CV_LOAD_IMAGE_COLOR);
    
    warpPerspective(image, warp_dst, warp_mat, warp_dst.size());
    Mat warp_roi = warp_dst(Rect(400, 352, 880, 200));
    
    cvtColor(image, gray, CV_BGR2GRAY);
    cv::line(gray, cv::Point(861,463),cv::Point(1092,706), Scalar(0),5);
    cv::line(gray,cv::Point(1092,706), cv::Point(1280,394),Scalar(0),5);
    cv::line(gray,cv::Point(1280,394), cv::Point(790,267), Scalar(0),5);
    cv::line(gray, cv::Point(790,267), cv::Point(861,463), Scalar(0),5);
    Mat roi = gray(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));

    
    roi.copyTo(output, mask);
    
cv:MatND hist;
    cv::calcHist(&output,
                 1,channels,
                 cv::Mat(),
                 hist,
                 1,
                 histSize,
                 ranges
                 );
    double maxVal = 0;
    double minVal = 0;
  //  cv::minMaxLoc(hist,&minVal, &maxVal,0, 0);
    cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));
    int hpt = static_cast<int>(0.004*histSize[0]);
    float sum = 0;
    float mean = 0;
    float var = 0;
    float stdev = 0;
    float z = 0;
   
    for(int i=1; i<histSize[0]; i++){
        if(maxVal<hist.at<float>(i)) maxVal = maxVal<hist.at<float>(i);
    }
    
    TOTAL = 0;
    for(int h=0; h < histSize[0]; h++){
        float binVal = hist.at<float>(h);
        cout<<binVal<<" ";
        file<<h<<"\t"<<binVal<<endl;
        int intensity = static_cast<int>(binVal*hpt/maxVal);
        cv::line(histImg, cv::Point(h,histSize[0]),
                 cv::Point(h,histSize[0]-intensity),
                 cv::Scalar::all(0));
        
        sum+= h*hist.at<float>(h);
        TOTAL+=hist.at<float>(h);
    }
    
    mean = sum/TOTAL;
    
    
    
    for( int i = 1; i < histSize[0]; i++ )
    {
        var+= pow(i - mean, 2)*hist.at<float>(i) ;
        
    }
    
    var = var/TOTAL;
    Mat warp_roi_gray;
    cvtColor(warp_roi, warp_roi_gray, CV_BGR2GRAY);
    
    cout<<"mean: "<<mean<<",var:"<<sqrt(var)<<"tota"<<TOTAL<<endl;
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, roi);
     imshow( "warp", warp_roi_gray);
    
    waitKey(0);
    imwrite( "/Users/sanjeevsingh/Desktop/roi.png", roi );
    imwrite( "/Users/sanjeevsingh/Desktop/warp.png", warp_roi_gray );
    file.close();
    
    return 0;
}