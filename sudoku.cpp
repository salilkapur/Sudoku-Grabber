#include "opencv2/imgproc/imgproc.hpp"
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

void digit_recog(Mat img)
{
	Mat digit;
	vector<vector<Point> > contour;
	vector<Vec4i> h;
	
	findContours(img,contour,h,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
	for(int i=0;i<contour.size();i++)
	{
		vector<Point> temp;
		temp=contour[i];
		img(boundingRect(temp)).copyTo(digit);
		namedWindow("Digit",0);
		imshow("Digit",digit);
		waitKey(500);
	}
}

//Function to seperate out the grid(seperate the block with biggest are(assumption))
Mat grid_extract(Mat img)
{
	int index;
	double max;
	Mat grid;
	grid=Mat::zeros(img.size(),CV_8UC1);
	vector<vector<Point> > contour;
	vector<Vec4i> h;
	vector<Point> req;
	
	findContours(img,contour,h,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
	max=contourArea(contour[0]);
	for(int i=0;i<contour.size();i++)
	{
		double temp;
		temp=contourArea(contour[i]);
		if(max<temp)
		{
			max=temp;
			index=i;
			req=contour[i];
		}
	}
	drawContours(grid,contour,index,Scalar(255,255,255),CV_FILLED,8,h);
	namedWindow("Grid",0);
	imshow("Grid",grid);
	waitKey(0);
	return grid(boundingRect(req));
}



//Function to threshold image(to prepare it for extracting the grid)
Mat thresh(Mat img)
{
	cvtColor(img,img,CV_BGR2GRAY);
	Mat img_t=Mat::zeros(img.size(),CV_8UC3);
	adaptiveThreshold(img,img_t,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,5,10);
	return img_t;
}

//Function to remove the lines from the grid(To seperate out digits from the grid)
Mat hough(Mat img)
{
	vector<Vec4i> lines;
	HoughLinesP(img,lines,1,CV_PI/180,100,30,10);
	for(int i=0; i<lines.size();i++)
	{
		Vec4i l=lines[i];
		line(img,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,0,0),10,CV_AA);
	}
	imshow("Digits",img);
	waitKey(0);
	return img;
}
		

int main(int argc,char** argv)
{
	Mat img;
	
	img=imread("./test_img/sample7.jpg");
	
	Mat img_t=Mat::zeros(img.size(),CV_8UC1);
	Mat img_f=Mat::zeros(img.size(),CV_8UC1);
	
	img_t=thresh(img);

	img_t=grid_extract(img_t);
	
	
	img_f=hough(img_t);
	digit_recog(img_f);
	namedWindow("Final",0);
	imshow("Final",img_f);
	
	waitKey(0);
	
	return 0;
}
