#pragma once

#include <ostream>

namespace urban
{
    namespace io
    {
        /**
         * @ingroup io
         * @brief formats an output stream to write graph adjacency matrices
         */
        class Adjacency_stream
        {
        public:
            /**
            * Reference constructor
            * @param _ios reference to input/output stream
            * @see Adjacency_stream(std::iostream && _ios)
            * @see Adjacency_stream(Adjacency_stream & _ios)
            * @see Adjacency_stream(Adjacency_stream && _ios)
            */
            Adjacency_stream(std::iostream & _ios): ios(_ios) {}
            /**
            * rvalue constructor
            * @param _ios rvalue reference to input/output stream
            * @see Adjacency_stream(std::iostream & _ios)
            * @see Adjacency_stream(Adjacency_stream & other)
            * @see Adjacency_stream(Adjacency_stream && other)
            */
            Adjacency_stream(std::iostream && _ios): ios(_ios) {}
            /**
            * Copy constructor
            * @param other reference to Adjacency stream
            * @see Adjacency_stream(std::iostream & _ios)
            * @see Adjacency_stream(std::iostream && _ios)
            * @see Adjacency_stream(Adjacency_stream && other)
            */
            Adjacency_stream(Adjacency_stream & other): ios(other.ios) {}
            /**
            * Move constructor
            * @param other reference to Adjacency stream
            * @see Adjacency_stream(std::iostream & _ios)
            * @see Adjacency_stream(std::iostream && _ios)
            * @see Adjacency_stream(Adjacency_stream & other)
            */
            Adjacency_stream(Adjacency_stream && other): ios(other.ios) {}

            /**
            * Defines operator<< for this stream.
            * @tparam T output value type
            * @param value value to output
            * @return reference to the Adjacency_stream
            */
            template<typename T>
            Adjacency_stream & operator<<(const T & value)
            {
                ios << value; 
                return *this;
            }

            /**
            * Defines operator<< for this stream.
            * @param func function applied to output stream
            * @return reference to the Adjacency_stream
            */
            Adjacency_stream & operator<<(std::ostream& (*func)(std::ostream&) )
            {
                func(ios);
                return *this;
            }
        private:
            /** reference to an output stream */
            std::iostream & ios;
        };
    }
}
