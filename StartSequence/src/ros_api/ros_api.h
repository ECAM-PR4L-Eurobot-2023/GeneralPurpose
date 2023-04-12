#ifndef ROS_API_H
#define ROS_API_H

#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int64.h>

#include "../data/displacement.h"
#include "topics.h"

#define DEFAULT_BAUDRATE (115200)


/* Struct used to store all the callback that will be used by the subscribers */
typedef struct {

    void (*on_set_rotation)(const std_msgs::Float32&) {NULL};
    void (*on_set_distance_ticks)(const std_msgs::Int64&) {NULL};
    void (*on_set_distance_mm)(const std_msgs::Float32&) {NULL};
    void (*on_set_display_score)(const std_msgs::Int16&) {NULL};
    void (*on_start_fan)(const std_msgs::Int16&) {NULL};
    void (*on_stop_fan)(const std_msgs::Empty&) {NULL};
    void (*on_close_door)(const std_msgs::Empty&) {NULL};
    void (*on_open_door)(const std_msgs::Empty&) {NULL};
} RosApiCallbacks;

class RosApi {
    RosApiCallbacks *callbacks;
    ros::NodeHandle nh;
    long baudrate {DEFAULT_BAUDRATE};

    // Publisher messages
    std_msgs::Empty distance_reached_msg;
    std_msgs::Int16 urgency_stop_msg;
    std_msgs::Int16 set_start_plate_msg;
    std_msgs::Empty start_msg;

    // Publishers
    ros::Publisher urgency_stop_pub;
    ros::Publisher set_start_plate_pub;
    ros::Publisher send_start_pub;

    // Subscribers
    ros::Subscriber<std_msgs::Float32> set_rotation_sub;
    ros::Subscriber<std_msgs::Int64> set_distance_ticks_sub;
    ros::Subscriber<std_msgs::Float32> set_distance_mm_sub;
    ros::Subscriber<std_msgs::Int16> set_display_score_sub;
    ros::Subscriber<std_msgs::Int16> start_fan_sub;
    ros::Subscriber<std_msgs::Empty> stop_fan_sub;
    ros::Subscriber<std_msgs::Empty> close_door_sub;
    ros::Subscriber<std_msgs::Empty> open_door_sub;

public:
    RosApi(RosApiCallbacks *, long=DEFAULT_BAUDRATE);

    void begin(void);
    void run(void);
    void pub_distance_reached(void);
    void pub_urgency_stop(int);
    void pub_set_start_plate(int);
    void pub_send_start(void);
};

#endif /* ROS_API_H */