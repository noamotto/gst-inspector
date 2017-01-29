#pragma once

#include "include/type_reader/type_reader.h"

class TypeResolver
{
  public:
    TypeResolver();

    ITypeReader *
    resolve_type(
        const GParamSpec *pspec);

    ~TypeResolver();

  private:
    std::map<GType, ITypeReader *> m_reader_map;
};