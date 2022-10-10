#pragma once

#include "GenericShapeObject.h"

namespace Xcurse
{
    class CompositeShape : public GenericShapeObject
    {
    public:
        CompositeShape();

        std::vector<GenericShapeObject> &get_member_shapes() const;

    protected:
        std::vector<GenericShapeObject> m_member_shapes;
    };
}