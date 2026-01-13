#pragma once

#include <cstring>
#include <string>
#include <cerrno>

namespace http_server
{
    namespace details
    {
        struct Status
        {
            int code{0};

            bool ok() const { return code == 0; }
            bool failed() const { return code != 0; }

            int value() const { return code; }

            std::string message() const
            {
                if (ok())
                {
                    return "Success";
                }

                return std::strerror(code);
            }

            static Status Success()
            {
                return Status{0};
            }

            static Status FromErrno()
            {
                return Status{errno};
            }
        };

        struct IoResult
        {
            int bytes{0};
            Status status{};

            bool retry() const
            {
                return status.value() == EAGAIN || status.value() == EWOULDBLOCK;
            }

            bool ok() const
            {
                return status.ok();
            }
        };
    }
}