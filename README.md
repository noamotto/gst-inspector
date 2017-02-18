# gst-inspect-server

The main goal of this repository is to create a simple interface to create custom tools for showing GStreamer's plugin information, in a same way like gst-inspect.

The project consists of 3 main layers

1. **Engine** - A dynamic engine that exports dictionaries of information about plugins, elements and parameters. It serves as the core for tools. Written in C++ for convenience.
2. **Code binding to Python** - This is mainly made to create a convenient environment for developing tools rapidly.
3. **HTTP Server for gst-inspect** - An example for a tool that can be made. A server that can show the list of installed plugins, show information about each plugin etc.

Right now the engine is in development.
