#include <bragi/midi_1/midi.hh>

#include <chrono>
#include <thread>

using namespace bragi::midi;

int main() {
    // Ensure at least 1 midi device available - this is guaranteed only for Windows
    std::shared_ptr<Output> output = std::make_shared<Output>(0);

    output->connect();

    // Using the Note class for RAII-style note on/off - play for 1 second
    {
        Note c(output);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Wait 0.5 seconds for next note on
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Sending NOTE ON/OFF message to play for 1 second
    output->send_msg(note_on(middle_c));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    output->send_msg(note_off(middle_c));

    // Wait 0.5 seconds for next note on
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Or format message to form NOTE ON/OFF and play for 1 second
    Message on = Message(MessageType::note_on).set_first_byte(middle_c).set_second_byte(max_velocity);
    output->send_msg(on);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    Message off = Message(MessageType::note_off).set_first_byte(middle_c).set_second_byte(max_velocity);
    output->send_msg(off);
}
