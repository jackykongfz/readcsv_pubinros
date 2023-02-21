#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <std_msgs/String.h>

using namespace std;

int main(int argc, char** argv) {
  ros::init(argc, argv, "csv_publisher");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<std_msgs::String>("csv_column", 1000);
  ros::Rate rate(50);

  string file_path = "/home/jackykong/velocity9_trajectory_setpoints.csv"; // replace with your file path
  ifstream file(file_path.c_str());

  if (!file.is_open()) {
    ROS_ERROR("Failed to open CSV file");
    return -1;
  }

  string line, word;
  vector<string> csv_row;
  while (getline(file, line)) {
    csv_row.clear();
    stringstream s(line);

    while (getline(s, word, ',')) {
      csv_row.push_back(word);
    }

    if (csv_row.size() >= 3) {
      std_msgs::String msg;
      msg.data = csv_row[2];
      pub.publish(msg);
      ROS_INFO("Publish x position: %f", atof(csv_row[2].c_str()));
    }
    rate.sleep();
  }

  return 0;
}
