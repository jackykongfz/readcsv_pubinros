#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <std_msgs/String.h>

using namespace std;

ifstream file;

// a function to read the csv file and publish one row at a time 
bool readcsv_andoutputonerow(float &x_cmd)
{
  string line, word;
  vector<string> csv_row;
  if (getline(file, line)) {
    csv_row.clear();
    stringstream s(line);

    while (getline(s, word, ',')) {
      csv_row.push_back(word);
    }

    if (csv_row.size() >= 3) {
      ROS_INFO("Publish original x position: %f", atof(csv_row[2].c_str()));
      x_cmd = atof(csv_row[2].c_str());
      // return atof(csv_row[2].c_str());
      return true;
    }
  }else{
    ROS_INFO("End of file");
    // return 0;
    return false;
  }
  // return 0;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "csv_publisher");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<std_msgs::String>("csv_column", 1000);
  ros::Rate rate(50);

  string file_path = "/home/jackykong/velocity9_trajectory_setpoints.csv"; // replace with your file path
  // ifstream file(file_path.c_str());
  file.open(file_path.c_str());

  if (!file.is_open()) {
    ROS_ERROR("Failed to open CSV file");
    return -1;
  }

  string line, word;
  vector<string> csv_row;
  // while (getline(file, line)) {
  //   csv_row.clear();
  //   stringstream s(line);

  //   while (getline(s, word, ',')) {
  //     csv_row.push_back(word);
  //   }

  //   if (csv_row.size() >= 3) {
  //     std_msgs::String msg;
  //     msg.data = csv_row[2];
  //     pub.publish(msg);
  //     ROS_INFO("Publish original x position: %f", atof(csv_row[2].c_str()));
  //   }
  //   rate.sleep();
  // }

  while (ros::ok()) {
    
    // input current yaw and z
    float yaw = 1.7;
    float z = 10;

    // a function to read the csv file and publish the data
    float poscmd[3];
    float poscmd_rotated[3];
    poscmd[1] = 0;
    poscmd[2] = z;    
    if(readcsv_andoutputonerow(poscmd[0])){
      // rotate poscmd by yaw
      poscmd_rotated[0] = poscmd[0]*cos(yaw) - poscmd[1]*sin(yaw);
      poscmd_rotated[1] = poscmd[0]*sin(yaw) + poscmd[1]*cos(yaw);
      poscmd_rotated[2] = poscmd[2];      
    }

    ROS_INFO("poscmd: %f, %f, %f", poscmd[0], poscmd[1], poscmd[2]);
    ROS_INFO("poscmd_rotated: %f, %f, %f", poscmd_rotated[0], poscmd_rotated[1], poscmd_rotated[2]);

    rate.sleep();
  }

  return 0;
}
