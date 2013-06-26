
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>

using namespace std;
using namespace cv;



int mainwrp( int argc, char** argv )
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
    ofstream file("/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Data/warped/warpintensity-front-rest.txt");
    
    
    histSize[0] = 256;
    hranges[0] = 0.0;
    hranges[1] = 255.0;
    ranges[0] = hranges;
    channels[0] = 0;
    
    DIR *dir;
    struct dirent *ent;
    std::string ext = "png";
    std::string s = "";
    string pwd = "";
    
    String filename;
    char* path = "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front_rest/";
    
    
    std::vector<cv::Point> polygon;
    //    polygon.push_back(Point(400,236));
    //    polygon.push_back(Point(640,390));
    //    polygon.push_back(Point(640,240));
    //    polygon.push_back(Point(365,145));
    
    // polygon.push_back(Point(411,348));
    // polygon.push_back(Point(552,480));
    // polygon.push_back(Point(640,352));
    // polygon.push_back(Point(400,288));
    
    polygon.push_back(Point(861,463));
    polygon.push_back(Point(1092,706));
    polygon.push_back(Point(1280,394));
    polygon.push_back(Point(790,267));
    
    Rect minRect = boundingRect(polygon);
    
    
    /// Set your 3 points to calculate the  Affine Transform

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
    
    if ((dir = opendir (path)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            std::string fname = ent->d_name;  // filename
            // if filename's last characters are extension
            if (fname.find(ext, (fname.length() - ext.length())) != std::string::npos){
                std::stringstream sstm;
                std::string fname = ent->d_name;  // filename
                sstm << path<< fname;
                filename = sstm.str();
                Mat gray;
                Mat output;
                image = imread(filename,CV_LOAD_IMAGE_COLOR);
                cvtColor(image, gray, CV_BGR2GRAY);
                /// Apply the Affine Transform just found to the src image
                // warpAffine( src, warp_dst, warp_mat, warp_dst.size() );
                warpPerspective(image, warp_dst, warp_mat, warp_dst.size());
                Mat roi = warp_dst(Rect(400, 352, 880, 200));
              //  Mat roi = gray(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
              //  roi.copyTo(output, mask);
                cout<<fname<<endl;
                
                //   Mat hist = getHistogramImage(output);
                
            cv:MatND hist;
                cv::calcHist(&roi,
                             1,channels,
                             cv::Mat(),
                             hist,
                             1,
                             histSize,
                             ranges
                             );
                double maxVal = 0;
                double minVal = 0;
                cv::minMaxLoc(hist,&minVal, &maxVal,0, 0);
                cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));
                int hpt = static_cast<int>(0.9*histSize[0]);
                float sum = 0;
                float mean = 0;
                float var = 0;
                float stdev = 0;
                float z = 0;
                //  cout<<hist.at<float>(0)<<endl;
                TOTAL = 0;
                for(int h=1; h < histSize[0]; h++){
                    float binVal = hist.at<float>(h);
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
                file<<fname<<"\t"<<mean<<"\t"<<sqrt(var)<<endl;
                cout<<"mean: "<<mean<<",var:"<<sqrt(var)<<endl;
            //    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
             //   imshow( source_window, roi);
             //      namedWindow( "image", CV_WINDOW_AUTOSIZE );
              //     imshow( "image", image);
                //   char q = waitKey(0);
                //   if(q=='q') {break;
                //      file.close();
                //   }
            //    if(waitKey(30) >= 0) break;
            }
            
        }
           file.close();
        
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    return 0;
}