#pragma once

#include "common.h"

class ParamInfo
{
    public:
    ParamInfo(
        GParamSpec * const pspec,
        GstElement *element);

    void get_param_keys(param_keys& keys) const;

    ~ParamInfo();

    private:
    GParamSpec * const m_pspec;
    param_keys m_param_keys;

    void ParamInfo::read_param_flags();
};