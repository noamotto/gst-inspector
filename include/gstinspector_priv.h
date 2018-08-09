/**
 *  Header for hidden API. Mainly used to allow testing
 */

#include "gstinspector.h"

#ifndef GST_INSPECTOR_PRIV_H
#define GST_INSPECTOR_PRIV_H

G_BEGIN_DECLS

#ifdef ENABLE_TESTING
#define TESTING_API GST_INSPECTOR_API
#else
#define TESTING_API
#endif

TESTING_API void gst_inspector_set_testing_mode(void);

G_END_DECLS

#endif //GST_INSPECTOR_PRIV_H
