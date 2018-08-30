/**
 *  @file gstinspectors.h
 *  
 *  @brief Header for all the stock inspectors
 */

#include "gstinspector.h"

#ifndef GST_INSPECTORS_H
#define GST_INSPECTORS_H

G_BEGIN_DECLS

GstStructure *param_inspector(GstElement *element);

G_END_DECLS

#endif // GST_INSPECTORS_H
