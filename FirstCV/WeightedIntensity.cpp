
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>

using namespace std;
using namespace cv;



int mainwi( int argc, char** argv )
{
    char* source_window = "Source image";
    
    
    
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
    int TOTAL = 0 ;
  //  ofstream file("/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Data/simpleintensity-vehicle-rest.txt");
    
    
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
    char* path = "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Empty/";
    
    
    std::vector<cv::Point> polygon;

    polygon.push_back(Point(861,463));
    polygon.push_back(Point(1092,706));
    polygon.push_back(Point(1280,394));
    polygon.push_back(Point(790,267));
    
    Rect minRect = boundingRect(polygon);
    
    //Mat polyMat = Mat::zeros(480, 640, CV_8UC1);
    Mat polyMat = Mat::zeros(720, 1280, CV_8UC1);
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(255), 8, 0);
    //rows = 480 = height, cols = 640 = width;
    Mat mask = polyMat(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
    //  TOTAL = mask.rows*mask.cols - 112289;
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
                Mat roi = gray(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
                roi.copyTo(output, mask);
                float histogram[256];
                for(int k=0;k<256; k++)
                    histogram[k]=0.0;
                int weightWidth = roi.cols*0.2;
                int Width = output.cols;
                int Height = output.rows;
                TOTAL = 0;
                float min_value,max_value=0;
                //weight to end pixels
                for(int j=0; j<Width; j++){
                    for(int i=0; i<Height; i++){
                        histogram[output.at<uchar>(i,j)]++;
                    }

                }
                
                for(int j=1; j<256; j++){

                    if(max_value<histogram[j]) max_value = histogram[j];
                    if(min_value>histogram[j]) min_value = histogram[j];
                }
                
              //  cv::Mat m = cv::Mat::zeros(height, width, CV_32F);
                Mat histImage = Mat::zeros(256, 256, CV_8UC1);
                for(int j=1; j<256; j++){
                    int intensity = static_cast<int>(histogram[j]*256/max_value);
                    cv::line(histImage, cv::Point(j,256),
                             cv::Point(j,256 -intensity),
                             cv::Scalar(255));
                }
                
                float sum = 0;
                float mean = 0;
                float var = 0;
                float stdev = 0;
                float z = 0;
                //  cout<<hist.at<float>(0)<<endl;
                TOTAL = 0;
                
                for(int h=1; h<256; h++){
                    sum+= h*histogram[h];
                    TOTAL+=histogram[h];
                }
              //  cout<<TOTAL<<endl;
                mean = sum/TOTAL;
                
                for( int i = 1; i < 256; i++ )
                {
                    var+= pow(i - mean, 2)*histogram[i] ;
                    
                }
                
                var = var/TOTAL;
                
              //  cout<<"mean: "<<mean<<",var:"<<sqrt(var)<<"Total:"<<TOTAL<<endl;

                
                var = var/TOTAL;
             //   file<<fname<<"\t"<<mean<<"\t"<<sqrt(var)<<endl;
            //    cout<<"mean: "<<mean<<",var:"<<sqrt(var)<<"Total:"<<TOTAL<<endl;
                //   namedWindow( source_window, CV_WINDOW_AUTOSIZE );
                //   imshow( source_window, histImg);
                   namedWindow( "image", CV_WINDOW_AUTOSIZE );
                   imshow( "image", histImage);
                //   char q = waitKey(0);
                //   if(q=='q') {break;
                //      file.close();
                //   }
                  if(waitKey(30) >= 0) break;
            }
            
        }
      //  file.close();
        
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    return 0;
}