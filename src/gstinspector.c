/**
 *  @defgroup inspectors Inspectors API
 *  The inspectors API is the core of the GstInspector library. It consists of 
 *  core API and implemented stock inspectors.
 * 
 *  @{
 * 
 *  @defgroup inspectors-core Core inspectors API
 *  The inspectors core API allows one to modify the list of registered inspectors
 *  for elements and plugins, and to inspect a plugin or a plugin feature, yielding 
 *  a structure with parsed data. The inspectors are stored in a list, and are 
 *  executed in the ordered they are sorted.
 * 
 *  Use gst_inspector_register_element_inspector() to register new element 
 *  inspectors, and gst_inspector_remove_element_inspector() to remove an 
 *  existing on from the list. To retrieve a list with the installed element inspector
 *  names, call gst_inspector_get_installed_element_inspectors().
 * 
 *  To inspect an element, one should use gst_inspector_inspect_element().
 * 
 *  Similar functions exist for plugin inspectors, substituting "element" with "plugin".
 * 
 *  More generic inspection methods are also implemented as part of this API. 
 *  To inspect a generic plugin feature one should call 
 *  gst_inspector_inspect_plugin_feature(). A more convenient wrapper to these 
 *  functions is gst_inspector_inspect_by_name(), which recieves a name and
 *  inspects the plugin or feature with that name.
 * 
 *  @defgroup element-inspectors Element inspectors
 *  This is a list of the stock element inspectors provided with the library. 
 *  They are registered by default.
 * 
 *  @defgroup plugin-inspectors Plugin inspectors
 *  This is a list of the stock plugin inspectors provided with the library. 
 *  They are registered by default.
 *  @}
 */

#include "gstinspectors.h"
#include "gstinspector_priv.h"

#include <stdio.h>

/**
 *  @addtogroup inspectors-core
 *  @{
 */

/**
 *  @brief Basic node struct for inspector nodes
 */
typedef struct _InspectorNode
{
    gchar *name;     /**< Node's name */
    gchar *longname; /**< Node's long (shown) name */
} InspectorNode;

/**
 *  @brief Element inspector node structure
 */
typedef struct _ElementInspectorNode
{
    InspectorNode node;              /**< Common node fields */

    GstElementInspectFunc inspector; /**< Inspector function */
} ElementInspectorNode;

/**
 *  @brief Plugin inspector node structure
 */
typedef struct _PluginInspectorNode
{
    InspectorNode node;              /**< Common node fields */

    GstPluginInspectFunc inspector; /**< Inspector function */
} PluginInspectorNode;

/**
 *  @brief Represents a list of inspectors
 */
typedef struct _InspectorList
{
    GList *list;  /**< List of inspectors */
    gsize length; /**< List's size */
} InspectorList;

/**
 *  @brief Data passed to inspector functions
 */
typedef struct _InspectorData
{
    GstStructure *inspect_data; /**< Dictionary to fill */
    GstObject *inspect_object;  /**< Object to inspect */
} InspectorData;

static InspectorList *element_inspectors = NULL;
static InspectorList *plugin_inspectors = NULL;
static gboolean is_inited = FALSE;

/**
 *  @brief Internal library initializer. Initializes and populates inspector lists
 *  with stock inspectors.
 */
