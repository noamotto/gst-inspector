#pragma once

#include "include/param_info.h"

class ElementInfo
{
    public:
    ElementInfo(const char * element_name, bool *succeeded);

    const char * get_element_name() const;
    const char * get_element_interfaces() const;
    const char * get_element_caps() const;
    const ParamInfo * get_element_params() const;

    ~ElementInfo();

    private:
    GstElementFactory *m_element_factory;
    GstElement *m_element;
};