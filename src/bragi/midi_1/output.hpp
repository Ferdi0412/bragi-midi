/**
 * @brief output.hpp
 * @brief Provides a class for sending MIDI messages
 */
#ifndef _BRAGI_MIDI_1_OUTPUT_HPP_
#define _BRAGI_MIDI_1_OUTPUT_HPP_

#include <bragi/midi_1/message.hpp>

#include <memory>
#include <mutex>

namespace bragi::midi {
class Output {
protected:
    struct Impl;
    struct ImplCleanup { void operator()(Impl* ptr) const; };

    std::unique_ptr<Impl, ImplCleanup> pimpl;
    std::mutex                         mutex;

public:
    /// @brief Disable empty constructor
    Output() = delete;

    /// @brief Cleans up resources used by output
    ~Output() = default;

    /// @brief Disable copy constructor to enforce single ownership
    Output(const Output&) = delete;

    /// @brief Disable copy assignment to enforce single ownership
    Output& operator=(const Output&) = delete;

    /**
     * @brief Select an available output
     *
     * @param [in] out_no Number of the port for the output
     *
     * @throws std::domain_error if @b out_no is invalid or does not exist
     * @throws std::system_error if failed to get details on device
     */
    Output(unsigned int out_no);

    /**
     * @brief Retrieve count of existing devices
     */
    static unsigned int output_count();

    /**
     * @brief Connect to a MIDI output
     *
     * @throws std::logic_error already connected
     * @throws std::runtime_error if not a valid output
     * @throws std::system_error if failed to connect
     */
    void connect();

    /**
     * @brief Disconnect from a MIDI output
     *
     * @throws std::logic_error if not connected
     * @throws std::runtime_error if not a valid output
     */
    void disconnect();

    /**
     * @brief Send a MIDI message to the output target
     *
     * @param [in] msg The message to send
     *
     * @throws Whatever is thrown by Message::validate()
     * @throws std::runtime_error if not a valid output
     * @throws std::logic_error if not connected
     * @throws std::system_error if failed to send
     */
    void send_msg(const Message& msg);

    /**
     * @brief Check if output is a port to a physical MIDI
     *
     * @throws std::runtime_error if not a valid output
     * @throws std::system_error if failed to check
     */
    bool physical_device() const;

    /**
     * @brief Get the manufacturer ID
     *
     * @throws std::runtime_error if not a valid output
     * @throws std::system_error if failed to check
     */
    uint16_t manufacturer_id() const;

    /**
     * @brief Get the product ID
     *
     * @throws std::runtime_error if not a valid output
     * @throws std::system_error if failed to check
     */
    uint16_t product_id() const;

    /**
     * @brief Get the product name
     *
     * @throws std::runtime_error if not a valid output
     * @throws std::system_error if failed to check
     */
    std::string product_name() const;
};
}

#endif
