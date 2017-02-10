#pragma once

#include "common.h"

class ITypeReader
{
    public:

    virtual void 
    fill_type(
        GParamSpec * const pspec,
        GValue * const value,
        param_keys& keys) = 0;
};