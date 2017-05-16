#pragma once

#include <ostream>

#include <vector>
#include <iterator>

#include <stdexcept>

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
            * Copy constructor
            * @param other reference to Adjacency stream
            * @see Adjacency_stream(std::iostream & _ios)
            * @see Adjacency_stream(std::iostream && _ios)
            * @see Adjacency_stream(Adjacency_stream && other)
            */
            Adjacency_stream(Adjacency_stream & other): ios(other.ios) {}

            /**
            * Defines operator << for this stream.
            * @tparam T output value type
            * @param value value to output
            * @return reference to the Adjacency_stream
            */
            template<typename T>
            Adjacency_stream & operator <<(T const& value)
            {
                ios << value; 
                return *this;
            }

            /**
            * Defines operator << for a matrix.
            * @param matrix matrix to output
            * @return reference to the Adjacency_stream
            */
            Adjacency_stream & operator <<(std::vector<bool> const& matrix)
            {
                auto n = std::sqrt(matrix.size());
                if(n * n != matrix.size())
                    throw std::logic_error("The adjacency matrix must be square!");

                for(std::size_t row(0); row != n; ++row)
                {
                    for(std::size_t col(0); col != n - 1; ++col)
                        ios << matrix.at(row * n + col) << " ";
                    ios << matrix.at(row * n + (n-1)) << std::endl;
                }
                return *this;
            }

            /**
            * Defines operator << for this stream.
            * @param func function applied to output stream
            * @return reference to the Adjacency_stream
            */
            Adjacency_stream & operator <<(std::ostream& (*func)(std::ostream&) )
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
