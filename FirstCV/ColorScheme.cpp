
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream.h>
#include <stdio.h>
#include <fstream.h>
#include <dirent.h>

using namespace std;
using namespace cv;

// Various color types for detected shirt colors.
enum                             {cBLACK=0,cWHITE, cGREY, cRED, cORANGE, cYELLOW, cGREEN, cAQUA, cBLUE, cPURPLE, cPINK,  NUM_COLOR_TYPES};
char* sCTypes[NUM_COLOR_TYPES] = {"Black", "White","Grey","Red","Orange","Yellow","Green","Aqua","Blue","Purple","Pink"};
uchar cCTHue[NUM_COLOR_TYPES] =    {0,       0,      0,     0,     20,      30,      55,    85,   115,    138,     161};
uchar cCTSat[NUM_COLOR_TYPES] =    {0,       0,      0,    255,   255,     255,     255,   255,   255,    255,     255};
uchar cCTVal[NUM_COLOR_TYPES] =    {0,      255,    120,   255,   255,     255,     255,   255,   255,    255,     255};

// Determine what type of color the HSV pixel is. Returns the colorType between 0 and NUM_COLOR_TYPES.
int getPixelColorType(int H, int S, int V)
{
    H = (int)180*((float)H/255);
	int color;
	if (V < 75)
		color = cBLACK;
	else if (V > 190 && S < 27)
		color = cWHITE;
	else if (S < 53 && V < 185)
		color = cGREY;
	else {	// Is a color
		if (H < 14)
			color = cRED;
		else if (H < 25)
			color = cORANGE;
		else if (H < 34)
			color = cYELLOW;
		else if (H < 73)
			color = cGREEN;
		else if (H < 102)
			color = cAQUA;
		else if (H < 127)
			color = cBLUE;
		else if (H < 149)
			color = cPURPLE;
		else if (H < 175)
			color = cPINK;
		else	// full circle
			color = cRED;	// back to Red
	}
	return color;
}
void reduce2basicColors(cv::Mat &image)
{
    // Create a HSV image showing the color types of the whole image, for debugging.
    Mat imageInHSV;// = Mat( image.cols, image.rows, CV_8UC3 );
    cvtColor(image, imageInHSV, CV_BGR2HSV);	// (note that OpenCV stores RGB images in B,G,R order.  
    
    cv::MatIterator_<cv::Vec3b> it = imageInHSV.begin<cv::Vec3b>(),
    it_end = imageInHSV.end<cv::Vec3b>();
    for(; it != it_end; ++it)
    {
        // work with pixel in here, e.g.:
        cv::Vec3b& pixel = *it; // reference to pixel in image
        pixel[0] = 0; // changes pixel in image
        
        uchar H = pixel[0];	// Hue
        uchar S = pixel[1]; // Saturation
        uchar V = pixel[2];	// Value (Brightness)
        // Determine what type of color the HSV pixel is.
        int ctype = getPixelColorType(H, S, V);
        //ctype = x / 60;
        // Show the color type on the displayed image, for debugging.
        pixel[0] = cCTHue[ctype];	// Hue
        pixel[1] = cCTSat[ctype];	// Full Saturation (except for black & white)
        pixel[2] = cCTVal[ctype];	// Full Brightness
    }
    
    // Display the HSV debugging image
    Mat imageHSV_RGB = Mat(imageInHSV.cols, imageInHSV.rows, 8, 3);
    cvtColor(imageInHSV, imageHSV_RGB, CV_HSV2BGR);	// (note that OpenCV stores RGB images in B,G,R order.
    namedWindow("Colors", 1);
    imshow("Colors", imageInHSV);
    
    if(waitKey(30) >= 0) exit(0);
}
int maincolor( int argc, char** argv )
{
    char* source_window = "Source image";
    
    
    
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
    int TOTAL = 0 ;
    // ofstream file("/Volumes/MacintoshHD2/SideEye/FinalVideos/Meng_HW3/Data/simpleintensity-empty.txt");
    
    
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
                cvtColor(image, gray, CV_BGR2GRAY);
                image = imread(filename,CV_LOAD_IMAGE_COLOR);
                Mat roi = image(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
                roi.copyTo(output, mask);
                imshow("img",output);
              //  waitKey(0);
                reduce2basicColors(output);
                /*
                Mat roi = gray(Rect(minRect.x, minRect.y, minRect.width-1, minRect.height-1));
                roi.copyTo(output, mask);
                cout<<fname<<endl;
                
                //   Mat hist = getHistogramImage(output);
                
            cv:MatND hist;
                cv::calcHist(&output,
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
                // file<<fname<<"\t"<<mean<<"\t"<<sqrt(var)<<endl;
                cout<<"mean: "<<mean<<",var:"<<sqrt(var)<<endl;
                namedWindow( source_window, CV_WINDOW_AUTOSIZE );
                imshow( source_window, histImg);
                //   namedWindow( "image", CV_WINDOW_AUTOSIZE );
                //   imshow( "image", output);
                //   char q = waitKey(0);
                //   if(q=='q') {break;
                //      file.close();
                //   }
                if(waitKey(30) >= 0) break;
                 */
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