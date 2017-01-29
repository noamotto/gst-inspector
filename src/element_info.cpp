#include "include/element_info.h"

ElementInfo::ElementInfo(const char *element_name, bool *succeeded)
{
    m_element_factory = gst_element_factory_find(element_name);
    if (NULL == m_element_factory)
    {
        *succeeded = false;
        return;
    }

    m_element = gst_element_factory_create(m_element_factory, element_name);
    if (NULL == m_element)
    {
        *succeeded = false;
        return;
    }

    *succeeded = true;
}

const char *
ElementInfo::get_element_name() const
{
    return NULL;
}

const char *
ElementInfo::get_element_interfaces() const
{
    return NULL;
}

const char *
ElementInfo::get_element_caps() const
{
    return NULL;
}

const ParamInfo *
ElementInfo::get_element_params() const
{
    return NULL;
}

ElementInfo::~ElementInfo()
{
    gst_object_unref(m_element);
    gst_object_unref(m_element_factory);
}