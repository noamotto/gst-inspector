#pragma once

#include "type_reader/type_reader.h"

class FlagsTypeReader : public ITypeReader
{
  public:
    void
    fill_type(
        GParamSpec *const pspec,
        GValue *const value,
        param_keys &keys) const;

  private:
    gchar *find_default(
        const GFlagsClass *flags_class,
        GValue *const value) const;

    void populate_options(
        const GFlagsClass *flags_class,
        param_keys &keys) const;
};