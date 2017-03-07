#include "../Brick/brick_projection.h"
#include "../../algorithms/projection/projection_algorithms.h"
#include "../../shadow/Point/point.h"

#include <opencv2/core.hpp>

#include <gdal_priv.h>

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

            RasterPrint & rescale(const double & _pixel_size);

            RasterPrint & operator+=(const RasterPrint & other);
            RasterPrint & operator-=(const RasterPrint & other);

            GUInt16* to_gdal(double reference[6], size_t & height, size_t & width) const;
        private:
            std::string name;
            shadow::Point reference_point;
            size_t height;
            size_t width;
            double pixel_size;
            cv::Mat image_matrix;
            void gdal_reference_init(double reference[6]) const;
        };

        RasterPrint & operator+(const RasterPrint & lhs, const RasterPrint & rhs);
        RasterPrint & operator-(const RasterPrint & lhs, const RasterPrint & rhs);
    }
    void swap(projection::RasterPrint & lhs, projection::RasterPrint & rhs);
}
