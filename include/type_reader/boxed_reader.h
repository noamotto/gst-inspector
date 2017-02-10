#pragma once

#include "type_reader/type_reader.h"

class BoxedTypeReader : public ITypeReader
{
public:
  void
  fill_type(
      GParamSpec *const pspec,
      GValue *const value,
      param_keys &keys);

private:
  void print_caps(
      const GstCaps *caps,
      param_keys &keys);
};