#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include "opencv2/ximgproc/segmentation.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <ctime>
#include <probot_grasping/Position.h>
 
using namespace cv;
using namespace cv::ximgproc::segmentation;
using namespace std;
 
 
#define selectMethod 1
//#define pixels_permm_x 1.2
//#define pixels_permm_y 1.2
#define region_left_x 180
#define region_right_x 490
#define region_upper_y 180
#define region_lower_y 440
#define region_width_lower_limit 60
#define region_width_upper_limit 85
#define region_height_lower_limit 85
#define region_height_upper_limit 120

//1 切换到快速但低调用的选择性搜索方法
//2 切换到高调用但速度慢的选择性搜索方法

Mat im;
probot_grasping::Position position;


void target_loction(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImageConstPtr cv_ptr;
    try
    {

        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); // Caution the type here.

    }
    catch (cv_bridge::Exception& ex)
    {

        ROS_ERROR("cv_bridge exception in target_loction: %s", ex.what());
        exit(-1);
    }

    im = cv_ptr->image.clone();
    
    // resize image
    //int newHeight = 400;
    //int newWidth = im.cols*newHeight/im.rows;
    //resize(im, im, Size(newWidth, newHeight));
 
 
    //使用默认参数创建选择性搜索分段对象
    Ptr<SelectiveSearchSegmentation> ss = createSelectiveSearchSegmentation();
 
    //设置我们将运行分段的输入图像
    ss->setBaseImage(im);
    
    //切换到快速但低调用的选择性搜索方法
    if(selectMethod == 1)
    {
        ss->switchToSelectiveSearchFast();
    }
        //切换到高调用但速度慢的选择性搜索方法
    else if (selectMethod == 2)
    {
        ss->switchToSelectiveSearchQuality();
    }
    
    //在输入图像上运行选择性搜索分割
    std::vector<Rect> rects;
    ss->process(rects);
    cout << "Total Number of Region Proposals: " << rects.size() << endl;
    
    //要显示的地区提案数量
    int numShowRects = 500;
 
    //增加/减少要显示的理由提案的总数
    int increment = 10;
 
    while(1) {
 
        //创建原始图像的副本
        Mat imOut = im.clone();
        
        //迭代所有区域提案
        for(int i = 0; i < rects.size(); i++) {
            if (i < numShowRects) {
               //
               if ((rects[i].x>region_left_x&&rects[i].x<region_right_x)
               &&(rects[i].y>region_upper_y&&rects[i].y<region_lower_y)
               &&(rects[i].width>region_width_lower_limit
               &&rects[i].width<region_width_upper_limit)
               &&(rects[i].height>region_height_lower_limit
               &&rects[i].height<region_height_upper_limit))
              {
                 rectangle(imOut, rects[i], Scalar(0, 255, 0));
                 position.u = rects[i].x + rects[i].width/2;
                 position.v = rects[i].y + 5;
                 cout << "position_x = "<<position.u<<endl;
                 cout << "position_y = "<<position.v<<endl;
                 cout << "i = "<<i<<endl;
                 cv::circle(imOut, cv::Point(position.u, position.v), 5, CV_RGB(255,0,0), -1);
                 break;
               }
            }
            else {
                break;
            }
        }       
        //显示输出
        imshow("Output", imOut);
        waitKey();
        //记录按键
        int k = waitKey();
        //1时增加 -1时减少  0推出
        //int  k = -1;
        cin>>k;
        
        if (k == 1) {
            //增加矩形的总数以增量显示
            numShowRects += increment;
        }
        else if (k == -1  && numShowRects > increment) {
            //减少通过增量显示的矩形总数
            numShowRects -= increment;
        }
        else if (k == 0) {
            break;
        }
    }
    ros::shutdown();
}

 
int main(int argc, char** argv)
{
   ros::init(argc, argv, "shibie_loction");
   ros::NodeHandle nh;
    
   //使用多线程加速
    setUseOptimized(true);
    setNumThreads(4);
    
    ros::Subscriber image_sub_ = nh.subscribe("/usb_cam/image_raw", 1, &target_loction);
    ros::spin();

    ros::Publisher position_info_pub = nh.advertise<probot_grasping::Position>("/position_info",10);
    ros::Rate loop_rate(1);

    while (ros::ok())
    {
      position_info_pub.publish(position);

      ROS_INFO("Publishe target position info: u = %d, v = %d", position.u, position.v);
      loop_rate.sleep();
     }

   return 0;
}
