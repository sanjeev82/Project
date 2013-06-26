
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>

using namespace std;
using namespace cv;




int mainrun( int argc, char** argv )
{
    char* source_window = "Source image";
    char* warp_window = "Warp";
    
    Point2f srcTri[4];
    Point2f dstTri[4];
    
    //  Mat rot_mat( 2, 3, CV_32FC1 );
    Mat warp_mat( 3, 3, CV_32FC1 );
    Mat warp_dst;
    
    DIR *dir;
    struct dirent *ent;
    std::string ext = "png";
    std::string s = "";
    Mat image;
    string pwd = "";
    
    /// Set your 3 points to calculate the  Affine Transform
    //Meng
 /*   srcTri[0] = Point2f( 400, 236 );
    srcTri[1] = Point2f( 640, 390 );
    srcTri[2] = Point2f( 640, 240);
    srcTri[3] = Point2f( 365, 145 );
    //Srihari
   */ srcTri[0] = Point2f( 411, 348 );
    srcTri[1] = Point2f( 552, 480 );
    srcTri[2] = Point2f( 640, 352);
    srcTri[3] = Point2f( 400, 288 );
    
    dstTri[0] = Point2f( 400, 480 );
    dstTri[1] = Point2f( 640, 480 );
    dstTri[2] = Point2f( 640, 352 );
    dstTri[3] = Point2f( 400, 352 );
    
    /// Get the Affine Transform
    //   warp_mat = getAffineTransform( srcTri, dstTri );
    warp_mat = getPerspectiveTransform(srcTri, dstTri);
    // warp_dst = Mat::zeros( src.rows, src.cols, src.type() );
    warp_dst = Mat::zeros( 480, 640, CV_8UC3 );
    
    //   const char* path = "Srihari_HW1/VehicleInNextLane";
    String filename;

    if ((dir = opendir (argv[1])) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            std::string fname = ent->d_name;  // filename
            // if filename's last characters are extension
            
            if (fname.find(ext, (fname.length() - ext.length())) != std::string::npos){

                image = imread(fname);
                Mat roi;
                Mat gray;

                /// Apply the Affine Transform just found to the src image
                // warpAffine( src, warp_dst, warp_mat, warp_dst.size() );
                warpPerspective(image, warp_dst, warp_mat, warp_dst.size());
                roi = warp_dst(Rect(420, 350, 220-1, 120-1));
              /*  int N = (220-1)*(120-1);
                cvtColor(roi, gray ,CV_RGB2GRAY);
                
                vector<Mat> bgr_planes;
                split( roi, bgr_planes );
                
                /// Establish the number of bins
                int histSize = 8;
                int grayhistSize = 256;
                
                /// Set the ranges ( for B,G,R) )
                float range[] = { 0, 256 } ;
                const float* histRange = { range };
                
                long colorHist[512];
                
                bool uniform = true; bool accumulate = false;
                
                Mat b_hist, g_hist, r_hist;
                Mat hist;
                
                /// Compute the color histograms:
                calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
                calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
                calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
                /// Compute gray histogram
                calcHist( &gray, 1, 0, Mat(), hist, 1, &grayhistSize, &histRange, uniform, accumulate );
                // Draw the histograms for B, G and R
                int hist_w = 512; int hist_h = 400;
                int bin_w = cvRound( (double) hist_w/histSize );
                
                Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
                Mat grayhistImage( hist_h, hist_w, CV_8UC1, Scalar(0) );

                
 
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
              //  double minVal, maxVal;
              //  normalize(hist, hist, 0, grayhistImage.rows, NORM_MINMAX, -1, Mat() );
                
              //  minMaxIdx(hist, &minVal, &maxVal);
                /// Draw for each channel
                float mean = 0, variance = 0, stdev = 0;
                for( int i = 1; i < grayhistSize; i++ )
                {
                    line( grayhistImage, Point( i, hist_h - 5) ,
                         Point( i, hist_h - cvRound(hist.at<float>(i)) ),
                         Scalar(255), 2, 8, 0  );
                    mean +=(i)*hist.at<float>(i);
                }
                mean = mean/N;
                
                for( int i = 1; i < grayhistSize; i++ )
                {
                    variance +=(hist.at<float>(i))*pow((i-mean),2);
                }
                variance = variance/N;
                
                stdev = sqrt(variance);
                
                cout<<stdev<<endl;
                
                for( int i = 1; i < grayhistSize; i++ )
                {
                    line( grayhistImage, Point( i, hist_h - 5) ,
                         Point( i, hist_h - cvRound(hist.at<float>(i)) ),
                         Scalar(255), 2, 8, 0  );
                    mean +=hist.at<float>(i);
                }
                
                /// Show what you got
                namedWindow("colorHist", CV_WINDOW_AUTOSIZE );
                imshow("colorHist", histImage);

                namedWindow( source_window, CV_WINDOW_AUTOSIZE );
                imshow( source_window, image );
                */
                std::stringstream sstm;
                namedWindow( warp_window, CV_WINDOW_AUTOSIZE );
                imshow( warp_window, roi );
                sstm << "warp/" << fname;
                filename = sstm.str();
                imwrite(filename, roi);
                
             //   namedWindow("grayHist", CV_WINDOW_AUTOSIZE );
               // imshow( "grayHist", grayhistImage );
                
                if(waitKey(30) >= 0) break;
            }
            
        }
        
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    return 0;
}