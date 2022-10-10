#pragma once

#include "GenericShapeObject.h"

namespace Xcurse
{
    class CompositeShape : public GenericShapeObject
    {
    public:
        CompositeShape();

        void add_shape(std::string name, GenericShapeObject o);
        void rm_shape(std::string name);
        GenericShapeObject get_shape(std::string name);

        std::vector<Position> &rasterise() override;

        GenericShapeObject &operator[](std::string name);

    protected:
        std::unordered_map<std::string, GenericShapeObject> m_shapes;
    };
}