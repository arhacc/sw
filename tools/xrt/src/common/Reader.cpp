//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/Reader.h>

#include <stdexcept>

// TODO: Test this function
#if 0
template<typename BR, typename T, T (*Tctor)(T*)>
size_t Reader<BR, T, Tctor>::read(std::span<T> _buf) {
    size_t _nbytes = _buf.size() * sizeof(T);
    size_t _bytesRead = 0;
    
    if constexpr (sizeof(T) > 1) {
        // copy incomplete object from buffer
        std::copy(
            incompleteObjectBuffer.begin(),
            incompleteObjectBuffer.end(),
            _buf.begin()
        );

        _nbytes -= incompleteObjectBuffer.size();
        _bytesRead += incompleteObjectBuffer.size();
    }

    _bytesRead += byteReader.read(
        std::span<uint8_t>(
            reinterpret_cast<uint8_t*>(_buf.data()) + _bytesRead,
            _nbytes
        )
    );

    // do not compile this check for char
    if constexpr (sizeof(T) > 1) {
        // incomplete object
        size_t _incompleteBytes = _bytesRead % sizeof(T);
        size_t _completeBytes = _bytesRead - _incompleteBytes;

        // copy incomplete object to buffer
        std::copy(
            _buf.begin() + _completeBytes,
            _buf.begin() + _completeBytes + _incompleteBytes,
            incompleteObjectBuffer.begin()
        );
    }

    size_t _objectsRead = _bytesRead / sizeof(T);

    if constexpr (Tctor != nullptr) {
        for (size_t i = 0; i < _objectsRead; i++) {
            Tctor(&_buf[i]);
        }
    }

    return _objectsRead;
}

#endif
