#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>




using namespace std;
using namespace cv;




int mainhue( int argc, char** argv )
{
    int bins = 4;
    int bin_size = 256/bins;
    
    
    Mat image;
    Mat hsv;
    
    int N = 0;
    
   // ofstream file("/Volumes/MacintoshHD2/SideEye/videos/MengHW1/huevehicle.txt");
    ofstream file("/Volumes/MacintoshHD2/SideEye/data/MengNew/huevehicle.txt");
    DIR *dir;
    struct dirent *ent;
    std::string ext = "png";
    std::string s = "";
    
    char* pwd = "/Volumes/MacintoshHD2/SideEye/videos/MengHW1/warpVehicle/";
    
    
    String filename;

    
    
    
    /// Get the Affine Transform
    //   warp_mat = getAffineTransform( srcTri, dstTri );
    //  warp_mat = getPerspectiveTransform(srcTri, dstTri);
    // warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    // warp_dst = Mat::zeros( 480, 640, CV_8UC3 );
    if ((dir = opendir (pwd)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            std::string fname = ent->d_name;  // filename
            // if filename's last characters are extension
            
            if (fname.find(ext, (fname.length() - ext.length())) != std::string::npos){
                cout<<fname<<endl;
                std::stringstream sstm;
                sstm << "/Volumes/MacintoshHD2/SideEye/videos/MengHW1/warpVehicle/"<< fname;
                image = imread(sstm.str());
                cvtColor(image, hsv, CV_BGR2HSV);
                std::vector<cv::Mat> v;
                cv::split(hsv, v);
             //   hranges[0] = 0.0;
            //    hranges[1] = 180.0;
              //  channels[0] = 0;
                N = image.rows * image.cols;
                
                /// Establish the number of bins
                int histSize = 180;
                
                /// Set the ranges ( for B,G,R) )
                float range[] = { 0, 180 } ;
                const float* histRange = { range };
                
                bool uniform = true; bool accumulate = false;
                
                Mat hist;
                
                /// Compute the histograms:
                calcHist( &v[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
                
                // Draw the histograms
                int hist_w = 400; int hist_h = 400;
                int bin_w = cvRound( (double) hist_w/histSize );
                
                Mat histImage( hist_h, hist_w, CV_8UC1, Scalar(0) );
                
                /// Normalize the result to [ 0, histImage.rows ]
                normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
                
                float mean = 0, variance = 0, stdev = 0;
                /// Draw for each channel
                for( int i = 1; i < histSize; i++ )
                {
                    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                         Scalar(255), 2, 8, 0  );
                    mean +=(i)*hist.at<float>(i);
                }
  
                mean = mean/N;
                
                for( int i = 1; i < histSize; i++ )
                {
                    variance +=(hist.at<float>(i))*pow((i-mean),2);
                }
                variance = variance/N;
                
                stdev = sqrt(variance);
                
                cout<<stdev<<endl;
                file<<stdev<<endl;
                

                

 /*
                for( int j=0; j<nl; j++){
                    for(int i=0; i<nc; i++){
                        r_value = image.at<cv::Vec3b>(j,i)[0]/bin_size;
                        g_value = image.at<cv::Vec3b>(j,i)[1]/bin_size;
                        b_value = image.at<cv::Vec3b>(j,i)[2]/bin_size;
                        image.at<cv::Vec3b>(j,i)[0] = (image.at<cv::Vec3b>(j,i)[0]/bin_size) * (bin_size/2);
                        image.at<cv::Vec3b>(j,i)[1] = (image.at<cv::Vec3b>(j,i)[1]/bin_size) * (bin_size/2);
                        image.at<cv::Vec3b>(j,i)[2] = (image.at<cv::Vec3b>(j,i)[2]/bin_size) * (bin_size/2);
                    }
                }*/

              //  namedWindow( "IMAGE", CV_WINDOW_AUTOSIZE );
              //  imshow("IMAGE", histImage);
              //  waitKey(0);
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    file.close();
    return 0;
}