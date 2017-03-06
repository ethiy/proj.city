#include "../Brick/brick_projection.h"
#include "../../algorithms/projection/projection_algorithms.h"
#include "../../shadow/Point/point.h"

#include <string>

namespace urban
{
    namespace projection
    {
        class RasterPrint
        {
        public:
            RasterPrint(void);
            RasterPrint(const RasterPrint & other);
            RasterPrint(RasterPrint && other);
            ~RasterPrint(void);

            void swap(RasterPrint & other);

            RasterPrint & operator=(const RasterPrint & other) noexcept;
            RasterPrint & operator=(RasterPrint && other) noexcept;

            RasterPrint & rescale(double _pixel_size);

            RasterPrint & operator+=(const RasterPrint & other);
            RasterPrint & operator-=(const RasterPrint & other);
        private:
            std::string name;
            Point reference_point;
            size_t length;
            size_t width;
            double pixel_size;
            std::vector<uint16_t> pixels;
        };

        RasterPrint & operator+=(const RasterPrint & lhs, const RasterPrint & rhs);
        RasterPrint & operator-=(const RasterPrint & lhs, const RasterPrint & rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);
}