static void _gst_inspector_init()
{
    if (!element_inspectors)
    {
        element_inspectors = g_slice_new0(InspectorList);
        gst_inspector_register_element_inspector(gst_inspector_inspect_factory_details,
                                                 GST_INSPECTOR_FACTORY_DETAILS_NAME,
                                                 GST_INSPECTOR_FACTORY_DETAILS_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_plugin,
                                                 GST_INSPECTOR_PLUGIN_NAME,
                                                 GST_INSPECTOR_PLUGIN_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_hierarchy,
                                                 GST_INSPECTOR_HIERARCHY_NAME,
                                                 GST_INSPECTOR_HIERARCHY_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_interfaces,
                                                 GST_INSPECTOR_INTERFACES_NAME,
                                                 GST_INSPECTOR_INTERFACES_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_pad_templates,
                                                 GST_INSPECTOR_PAD_TEMPLATES_NAME,
                                                 GST_INSPECTOR_PAD_TEMPLATES_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_clocking,
                                                 GST_INSPECTOR_CLOCKING_NAME,
                                                 GST_INSPECTOR_CLOCKING_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_uri_handler,
                                                 GST_INSPECTOR_URI_HANDLER_NAME,
                                                 GST_INSPECTOR_URI_HANDLER_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_pads,
                                                 GST_INSPECTOR_PADS_NAME,
                                                 GST_INSPECTOR_PADS_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_properties,
                                                 GST_INSPECTOR_PROPERTIES_NAME,
                                                 GST_INSPECTOR_PROPERTIES_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_signals,
                                                 GST_INSPECTOR_SIGNALS_NAME,
                                                 GST_INSPECTOR_SIGNALS_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_actions,
                                                 GST_INSPECTOR_ACTIONS_NAME,
                                                 GST_INSPECTOR_ACTIONS_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_children,
                                                 GST_INSPECTOR_CHILDREN_NAME,
                                                 GST_INSPECTOR_CHILDREN_LONGNAME, -1);

        gst_inspector_register_element_inspector(gst_inspector_inspect_element_presets,
                                                 GST_INSPECTOR_PRESETS_NAME,
                                                 GST_INSPECTOR_PRESETS_LONGNAME, -1);
    }

    if (!plugin_inspectors)
    {
        plugin_inspectors = g_slice_new0(InspectorList);
        gst_inspector_register_plugin_inspector(gst_inspector_inspect_plugin_details,
                                                GST_INSPECTOR_PLUGIN_DETAILS_NAME,
                                                GST_INSPECTOR_PLUGIN_DETAILS_LONGNAME, -1);
        gst_inspector_register_plugin_inspector(gst_inspector_inspect_plugin_features,
                                                GST_INSPECTOR_PLUGIN_FEATURES_NAME,
                                                GST_INSPECTOR_PLUGIN_FEATURES_LONGNAME, -1);
    }

    is_inited = TRUE;
}

/**
 *  @brief Checks if the library is initialied and initializes if not
 */
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
 *  @brief Clears an inspector list
 *
 *  The list is deleted by iterating on each node and deleting it
 *
 *  @param list List to clear
 */
