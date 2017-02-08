#pragma once

#include "Face/face.h"

#include "../geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <map>
#include <string>
#include <ostream>

namespace urban
{
    namespace shadow
    {
        class Mesh
        {
        public:
            /*! Empty constructor*/
            Mesh(void);
            /*! Copy constructor*/
            Mesh(const Mesh &);
            /*! Move constructor*/
            Mesh(Mesh &&);
            Mesh(Lib3dsMesh*);
            Mesh(const Polyhedron &);
            Mesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>);
            ~Mesh(void);

            void swap(Mesh &);

            Mesh & operator=(const Mesh &) noexcept;
            Mesh & operator=(Mesh &&) noexcept;

            std::string get_name(void) const noexcept;
            size_t get_number_points(void) const noexcept;
            std::map<size_t, Point> get_points(void) const noexcept;
            size_t get_number_faces(void) const noexcept;
            std::map<size_t, Face> get_faces(void) const noexcept;
            Bbox bbox(void) const noexcept;

            size_t get_index(const Polyhedron::Halfedge & h);

            Lib3dsMesh* to_3ds(void);
        private:
            std::string name;
            std::map<size_t, Point> points;
            std::map<size_t, Face> faces;
            Bbox bounding_box;
            void compute_box(void);
            
            friend std::ostream& operator<<(std::ostream &, const Mesh &);
        };
    }
    void swap(shadow::Mesh &, shadow::Mesh &);
}
