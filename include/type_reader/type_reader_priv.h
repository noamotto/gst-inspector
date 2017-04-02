/***********************************************************************/
/*                Implementation for type readers                      */
/*              Private header - should not be used                    */
/***********************************************************************/

#ifndef TYPE_READER_PRIV_H
#define TYPE_READER_PRIV_H

#include "type_reader.h"

// Internal functions
void _gst_init_type_readers();

//Built-in type readers
void gst_bool_type_reader_fill_type(const GParamSpec *pspec,
                                    const GValue *value,
                                    GstStructure *const dictionary);

void gst_boxed_type_reader_fill_type(const GParamSpec *pspec,
                                     const GValue *value,
                                     GstStructure *const dictionary);

void gst_double_type_reader_fill_type(const GParamSpec *pspec,
                                      const GValue *value,
                                      GstStructure *const dictionary);

void gst_enum_type_reader_fill_type(const GParamSpec *pspec,
                                    const GValue *value,
                                    GstStructure *const dictionary);

void gst_flags_type_reader_fill_type(const GParamSpec *pspec,
                                     const GValue *value,
                                     GstStructure *const dictionary);

void gst_float_type_reader_fill_type(const GParamSpec *pspec,
                                     const GValue *value,
                                     GstStructure *const dictionary);

void gst_fraction_type_reader_fill_type(const GParamSpec *pspec,
                                        const GValue *value,
                                        GstStructure *const dictionary);

void gst_int_type_reader_fill_type(const GParamSpec *pspec,
                                   const GValue *value,
                                   GstStructure *const dictionary);

void gst_int64_type_reader_fill_type(const GParamSpec *pspec,
                                     const GValue *value,
                                     GstStructure *const dictionary);

void gst_long_type_reader_fill_type(const GParamSpec *pspec,
                                    const GValue *value,
                                    GstStructure *const dictionary);

void gst_object_type_reader_fill_type(const GParamSpec *pspec,
                                      const GValue *value,
                                      GstStructure *const dictionary);

void gst_pointer_type_reader_fill_type(const GParamSpec *pspec,
                                       const GValue *value,
                                       GstStructure *const dictionary);

void gst_string_type_reader_fill_type(const GParamSpec *pspec,
                                      const GValue *value,
                                      GstStructure *const dictionary);

void gst_uint_type_reader_fill_type(const GParamSpec *pspec,
                                    const GValue *value,
                                    GstStructure *const dictionary);

void gst_uint64_type_reader_fill_type(const GParamSpec *pspec,
                                      const GValue *value,
                                      GstStructure *const dictionary);

void gst_ulong_type_reader_fill_type(const GParamSpec *pspec,
                                     const GValue *value,
                                     GstStructure *const dictionary);

//GBoxed handling
void gst_boxed_reader_parse_content(const gpointer boxed,
                                    GstStructure *const dictionary);

//GObject handling
void gst_object_reader_parse_content(const GObject *boxed,
                                     GstStructure *const dictionary);

#endif //TYPE_READER_PRIV_H