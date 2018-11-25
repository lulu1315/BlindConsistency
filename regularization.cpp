#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "regularization.h"

using namespace std;

int main(int argc, char **argv)
{
    char *infile_root=  argv[1];
    char *processedfile_root=  argv[2];
    char *file_ext=   argv[3];
	float lambdaT = atof(argv[4]);
    char *outfile_root=  argv[5];
    int fstart= atoi(argv[6]);
    int fend=   atoi(argv[7]);
    //int W = atoi(argv[8]);
    //int H = atoi(argv[9]);
        
    cv::Mat instreamer,processedstreamer,outputstreamer,firstframe,backwardoflow;
    
    //read first frame to get size
    char firstname[200];
    sprintf(firstname,"%s.%04d.%s",infile_root,fstart,file_ext);
    firstframe = cv::imread(firstname, CV_LOAD_IMAGE_COLOR);
    int W = firstframe.cols;
    int H = firstframe.rows;
    cout << "first input frame : " << firstname << "size : " << W << "x" << H << endl;
    //image vectors
    std::vector<float> prevInput(W*H*3);
	std::vector<float> curInput(W*H*3);
	std::vector<float> curProcessed(W*H*3);
	std::vector<float> prevSolution(W*H*3);
	std::vector<float> curSolution(W*H*3);
    std::vector<float> optflowBackward(W*H*2);
    
    //main loop
    for (int i=fstart; i<=fend; i++)
    {
        char inname[200];
        sprintf(inname,"%s.%04d.%s",infile_root,i,file_ext);
        cout << "inputfile : " << inname << endl;
        char processedfilename[200];
        sprintf(processedfilename,"%s.%04d.%s",processedfile_root,i,file_ext);
        cout << "processedfile : " << processedfilename << endl;
        char outfilename[200];
        sprintf(outfilename,"%s.%04d.%s",outfile_root,i,file_ext);
        cout << "outputfile : " << outfilename << endl;
        char oflowname[200];
        sprintf(oflowname,"%s.%04d.exr",outfile_root,i);
        cout << "opticalflowfile : " << oflowname << endl;
        char quivername[200];
        sprintf(quivername,"%s_quiver.%04d.%s",outfile_root,i,file_ext);
        cout << "quiverfile : " << quivername << endl;
        
        //read infile
        instreamer = cv::imread(inname, CV_LOAD_IMAGE_COLOR);
        processedstreamer = cv::imread(processedfilename, CV_LOAD_IMAGE_COLOR);
		
        //fill vectors curInput and curProcessed with image value
        int k=0;
        for (int y = 0; y < instreamer.rows; y++) {
            for (int x = 0; x < instreamer.cols; x++) {
                cv::Vec3b IN = instreamer.at<cv::Vec3b>(y, x);
                curInput[k*3 + 0] = IN.val[2]/255.;
                curInput[k*3 + 1] = IN.val[1]/255.;
                curInput[k*3 + 2] = IN.val[0]/255.;
                cv::Vec3b PROCESS = processedstreamer.at<cv::Vec3b>(y, x);
                curProcessed[k*3 + 0] = PROCESS.val[2]/255.;
                curProcessed[k*3 + 1] = PROCESS.val[1]/255.;
                curProcessed[k*3 + 2] = PROCESS.val[0]/255.;
                k++;
            }
        }
        
        curSolution = curProcessed;
		solve_frame<float>(&prevInput[0], &curInput[0], &curProcessed[0], &prevSolution[0], &curSolution[0], &optflowBackward[0], W, H, lambdaT, i==fstart);		

		prevInput = curInput;
		prevSolution = curSolution;		
		
        //preview opticalflow
        backwardoflow = cv::Mat::zeros(cv::Size(instreamer.cols, instreamer.rows), CV_32FC3);
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
			float flowx= optflowBackward[(y*W+x)*2+0]-x;
			float flowy= optflowBackward[(y*W+x)*2+1]-y;
            float flowz= 0;
            backwardoflow.at<cv::Vec3f>(y, x) = cv::Vec3f(flowz,flowy,flowx);
            }
        }
        
        const int resolution = 20;
        for (int y = 0; y < instreamer.rows; y += resolution) {
            for (int x = 0; x < instreamer.cols; x += resolution) {
			cv::Vec3f v = backwardoflow.at<cv::Vec3f>(y, x);
			cv::Point2f p(x, y);
			cv::Point2f p2(x-v[2], y-v[1]);
			cv::arrowedLine(instreamer, p, p2, cv::Scalar(255,0,  0), 1.5, 8, 0, 0.3);
            }
        }
        cv::imwrite(quivername, instreamer);
        //preview
        //cv::namedWindow("oflow");
        //cv::imshow("oflow", backwardoflow);
        //cv::waitKey(0);
        
		//back in opencv frame
        outputstreamer = cv::Mat::zeros(cv::Size(instreamer.cols, instreamer.rows), CV_8UC3);
        k=0;
        for (int y = 0; y < instreamer.rows; y++) {
            for (int x = 0; x < instreamer.cols; x++) {
                float Ox= min(255., max(0., curSolution[k*3+0]*255.));
                float Oy= min(255., max(0., curSolution[k*3+1]*255.));
                float Oz= min(255., max(0., curSolution[k*3+2]*255.));
                //outputstreamer.at<cv::Vec3b>(y, x) = cv::Vec3b(int(Oz),int(Oy),int(Ox));
                outputstreamer.at<cv::Vec3b>(y, x) = cv::Vec3b(Oz,Oy,Ox);
                k++;
            }
        }
        
        cv::imwrite(outfilename, outputstreamer);
    }
}
