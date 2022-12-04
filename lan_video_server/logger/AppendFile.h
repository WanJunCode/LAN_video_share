#pragma once

#include <string.h>
#include <assert.h>

class AppendFile
{
public:
    explicit AppendFile(std::string filename) : fp_(::fopen(filename.c_str(), "ae"))
    {
        assert(fp_);
        ::setbuffer(fp_, buffer_, sizeof buffer_);
    }

    ~AppendFile()
    {
        ::fclose(fp_);
    }

    void append(const char *logline, size_t len)
    {
        size_t written = 0;

        while (written != len)
        {
            size_t remain = len - written;
            size_t n = write(logline + written, remain);
            if (n != remain)
            {
                int err = ferror(fp_);
                if (err)
                {
                    fprintf(stderr, "AppendFile::append() failed\n");
                    break;
                }
            }
            written += n;
        }

        writtenBytes_ += written;
    }

    void flush()
    {
        ::fflush(fp_);
    }

    off_t writtenBytes() const { return writtenBytes_; }

private:
    size_t write(const char *logline, size_t len)
    {
        return ::fwrite_unlocked(logline, 1, len, fp_);
    }

    FILE *fp_;
    char buffer_[64 * 1024];
    off_t writtenBytes_ = {0};
};