static void inspector_list_clear(InspectorList *list)
{
    while (list->list)
    {
        inspector_list_remove(list, list->list);
    }
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

/**
 *  @brief Retrieves the name of given inspector node. Used to get the list 
 *  of inspector names 
 * 
 *  @param node Node to get name from
 *  @param names_list List to add the name to
 */
static void get_node_names(InspectorNode *node, GPtrArray *names_list)
{
    g_ptr_array_add(names_list, node->name);
}

/**
 *  @brief Retrieves a list of names of all inspector nodes in a given list
 *
 *  @param list An inspector list
 *  @returns An array of all inspector nodes in the list
 */
static gchar **inspector_list_get_names(InspectorList *list)
{
    GPtrArray *name_list = g_ptr_array_sized_new((guint)list->length);

    g_list_foreach(list->list, (GFunc)&get_node_names, name_list);
    return (gchar **)g_ptr_array_free(name_list, FALSE);
}

/**
 *  @brief Creates an error dictionary with given string, to return as error.
 * 
 *  @param error_string Error string to put in the dictionary
 *  @returns Newly created error dictionary
 */
static GstStructure *create_error_dict(gchar *error_string)
{
    GstStructure *err = gst_structure_new_empty("Error");
    gst_dictionary_set_string(err, "Error", error_string);
    return err;
}

/**
 *  @brief Creates an error dictionary with given static string, to return as error.
 * 
 *  @param error_string Error string to put in the dictionary
 *  @returns Newly created error dictionary
 */
static GstStructure *create_static_error_dict(const gchar *error_string)
{
    GstStructure *err = gst_structure_new_empty("Error");
    gst_dictionary_set_static_string(err, "Error", error_string);
    return err;
}

/**
 *  @brief Registers a new element inspector to the library
 *
 *  @param inspector The inspector function to install
 *  @param name Name of the inspector. Used as ID and should be unique for
 *  the inspector. Expected to be static
 *  @param longname Long name for the inspector. Used as the name of the dictionary
 *  holding the inspected results, as a short description of the inspection result
 *  the inspector. Expected to be static.
 *  @param position Position to insert the inspector to in the list.
 *  Negative numbers or numbers bigger than the list's length append the
 *  inspector to the end of the list.
 *
 *  @returns TRUE if the operation succeeded. Otherwise FALSE is returned and
 *  an error message is issued.
 */
gboolean gst_inspector_register_element_inspector(GstElementInspectFunc inspector,
                                                  gchar *name,
                                                  gchar *longname,
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
    new_inspector->node.longname = longname;
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
 *  @brief Clears the element inspector's list
 */
void gst_inspector_clear_element_inspectors()
{
    CHECK_INIT;

    inspector_list_clear(element_inspectors);
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
 *  the inspector. Expected to be static
 *  @param longname Long name for the inspector. Used as the name of the dictionary
 *  holding the inspected results, as a short description of the inspection result
 *  the inspector.
 *  @param position Position to insert the inspector to in the list.
 *  Negative numbers or numbers bigger than the list's length append the
 *  inspector to the end of the list.
 *
 *  @returns TRUE if the operation succeeded. Otherwise FALSE is returned and
 *  an error message is issued.
 */
gboolean gst_inspector_register_plugin_inspector(GstPluginInspectFunc inspector,
                                                 gchar *name,
                                                 gchar *longname,
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
    new_inspector->node.longname = longname;
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
 *  @brief Clears the plugin inspector's list
 */
void gst_inspector_clear_plugin_inspectors()
{
    CHECK_INIT;

    inspector_list_clear(plugin_inspectors);
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
 *  @brief Runs all installed element inspectors on given element
 * 
 *  @param node Current element inspector to run
 *  @param data Data object that holds the element and accumulated data
 */
static void run_element_inspectors(ElementInspectorNode *node, InspectorData *data)
{
    GValue result = G_VALUE_INIT;
    node->inspector(GST_ELEMENT(data->inspect_object), &result);

    if (G_IS_VALUE(&result))
    {
        gst_dictionary_set_value(data->inspect_data, node->node.longname, &result);
    }
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
    InspectorData data;
    GstStructure *result;

    CHECK_INIT;

    if (!GST_IS_ELEMENT_FACTORY(factory))
    {
        return create_error_dict(gst_info_strdup_printf(
            "%" GST_PTR_FORMAT " is not a valid GstElementFactory", factory));
    }

    factory =
        GST_ELEMENT_FACTORY(gst_plugin_feature_load(GST_PLUGIN_FEATURE(factory)));

    if (!factory)
    {
        return create_static_error_dict("element plugin couldn't be loaded");
    }

    element = gst_element_factory_create(factory, NULL);
    if (!element)
    {
        return create_static_error_dict("couldn't construct element for some reason");
    }

    result = gst_structure_new_empty(GST_OBJECT_NAME(factory));
    data.inspect_data = result;
    data.inspect_object = GST_OBJECT_CAST(element);

    g_list_foreach(element_inspectors->list, (GFunc)run_element_inspectors, &data);
    gst_object_unref(element);
    return result;
}

/**
 *  @brief Runs all installed plugin inspectors on given plugin
 * 
 *  @param node Current plugin inspector to run
 *  @param data Data object that holds the plugin and accumulated data
 */
static void run_plugin_inspectors(PluginInspectorNode *node, InspectorData *data)
{
    GValue result = G_VALUE_INIT;
    node->inspector(GST_PLUGIN(data->inspect_object), &result);

    if (G_IS_VALUE(&result))
    {
        gst_dictionary_set_value(data->inspect_data, node->node.longname, &result);
    }
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
    GstStructure *result;
    InspectorData data;

    CHECK_INIT;

    if (!GST_IS_PLUGIN(plugin))
    {
        return create_error_dict(gst_info_strdup_printf(
            "%" GST_PTR_FORMAT " is not a valid GstPlugin", plugin));
    }

    result = gst_structure_new_empty(gst_plugin_get_name(plugin));
    data.inspect_data = result;
    data.inspect_object = GST_OBJECT_CAST(plugin);

    g_list_foreach(plugin_inspectors->list, (GFunc)run_plugin_inspectors, &data);
    return result;
}

static GstStructure *inspect_typefind(GstPluginFeature *feature)
{
    GstStructure *results, *factory_dict;
    GstTypeFindFactory *factory;
    GstPlugin *plugin;
    GstCaps *caps;
    guint rank;
    const gchar *const *extensions;

    factory = GST_TYPE_FIND_FACTORY(gst_plugin_feature_load(feature));
    if (!factory)
    {
        return create_static_error_dict("typefind plugin couldn't be loaded");
    }

    results = gst_structure_new_empty(GST_OBJECT_NAME(factory));

    rank = gst_plugin_feature_get_rank(feature);
    factory_dict = gst_structure_new_empty("factory");
    gst_dictionary_set_string(factory_dict, "Rank", get_rank_name((gint)rank));

    gst_dictionary_set_static_string(factory_dict, "Name", GST_OBJECT_NAME(factory));
    gst_dictionary_set_sub_dictionary(results, "Factory Details", factory_dict);

    caps = gst_type_find_factory_get_caps(factory);
    if (caps)
    {
        GValue caps_arr = G_VALUE_INIT;
        parse_caps(caps, &caps_arr);

        gst_dictionary_set_array(results, "Caps", &caps_arr);
    }
    extensions = gst_type_find_factory_get_extensions(factory);
    if (extensions && *extensions)
    {
        GValue extensions_arr = G_VALUE_INIT;
        guint i = 0;

        g_value_init(&extensions_arr, GST_TYPE_LIST);

        while (extensions[i])
        {
            gst_array_append_static_string(&extensions_arr, extensions[i]);
            i++;
        }
        gst_dictionary_set_array(results, "Extensions", &extensions_arr);
    }

    plugin = gst_plugin_feature_get_plugin(GST_PLUGIN_FEATURE(factory));
    if (plugin)
    {
        GValue val = G_VALUE_INIT;
        gst_inspector_inspect_plugin_details(plugin, &val);
        gst_dictionary_set_value(results, "Plugin Details", &val);
        gst_object_unref(plugin);
    }

    gst_object_unref(factory);
    return results;
}

static GstStructure *inspect_tracer(GstPluginFeature *feature)
{
    GstStructure *results;
    GstTracerFactory *factory;
    GstTracer *tracer;
    GstPlugin *plugin;
    GValue tracer_ifaces = G_VALUE_INIT;
    GValue tracer_hierarchy = G_VALUE_INIT;

    factory = GST_TRACER_FACTORY(gst_plugin_feature_load(feature));
    if (!factory)
    {
        return create_static_error_dict("tracer plugin couldn't be loaded");
    }

    tracer = (GstTracer *)g_object_new(gst_tracer_factory_get_tracer_type(factory), NULL);
    if (!tracer)
    {
        gst_object_unref(factory);
        return create_static_error_dict("couldn't construct tracer for some reason");
    }

    results = gst_structure_new_empty(GST_OBJECT_NAME(factory));

    gst_dictionary_set_static_string(results, "Factory Name", GST_OBJECT_NAME(factory));

    plugin = gst_plugin_feature_get_plugin(GST_PLUGIN_FEATURE(factory));
    if (plugin)
    {
        GValue val = G_VALUE_INIT;
        gst_inspector_inspect_plugin_details(plugin, &val);
        gst_dictionary_set_value(results, "Plugin Details", &val);
        gst_object_unref(plugin);
    }

    parse_type_hierarchy(G_OBJECT_TYPE(tracer), &tracer_hierarchy);
    gst_dictionary_set_array(results, "Type Hierarchy", &tracer_hierarchy);

    parse_type_interfaces(G_OBJECT_TYPE(tracer), &tracer_ifaces);
    if (G_IS_VALUE(&tracer_ifaces))
    {
        gst_dictionary_set_array(results, "Implemented Interfaces",
                                 &tracer_ifaces);
    }

    // TODO: Inspect registered hooks
    // Will require access to private headers

    // TODO: Inspect tracer's record logs

    gst_object_unref(tracer);
    gst_object_unref(factory);
    return results;
}

/**
 *  @brief A generic inspection function for different kinds of GstPluginFeatures.
 * 
 *  @param feature A GstPluginFeature to inspect
 *
 *  @returns GstStructure with the inspection results
 */
GstStructure *gst_inspector_inspect_plugin_feature(GstPluginFeature *feature)
{
    if (!GST_IS_PLUGIN_FEATURE(feature))
    {
        return create_error_dict(gst_info_strdup_printf(
            "%" GST_PTR_FORMAT " is not a valid GstPluginFeature", feature));
    }

    if (GST_IS_ELEMENT_FACTORY(feature))
    {
        return gst_inspector_inspect_element(GST_ELEMENT_FACTORY(feature));
    }
    else if (GST_IS_TYPE_FIND_FACTORY(feature))
    {
        return inspect_typefind(feature);
    }
    else if (GST_IS_TRACER_FACTORY(feature))
    {
        return inspect_tracer(feature);
    }

    return create_error_dict(gst_info_strdup_printf(
        "%" GST_PTR_FORMAT " is an unsupported GstPluginFeature", feature));
}

/**
 *  @brief Inspects an object using a given name.
 * 
 *  If a plugin and an element shares the same name, the element takes
 *  precedence.
 * 
 *  @param object_name Name of object to inspect
 * 
 *  @returns GstStructure with the inspection results
 */
GstStructure *gst_inspector_inspect_by_name(const gchar *object_name)
{
    GstPluginFeature *feature;
    GstPlugin *plugin;

    CHECK_INIT;

    g_return_val_if_fail(object_name != NULL,
                         create_static_error_dict("Object name cannot be NULL"));

    feature = gst_registry_lookup_feature(gst_registry_get(), object_name);
    if (feature)
    {
        return gst_inspector_inspect_plugin_feature(feature);
    }

    // Try to find plugin
    plugin = gst_registry_find_plugin(gst_registry_get(), object_name);
    if (plugin)
    {
        return gst_inspector_inspect_plugin(plugin);
    }

    return create_error_dict(g_strdup_printf("Could not find object named %s",
                                             object_name));
}

/**
 *  @brief Retrieves a list of installed plugins
 * 
 *  @param flags Plugin flags to look for. If not 0, only plugins matching the
 *      given flags will be returned.
 *  @param version Minimal plugin version to look for
 *  @param list GValue initialized with GST_TYPE_LIST, to be filled with the 
 *      list of found plugins
 */
void gst_inspector_get_installed_plugins(GstPluginFlags flags,
                                         const gchar *version,
                                         GValue *list)
{
    GList *plugin_list, *plugin_iter;
    guint minver_maj, minver_min, minver_micro;
    guint ver_maj, ver_min, ver_micro;

    g_return_if_fail(GST_VALUE_HOLDS_LIST(list));

    minver_maj = GST_VERSION_MAJOR;
    minver_min = GST_VERSION_MINOR;
    minver_micro = GST_VERSION_MICRO;

    if (version)
    {
        if (sscanf(version, "%u.%u.%u", &minver_maj, &minver_min, &minver_micro) < 2)
        {
            g_critical("Cannot parse passed version string. Ignoring version check");
            minver_maj = GST_VERSION_MAJOR;
            minver_min = GST_VERSION_MINOR;
            minver_micro = GST_VERSION_MICRO;
        }
    }

    plugin_list = gst_registry_get_plugin_list(gst_registry_get());

    for (plugin_iter = plugin_list; plugin_iter != NULL; plugin_iter = plugin_iter->next)
    {
        GstPlugin *plugin = plugin_iter->data;

        if (sscanf(gst_plugin_get_version(plugin), "%u.%u.%u", &ver_maj,
                   &ver_min, &ver_micro) < 2)
        {
            g_critical("Cannot parse plugin %s version. Ignoring version check",
                       gst_plugin_get_name(plugin));
            ver_maj = minver_maj;
            ver_min = minver_min;
            ver_micro = minver_micro;
        }

        if ((flags == 0 && !GST_OBJECT_FLAG_IS_SET(plugin, GST_PLUGIN_FLAG_BLACKLISTED)) ||
            (flags != 0 && GST_OBJECT_FLAG_IS_SET(plugin, (guint)flags)))
        {
            if (ver_maj > minver_maj ||
                (ver_maj == minver_maj && ver_min > minver_min) ||
                (ver_maj == minver_maj && ver_min == minver_min && ver_micro >= minver_micro))
            {
                gst_array_append_static_string(list, gst_plugin_get_name(plugin));
            }
        }
    }

    gst_plugin_list_free(plugin_list);
}

/**
 *  @brief Retrieves a list of installed features
 * 
 *  @param flags Plugin flags to look for. If not 0, only features in 
 *      plugins matching the given flags will be returned.
 *  @param version Minimal feature version to look for 
 *  @param list GValue initialized with GST_TYPE_LIST, to be filled with the 
 *      list of found features
 */
void gst_inspector_get_installed_features(GstPluginFlags flags,
                                          const gchar *version,
                                          GValue *list)
{
    GList *plugin_list, *plugin_iter;
    guint minver_maj, minver_min, minver_micro;

    g_return_if_fail(GST_VALUE_HOLDS_LIST(list));

    minver_maj = GST_VERSION_MAJOR;
    minver_min = GST_VERSION_MINOR;
    minver_micro = GST_VERSION_MICRO;

    if (version)
    {
        if (sscanf(version, "%u.%u.%u", &minver_maj, &minver_min, &minver_micro) < 2)
        {
            g_critical("Cannot parse passed version string. Ignoring version check");
            minver_maj = GST_VERSION_MAJOR;
            minver_min = GST_VERSION_MINOR;
            minver_micro = GST_VERSION_MICRO;
        }
    }

    plugin_list = gst_registry_get_plugin_list(gst_registry_get());

    for (plugin_iter = plugin_list; plugin_iter != NULL;
         plugin_iter = plugin_iter->next)
    {
        GstPlugin *plugin = plugin_iter->data;

        if ((flags == 0 && !GST_OBJECT_FLAG_IS_SET(plugin, GST_PLUGIN_FLAG_BLACKLISTED)) ||
            (flags != 0 && GST_OBJECT_FLAG_IS_SET(plugin, (guint)flags)))
        {
            GList *feature_list, *feature_iter;
            feature_list = gst_registry_get_feature_list_by_plugin(
                gst_registry_get(),
                gst_plugin_get_name(plugin));

            for (feature_iter = feature_list; feature_iter != NULL;
                 feature_iter = feature_iter->next)
            {
                if (gst_plugin_feature_check_version(feature_iter->data,
                                                     minver_maj,
                                                     minver_min,
                                                     minver_micro))
                {
                    gst_array_append_static_string(
                        list,
                        gst_plugin_feature_get_name(feature_iter->data));
                }
            }

            gst_plugin_feature_list_free(feature_list);
        }
    }

    gst_plugin_list_free(plugin_list);
}

/**
 *  @brief Retrieves a dictionary of lists of installed features, one for each type
 * 
 *  @param flags Plugin flags to look for. If not 0, only features in 
 *      plugins matching the given flags will be returned.
 *  @param version Minimal feature version to look for
 * 
 *  @returns A dictionary holding lists of feature names. Free after use
 * 
 *  The dictionary's structure will be as follows:
 *  - @b elements - List of names of installed elements
 *  - @b typefinders - List of names of installed typefind functions
 *  - @b device-providers - List of names of installed device providers
 *  - @b tracers - List of names of installed tracers
 *  - @b others - List of names of installed features which are not one of the above
 */
GstStructure *gst_inspector_get_installed_features_grouped(GstPluginFlags flags,
                                                           const gchar *version)
{
    GstStructure *result = gst_structure_new_empty("features");
    GList *plugin_list, *plugin_iter;
    GValue elements_list = G_VALUE_INIT;
    GValue typefinders_list = G_VALUE_INIT;
    GValue devices_list = G_VALUE_INIT;
    GValue tracers_list = G_VALUE_INIT;
    GValue others_list = G_VALUE_INIT;
    guint minver_maj, minver_min, minver_micro;

    minver_maj = GST_VERSION_MAJOR;
    minver_min = GST_VERSION_MINOR;
    minver_micro = GST_VERSION_MICRO;

    g_value_init(&elements_list, GST_TYPE_LIST);
    g_value_init(&typefinders_list, GST_TYPE_LIST);
    g_value_init(&devices_list, GST_TYPE_LIST);
    g_value_init(&tracers_list, GST_TYPE_LIST);
    g_value_init(&others_list, GST_TYPE_LIST);

    if (version)
    {
        if (sscanf(version, "%u.%u.%u", &minver_maj, &minver_min, &minver_micro) < 2)
        {
            g_critical("Cannot parse passed version string. Ignoring version check");
            minver_maj = GST_VERSION_MAJOR;
            minver_min = GST_VERSION_MINOR;
            minver_micro = GST_VERSION_MICRO;
        }
    }

    plugin_list = gst_registry_get_plugin_list(gst_registry_get());

    for (plugin_iter = plugin_list; plugin_iter != NULL;
         plugin_iter = plugin_iter->next)
    {
        GstPlugin *plugin = plugin_iter->data;

        if ((flags == 0 && !GST_OBJECT_FLAG_IS_SET(plugin, GST_PLUGIN_FLAG_BLACKLISTED)) ||
            (flags != 0 && GST_OBJECT_FLAG_IS_SET(plugin, (guint)flags)))
        {
            GList *feature_list, *feature_iter;
            feature_list = gst_registry_get_feature_list_by_plugin(
                gst_registry_get(),
                gst_plugin_get_name(plugin));

            for (feature_iter = feature_list; feature_iter != NULL;
                 feature_iter = feature_iter->next)
            {
                if (gst_plugin_feature_check_version(feature_iter->data,
                                                     minver_maj,
                                                     minver_min,
                                                     minver_micro))
                {
                    if (GST_IS_ELEMENT_FACTORY(feature_iter->data))
                    {
                        gst_array_append_static_string(
                            &elements_list,
                            gst_plugin_feature_get_name(feature_iter->data));
                    }
                    else if (GST_IS_TYPE_FIND_FACTORY(feature_iter->data))
                    {
                        gst_array_append_static_string(
                            &typefinders_list,
                            gst_plugin_feature_get_name(feature_iter->data));
                    }
                    else if (GST_IS_DEVICE_PROVIDER_FACTORY(feature_iter->data))
                    {
                        gst_array_append_static_string(
                            &devices_list,
                            gst_plugin_feature_get_name(feature_iter->data));
                    }
                    else if (GST_IS_TRACER_FACTORY(feature_iter->data))
                    {
                        gst_array_append_static_string(
                            &tracers_list,
                            gst_plugin_feature_get_name(feature_iter->data));
                    }
                    else
                    {
                        gst_array_append_static_string(
                            &others_list,
                            gst_plugin_feature_get_name(feature_iter->data));
                    }
                }
            }

            gst_plugin_feature_list_free(feature_list);
        }
    }

    gst_dictionary_set_array(result, "elements", &elements_list);
    gst_dictionary_set_array(result, "typefinders", &typefinders_list);
    gst_dictionary_set_array(result, "device-providers", &devices_list);
    gst_dictionary_set_array(result, "tracers", &tracers_list);
    gst_dictionary_set_array(result, "others", &others_list);

    gst_plugin_list_free(plugin_list);
    return result;
}

/** @}*/
