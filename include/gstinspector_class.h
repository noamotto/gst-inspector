/**
 *  GstInspector - The core class of gst-inspector 
 */

#include "gstdictionary.h"

#ifndef GST_INSPECTOR_H
#define GST_INSPECTOR_H

G_BEGIN_DECLS

#define GST_TYPE_INSPECTOR (gst_inspector_get_type())
#define GST_INSPECTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_INSPECTOR, GstInspector))
#define GST_IS_INSPECTOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_INSPECTOR))
#define GST_INSPECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_INSPECTOR, GstInspectorClass))
#define GST_IS_INSPECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_INSPECTOR))
#define GST_INSPECTOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), GST_TYPE_INSPECTOR, GstInspectorClass))

/**
 *  @brief Inspection function. Called to inspect an object
 * 
 *  This is a protoype function for GstElementFactory and GstPlugin inspectors 
 *  (while leaving an opening for any other type). One should use the 
 *  subclasses' "inspect" function for development of one of those types 
 *  inspectors.
 * 
 *  @param object An object to inspect. The function should not take ownership
 *  on the received object and should not free it.
 * 
 *  @return A dictionary filled with the inspected data from the object, or 
 *  NULL if no data was found.
 */
typedef GstStructure *(*GstInspectFunc)(GstObject *object);

/**
 *  @brief An abstract inspector instance
 */
typedef struct _GstInspector
{
    GstObject parent;
} GstInspector;

/**
 *  @brief An abstract inspector class
 */
typedef struct _GstInspectorClass
{
    GstObjectClass klass;

    GstInspectFunc inspect;
} GstInspectorClass;

GType gst_inspector_get_type(void);

gboolean gst_inspector_register(GstInspector *inspector, const gchar *name);
GstStructure *gst_inspector_inspect(GstObject *object);
GstStructure *gst_inspector_inspect_custom(GstObject *object,
                                           const gchar *fist_instpector,
                                           ...);

G_END_DECLS

#endif //GST_INSPECTOR_H
