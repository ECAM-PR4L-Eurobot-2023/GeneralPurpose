#include "ros_api.h"

RosApi::RosApi(RosApiCallbacks *callbacks, long baudrate) :
    callbacks(callbacks), 
    baudrate(baudrate),
    set_start_plate_pub(TOPIC_SET_START_PLATE, &set_start_plate_msg),
    urgency_stop_pub(TOPIC_URGENCY_STOP, &urgency_stop_msg),
    send_start_pub(TOPIC_START, &start_msg),
    set_distance_ticks_sub(TOPIC_SET_DISTANCE_TICKS, callbacks->on_set_distance_ticks),
    set_distance_mm_sub(TOPIC_SET_DISTANCE_MM, callbacks->on_set_distance_mm),
    set_display_score_sub(TOPIC_SET_DISPLAY_SCORE,callbacks->on_set_display_score),
    start_fan_sub(TOPIC_FAN_ON,callbacks->on_start_fan),
    stop_fan_sub(TOPIC_FAN_OFF, callbacks->on_stop_fan),
    close_door_sub(TOPIC_CLOSE_DOOR, callbacks->on_close_door),
    open_door_sub(TOPIC_OPEN_DOOR, callbacks->on_open_door),
    disguise_sub(TOPIC_DISGUISE, callbacks->on_disguise),
    disguise_release_sub(TOPIC_DISGUISE_RELEASE, callbacks->on_disguise_release),
    end_sub(TOPIC_END,callbacks->on_end) {}


void RosApi::begin(void) {
    nh.getHardware()->setBaud(baudrate);

    // // Init ROS node
    nh.initNode();

    // // Advertize publishers
    nh.advertise(urgency_stop_pub);
    nh.advertise(set_start_plate_pub);
    nh.advertise(send_start_pub);

    // // Subscribe
    nh.subscribe(set_distance_ticks_sub);
    nh.subscribe(set_distance_mm_sub);
    nh.subscribe(set_display_score_sub);
    nh.subscribe(start_fan_sub);
    nh.subscribe(stop_fan_sub);
    nh.subscribe(close_door_sub);
    nh.subscribe(open_door_sub);
    nh.subscribe(disguise_sub);
    nh.subscribe(disguise_release_sub);
    nh.subscribe(end_sub);
}

void RosApi::run(void) {
    nh.spinOnce();
}


void RosApi::pub_urgency_stop(int urgency_stop) {
    urgency_stop_msg.data = (int16_t)urgency_stop;
    urgency_stop_pub.publish(&urgency_stop_msg);
}

void RosApi::pub_set_start_plate(int start_plate) {
    set_start_plate_msg.data = (int16_t)start_plate;
    set_start_plate_pub.publish(&set_start_plate_msg);
}

void RosApi::pub_send_start(void){
    send_start_pub.publish(&start_msg);
}