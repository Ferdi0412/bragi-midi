/**
 * @file constants.hpp
 * @brief MIDI constants for ease-of-use
 */
#ifndef _BRAGI_MIDI_V1_CONSTANTS_HPP_
#define _BRAGI_MIDI_V1_CONSTANTS_HPP_

#include <cstdint>

namespace bragi::midi::v1 {
/**
 * @brief Middle C note for NOTE ON and NOTE OFF messages
 */
constexpr uint8_t middle_c = 0x3C;

/**
 * @brief Max velocity
 */
constexpr uint8_t max_velocity = 0x7F;

// /**
//  * @brief Common controller numbers
//  */
// class ControllerType {
//     public:
//         /**
//          * @brief A modulation wheel
//          */
//         constexpr static uint8_t modulation = 0x01;
//         constexpr static uint8_t breath_controller = 0x02;
//         constexpr static uint8_t foot_controller   = 0x04;
//         constexpr static uint8_t portamento_time   = 0x05;
//         constexpr static uint8_t data_entry_slider = 0x06;
//         constexpr static uint8_t main_volume       = 0x07;
//         constexpr static uint8_t balance           = 0x08;
//         constexpr static uint8_t pan               = 0x0A;
//         constexpr static uint8_t expression        = 0x0B;


// };
}

#endif //_BRAGI_MIDI_1_CONSTANTS_HPP_//
