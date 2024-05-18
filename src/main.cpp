#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Vector3.h>
#include <tf/tf.h>

class PoseToAnglesConverter
{
public:
    PoseToAnglesConverter()
    {
        
        pose_sub_ = nh_.subscribe("slam_out_pose", 10, &PoseToAnglesConverter::poseCallback, this);

        
        angles_pub_ = nh_.advertise<geometry_msgs::Vector3>("angles", 10);
    }

private:
    void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
    {
        
        tf::Quaternion q(
            msg->pose.orientation.x,
            msg->pose.orientation.y,
            msg->pose.orientation.z,
            msg->pose.orientation.w
        );
        
        double roll, pitch, yaw;
        tf::Matrix3x3(q).getRPY(roll, pitch, yaw);

        
        float roll_deg = normalizeAngle(roll * 180.0 / M_PI);
        float pitch_deg = normalizeAngle(pitch * 180.0 / M_PI);
        float yaw_deg = normalizeAngle(yaw * 180.0 / M_PI);

        
        geometry_msgs::Vector3 angles_msg;
        angles_msg.x = roll_deg;
        angles_msg.y = pitch_deg;
        angles_msg.z = yaw_deg;
        angles_pub_.publish(angles_msg);
    }

    float normalizeAngle(float angle)
    {
        if (angle < 0) {
            angle += 360.0;
        }
        return fmod(angle, 360.0);
    }

    ros::NodeHandle nh_;
    ros::Subscriber pose_sub_;
    ros::Publisher angles_pub_;
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "pose_to_yaw_node");

    PoseToAnglesConverter converter;

    ros::spin();

    return 0;
}