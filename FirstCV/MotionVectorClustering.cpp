
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>

using namespace std;
using namespace cv;



int maincluster( int argc, char** argv )
{
    char* source_window = "Source image";
    
    ofstream file("/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Data/opticalflow/front-rest2.txt");
    
    DIR *dir;
    struct dirent *ent;
    std::string ext = "png";
    std::string s = "";
    string pwd = "";
    
    String filename;
  //  char* path = "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/testOF/";
    char* path = "/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Single/front_rest/";
    
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
    // cout<<TOTAL<<endl;
    // Mat mask = polyMat(Rect(minRect.x, minRect.y, 100, 100));
    
    int step = 50;
    double scale = 1.5;
    Scalar color = CV_RGB(0, 0, 0);
    
    Mat image = Mat::zeros( 720, 1280, CV_8UC3 );
    Mat prevgray;
  //  Mat samples(step*step, 2, CV_32F);
    
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
                //    Mat output;
                
                image = imread(filename,CV_LOAD_IMAGE_COLOR);
                cvtColor(image, gray, CV_BGR2GRAY);
                Mat display = image(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
                Mat roi = gray(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
                Mat gray2, gray1;
                //   pyrDown(roi,gray2, Size( roi.cols/2, roi.rows/2 ), BORDER_DEFAULT );
                //   pyrDown(gray2,gray1, Size( gray2.cols/2, gray2.rows/2 ), BORDER_DEFAULT );
                //  roi.copyTo(output);//, mask);
                //   cout<<fname<<endl;
                
                Mat flow, cflow;
                
                
                
                
                int countneg = 0;
                int countpos = 0;
                int total = 0;
                float slopes[step*step];
                if( prevgray.data )
                {
                    calcOpticalFlowFarneback(prevgray, roi, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
                    //  calcOpticalFlowFarneback(prevgray, gray1, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
                    
                    cvtColor(prevgray, cflow, CV_GRAY2BGR);
                    
                    int count = 0;
                    for(int y = 0; y < roi.rows; y += step)
                        for(int x = 0; x < roi.cols; x += step)
                        {
                            const Point2f& fxy = flow.at<Point2f>(y, x);
                            float dist = sqrt(pow(fxy.x,2) + pow(fxy.y,2));
                            slopes[count] = fxy.y/fxy.x;
                         //   samples.at<float>(count,1) = dist;
                         //   samples.at<float>(count,0) = fxy.y/fxy.x;
                            count++;
                            
                        }
                    
                    int i, j, flag = 1;    // set flag to 1 to start first pass
                    int temp;             // holding variable
                    for(i = 1; (i <= count) && flag; i++)
                    {
                        flag = 0;
                        for (j=0; j < (count -1); j++)
                        {
                            if (slopes[j+1] > slopes[j])      // ascending order simply changes to <
                            {
                                temp = slopes[j];             // swap elements
                                slopes[j] = slopes[j+1];
                                slopes[j+1] = temp;
                                flag = 1;               // indicates that a swap occurred.
                            }
                        }
                    }
                    
                    for(int i=0;i<count;i++)
                        cout<<slopes[count];
                    
                    int clusterCount = 3;
                    Mat labels;
                    int attempts = 5;
                    Mat centers;
                 //   kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 0.0001, 10000), attempts, KMEANS_PP_CENTERS, centers );
                    
                    count=0;
                    for(int y = 0; y < roi.rows; y += step)
                        for(int x = 0; x < roi.cols; x += step)
                        {
                        //    int cluster_idx = labels.at<int>(count,0);
                        //    cout<<cluster_idx<<",";
                            const Point2f& fxy = flow.at<Point2f>(y, x);
                            float dist = sqrt(pow(fxy.x,2) + pow(fxy.y,2));
                     //       circle(image, Point(centers.at<float>(cluster_idx, 0),
                        //                        centers.at<float>(cluster_idx, 1)),
                        //           2, Scalar(255,0,0), -1);
                            line(display, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),color);
                            circle(display, Point(x,y), 2, color, -1);

                                if(fxy.x>=0 && fxy.y>=0 && dist>2){ countpos++;
                              //      cout<< fxy.x<<","<<fxy.y<<endl;
                                    line(display, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),color);
                                    circle(display, Point(x,y), 2, color, -1);
                                }
                            if(fxy.x<0 && fxy.y<0 && dist>10){ countneg++;
                             //   cout<< fxy.x<<","<<fxy.y<<endl;
                                line(display, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),CV_RGB(255, 0, 0));
                                circle(display, Point(x,y), 2, CV_RGB(255, 0, 0), -1);
                            }
                            
                            
                            
                        }
                }
                
                // std::swap(prevgray, gray1);
                std::swap(prevgray, roi);
                
                ///////
                //   total++;
                float rationeg = (float)countneg*step*step/(roi.rows*roi.cols);
                float ratiopos = (float)countpos*step*step/(roi.rows*roi.cols);
                cout<<fname<<"\t"<<rationeg*100<<"\t"<<ratiopos*100<<endl;
                //   file<<fname<<"\t"<<(rationeg-ratiopos)*100<<"\t"<<ratiopos*100<<endl;
                file<<fname<<"\t"<<(rationeg-ratiopos)*100<<endl;
                
                namedWindow("roi", CV_WINDOW_AUTOSIZE );
                imshow("roi", display );
                waitKey(0);
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