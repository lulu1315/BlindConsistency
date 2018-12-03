#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "opencv2/optflow.hpp"
#include "opencv2/core/ocl.hpp"
#include <omp.h>

using namespace std;
using namespace cv;
using namespace optflow;

template<typename T, typename Tflow>
void opt_flow_deepflow(const T* imgA, const T* imgB, int W, int H, Tflow* optflow) { // images in the range 0..1

int useGpu = 1;
Mat img_cvA, img_cvB;
img_cvA = cv::Mat::zeros(H,W, CV_8UC3);
img_cvB = cv::Mat::zeros(H,W, CV_8UC3);

//reformat for opencv
#pragma omp parallel for
for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
        img_cvA.at<cv::Vec3b>(y, x) = cv::Vec3b(imgA[(y*W+x)*3+2]*255, imgA[(y*W+x)*3+1]*255, imgA[(y*W+x)*3+0]*255);
        img_cvB.at<cv::Vec3b>(y, x) = cv::Vec3b(imgB[(y*W+x)*3+2]*255, imgB[(y*W+x)*3+1]*255, imgB[(y*W+x)*3+0]*255);
        //img_cvA.at<cv::Vec3b>(y, x) = cv::Vec3b(imgA[(y*W+x)*3+0]*255, imgA[(y*W+x)*3+1]*255, imgA[(y*W+x)*3+2]*255);
        //img_cvB.at<cv::Vec3b>(y, x) = cv::Vec3b(imgB[(y*W+x)*3+0]*255, imgB[(y*W+x)*3+1]*255, imgB[(y*W+x)*3+2]*255);
		}
	}        
	
//namedWindow( "lulu", WINDOW_AUTOSIZE );
//imshow( "lulu", img_cvA );
//waitKey(0);

cvtColor(img_cvA, img_cvA, COLOR_BGR2GRAY);
cvtColor(img_cvB, img_cvB, COLOR_BGR2GRAY);

Mat_<Point2f> flow;
flow = Mat(H,W, CV_32FC2);

cv::ocl::setUseOpenCL(useGpu);
printf("OpenCL Enabled: %u\n", useGpu && cv::ocl::haveOpenCL());

Ptr<DenseOpticalFlow> algorithm;
algorithm = createOptFlow_DeepFlow();

if (useGpu)
        algorithm->calc(img_cvA, img_cvB, flow.getUMat(ACCESS_RW));
    else
        algorithm->calc(img_cvA, img_cvB, flow);
    
//reformat flow
for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
        Vec2f myflow = flow.at<cv::Vec2f>(y, x);
        optflow[(y*W+x)*2+0] = x+myflow.val[0];
        optflow[(y*W+x)*2+1] = y+myflow.val[1];
		}
	}  

}
