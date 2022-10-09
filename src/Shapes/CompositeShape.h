#pragma once

#include "SingletonShape.h"

namespace Xcurse
{
    class CompositeShape : public GenericShapeObject
    {
    public:
        CompositeShape();

        std::vector<SingletonShape> &get_member_shapes() const;

    protected:
        std::vector<SingletonShape> m_member_shapes;
    };
}