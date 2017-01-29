#pragma once

#include "include/type_reader/type_reader.h"

class EnumTypeReader : public ITypeReader
{
  public:
    void
    fill_type(
        GParamSpec *const pspec,
        GValue *const value,
        param_keys &keys);

  private:
    void find_default(
        const GEnumValue *values,
        GValue *const value,
        param_keys &keys);

    void populate_options(
        const GEnumValue *values,
        param_keys &keys);
};