#ifndef ROS_API_H
#define ROS_API_H

#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int64.h>

#include "../data/displacement.h"
#include "msg/Displacement.h"
#include "msg/PidParameters.h"
#include "msg/Position.h"
#include "topics.h"

#define DEFAULT_BAUDRATE (115200)


/* Struct used to store all the callback that will be used by the subscribers */
typedef struct {
    void (*on_set_displacement)(const msgs::Displacement&) {NULL};
    void (*on_set_position)(const msgs::Position&) {NULL};
    void (*on_set_rotation)(const std_msgs::Float32&) {NULL};
    void (*on_set_distance_ticks)(const std_msgs::Int64&) {NULL};
    void (*on_set_distance_mm)(const std_msgs::Float32&) {NULL};
    void (*on_set_pid_left_wheel)(const msgs::PidParameters&) {NULL};
    void (*on_set_pid_right_wheel)(const msgs::PidParameters&) {NULL};
    void (*on_set_pid_position_sub)(const msgs::PidParameters&) {NULL};
    void (*on_set_pid_rotation_sub)(const msgs::PidParameters&) {NULL};
    void (*on_set_display_score)(const std_msgs::Int16&) {NULL};
    void (*on_start_fan)(const std_msgs::Int16&) {NULL};
    void (*on_stop_fan)(const std_msgs::Empty&) {NULL};
} RosApiCallbacks;

class RosApi {
    RosApiCallbacks *callbacks;
    ros::NodeHandle nh;
    long baudrate {DEFAULT_BAUDRATE};

    // Publisher messages
    std_msgs::Empty distance_reached_msg;
    std_msgs::Int16 urgency_stop_msg;
    std_msgs::Int16 set_start_plate_msg;
    msgs::Displacement data_all_msg;
    std_msgs::Empty start_msg;

    // Publishers
    ros::Publisher distance_reached_pub;
    ros::Publisher urgency_stop_pub;
    ros::Publisher data_all_pub;
    ros::Publisher set_start_plate_pub;
    ros::Publisher send_start_pub;

    // Subscribers
    ros::Subscriber<msgs::Displacement> set_displacement_sub;
    ros::Subscriber<msgs::Position> set_position_sub;
    ros::Subscriber<std_msgs::Float32> set_rotation_sub;
    ros::Subscriber<std_msgs::Int64> set_distance_ticks_sub;
    ros::Subscriber<std_msgs::Float32> set_distance_mm_sub;
    ros::Subscriber<msgs::PidParameters> set_pid_left_wheel_sub;
    ros::Subscriber<msgs::PidParameters> set_pid_right_wheel_sub;
    ros::Subscriber<msgs::PidParameters> set_pid_position_sub;
    ros::Subscriber<msgs::PidParameters> set_pid_rotation_sub;
    ros::Subscriber<std_msgs::Int16> set_display_score_sub;
    ros::Subscriber<std_msgs::Int16> start_fan_sub;
    ros::Subscriber<std_msgs::Empty> stop_fan_sub;

public:
    RosApi(RosApiCallbacks *, long=DEFAULT_BAUDRATE);

    void begin(void);
    void run(void);
    void pub_distance_reached(void);
    void pub_urgency_stop(int);
    void pub_data_all(data::Displacement);
    void pub_set_start_plate(int);
    void pub_send_start(void);
};

#endif /* ROS_API_H */