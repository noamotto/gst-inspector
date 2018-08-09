#include "gstinspector_priv.h"

typedef struct _InspectorNode
{
    gchar *name;
} InspectorNode;

typedef struct _ElementInspectorNode
{
    InspectorNode node;

    GstElementInspectFunc inspector;
} ElementInspectorNode;

typedef struct _PluginInspectorNode
{
    InspectorNode node;

    GstPluginInspectFunc inspector;
} PluginInspectorNode;

typedef struct _InspectorList
{
    GList *list;
    gsize length;
} InspectorList;

typedef struct _InspectorData
{
    GstStructure *inspect_data;
    GstObject *inspect_object;
} InspectorData;

static InspectorList *element_inspectors = NULL;
static InspectorList *plugin_inspectors = NULL;
static gboolean is_inited = FALSE;
static gboolean testing_mode = FALSE;

/**
 *  @brief Internal library initializer. Initializes and populates inspector lists
 *  with stock inspectors.
 */
static void _gst_inspector_init()
{
    if (!element_inspectors)
    {
        element_inspectors = g_slice_new0(InspectorList);
        if (!testing_mode)
        {
            //Populate stock inspectors
        }
    }

    if (!plugin_inspectors)
    {
        plugin_inspectors = g_slice_new0(InspectorList);
        if (!testing_mode)
        {
            //Populate stock inspectors
        }
    }
}

/**
 *  @brief Sets the system to testing mode.
 * 
 *  This function is hidden from normal users, and serves as an internal function
 *  for testing purposes. The function does the following things:
 *  1. Initializes the system without stock inspectors
 */
void gst_inspector_set_testing_mode()
{
    testing_mode = TRUE;
}

#define CHECK_INIT              \
    if (G_UNLIKELY(!is_inited)) \
    {                           \
        _gst_inspector_init();  \
    }

/**
 *  @brief List lookup function for inspector nodes. Compares by name. The
 *  function uses strcmp, but it isn't used for sorting.
 *
 *  @param node Node to compare name with
 *  @param lookup_name Name to compare with
 *
 *  @return strcmp result between the node's name and the lookup name
 */
static gint inspector_lookup_func(const InspectorNode *node,
                                  const gchar *lookup_name)
{
    return g_strcmp0(node->name, lookup_name);
}

/**
 *  @brief Adds an InspectorNode to a given inspector's list
 *
 *  Note that this function does not check the input parameters.
 *
 *  @param list List to add the inspector to
 *  @param new_node The new InspectorNode to add
 *  @param position the position to add the inspector in the list,
 *  as passed to g_list_insert().
 */
static void inspector_list_add(InspectorList *list, InspectorNode *new_node, gint position)
{
    list->list = g_list_insert(list->list, new_node, position);
    ++list->length;
}

/**
 *  @brief Removes an inspector from a given list.
 *
 *  The function accepts a GList node as input, for effeciency, as most of the times
 *  the nedded inspector is looked up beforehand.
 *
 *  @param list List to remove the inspector from
 *  @param inspector_node The GList containing the inspector to remove
 */
static void inspector_list_remove(InspectorList *list, GList *inspector_node)
{
    list->list = g_list_remove_link(list->list, inspector_node);
    g_slice_free(ElementInspectorNode, inspector_node->data);
    g_list_free(inspector_node);
    --list->length;
}

/**
 *  @brief Lookups an inspector from a given list.
 *
 *  The function returns a GList node as output, for effeciency, as most of the times
 *  the GList node is needed as well.
 *
 *  @param list List to look for the inspector in
 *  @param name The name of the inspector to look for
 *
 *  @return The GList node associated with the found inspector, or NULL if an
 *  inspector wasn't found.
 */
static GList *inspector_list_lookup(InspectorList *list, const gchar *name)
{
    return g_list_find_custom(list->list, name, (GCompareFunc)&inspector_lookup_func);
}

static void get_node_names(InspectorNode *node, GPtrArray *names_list)
{
    g_ptr_array_add(names_list, node->name);
}

static gchar **inspector_list_get_names(InspectorList *list)
{
    GPtrArray *name_list = g_ptr_array_sized_new((guint)list->length);

    g_list_foreach(list->list, (GFunc)&get_node_names, name_list);
    return (gchar **)g_ptr_array_free(name_list, FALSE);
}

static GstStructure *create_error_dict(const gchar *error_string)
{
    GstStructure *err = gst_structure_new_empty("Error");
    gst_dictionary_set_static_string(err, "Description", error_string);
    return err;
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
                                                  gchar *name,
                                                  gint position)
{
    CHECK_INIT;

    if (inspector_list_lookup(element_inspectors, name) != NULL)
    {
        g_warning("Trying to register element inspector with already exists name %s. Not registering",
                  name);
        return FALSE;
    }

    //Insert the inspector at given position
    ElementInspectorNode *new_inspector = g_slice_new0(ElementInspectorNode);
    new_inspector->node.name = name;
    new_inspector->inspector = inspector;
    inspector_list_add(element_inspectors, (InspectorNode *)new_inspector, position);

    return TRUE;
}

/**
 *  @brief Removes an element inspector from the list of element inspectors
 *
 *  @param name The name of the inspector to remove.
 *
 *  @returns TRUE if the inspector was removed successfully. Otherwise FALSE.
 */
