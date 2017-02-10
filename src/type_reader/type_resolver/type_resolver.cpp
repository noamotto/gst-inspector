#include "type_reader/type_resolver/type_resolver.h"

#include "type_reader/bool_reader.h"
#include "type_reader/string_reader.h"
#include "type_reader/long_reader.h"
#include "type_reader/ulong_reader.h"
#include "type_reader/int_reader.h"
#include "type_reader/int64_reader.h"
#include "type_reader/uint_reader.h"
#include "type_reader/uint64_reader.h"
#include "type_reader/float_reader.h"
#include "type_reader/double_reader.h"

TypeResolver::TypeResolver()
{
    m_reader_map[G_TYPE_PARAM_BOOLEAN] = new BoolTypeReader;
    m_reader_map[G_TYPE_PARAM_STRING] = new StringTypeReader;
    m_reader_map[G_TYPE_PARAM_LONG] = new LongTypeReader;
    m_reader_map[G_TYPE_PARAM_ULONG] = new ULongTypeReader;
    m_reader_map[G_TYPE_PARAM_INT] = new IntTypeReader;
    m_reader_map[G_TYPE_PARAM_INT64] = new Int64TypeReader;
    m_reader_map[G_TYPE_PARAM_UINT] = new UIntTypeReader;
    m_reader_map[G_TYPE_PARAM_UINT64] = new UInt64TypeReader;
    m_reader_map[G_TYPE_PARAM_FLOAT] = new FloatTypeReader;
    m_reader_map[G_TYPE_PARAM_DOUBLE] = new DoubleTypeReader;
}

ITypeReader *
TypeResolver::resolve_type(
    const GParamSpec *pspec)
{
    GType param_type = G_PARAM_SPEC_TYPE(pspec);
    std::map<GType, ITypeReader *>::iterator reader = m_reader_map.find(param_type);

    return (reader != m_reader_map.end()) ? reader->second : NULL;
}

TypeResolver::~TypeResolver()
{
    std::map<GType, ITypeReader *>::iterator reader;
    for(reader = m_reader_map.begin(); reader != m_reader_map.end(); reader++)
    {
        delete reader->second;
    }
}