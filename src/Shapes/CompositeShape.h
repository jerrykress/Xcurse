#pragma once

#include "SingletonShape.h"

namespace Xcurse
{
    class CompositeShape : public GenericShapeObject
    {
    public:
        std::vector<SingletonShape> &get_member_shapes() const;

    private:
        std::vector<SingletonShape> m_member_shapes;
    };
}