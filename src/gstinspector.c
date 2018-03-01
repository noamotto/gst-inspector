#include "gstinspector.h"

typedef struct _ElementInspectorField
{
    const gchar *name;
    GstElementInspectFunc inspector;
} ElementInspectorField;

typedef struct _PluginInspectorField
{
    const gchar *name;
    GstPluginInspectFunc inspector;
} PluginInspectorField;

static GList *element_inspectors = NULL;
static GList *plugin_inspectors = NULL;

static gint element_inspector_lookup_func(const ElementInspectorField *element,
                                          const gchar *lookup_name)
{
    return g_strcmp0(element->name, lookup_name);
}

static gint plugin_inspector_lookup_func(const PluginInspectorField *element,
                                         const gchar *lookup_name)
{
    return g_strcmp0(element->name, lookup_name);
}

/**
 *  @brief Registers a new element inspector to the library
 * 
 *  @param inspector The inspector function to install
 *  @param name Name of the inspector. Used as ID and should be unique for 
 *  the inspector. The name is expected to be static, and is expected to stay
 *  valid until the end of the program's lifetime.
 *  @param position Position to insert the inspector to in the list.
 *  Negative numbers or numbers bigger than the list's length append the 
 *  inspector to the end of the list.
 * 
 *  @returns TRUE if the operation succeeded. Otherwise FALSE is returned and 
 *  an error message is issued.
 */
gboolean gst_inspector_register_element_inspector(GstElementInspectFunc inspector,
                                                  const gchar *name,
                                                  gint position)
{
    if (g_list_find_custom(element_inspectors, name,
                           (GCompareFunc)&element_inspector_lookup_func) != NULL)
    {
        g_warning("Trying to register inspector with existant name %s. Not registering",
                  name);
        return FALSE;
    }

    //Insert the inspector at given place
    ElementInspectorField *new_inspector = g_slice_new0(ElementInspectorField);
    new_inspector->name = name;
    new_inspector->inspector = inspector;
    element_inspectors = g_list_insert(element_inspectors, new_inspector, position);

    return TRUE;
}

gboolean gst_inspector_remove_element_inspector(const gchar *name)
{
    GList *inspector_node = g_list_find_custom(element_inspectors, name,
                                               (GCompareFunc)&element_inspector_lookup_func);

    if (!inspector_node)
    {
        g_warning("Trying to remove nonexist inspector %s. Doing nothing", name);
        return FALSE;
    }

    //Remove link from list
    element_inspectors = g_list_remove_link(element_inspectors, inspector_node);
    g_slice_free(ElementInspectorField, inspector_node->data);
    g_list_free(inspector_node);
    return TRUE;
}

GList *gst_inspector_get_installed_element_inspectors()
{
    return NULL;
}

/**
 *  @brief Registers a new plugin inspector to the library
 * 
 *  @param inspector The inspector function to install
 *  @param name Name of the inspector. Used as ID and should be unique for 
 *  the inspector
 *  @param position Position to insert the inspector to in the list.
 *  Negative numbers or numbers bigger than the list's length append the 
 *  inspector to the end of the list.
 * 
 *  @returns TRUE if the operation succeeded. Otherwise FALSE is returned and 
 *  an error message is issued.
 */
gboolean gst_inspector_register_plugin_inspector(GstPluginInspectFunc inspector,
                                                 const gchar *name,
                                                 gint position)
{
    return TRUE;
}

gboolean gst_inspector_remove_plugin_inspector(const gchar *name)
{
    return TRUE;
}

GList *gst_inspector_get_installed_plugin_inspectors()
{
    return NULL;
}

GstStructure *gst_inspector_inspect_element(GstElementFactory *element)
{
    return NULL;
}

GstStructure *gst_inspector_inspect_plugin(GstPlugin *plugin)
{
    return NULL;
}

GstStructure *gst_inspector_inspect_by_name(const gchar *object_name)
{
    return NULL;
}
