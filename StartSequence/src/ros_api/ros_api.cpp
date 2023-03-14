#include "ros_api.h"

RosApi::RosApi(RosApiCallbacks *callbacks, long baudrate) :
    callbacks(callbacks), 
    baudrate(baudrate),
    set_start_plate_pub(TOPIC_SET_START_PLATE, &set_start_plate_msg),
    distance_reached_pub(TOPIC_DISTANCE_REACHED, &distance_reached_msg),
    urgency_stop_pub(TOPIC_URGENCY_STOP, &urgency_stop_msg),
    data_all_pub(TOPIC_DATA_ALL, &data_all_msg),
    set_displacement_sub(TOPIC_SET_DISPLACEMENT, callbacks->on_set_displacement),
    set_position_sub(TOPIC_SET_POSITION, callbacks->on_set_position),
    set_rotation_sub(TOPIC_SET_ROTATION, callbacks->on_set_rotation),
    set_distance_ticks_sub(TOPIC_SET_DISTANCE_TICKS, callbacks->on_set_distance_ticks),
    set_distance_mm_sub(TOPIC_SET_DISTANCE_MM, callbacks->on_set_distance_mm),
    set_pid_left_wheel_sub(TOPIC_SET_PID_LEFT_WHEEL, callbacks->on_set_pid_left_wheel),
    set_pid_right_wheel_sub(TOPIC_SET_PID_RIGHT_WHEEL, callbacks->on_set_pid_right_wheel),
    set_pid_position_sub(TOPIC_SET_PID_POSITION, callbacks->on_set_pid_position_sub),
    set_pid_rotation_sub(TOPIC_SET_PID_ANGLE, callbacks->on_set_pid_rotation_sub),
    set_display_score_sub(TOPIC_SET_DISPLAY_SCORE,callbacks->on_set_display_score),
    start_fan_sub(TOPIC_FAN_ON,callbacks->on_start_fan),
    stop_fan_sub(TOPIC_FAN_OFF, callbacks->on_stop_fan) {}


void RosApi::begin(void) {
    nh.getHardware()->setBaud(baudrate);

    // // Init ROS node
    nh.initNode();

    // // Advertize publishers
    nh.advertise(distance_reached_pub);
    nh.advertise(urgency_stop_pub);
    nh.advertise(data_all_pub);
    nh.advertise(set_start_plate_pub);

    // // Subscribe
    nh.subscribe(set_displacement_sub);
    nh.subscribe(set_position_sub);
    nh.subscribe(set_rotation_sub);
    nh.subscribe(set_distance_ticks_sub);
    nh.subscribe(set_distance_mm_sub);
    nh.subscribe(set_pid_left_wheel_sub);
    nh.subscribe(set_pid_right_wheel_sub);
    nh.subscribe(set_pid_position_sub);
    nh.subscribe(set_pid_rotation_sub);
    nh.subscribe(set_display_score_sub);
    nh.subscribe(start_fan_sub);
    nh.subscribe(stop_fan_sub);
}

void RosApi::run(void) {
    nh.spinOnce();
}

void RosApi::pub_distance_reached(void) {
    distance_reached_pub.publish(&distance_reached_msg);
}

void RosApi::pub_urgency_stop(int urgency_stop) {
    urgency_stop_msg.data = (int16_t)urgency_stop;
    urgency_stop_pub.publish(&urgency_stop_msg);
}

void RosApi::pub_data_all(data::Displacement displacement) {
    data_all_msg.angle_start = displacement.angle_start;
    data_all_msg.angle_end = displacement.angle_end;
    data_all_msg.distance = displacement.distance;
    data_all_pub.publish(&data_all_msg);
}

void RosApi::pub_set_start_plate(int start_plate) {
    set_start_plate_msg.data = (int16_t)start_plate;
    set_start_plate_pub.publish(&set_start_plate_msg);
}