#pragma once

#include <array>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        /** 
         * @ingroup shadow
         * @brief Bbox class representing a 3D facet.
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
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double xmin(void) const noexcept;

            /**
             * Access xmax.
             * @see double xmin(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double xmax(void) const noexcept;

            /**
             * Access ymin.
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double ymin(void) const noexcept;

            /**
             * Access ymax.
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double zmin(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double ymax(void) const noexcept;
            
            /**
             * Access zmin.
             * @see double xmin(void) const noexcept;
             * @see double xmax(void) const noexcept;
             * @see double ymin(void) const noexcept;
             * @see double ymax(void) const noexcept;
             * @see double zmax(void) const noexcept;
             */
            double zmin(void) const noexcept;

            /**
             * Access zmax.
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

            Bbox & operator=(const Bbox & other) noexcept;
            Bbox & operator=(Bbox && other) noexcept;

            Bbox & operator+=(const Bbox & other);

        private:
            std::array<double, 6> extreemes;
            friend std::ostream & operator<<(std::ostream & os, const Bbox & bbox);
        };
        
        void swap(Bbox & lhs, Bbox & rhs);
        Bbox & operator+(Bbox & lhs, const Bbox & rhs);
    }
}
