#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <dirent.h>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int mainpct( int argc, char** argv )
{
    
    int height = 720;
    int width = 1280;
    
    //cap.set(CV_CAP_PROP_POS_MSEC, 1200.2);
    
    Mat polyMat = Mat::zeros(height, width, CV_8UC1);
    std::vector<cv::Point> polygon;
    polygon.push_back(Point(861,463));
    polygon.push_back(Point(1092,706));
    polygon.push_back(Point(1280,394));
    polygon.push_back(Point(790,267));
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(255), 8, 0);
    //rows = 480 = height, cols = 640 = width;
    
    Rect minRect = boundingRect(polygon);
    Point2f srcTri[4];
    Point2f dstTri[4];
    
    //  Mat rot_mat( 2, 3, CV_32FC1 );
    Mat warp_mat( 3, 3, CV_32FC1 );
    Mat warp_dst;
    
    DIR *dir;
    struct dirent *ent;
    std::string ext = "png";
    std::string s = "";
    string pwd = "";
    
    String filename;
    char* path = "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/all/";
    
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
    
    Mat image;
    Mat saveImage;
    // String filename;
    char q = '0';
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
                saveImage = imread(filename,CV_LOAD_IMAGE_COLOR);
                rectangle(image, minRect, Scalar(225,22,199), 1, 8,0);
                Point tl = minRect.tl();
                int step = minRect.width/10;
                for(int a=0; a<minRect.width; a=a+step){
                    Point p1(tl.x+a, 720-1);
                    Point p2(tl.x+a,0);
                    line(image, p1, p2, Scalar(255,255,0));
                }
                cvtColor(image, gray, CV_BGR2GRAY);
                
                namedWindow("frame", CV_WINDOW_AUTOSIZE );
                imshow("frame", image);
                std::stringstream sstm1;
                std::string savepath;
                char q = waitKey(0);
                if(q=='1'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/1/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='2'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/2/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='3'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/3/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='4'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/4/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='5'){
                    sstm1<<"/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/5/" << fname;
                    savepath = sstm1.str();

                }
                else if(q=='6'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/6/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='7'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/7/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='8'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/8/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='9'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/9/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='0'){
                    sstm1 << "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front/10/" <<fname;
                    savepath = sstm1.str();

                }
                else if(q=='q')break;
                else if(q=='n'){
                    sstm1<<"/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front_rest/"<<fname;
                    savepath = sstm1.str();
                    
                }
                else continue;
                imwrite( savepath   , saveImage);
                
                cout<<savepath<<endl;
                
            }
        }
        //   file.close();
        
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    return 0;
}
