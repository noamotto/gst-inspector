#include "gstinspectors.h"
#include "gstinspector_priv.h"
#include "type_reader/type_reader.h"

static void parse_direction(GstStaticPadTemplate *padtemplate, GstStructure *template_dict)
{
    if (padtemplate->direction == GST_PAD_SRC)
    {
        gst_dictionary_set_static_string(template_dict, "Direction", "SRC");
    }
    else if (padtemplate->direction == GST_PAD_SINK)
    {
        gst_dictionary_set_static_string(template_dict, "Direction", "SINK");
    }
    else
    {
        gst_dictionary_set_static_string(template_dict, "Direction", "UNKNOWN");
    }
}

static void parse_availability(GstStaticPadTemplate *padtemplate, GstStructure *template_dict)
{
    if (padtemplate->presence == GST_PAD_ALWAYS)
    {
        gst_dictionary_set_static_string(template_dict, "Availability", "Always");
    }
    else if (padtemplate->presence == GST_PAD_SOMETIMES)
    {
        gst_dictionary_set_static_string(template_dict, "Availability", "Sometimes");
    }
    else if (padtemplate->presence == GST_PAD_REQUEST)
    {
        gst_dictionary_set_static_string(template_dict, "Availability", "On request");
    }
    else
    {
        gst_dictionary_set_static_string(template_dict, "Availability", "UNKNOWN");
    }
}

static void parse_pad_template(GstElement *element,
                               GstStaticPadTemplate *padtemplate,
                               GstStructure *template_dict)
{
    GstPadTemplate *tmpl = gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(element),
                                                              padtemplate->name_template);
    if (tmpl != NULL)
    {
        GType pad_type = GST_PAD_TEMPLATE_GTYPE(tmpl);

        if (pad_type != G_TYPE_NONE && pad_type != GST_TYPE_PAD)
        {
            gpointer pad_klass;
            GParamSpec **pspecs;
            guint n_properties;
            GstStructure *params_dict = gst_structure_new_empty("params");

            pad_klass = g_type_class_ref(pad_type);
            gst_dictionary_set_static_string(template_dict, KEY_TYPE, g_type_name(pad_type));

            pspecs = g_object_class_list_properties(G_OBJECT_GET_CLASS(G_OBJECT(pad_klass)),
                                                    &n_properties);

            for (guint i = 0; i < n_properties; i++)
            {
                gst_dictionary_set_sub_dictionary(params_dict, pspecs[i]->name,
                                                  parse_object_property(NULL, pspecs[i]));
            }
            g_type_class_unref(pad_klass);

            gst_dictionary_set_sub_dictionary(template_dict, "Pad Properties", params_dict);
        }
    }
}

void gst_inspector_inspect_pad_templates(GstElement *element, GValue *result)
{
    const GList *pads;
    GstElementFactory *factory;
    GstStructure *dictionary;

    g_return_if_fail(GST_IS_ELEMENT(element));
    factory = gst_element_get_factory(element);

    dictionary = gst_structure_new_empty("padtamplates");

    if (gst_element_factory_get_num_pad_templates(factory) == 0)
    {
        g_value_init(result, G_TYPE_STRING);
        g_value_set_static_string(result, "none");
    }
    else
    {
        GArray *templ_array = g_array_new(FALSE, FALSE, sizeof(GValue));
        g_array_set_clear_func(templ_array, (GDestroyNotify)g_value_unset);
        g_value_init(result, G_TYPE_ARRAY);
        
        pads = gst_element_factory_get_static_pad_templates(factory);
        while (pads)
        {
            GstStaticPadTemplate *padtemplate = (GstStaticPadTemplate *)(pads->data);
            GstStructure *template_dict = gst_structure_new_empty("template");

            pads = g_list_next(pads);

            gst_dictionary_set_static_string(template_dict, KEY_NAME, padtemplate->name_template);

            parse_direction(padtemplate, template_dict);
            parse_availability(padtemplate, template_dict);

            if (padtemplate->static_caps.string)
            {
                GstCaps *caps = gst_static_caps_get(&padtemplate->static_caps);

                gst_dictionary_set_array(template_dict, "Capabilities", parse_caps(caps));

                gst_caps_unref(caps);
            }

            parse_pad_template(element, padtemplate, template_dict);

            g_array_add_subdictionary(templ_array, template_dict);
        }

        g_value_take_boxed(result, templ_array);
    }
}