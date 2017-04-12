#pragma once

#include <CGAL/Bbox_3.h>

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        /** 
         * @ingroup shadow
         * @brief Bbox class representing a 3D Bounding box.
         * 
         * Shadow Bbox is member class of Shadow Mesh:
         *  - It stores the bounding box of the 3d Mesh
         */
        class Bbox
        {
        public:
            /**
             * Empty Bbox constructor.
             * The empty bounding box is definned with a lower left corner point at (∞,∞,∞) and with upper right corner point at (−∞,−∞,−∞)
             * @see Bbox(const Bbox &)
             * @see Bbox(Bbox &&)
             * @see Bbox(const std::array<double, 3> &);
             * @see Bbox(double , double , double , double , double , double );
             * @see ~Bbox(void)
             */
            Bbox(void);
            /**
             * Classic constructor.
             * @see Bbox(void);
             * @see Bbox(const Bbox &)
             * @see Bbox(Bbox &&)
             * @see Bbox(const std::array<double, 3> &);
             * @see ~Bbox(void)
             */
            Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
            /**
             * Bbox constructor from point coordinates.
             * @see Bbox(void);
             * @see Bbox(const Bbox &)
             * @see Bbox(Bbox &&)
             * @see Bbox(double , double , double , double , double , double );
             * @see ~Bbox(void)
             */
            Bbox(const std::array<double, 3> & coordinates);
            /**
             * Bbox copy constructor.
             * @see Bbox(void);
             * @see Bbox(Bbox &&)
             * @see Bbox(const std::array<double, 3> &);
             * @see Bbox(double , double , double , double , double , double );
             * @see ~Bbox(void)
             */
            Bbox(const Bbox & other);
            /**
             * Bbox move constructor.
             * @see Bbox(void);
             * @see Bbox(const Bbox &)
             * @see Bbox(const std::array<double, 3> &);
             * @see Bbox(double , double , double , double , double , double );
             * @see ~Bbox(void)
             */
            Bbox(Bbox && other);
            /**
             * Bbox destructor.
             * @see Bbox(void);
             * @see Bbox(const Bbox &)
             * @see Bbox(Bbox &&)
             * @see Bbox(const std::array<double, 3> &);
             * @see Bbox(double , double , double , double , double , double );
             */
            ~Bbox(void);

            /**
             * Access xmin.
             * @return xmin
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double xmin(void) const noexcept;
            /**
             * Access xmax.
             * @return xmax
             * @see double xmin(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double xmax(void) const noexcept;
            /**
             * Access ymin.
             * @return ymin
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double ymin(void) const noexcept;
            /**
             * Access ymax.
             * @return ymax
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double ymax(void) const noexcept;
            /**
             * Access zmin.
             * @return zmin
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double zmin(void) const noexcept;
            /**
             * Access zmax.
             * @return zmax
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             */
            double zmax(void) const noexcept;

            /**
             * Swap `this` with `other`.
             * @param other an other Bbox to swap with
             * @see swap(shadow::Bbox &, shadow::Bbox &)
             */
            void swap(Bbox & other);

            /**
             * Copy assignement operator.
             * @param other an other Bbox
             * @return this copy of other
             * @see operator=(Bbox && other) noexcept;
             */
            Bbox & operator=(const Bbox & other) noexcept;
            /**
             * Move assignement operator.
             * @param other an other Bbox
             * @return this
             * @see operator=(const Bbox & other) noexcept;
             */
            Bbox & operator=(Bbox && other) noexcept;

            /**
             * plus operator.
             * @param other an other Bbox
             * @return bounding box union
             * @see operator+(const Bbox & lhs, const Bbox & rhs);
             */
            Bbox & operator+=(const Bbox & other);

            /**
             * Convert shadow Bbox to CGAL::Bbox_3.
             * @return CGAL Bbox_3
             */
            CGAL::Bbox_3 to_cgal(void) const noexcept;

        private:
            std::array<double, 6> extreemes;
            /**
            * Outstreaming Bbox
            * @param os the output stream
            * @param bbox the bbox to stream
            * @return the output stream
            */
            friend std::ostream & operator<<(std::ostream & os, const Bbox & bbox);
        };
        
        /**
         * plus operator.
         * @param lhs a Bbox
         * @param rhs a Bbox
         * @return bounding box union
         * @see operator+(const Bbox & lhs, const Bbox & rhs);
         */
        Bbox operator+(const Bbox & lhs, const Bbox & rhs);
    }
    /**
     * Swap `lhs` with `rhs`.
     * @param lhs a Bbox
     * @param rhs a Bbox
     * @see swap(shadow::Bbox & other)
     */
    void swap(shadow::Bbox & lhs, shadow::Bbox & rhs);
}
