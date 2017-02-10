#pragma once

#include "type_reader/type_reader.h"

class FlagsTypeReader : public ITypeReader
{
  public:
    void
    fill_type(
        GParamSpec *const pspec,
        GValue *const value,
        param_keys &keys);

  private:
    gchar *find_default(
        const GFlagsClass *flags_class,
        GValue *const value);

    void populate_options(
        const GFlagsClass *flags_class,
        param_keys &keys);
};