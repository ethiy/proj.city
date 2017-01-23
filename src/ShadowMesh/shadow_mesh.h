#pragma once

#include "Face/face.h"

#include "../UrbanObject/geometry_definitions.h"

#include <lib3ds/types.h>
#include <lib3ds/mesh.h>

#include <map>
#include <string>
#include <ostream>

namespace urban
{
    class ShadowMesh
    {
    public:
        ShadowMesh(void);
        ShadowMesh(const ShadowMesh &);
        ShadowMesh(Lib3dsMesh*);
        ShadowMesh(Polyhedron);
        ShadowMesh(std::string, std::map<size_t, Point>, std::map<size_t, Face>);
        ~ShadowMesh(void);

        void swap(ShadowMesh &);

        ShadowMesh & operator=(ShadowMesh);

        std::string get_name(void) const noexcept;
        size_t get_number_points(void) const noexcept;
        std::map<size_t, Point> get_points(void) const noexcept;
        size_t get_number_faces(void) const noexcept;
        std::map<size_t, Face> get_faces(void) const noexcept;
        Bbox bbox(void) const noexcept;

        Lib3dsMesh* to_3ds(void);
    private:
        std::string name;
        std::map<size_t, Point> points;
        std::map<size_t, Face> faces;
        Bbox bounding_box;
        void compute_box(void);
        
        friend std::ostream& operator<<(std::ostream &, const ShadowMesh &);
    };

    void swap(ShadowMesh &, ShadowMesh &);
}
