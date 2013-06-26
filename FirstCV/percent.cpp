#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int mainpercent( int argc, char** argv )
{
    VideoCapture cap("/Volumes/MacintoshHD2/SideEye/videos/Srihari_HW1.MOV"); // read the video file
    if(!cap.isOpened())  // check if we succeeded
        return -1;
     
    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    cap.set(CV_CAP_PROP_POS_MSEC, 1200.2);
    
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
    Point2f srcTri[4];
    Point2f dstTri[4];
    
    //  Mat rot_mat( 2, 3, CV_32FC1 );
    Mat warp_mat( 3, 3, CV_32FC1 );
    Mat warp_dst;
    
    Mat image;
    string pwd = "";
    
    String filename;
    
    /// Set your 3 points to calculate the  Affine Transform
    //Meng City
    /* srcTri[0] = Point2f( 400, 236 );
     srcTri[1] = Point2f( 640, 390 );
     srcTri[2] = Point2f( 640, 240);
     srcTri[3] = Point2f( 365, 145 );*/
    //Srihari
     srcTri[0] = Point2f( 411, 348 );
     srcTri[1] = Point2f( 552, 480 );
     srcTri[2] = Point2f( 640, 352);
     srcTri[3] = Point2f( 400, 288 );
    //Meng HW
    /* srcTri[0] = Point2f( 362,382 );
     srcTri[1] = Point2f( 427,480 );
     srcTri[2] = Point2f( 640,372 );
     srcTri[3] = Point2f( 330,280 );*/
    //Meng new
   /* srcTri[0] = Point2f( 861,463 ); //1280x720
    srcTri[1] = Point2f( 1092,706 );
    srcTri[2] = Point2f( 1280,394 );
    srcTri[3] = Point2f( 790,267 );
    
    dstTri[0] = Point2f( 750, 720 );
    dstTri[1] = Point2f( 1280, 720 );
    dstTri[2] = Point2f( 1280, 500 );
    dstTri[3] = Point2f( 750, 500 );*/
    
    dstTri[0] = Point2f( 400, 480 );
    dstTri[1] = Point2f( 640, 480 );
    dstTri[2] = Point2f( 640, 352 );
    dstTri[3] = Point2f( 400, 352 );
    
    /// Get the Affine Transform
    //   warp_mat = getAffineTransform( srcTri, dstTri );
    warp_mat = getPerspectiveTransform(srcTri, dstTri);
    // warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    warp_dst = Mat::zeros( 480, 640, CV_8UC3 );
    
    Mat frame;
    // String filename;
    char q = '0';
    for(int i=0, j=0, k=0, l=0;;)
    {
        l++;
        // snprintf(buffer, sizeof(char) * 64, "/Users/sanjeevsingh/Desktop/SideEye/train/file%i.jpg", i);
        Mat output;
        Mat roi;
        cap >> frame; // get a new frame from camera
        output = frame;
        // cvtColor(frame, currGray, CV_BGR2GRAY);
        //  rectangle(frame, minRect, Scalar(255,0,0));
        //roi = frame(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
        // roi.copyTo(output, mask);
        //   s = "train/";s.append(i); s.append(".jpg");
        warpPerspective(output, warp_dst, warp_mat, warp_dst.size());
      //  roi = warp_dst(Rect(750, 500, 530-1, 220-1));
        roi = warp_dst(Rect(420, 350, 220-1, 120-1));
        for(int a=22; a<220; a=a+22){
            Point p1(0+a, 120-1);
            Point p2(0+a,0);
            line(roi, p1, p2, Scalar(255,255,0));
        }
        
        namedWindow("frame", CV_WINDOW_AUTOSIZE );
        //  namedWindow("roi", CV_WINDOW_AUTOSIZE );
     //   imshow("roi", roi );
        imshow("frame", frame);
        //imshow("roi", warp_dst);
        std::stringstream sstm;
        if(l%2==0)q = waitKey(0);
        if(q=='1'){
            sstm << "Vehicle/1/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='2'){
            sstm << "Vehicle/2/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='3'){
            sstm << "Vehicle/3/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='4'){
            sstm << "Vehicle/4/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='5'){
            sstm << "Vehicle/5/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='6'){
            sstm << "Vehicle/6/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='7'){
            sstm << "Vehicle/7/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='8'){
            sstm << "Vehicle/8/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='9'){
            sstm << "Vehicle/9/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='0'){
            sstm << "Vehicle/10/" << i<<".png";
            filename = sstm.str();
            i++;
        }
        else if(q=='n'){
            sstm << "NoVehicle/" << j<<".png";
            filename = sstm.str();
            j++;
        }
        else if(q=='m'){
            sstm << "unknown/" << k<<".png";
            filename = sstm.str();
            k++;
        }
        else if(q=='q')break;
        else continue;
        imwrite( filename   , frame);
        
        
        cout<<i<<","<<j<<","<<k<<endl;
        
    }
    // file.close();
    // file2.close();
    return 0;
}
