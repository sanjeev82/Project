#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>




using namespace std;
using namespace cv;
int bins = 4;
int bin_size = 256/bins;

void sort(int *array, int length)
{
    int i, j, temp;
    int test; /*use this only if unsure whether the list is already sorted or not*/
    for(i = length - 1; i > 0; i--)
    {
        test=0;
        for(j = 0; j < i; j++)
        {
            if(array[j] > array[j+1]) /* compare neighboring elements */
            {
                temp = array[j]; /* swap array[j] and array[j+1] */
                array[j] = array[j+1];
                array[j+1] = temp;
                test=1;
            }
        } /*end for j*/
        if(test==0) break; /*will exit if the list is sorted!*/
    } /*end for i*/
    
}/*end bubbleSort*/
        
        int mainshad( int argc, char** argv )
    {
        Point2f srcTri[4];
        Point2f dstTri[4];
        
        Mat warp_mat( 3, 3, CV_32FC1 );
        Mat warp_dst;
        
        Mat image;

        ofstream file("/Volumes/MacintoshHD2/SideEye/videos/MengHW1/novehicle.txt");
        DIR *dir;
        struct dirent *ent;
        std::string ext = "png";
        std::string s = "";

        char* pwd = "/Volumes/MacintoshHD2/SideEye/videos/MengHW1/warpNoVehicle/";

        
        String filename;
        int R[bins];
        int G[bins];
        int B[bins];


        
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
                    sstm << "/Volumes/MacintoshHD2/SideEye/videos/MengHW1/warpNoVehicle/"<< fname;
                    for(int i = 0; i<bins; i++){
                        R[i] = 0;
                        G[i] = 0;
                        B[i] = 0;
                    }
        image = imread(sstm.str());
        int nl = image.rows;
        int nc = image.cols;
        int total = nl * nc;
        int r_value, g_value, b_value;
        cout<<nl<<endl;
        cout<<nc<<endl;
        // Mat roi;
        // Mat gray;
        // warpPerspective(image, warp_dst, warp_mat, warp_dst.size());
        // roi = warp_dst(Rect(420, 350, 220-1, 120-1));
        for( int j=0; j<nl; j++){
            for(int i=0; i<nc; i++){
                r_value = image.at<cv::Vec3b>(j,i)[0]/bin_size;
                g_value = image.at<cv::Vec3b>(j,i)[1]/bin_size;
                b_value = image.at<cv::Vec3b>(j,i)[2]/bin_size;
                image.at<cv::Vec3b>(j,i)[0] = (image.at<cv::Vec3b>(j,i)[0]/bin_size) * (bin_size/2);
                image.at<cv::Vec3b>(j,i)[1] = (image.at<cv::Vec3b>(j,i)[1]/bin_size) * (bin_size/2);
                image.at<cv::Vec3b>(j,i)[2] = (image.at<cv::Vec3b>(j,i)[2]/bin_size) * (bin_size/2);
                R[r_value]++;
                G[g_value]++;
                B[b_value]++;
            }
        }
        for(int i = 0; i<bins; i++){
            R[i] = (R[i]/(float)total)*100;
            G[i] = (G[i]/(float)total)*100;
            B[i] = (B[i]/(float)total)*100;
        }
        sort(R, bins);
        sort(G, bins);
        sort(B, bins);
        for(int i=0; i<bins; i++){
            cout<<"R: "<<R[i]<<",";
        }
        cout<<endl;
        for(int i=0; i<bins; i++){
            cout<<"G: "<<G[i]<<",";
        }
        cout<<endl;
        for(int i=0; i<bins; i++){
            cout<<"B: "<<B[i]<<",";
        }
        cout<<endl;
        file<<R[3]<<"\t"<<G[3]<<"\t"<<B[3]<<endl;
        namedWindow( "IMAGE", CV_WINDOW_AUTOSIZE );
        imshow( "IMAGE", image );
        char c = waitKey(0);
                    if(c=='q') break;
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