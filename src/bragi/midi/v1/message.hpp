/**
 * @file message.hpp
 * @brief MIDI message-related
 */
#ifndef _BRAGI_MIDI_V1_MESSAGE_HPP_
#define _BRAGI_MIDI_V1_MESSAGE_HPP_

#include <string>
#include <cstdint>

#include <bragi/midi/v1/constants.hpp>

namespace bragi::midi::v1 {
/**
 * @brief Helper function to determine the size of a given midi message based on its message type
 *
 * @param [in] msg_type Message type byte
 * @returns Size of the message - 1 if no data, 2 if 1 data byte, and so on...
 * @throws std::invalid_argument if the first bit of @b msg_type is @c 0 as this indicates a data byte
 * @throws std::runtime_error if the message type is not recognized by this library
 *
 * @note For a @b system_exclusive the return is @c 0 as there is no fixed size for this message type
 */
size_t message_size(uint8_t msg_type);

/**
 * @brief Enums for the various MIDI message types
 *
 * The first byte in a MIDI message must be the MessageType. This is differentiated by the first bit, which must be 1
 * for any MIDI message type.
 *
 * The MIDI messages can be characterized as channel-specific or channel-independent. All message types with
 * the first bit set to 1 are channel-independent - eg. Any other message is chennel-specific.
 *
 * To set a channel-specific message, set the 4 least significant bits. The valid range of channels is is [0, 16).
 *
 * Example of setting channel:
 * @code
 * // Set note_on to channel 5:
 * uint8_t c5_note_on = MessageType | 5;
 *
 * // This is an invalid message type invalid as channel 16 is not viable:
 * uint8_t c16_note_on = MessageType | 16;
 * @endcode
 */
class MessageType {
    public:
        /**
         * @brief Note OFF message
         *
         * This has @c 2 data bytes:
         *  @li pitch
         *  @li velocity
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t note_off          = 0x80;

        /**
         * @brief Note ON message
         *
         * This has @c 2 data bytes:
         *  @li pitch
         *  @li velocity
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t note_on           = 0x90;

        /**
         * @brief Key pressure message
         *
         * This has @c 2 data bytes:
         *  @li key
         *  @li pressure
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t key_pressure      = 0xA0;

        /**
         * @brief Controller change message
         *
         * This has @c 2 data bytes:
         *  @li controller
         *  @li change
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t controller_change = 0xB0;

        /**
         * @brief Program change message
         *
         * This has @c 1 data byte:
         *  @li new_preset
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t program_change    = 0xC0;

        /**
         * @brief Channel pressure message
         *
         * This has @c 1 data byte:
         *  @li pressure
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t channel_pressure  = 0xD0;

        /**
         * @brief Pitch bend message
         *
         * This has a @c 2-byte integer as data:
         *  @li bend LSB
         *  @li bend MSB
         *
         * @note This is a channel-specific message
         */
        constexpr static uint8_t pitch_bend        = 0xE0;

        /**
         * @brief System exclusive message
         *
         * This can have an unlimited number of bytes, followed by a @b end_system_exclusive byte
         */
        constexpr static uint8_t system_exclusive  = 0xF0;

        /**
         * @brief Song position message
         *
         * This has a @c 2-byte integer as data:
         *  @li position LSB
         *  @li position MSB
         */
        constexpr static uint8_t song_position     = 0xF2;

        /**
         * @brief Song select message
         *
         * This has @c 1 data byte:
         *  @li song number
         */
        constexpr static uint8_t song_select       = 0xF3;

        /**
         * @brief Bus select message
         *
         * This has @c 1 data byte:
         *  @li bus number
         */
        constexpr static uint8_t bus_select        = 0xF5;

        /**
         * @brief Tune request message
         */
        constexpr static uint8_t tune_request      = 0xF6;

        /**
         * @brief End of system exclusive message
         *
         * @warning This must follow a @b system_exclusive and end the message
         */
        constexpr static uint8_t end_of_system_exclusive = 0xF7;

        /**
         * @brief Timing tick message
         */
        constexpr static uint8_t timing_tick             = 0xF8;

        /**
         * @brief Start song message
         */
        constexpr static uint8_t start_song              = 0xFA;

        /**
         * @brief Continue song message
         */
        constexpr static uint8_t continue_song           = 0xFB;

        /**
         * @brief Stop song message
         */
        constexpr static uint8_t stop_song               = 0xFC;

        /**
         * @brief Active sensing message
         */
        constexpr static uint8_t active_sensing          = 0xFE;

