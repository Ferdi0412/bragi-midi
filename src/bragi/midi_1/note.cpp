#include <bragi/midi_1/note.hpp>
#include <bragi/midi_1/message.hpp>

#include <exception>

namespace bragi::midi {
Note::Note(std::weak_ptr<Output> output, uint8_t pitch, uint8_t velocity, uint8_t channel):
        output(output),
        pitch(pitch),
        velocity(velocity),
        channel(channel)
    {
        std::shared_ptr<Output> temp = output.lock();

        temp->send_msg(note_on(pitch, velocity, channel));
    }

Note::~Note() {
    try {
        std::shared_ptr<Output> temp = output.lock();

        temp->send_msg(note_off(pitch, velocity, channel));
    } catch ( std::exception ) {

    }
}
}