gboolean gst_inspector_remove_element_inspector(const gchar *name)
{
    GList *inspector_node = NULL;

    CHECK_INIT;

    inspector_node = inspector_list_lookup(element_inspectors, name);

    if (!inspector_node)
    {
        g_warning("Trying to remove nonexist element inspector %s. Doing nothing", name);
        return FALSE;
    }

    //Remove link from list
    inspector_list_remove(element_inspectors, inspector_node);
    return TRUE;
}

/**
 *  @brief Retrieves a list of registered element inspectors' names, ordered by
 *  registration order.
 *
 *  Note that the list is generated every time the function is called, hence it is
 *  not so effecient.
 *
 *  @returns An array of the registered element inspectors' names. The array
 *  should be freed after use, with g_free, but the names should not. Note that if
 *  the list is empty, NULL is returned.
 */
gchar **gst_inspector_get_installed_element_inspectors()
{
    gchar **names_array = NULL;

    CHECK_INIT;

    names_array = inspector_list_get_names(element_inspectors);

    return names_array;
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
                                                 gchar *name,
                                                 gint position)
{
    CHECK_INIT;

    if (inspector_list_lookup(plugin_inspectors, name) != NULL)
    {
        g_warning("Trying to register plugin inspector with already exists name %s. Not registering",
                  name);
        return FALSE;
    }

    //Insert the inspector at given place
    PluginInspectorNode *new_inspector = g_slice_new0(PluginInspectorNode);
    new_inspector->node.name = name;
    new_inspector->inspector = inspector;
    inspector_list_add(plugin_inspectors, (InspectorNode *)new_inspector, position);

    return TRUE;
}

/**
 *  @brief Removes a plugin inspector from the list of plugin inspectors
 *
 *  @param name The name of the inspector to remove.
 *
 *  @returns TRUE if the inspector was removed successfully. Otherwise FALSE.
 */
gboolean gst_inspector_remove_plugin_inspector(const gchar *name)
{
    GList *inspector_node = NULL;

    CHECK_INIT;

    inspector_node = inspector_list_lookup(plugin_inspectors, name);

    if (!inspector_node)
    {
        g_warning("Trying to remove nonexist plugin inspector %s. Doing nothing", name);
        return FALSE;
    }

    inspector_list_remove(plugin_inspectors, inspector_node);
    return TRUE;
}

/**
 *  @brief Retrieves a list of registered plugin inspectors' names, ordered by
 *  registration order.
 *
 *  Note that the list is generated every time the function is called, hence it is
 *  not so effecient.
 *
 *  @returns An array of the registered plugin inspectors' names. The array
 *  should be freed after use, with g_free, but the names should not. Note that if
 *  the list is empty, NULL is returned.
 */
gchar **gst_inspector_get_installed_plugin_inspectors()
{
    gchar **names_array = NULL;

    CHECK_INIT;

    names_array = inspector_list_get_names(plugin_inspectors);

    return names_array;
}

/**
 *  @brief Runs all installed element inspectors on given element. Used as 
 *  GList foreach function.
 * 
 *  @param node Current element inspector to run
 *  @param data Data object that holds the element and accumulated data
 */
static void run_element_inspectors(ElementInspectorNode *node, InspectorData *data)
{
    GstStructure *result = node->inspector(GST_ELEMENT(data->inspect_object));
    gst_dictionary_set_sub_dictionary(data->inspect_data, gst_structure_get_name(result),
                                      result);
}

/**
 *  @brief Inspects an element using the registered element inpectors
 * 
 *  @param factory GstElementFactory the represents the element to inspect
 *
 *  @returns GstStructure with the inspection results
 */
GstStructure *gst_inspector_inspect_element(GstElementFactory *factory)
{
    GstElement *element;
    InspectorData *data;
    GstStructure *result;

    CHECK_INIT;

    factory =
        GST_ELEMENT_FACTORY(gst_plugin_feature_load(GST_PLUGIN_FEATURE(factory)));

    if (!factory)
    {
        return create_error_dict("element plugin couldn't be loaded");
    }

    element = gst_element_factory_create(factory, NULL);
    if (!element)
    {
        return create_error_dict("couldn't construct element for some reason");
    }

    data = g_new(InspectorData, 1);
    result = gst_structure_new_empty(GST_OBJECT_NAME(factory));
    data->inspect_data = result;
    data->inspect_object = GST_OBJECT_CAST(element);

    g_list_foreach(element_inspectors->list, (GFunc)run_element_inspectors, data);
    g_free(data);
    return result;
}

/**
 *  @brief Inspects a plugin using the registered plugin inpectors
 * 
 *  @param plugin GstPlugin that represents the plugin to inspect
 *
 *  @returns GstStructure with the inspection results
 */
GstStructure *gst_inspector_inspect_plugin(GstPlugin *plugin)
{
    CHECK_INIT;

    return NULL;
}

/**
 *  Inspects an object using a given name. Note that if a plugin and an 
 *  element shares the same name, the element takes precedence.
 * 
 *  @param object_name Name of object to inpect
 * 
 *  @returns GstStructure with the inspection results
 */
GstStructure *gst_inspector_inspect_by_name(const gchar *object_name)
{
    CHECK_INIT;

    return NULL;
}
