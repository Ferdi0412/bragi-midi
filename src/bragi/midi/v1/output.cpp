#ifndef _WIN32
    #error "No implementation for non-windows machines added!"
#endif

#include <Windows.h>
#include <mmeapi.h>
// #include <mmsystem.h>
// winmm.lib

#include <stdexcept>
#include <system_error>

#include <bragi/midi/v1/output.hpp>

namespace bragi::midi::v1 {
void throw_sys_err(int err_code) {
    throw std::system_error(std::error_code(err_code, std::system_category()));
}

/********************************/
/* Impl                         */
/********************************/
struct Output::Impl {
    UINT        device;
    MIDIOUTCAPS details = {0};
    HMIDIOUT    connection = nullptr;

    Impl(UINT dev): device(dev) {
        int err = ::midiOutGetDevCaps(dev, &details, sizeof(details));

        if ( err != MMSYSERR_NOERROR )
            throw_sys_err(err);
    }

    void disconnect() {
        if ( connection ) {
            ::midiOutClose(connection);
            connection = nullptr;
        }
    }

    void connect() {
        if ( connection )
            throw std::logic_error("Already connected!");

        int err = ::midiOutOpen(&connection, device, 0, 0, CALLBACK_NULL);

        if ( err != MMSYSERR_NOERROR )
            throw_sys_err(err);
    }

    void send_short(uint8_t msg_type, uint8_t b1, uint8_t b2) {
        if ( !connection )
            throw std::logic_error("Not connected!");

        DWORD msg = msg_type | b1 << 8 | b2 << 16;
        int err = ::midiOutShortMsg(connection, msg);
        if ( err != MMSYSERR_NOERROR )
            throw_sys_err(err);
    }

    ~Impl() {
        disconnect();
    }
};

void Output::ImplCleanup::operator()(Impl* ptr) const {
    if ( ptr )
        delete ptr;
}

/********************************/
/* Implementation               */
/********************************/
Output::Output(unsigned int out_no) {
    pimpl.reset(new Impl(out_no));
}

unsigned int Output::output_count() {
    return ::midiOutGetNumDevs();
}

void Output::connect() {
    std::lock_guard<std::mutex> lock(mutex);
    pimpl->connect();
}

void Output::disconnect() {
    std::lock_guard<std::mutex> lock(mutex);
    pimpl->disconnect();
}

void Output::send_msg(const Message& msg) {
    std::lock_guard<std::mutex> lock(mutex);
    msg.validate();

    switch( msg.size() ) {
        case 1:
            return pimpl->send_short(msg.message_type_raw(), 0, 0);

        case 2:
            return pimpl->send_short(msg.message_type_raw(), msg.get_first_byte(), 0);

        case 3:
            return pimpl->send_short(msg.message_type_raw(), msg.get_first_byte(), msg.get_second_byte());

        default:
            // send_long(...);
            throw std::runtime_error("Not implemented for size greater than 3 yet!");
    }
}

bool Output::physical_device() const {
    return pimpl->details.wTechnology == MOD_MIDIPORT;
}

uint16_t Output::manufacturer_id() const {
    return pimpl->details.wMid;
}

uint16_t Output::product_id() const {
    return pimpl->details.wPid;
}

std::string Output::product_name() const {
    if ( pimpl->details.szPname )
        // return {pimpl->details.szPname, pimpl->details.szPname + wcslen(pimpl->details.szPname)};
        return {pimpl->details.szPname, pimpl->details.szPname + strlen(pimpl->details.szPname)};

    return "";
}
}




// struct _OutputImpl {
//     HMIDIOUT    target = nullptr;
//     UINT        port;
//     MIDIOUTCAPS details = {0};
//     bool        connected = false;
// };

// static inline _OutputImpl* get_output(void* pimpl) {
//     if ( pimpl == nullptr )
//         throw std::runtime_error("Incomplete Output!");

//     _OutputImpl* impl = reinterpret_cast<_OutputImpl*>(pimpl);

//     if ( impl->target == nullptr )
//         throw std::runtime_error("Malformed Output!");

//     return impl;
// }

// static void cleanup(void* pimpl) {
//     if ( !pimpl )
//         return;

//     try {
//         _OutputImpl* impl = get_output(pimpl);

//         if ( impl->connected ) {
//             ::midiOutClose(impl->target);
//         }


//     } catch ( std::runtime_error ) {
//         return;
//     }
// }

// namespace bragi::midi {
//     Output::~Output() {
//         cleanup(pimpl);
//     }
// }
