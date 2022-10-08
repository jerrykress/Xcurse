#pragma once

#include "GenericShapeObject.h"

namespace Xcurse
{
    class SingletonShape : public GenericShapeObject, public Stylable
    {
    public:
        SingletonShape();
    };
}