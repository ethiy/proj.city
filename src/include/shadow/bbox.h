#pragma once

#include <CGAL/Bbox_3.h>

#include <array>
#include <valarray>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Point;

        /** 
         * @ingroup shadow_group
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
             * @see Bbox(Bbox const& other);
             * @see Bbox(Bbox && other);
             * @see Bbox(std::array<double, 3> const& coordinates);
             * @see Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
             * @see ~Bbox(void)
             */
            Bbox(void);
            /**
             * Classic constructor.
             * @see Bbox(void);
             * @see Bbox(Bbox const& other);
             * @see Bbox(Bbox && other);
             * @see Bbox(std::array<double, 3> const& coordinates);
             * @see ~Bbox(void)
             */
            Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
            /**
             * Bbox constructor from point coordinates.
             * @see Bbox(void);
             * @see Bbox(Bbox const& other);
             * @see Bbox(Bbox && other);
             * @see Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
             * @see ~Bbox(void)
             */
            Bbox(std::valarray<double> const& coordinates);
            /**
             * Bbox constructor from Point.
             * @see Bbox(void);
             * @see Bbox(Bbox const& other);
             * @see Bbox(Bbox && other);
             * @see Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
             * @see ~Bbox(void)
             */
            Bbox(Point const& point);
            /**
             * Bbox copy constructor.
             * @see Bbox(void);
             * @see Bbox(Bbox && other);
             * @see Bbox(std::array<double, 3> const& coordinates);
             * @see Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
             * @see ~Bbox(void)
             */
            Bbox(Bbox const& other);
            /**
             * Bbox move constructor.
             * @see Bbox(void);
             * @see Bbox(Bbox const& other);
             * @see Bbox(std::array<double, 3> const& coordinates);
             * @see Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
             * @see ~Bbox(void)
             */
            Bbox(Bbox && other);
            /**
             * Bbox destructor.
             * @see Bbox(void);
             * @see Bbox(Bbox const& other);
             * @see Bbox(Bbox && other);
             * @see Bbox(std::array<double, 3> const& coordinates);
             * @see Bbox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
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
            double const& xmin(void) const noexcept;
            /**
             * Access xmax.
             * @return xmax
             * @see double xmin(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double const& xmax(void) const noexcept;
            /**
             * Access ymin.
             * @return ymin
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double const& ymin(void) const noexcept;
            /**
             * Access ymax.
             * @return ymax
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double const& ymax(void) const noexcept;
            /**
             * Access zmin.
             * @return zmin
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double const& zmin(void) const noexcept;
            /**
             * Access zmax.
             * @return zmax
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             */
            double const& zmax(void) const noexcept;
            /**
             * Access xmin.
             * @return xmin
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double & xmin(void) noexcept;
            /**
             * Access xmax.
             * @return xmax
             * @see double xmin(void) noexcept;
             * @see double ymin(void) noexcept;
             * @see double ymax(void) noexcept;
             * @see double zmin(void) noexcept;
             * @see double zmax(void) noexcept;
             */
            double & xmax(void) noexcept;
            /**
             * Access ymin.
             * @return ymin
             * @see double xmin(void) noexcept;
             * @see double xmax(void) noexcept;
             * @see double ymax(void) noexcept;
             * @see double zmin(void) noexcept;
             * @see double zmax(void) noexcept;
             */
            double & ymin(void) noexcept;
            /**
             * Access ymax.
             * @return ymax
             * @see double xmin(void) noexcept;
             * @see double xmax(void) noexcept;
             * @see double ymin(void) noexcept;
             * @see double zmin(void) noexcept;
             * @see double zmax(void) noexcept;
             */
            double & ymax(void) noexcept;
            /**
             * Access zmin.
             * @return zmin
             * @see double xmin(void) noexcept;
             * @see double xmax(void) noexcept;
             * @see double ymin(void) noexcept;
             * @see double ymax(void) noexcept;
             * @see double zmax(void) noexcept;
             */
            double & zmin(void) noexcept;
            /**
             * Access zmax.
             * @return zmax
             * @see double xmin(void) noexcept;
             * @see double xmax(void) noexcept;
             * @see double ymin(void) noexcept;
             * @see double ymax(void) noexcept;
             * @see double zmin(void) noexcept;
             */
            double & zmax(void) noexcept;

            /**
             * Swap `this` with `other`.
             * @param other an other Bbox to swap with
             * @see swap(shadow::Bbox &, shadow::Bbox &)
             */
            void swap(Bbox & other);

            /**
             * Copy assignment operator.
             * @param other an other Bbox
             * @return this copy of other
             * @see operator=(Bbox && other) noexcept;
             */
            Bbox & operator =(Bbox const& other) noexcept;
            /**
             * Move assignment operator.
             * @param other an other Bbox
             * @return this
             * @see operator=(Bbox const& other) noexcept;
             */
            Bbox & operator =(Bbox && other) noexcept;

            /**
             * plus operator.
             * @param other an other Bbox
             * @return bounding box union
             * @see operator+(Bbox const& lhs, Bbox const& rhs);
             */
            Bbox & operator +=(Bbox const& other);

            /**
             * Convert shadow Bbox to CGAL::Bbox_3.
             * @return CGAL Bbox_3
             */
            CGAL::Bbox_3 to_cgal(void) const noexcept;

        private:
            std::array<double, 6> extremes;
            /**
            * Outstreaming Bbox
            * @param os the output stream
            * @param bbox the bbox to stream
            * @return the output stream
            */
            friend std::ostream & operator <<(std::ostream & os, Bbox const& bbox);
        };
        
        /**
         * plus operator.
         * @param lhs a Bbox
         * @param rhs a Bbox
         * @return bounding box union
         * @see operator+(Bbox const& lhs, Bbox const& rhs);
         */
        Bbox operator +(Bbox const& lhs, Bbox const& rhs);

        /**
        * Swap `lhs` with `rhs`.
        * @param lhs a Bbox
        * @param rhs a Bbox
        * @see swap(Bbox & other)
        */
        void swap(Bbox & lhs, Bbox & rhs);
    }
}
