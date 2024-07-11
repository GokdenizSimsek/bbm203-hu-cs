//
// Created by alperen on 2.10.2023.
//

#include "Log.h"

Log::Log(const string &_timestamp, const string &_message, int _number_of_frames, int _number_of_hops, const string &_sender_id,
         const string &_receiver_id, bool _success, ActivityType _type) {
    timestamp = _timestamp;
    message_content = _message;
    number_of_frames = _number_of_frames;
    number_of_hops = _number_of_hops;
    sender_id = _sender_id;
    receiver_id = _receiver_id;
    success_status = _success;
    activity_type = _type;
}

Log::~Log() {
    // TODO: Free any dynamically allocated memory if necessary.
}