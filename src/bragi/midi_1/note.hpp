/**
 * @file note.hpp
 * @brief To make it easy to play around with, a RAII style NOTE ON/OFF object
 */
#ifndef _BRAGI_MIDI_1_NOTE_HPP_
#define _BRAGI_MIDI_1_NOTE_HPP_

#include <cstdint>
#include <memory>

#include <bragi/midi_1/output.hpp>
#include <bragi/midi_1/constants.hpp>

namespace bragi::midi {
/**
 * @brief RAII implementation to play a single note
 *
 * @warning This uses a weak pointer to the output, so no NOTE OFF is guaranteed if this is in the same code-block as
 *          the actual output.
 *
 * @code
 * std::shared_ptr<Output> output = std::make_shared<Output>(0);
 *
 * {
 *  // This note is guaranteed to play and stop...
 *  Note(output);
 *  Sleep(1000);
 * }
 *
 * // This note is guaranteed to play, but not guaranteed to stop... - undefined behaviour
 * Note(output);
 * Sleep(1000);
 */
class Note {
    protected:
        std::weak_ptr<Output> output;
        uint8_t               pitch;
        uint8_t               velocity;
        uint8_t               channel;

    public:
        /**
         * @brief Construct using a non-owning pointer to the output and the note parameters.
         */
        Note(std::weak_ptr<Output> output, uint8_t pitch = middle_c, uint8_t velocity = max_velocity, uint8_t channel = 0);

        /// @brief Custom destructor to ensure NOTE OFF plays assuming output is still alive
        ~Note();
};
}

#endif //_BRAGI_MIDI_1_NOTE_HPP_//