        /**
         * @brief System reset message
         */
        constexpr static uint8_t system_reset            = 0xFF;

};

/**
 * @brief A MIDI message
 *
 * Enforces correctness checks on the various fields, to ensure a valid and properly formatted MIDI message is
 * serialized or parsed
 *
 * The default message is NOTE OFF on MIDDLE C
 */
class Message {
    protected:
        std::basic_string<uint8_t> bytes = {0x08, 0x3C, 0x3C, 0x00}; // reinterpret_cast<uint8_t*>("\x08\x8F\x8F");

    public:
        /// @brief Use default constructor - NOTE OFF on MIDDLE C
        Message() = default;

        /// @brief Use default destructor
        ~Message() = default;

        /**
         * @brief Create a midi message, with all data bytes set to 0
         *
         * @throws std::invalid_argument if the msg_type is not a supported MessageType
         */
        Message(uint8_t msg_type);

        /**
         * @brief Get the message type - drops the channel section if appropriate
         *
         * @throws std::runtime_error if it does not contain an actual MIDI message
         */
        uint8_t message_type() const;

        /**
         * @brief Get the message type including the channel
         *
         * @throws std::runtime_error if it does not contain an actual MIDI message
         */
        uint8_t message_type_raw() const;

        /**
         * @brief Parse a MIDI message
         *
         * @throws std::domain_error if the message type is invalid
         * @throws std::underflow_error if the message is missing data
         */
        static Message parse(std::basic_string<uint8_t> msg);

        /**
         * @brief Serialize a MIDI message
         *
         * @throws std::runtime_error if the message is improperly formatted
         */
        std::basic_string<uint8_t> serialize() const;

        // /**
        //  * @brief Create a system exclusive message
        //  */
        // static Message system_exclusive(std::basic_string<uint8_t> data);

        // /**
        //  * @brief Get data from system exclusive message
        //  *
        //  * @throws std::domain_error if the message type is not system_exclusive
        //  */
        // std::basic_string<uint8_t> get_system_exclusive_data() const;

        /**
         * @brief Set the channel on the message
         *
         * @throws std::range_error if the value is greater than 15
         * @throws std::domain_error if the message type does not support channels
         */
        Message& set_channel(uint8_t channel);

        /**
         * @brief Get the channel of the message
         *
         * @throws std::domain_error if the message type does not support channels
         */
        uint8_t get_channel() const;

        /**
         * @brief Set the first data byte
         *
         * @throws std::domain_error if the message type does not have at least 1 data byte
         * @throws std::range_error if the value is greater than 0x7F
         */
        Message& set_first_byte(uint8_t val);

        /**
         * @brief Get the first data byte
         *
         * @throws std::domain_error if the message type does not have 1 byte of data
         */
        uint8_t get_first_byte() const;

        /**
         * @brief Set the second data byte
         *
         * @throws std::domain_error if the message type does not have at least 2 data bytes
         * @throws std::range_error if the value is greater than 0x7F
         */
        Message& set_second_byte(uint8_t val);

        /**
         * @brief Get the second byte of data
         *
         * @throws std::domain_error if the message type does not have 2 bytes of data
         */
        uint8_t get_second_byte() const;

        /**
         * @brief Set a 2-byte integer in the data
         *
         * @throws std::domain_error if the message type does not use a 2-byte integer
         */
        Message& set_int(uint16_t val);

        /**
         * @brief Get a 2-byte integer from the data
         *
         * @throws std::domain_error if the message type does not use a 2-byte integer
         * @throws std::range_error if the value is greater than 0x3FFFF
         */
        uint16_t get_int() const;

        /**
         * @brief Validate the message
         *
         * @throws std::domain_error if malformed
         * @throws std::invalid_argument if empty
         */
        const Message& validate() const;

        /**
         * @brief Get size of the message
         *
         * @throws std::domain_error if malformed
         */
        size_t size() const;
};

/**
 * @brief Shortcut to create a NOTE ON message
 *
 * @param [in] pitch Pitch of note
 * @param [in] velocity Velocity of note on
 * @param [in] channel Channel of message
 *
 * @throws std::range_error if any parameter is too large
 */
Message note_on(uint8_t pitch, uint8_t velocity = max_velocity, uint8_t channel = 0);

/**
 * @brief Shortcut to create a NOTE OFF message
 *
 * @param [in] pitch Pitch of note
 * @param [in] velocity Velocity of note off
 * @param [in] channel Channel of message
 *
 * @throws std::range_error if any parameter is too large
 */
Message note_off(uint8_t pitch, uint8_t velocity = max_velocity, uint8_t channel = 0);
}

#endif
