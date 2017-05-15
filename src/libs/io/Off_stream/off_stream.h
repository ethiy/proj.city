#pragma once

#include <ostream>

namespace urban
{
    namespace io
    {
        /**
         * @ingroup io
         * @brief formats an output stream to the OFF
         */
        class Off_stream
        {
        public:
            /**
            * Reference constructor
            * @param _ios reference to input/output stream
            * @see Off_stream(std::ostream && _ios)
            * @see Off_stream(Off_stream & _ios)
            * @see Off_stream(Off_stream && _ios)
            */
            Off_stream(std::iostream & _ios): ios(_ios) {}
            /**
            * rvalue constructor
            * @param _ios rvalue reference to input/output stream
            * @see Off_stream(std::iostream & _ios)
            * @see Off_stream(Off_stream & other)
            * @see Off_stream(Off_stream && other)
            */
            Off_stream(std::iostream && _ios): ios(_ios) {}
            /**
            * Copy constructor
            * @param _ios reference to Adjacency stream
            * @see Off_stream(std::iostream & _ios)
            * @see Off_stream(std::iostream && _ios)
            * @see Off_stream(Off_stream && other)
            */
            Off_stream(Off_stream & other): ios(other.ios) {}
            /**
            * Move constructor
            * @param _ios reference to Adjacency stream
            * @see Off_stream(std::iostream & _ios)
            * @see Off_stream(std::iostream && _ios)
            * @see Off_stream(Off_stream & other)
            */
            Off_stream(Off_stream && other): ios(other.ios) {}

            /**
            * Defines operator<< for this stream.
            * @tparam T output value type
            * @param value value to output
            * @return reference to the Off_stream
            */
            template<typename T>
            Off_stream & operator<<(const T & value)
            {
                ios << value; 
                return *this;
            }

            /**
            * Defines operator>> for this stream.
            * @tparam T input value type
            * @param value value to input
            * @return reference to the Off_stream
            */
            template<typename T>
            Off_stream & operator>>(const T & value)
            {
                ios >> value; 
                return *this;
            }

            /**
            * Defines operator<< for this stream.
            * @param func function applied to input/output stream
            * @return reference to the Off_stream
            */
            Off_stream & operator<<(std::iostream& (*func)(std::iostream&) )
            {
                func(ios);
                return *this;
            }
        private:
            /** reference to a stream */
            std::iostream & ios;
        };
    }
}
