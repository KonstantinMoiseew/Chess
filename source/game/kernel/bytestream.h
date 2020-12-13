#pragma once

#include <vector>
#include <type_traits>

class obytestream
{
public:

    const std::vector<char>& GetBuffer() const;

protected:

    template <typename T, typename >
    friend void operator << (obytestream& os, const T& value);

    std::vector<char> buffer_;
};

class ibytestream
{
public:

    ibytestream(std::vector<char>&& data);

protected:

    template <typename T, typename >
    friend bool operator >> (ibytestream& os, T& value);

    std::vector<char> buffer_;
    uint16_t readPos_ = 0;
};


template <typename T,
          typename = std::enable_if_t< !std::is_pointer< T >::value> >
void operator << (obytestream& os, const T& value)
{
    os.buffer_.insert(os.buffer_.end(), (const char*)&value, (const char*)(&value+1));
}

template <typename T,
          typename = std::enable_if_t< !std::is_pointer< T >::value> >
bool operator >> (ibytestream& is, T& value)
{
    if (is.readPos_ + sizeof(T) > is.buffer_.size())
        return false;
    value = (*(T*)(is.buffer_.data() + is.readPos_));
    is.readPos_ += sizeof(T);
    return true;
}
