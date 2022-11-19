#pragma once



template<size_t LENGTH>
class FixedLenBuffer
{
private:
    /* data */
    char *current_positon;
    char data[LENGTH];

public:
    FixedLenBuffer() {
        memset(data, 0, LENGTH);
        current_positon = data;
    }
    ~FixedLenBuffer() {

    }

    size_t AvailableSize() {
        return data[LENGTH] - current_positon;
    }

    size_t Size() {
        return current_positon - data;
    }

    char *Current() {
        return current_positon;
    }

    // void append(const char *buf, size_t len)
    // {
    //     if (implicit_cast<size_t>(AvailableSize()) > len)
    //     {
    //     memcpy(cur_, buf, len);
    //     cur_ += len;
    //     }
    // }

};