#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <fstream.h>
#include <dirent.h>

#include <iostream>

using namespace cv;
using namespace std;

void help()
{
	cout <<
    "\nThis program demonstrates chamfer matching -- computing a distance between an \n"
    "edge template and a query edge image.\n"
    "Call:\n"
    "./chamfermatching [<image edge map> <template edge map>]\n"
    "By default\n"
    "the inputs are ./chamfermatching logo_in_clutter.png logo.png\n"<< endl;
}
int mainch( int argc, char** argv )
{
 srand(time(NULL));
    DIR *dir;
    struct dirent *ent;
    std::string ext = "png";
    std::string s = "";
    string pwd = "";
   //   ofstream file("/Users/sanjeevsingh/Desktop/cv2.txt");


    // approxPolyDP(<#InputArray curve#>, <#OutputArray approxCurve#>, <#double epsilon#>, <#bool closed#>)
    std::vector<cv::Point> polygon;
    //    polygon.push_back(Point(400,236));
    //    polygon.push_back(Point(640,390));
    //    polygon.push_back(Point(640,240));
    //    polygon.push_back(Point(365,145));
    
    polygon.push_back(Point(411,348));
    polygon.push_back(Point(552,480));
    polygon.push_back(Point(640,352));
    polygon.push_back(Point(400,288));
    
//    polygon.push_back(Point(861,463));
//    polygon.push_back(Point(1092,706));
//    polygon.push_back(Point(1280,394));
//    polygon.push_back(Point(790,267));
    
    Rect minRect = boundingRect(polygon);
    
    Mat polyMat = Mat::zeros(480, 640, CV_8UC1);
  //  Mat polyMat = Mat::zeros(720, 1280, CV_8UC1);
    //fills the roi of the visible road
    fillConvexPoly(polyMat, polygon, Scalar(125), 8, 0);
    //rows = 480 = height, cols = 640 = width;
 //   Mat mask = polyMat(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
    Mat mask = polyMat(Rect(minRect.x, minRect.y, 100, 100));

    Mat image = Mat::zeros( 480, 640, CV_8UC3 );
   // Mat image = Mat::zeros( 720, 1280, CV_8UC3 );
    Mat tpl = imread("/Users/sanjeevsingh/Desktop/drawn.png",0);
    Canny(tpl, tpl, 5, 50, 3);
    namedWindow("result", CV_WINDOW_AUTOSIZE );
    String filename;
    char* path = "/Volumes/MacintoshHD2/SideEye/videos/Vehicle/2/";
   // char* path =  "/Volumes/MacintoshHD2/SideEye/images/meng4/Vehicle/3/";
    if ((dir = opendir (path)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            std::stringstream sstm;
            std::string fname = ent->d_name;  // filename
            // if filename's last characters are extension
            
            if (fname.find(ext, (fname.length() - ext.length())) != std::string::npos){
                sstm << path<< fname;
                filename = sstm.str();
                image = imread(filename,CV_LOAD_IMAGE_COLOR);
               // image = imread("/Volumes/MacintoshHD2/SideEye/videos/Vehicle/2/100.png",CV_LOAD_IMAGE_COLOR);
        Mat currGray;
        Mat output;
        Mat roi;
        Mat result;
            //    currGray = image;
        cvtColor(image, currGray, CV_BGR2GRAY);
        roi = currGray(Rect(minRect.x, minRect.y - 60
                            , minRect.width-60
                            -1, minRect.height-1));
            //    roi = currGray(Rect(minRect.x, minRect.y - 30
              //                      , 100 , 100));
                roi.copyTo(output);//, mask);


  //  Mat img = imread("/Users/sanjeevsingh/Desktop/caredge.png",0);
    //Mat cimg;
    cvtColor(roi, result, CV_GRAY2BGR);

    
    // if the image and the template are not edge maps but normal grayscale images,
    // you might want to uncomment the lines below to produce the maps. You can also
    // run Sobel instead of Canny.
    
    Canny(output, output, 68, 100, 3);
    tpl = imread("/Users/sanjeevsingh/Desktop/drawn.png",0);
   // imshow("roi", output);
  //  imshow("tpl", tpl);
  //  waitKey(0);
    vector<vector<Point> > results;
    vector<float> costs;
                double templScale = 1;
                int maxMatches = 1; //20
                double minMatchDistance = 1.0;
                int padX = 3;
                int padY = 3;
                int scales = 5;
                double minScale = 0.6;
                double maxScale = 1.6;
                double orientationWeight = 0.5;
                double truncate = 20;

 //   int best = chamerMatching( roi, tpl, results, costs );
    int best = chamerMatching( output, tpl, results, costs, templScale, maxMatches, minMatchDistance, padX,
                padY, scales, minScale, maxScale, orientationWeight, truncate);

    if( best < 0 )
    {
        cout << "not found;\n";
       // file <<filename<<"\t"<<0<<endl;
     //   imshow("result", roi);
        //waitKey(0);
       // if(waitKey(30) >= 0) break;
       // else
         //   continue;
        
     //   return 0;
    }
    else {// cout<<"found\n";
        float cost = costs[best];
        cout<<cost<<"\n";
       // file <<filename<<"\t"<<cost*100<<endl;
      //  if(cost*100>=23.00)// cost= 0;//25.36*(100+rand() % 100);
       // file <<filename<<"\t"<<cost*100<<endl;

    size_t i, n = results[best].size();
      //   if(cost<0.23)
    for( i = 0; i < n; i++ )
    {   
        Point pt = results[best][i];
        if( pt.inside(Rect(0, 0, result.cols, result.rows)) )
            result.at<Vec3b>(pt) = Vec3b(0, 255, 0);
    }
      //  continue;
         namedWindow( "result", CV_WINDOW_AUTOSIZE );
        imshow("result", result);
        char q = waitKey(0);
      //  if(q=='1')
       //  file <<filename<<"\t"<<cost*100<<endl;
      //  if(waitKey(30) >= 0) break;
    }//else ends
                
            }
        }
       
            closedir (dir);
        } else {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }
  //   file.close();
    return 0;
}