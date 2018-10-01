### Stability:
- Travis (linux-gcc): [![Travis](https://travis-ci.org/noamotto/gst-inspector.svg?branch=master)](https://travis-ci.org/noamotto/gst-inspector)

Test coverage: [![Coverage Status](https://coveralls.io/repos/github/noamotto/gst-inspector/badge.svg?branch=master)](https://coveralls.io/github/noamotto/gst-inspector?branch=master)


# gst-inspector

## Motivation

While working with GStreamer's library I had to implement additional types and interfaces, and I encountered two problems working with the current gst-inspect program:

1. **Flexability** - gst-inspect does not allow customizations, unless you modify it and build it separately.
2. **Portability** - gst-inspect is command line tool that prints to stdout, which is restricting on custom environments, where a GUI or remote interface can serve better

In this library I will try to address those two problems, and offer a solution.

## Requirements

- gst-inspector requires only GStreamer to be installed, as it uses only GStreamer (and GLib). GStreamer 1.14 and above is required to build it.

- gst-inspector comes with CMake scripts (requires CMake 3.0 and above) which suppose to work on both Windows and Linux (ubuntu), which are the tested platforms.

## Concepts

### Inspectors

The core of gst-inspector is the inspector. The inspector is essentially a function that recieves an object and returns inspected data about it. Currently there are inspectors for plugins and elements. Those inspectors are registered to the library, allowing one to add more functionality quite easily.

### Dictionary

This library outputs data in a dictionary style, with JSON format in thought. A dictionary holds fields, and can hold strings, arrays and sub-dictionaries.

## Roadmap

The project consists of 2 phases

1. **Engine** - The core library, which should support exporting dictionaries of information about plugins and plugin fearures. Written in C, using the GLib and GStreamer libraries.
2. **Code binding via SWIG** - This is mainly made to create a convenient environment for developing tools rapidly.

Right now the engine is in development and testing.
