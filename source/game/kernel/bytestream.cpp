#include "bytestream.h"

const std::vector<char>& obytestream::GetBuffer() const
{
    return buffer_;
}

ibytestream::ibytestream(std::vector<char>&& data)
    : buffer_(std::move(data))
{
}
