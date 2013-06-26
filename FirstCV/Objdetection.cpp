
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <dirent.h>

#include "opencv2/highgui/highgui_c.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
string sideeye_cascade_name = "/Users/sanjeevsingh/Desktop/cascade100HOG.xml";
CascadeClassifier sideeye_cascade;
string window_name = "Capture - BlindSpot detection";

RNG rng(12345);

/**
 * @function main
 */
int mainobj( int agrc, char** argv )
{
    
    VideoCapture cap("/Users/sanjeevsingh/Desktop/SideEye/images/Video_Meng/1.mp4"); // read the video file
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    //-- 1. Load the cascade
    if( !sideeye_cascade.load( sideeye_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    
    
    Mat frame;
    for(;;)
    {
        cap >> frame ;
        detectAndDisplay( frame );
        
        int c = waitKey(10);
        if( (char)c == 'c' ) { break; }
        
    }
    /*
    DIR *dir;
    struct dirent *ent;
    std::string ext = "jpg";
    std::string s = "";
    Mat image;
    string pwd = "";
    char* path = "/Users/sanjeevsingh/Dropbox/Ubuntu/100/100pos/";
    
    String filename;
    
    if ((dir = opendir (path)) != NULL) {
        // print all the files and directories within directory 
        while ((ent = readdir (dir)) != NULL) {
             std::stringstream sstm;
            std::string fname = ent->d_name;  // filename
            // if filename's last characters are extension
            
            if (fname.find(ext, (fname.length() - ext.length())) != std::string::npos){
                sstm << path << fname;
                filename = sstm.str();
                cout<<filename<<endl;
                image = imread(filename);
                detectAndDisplay( image );
                waitKey(0);
            }
            
        }
        
        closedir (dir);
    } else {
        // could not open directory 
        perror ("");
        return EXIT_FAILURE;
    }
    */
    return 0;
}

/**
 * @function detectAndDisplay
 */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;
    
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    
    //-- Detect faces
    sideeye_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(200, 200) );
    //detectMultiScale(const Mat& image, vector<Rect>& objects, double scaleFactor=1.1, int minNeighbors=3, int flags=0, Size minSize=Size(), Size maxSize=Size()
    
    
    for( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
        
        
    }
    Mat zoomed = Mat (220, 250, CV_8UC1);
    cv::resize(frame, zoomed, zoomed.size(), 0, 0, INTER_LINEAR);
    
    //-- Show what you got
    imshow( window_name, zoomed );
}
