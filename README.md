# gst-inspector

The main goal of this repository is to create a simple interface to create custom tools for showing GStreamer's plugin information, in a same way like gst-inspect.

The project consists of 3 main layers

1. **Engine** - A dynamic engine that exports dictionaries of information about plugins, elements and parameters. It serves as the core for tools. Written in C, using GLib and GStreamer libraries.
2. **Code binding via SWIG** - This is mainly made to create a convenient environment for developing tools rapidly.
3. **Example Programs** - To show and test the results.

Right now the engine is in development.
