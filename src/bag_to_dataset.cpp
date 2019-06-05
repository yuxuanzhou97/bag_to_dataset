#include <iostream>
#include <fstream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/MagneticField.h>
#include <opencv2/opencv.hpp>
using namespace std;

rosbag::Bag bag;
string dataset_dir;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "bag_to_dataset");
  ros::NodeHandle nh("~");
  bag.open("/home/zhouyuxuan/2019-04-25-19-00-07.bag", rosbag::bagmode::Read);
  dataset_dir="/home/zhouyuxuan/data/2019-04-25-19-00-07";
  string img0_dir=dataset_dir+"/img0";
  string img1_dir=dataset_dir+"/img1";
  ofstream ofs_imu(dataset_dir+"/imu.txt");
  ofstream ofs_mag(dataset_dir+"/mag.txt");
  ofstream ofs_img0_stamp(dataset_dir+"/img0_stamp.txt");
  ofstream ofs_img1_stamp(dataset_dir+"/img1_stamp.txt");


//rosbag::View view_imu(bag, rosbag::TopicQuery("/imu/data"));
rosbag::View view_imu(bag, rosbag::TopicQuery("/mynteye/imu/data_raw"));
for(rosbag::MessageInstance const m : view_imu)
{
  /*
    std_msgs::Int32::ConstPtr i = m.instantiate<std_msgs::Int32>();
    if (i != NULL)
        std::cout << i->data << std::endl;
    */
    sensor_msgs::Imu::ConstPtr imu=m.instantiate<sensor_msgs::Imu>();
    if(imu!=NULL)
      ofs_imu<<(long long)(imu->header.stamp.toSec()*1e9)<<" "
      <<imu->angular_velocity.x<<" "
      <<imu->angular_velocity.y<<" "
      <<imu->angular_velocity.z<<" "
      <<imu->linear_acceleration.x<<" "
      <<imu->linear_acceleration.y<<" "
      <<imu->linear_acceleration.z<<" "
      <<endl;

}

rosbag::View view_mag(bag, rosbag::TopicQuery("/imu/mag"));
for(rosbag::MessageInstance const m : view_mag)
{
    sensor_msgs::MagneticField::ConstPtr mag=m.instantiate<sensor_msgs::MagneticField>();
    if(mag!=NULL)
      ofs_mag<<(long long)(mag->header.stamp.toSec()*1e9)<<" "
      <<mag->magnetic_field.x<<" "
      <<mag->magnetic_field.y<<" "
      <<mag->magnetic_field.z<<" "
      <<endl;
}

rosbag::View view_img0(bag, rosbag::TopicQuery("/mynteye/left/image_raw"));
for(rosbag::MessageInstance const m : view_img0)
{
    sensor_msgs::Image::ConstPtr img=m.instantiate<sensor_msgs::Image>();
    if(img!=NULL)
    {
      ofs_img0_stamp<<(long long)(img->header.stamp.toSec()*1e9)<<" "
      <<(long long)(img->header.stamp.toSec()*1e9)<<".jpg"<<endl;
      auto img_ptr = cv_bridge::toCvShare(img,sensor_msgs::image_encodings::MONO8);
      const cv::Mat &img_mat = img_ptr->image;
      cv::imwrite(img0_dir+"/"+to_string((long long)(img->header.stamp.toSec()*1e9))+".jpg",img_mat);
    }
}

rosbag::View view_img1(bag, rosbag::TopicQuery("/mynteye/right/image_raw"));
for(rosbag::MessageInstance const m : view_img1)
{
    sensor_msgs::Image::ConstPtr img=m.instantiate<sensor_msgs::Image>();
    if(img!=NULL)
    {
      ofs_img1_stamp<<(long long)(img->header.stamp.toSec()*1e9)<<" "
      <<(long long)(img->header.stamp.toSec()*1e9)<<".jpg"<<endl;
      auto img_ptr = cv_bridge::toCvShare(img,sensor_msgs::image_encodings::MONO8);
      const cv::Mat &img_mat = img_ptr->image;
      cout<<img1_dir+"/"+to_string((long long)(img->header.stamp.toSec()*1e9))+".jpg"<<endl;
      cv::imwrite(img1_dir+"/"+to_string((long long)(img->header.stamp.toSec()*1e9))+".jpg",img_mat);
    }
}



    return 0;


}
