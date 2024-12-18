#include <bragi/midi/v1/message.hpp>

#include <stdexcept>

namespace bragi::midi::v1 {
size_t message_size(uint8_t msg_type) {
    if ( !(msg_type & 0x80) )
        throw std::invalid_argument("First byte must be 1 to check message type!");

    // Get the channel-specific type section
    uint8_t chan_type = msg_type & 0xF0;

    if ( chan_type < 0xF0 )
        return chan_type < MessageType::program_change || chan_type == MessageType::pitch_bend ? 3 : 2;

    switch ( msg_type ) {
        case MessageType::system_exclusive:
            return 0;

        case MessageType::song_position:
            return 3;

        case MessageType::song_select:
        case MessageType::bus_select:
            return 2;

        case MessageType::tune_request:
        case MessageType::timing_tick:
        case MessageType::start_song:
        case MessageType::continue_song:
        case MessageType::stop_song:
        case MessageType::active_sensing:
        case MessageType::system_reset:
            return 1;

        default:
            throw std::runtime_error("Could not recognize the message type!");
    }
}

Message::Message(uint8_t msg_type) {
    bytes.assign(message_size(msg_type), 0);
    if ( bytes.size() == 0 ) // For system_exclusive
        bytes = {MessageType::system_exclusive, 0};
    bytes[0] = msg_type;
}

uint8_t Message::message_type() const {
    if ( (bytes[0] & 0xF0) < 0xF0 )
        return bytes[0] & 0xF0;

    return bytes[0];
}

uint8_t Message::message_type_raw() const {
    return bytes[0];
}

Message Message::parse(std::basic_string<uint8_t> msg) {
    if ( msg.size() < 1 )
        throw std::underflow_error("Empty midi message!");

    size_t msg_size = message_size(msg[0]);

    if ( msg_size < msg.size() )
        throw std::underflow_error("Missing data!");

    Message message;

    if ( msg_size > 0 )
        message.bytes = msg.substr(0, msg_size);

    else  {
        bool found = false;
        for ( size_t i = 0; i < msg.size(); i++ ) {
            if ( msg[i] == MessageType::end_of_system_exclusive ) {
                found = true;
                message.bytes = msg.substr(0, i);
                break;
            }
        }
        if ( !found )
            throw std::invalid_argument("Malformed system exclusive!");
    }

    return message;
}

std::basic_string<uint8_t> Message::serialize() const {
    return bytes;
}

// Message Message::system_exclusive(std::basic_string<uint8_t> data) {
//     data.insert(0, MessageType::system_exclusive);
//     data.push_back(MessageType::end_of_system_exclusive);

//     return Message::parse(std::move(data));
// }

Message& Message::set_channel(uint8_t channel) {
    if ( channel > 15 )
        throw std::range_error("Channel too large!");

    if ( (bytes[0] & 0xF0) < 0xF0 )
        bytes[0] |= channel;

    else
        throw std::domain_error("Channel not supported!");

    return *this;
}

uint8_t Message::get_channel() const {
    if ( (bytes[0] & 0xF0) < 0xF0 )
        return bytes[0] & 0x0F;

    throw std::domain_error("Channel not supported!");
}

Message& Message::set_first_byte(uint8_t val) {
    if ( bytes.size() < 2 )
        throw std::domain_error("No data byte!");

    bytes[1] = val;

    return *this;
}

uint8_t Message::get_first_byte() const {
    if ( bytes.size() < 2 )
        throw std::domain_error("No data byte!");

    return bytes[1];
}

Message& Message::set_second_byte(uint8_t val) {
    if ( bytes.size() < 3 )
        throw std::domain_error("No second data byte!");

    bytes[2] = val;

    return *this;
}

uint8_t Message::get_second_byte() const {
    if ( bytes.size() < 3 )
        throw std::domain_error("No second data byte!");

    return bytes[2];
}

Message& Message::set_int(uint16_t val) {
    if ( val > 0x3FFF )
        throw std::range_error("Value to large!");

    if ( bytes[0] == MessageType::song_position || (bytes[0] & 0xF0) == MessageType::pitch_bend ) {
        bytes[1] = val >> 7;
        bytes[2] = val & 0x7F;
    }

    else
        throw std::domain_error("Non int data!");

    return *this;
}

uint16_t Message::get_int() const {
    if ( bytes[0] == MessageType::song_position || (bytes[0] & 0xF0) == MessageType::pitch_bend )
        return bytes[2] | bytes[1] << 7;
    throw std::domain_error("Non int data!");
}

const Message& Message::validate() const {
    // All operations should lead to a valid message...
    return *this;
}

size_t Message::size() const {
    return bytes.size();
}

Message note_on(uint8_t pitch, uint8_t velocity, uint8_t channel) {
    Message message(MessageType::note_on | channel);
    message.set_first_byte(pitch);
    message.set_second_byte(velocity);
    return message;
}

Message note_off(uint8_t pitch, uint8_t velocity, uint8_t channel) {
    Message message(MessageType::note_off | channel);
    message.set_first_byte(pitch);
    message.set_second_byte(velocity);
    return message;
}
}
