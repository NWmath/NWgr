//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Thejaka Kanewala
//

// This must be included only in the CPP file, and only once.

#ifndef SGL_TEST_HEADER
#define SGL_TEST_HEADER
#else
#error "test_header is included multiple times. test header must be included only once"
#endif

#include "nwgraph/containers/compressed.hpp"
#include "nwgraph/edge_list.hpp"
#include "nwgraph/io/mmio.hpp"

#include <catch2/catch.hpp>
