/**
 * @file imdb-graph.hpp
 *
 * @copyright SPDX-FileCopyrightText: 2022 Batelle Memorial Institute
 * @copyright SPDX-FileCopyrightText: 2022 University of Washington
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * @authors
 *   Andrew Lumsdaine
 *
 */

#include <string>
#include <vector>

std::vector<std::string> actors{
  "Tom Cruise",        
  "Kevin Bacon",    
  "Hugo Weaving",
  "Carrie-Anne Moss",
  "Natalie Portman",
  "Jack Nicholson",
  "Kelly McGillis", 
  "Harrison Ford", 
  "Sebastian Stan", 
  "Mila Kunis",
  "Michelle Pfeiffer", 
  "Keanu Reeves",   
  "Julia Roberts",
};

std::vector<std::string> movies{
    "A Few Good Men",
    "Top Gun",
    "Black Swan",
    "V for Vendetta",
    "The Matrix",
    "Witness",
    "What Lies Beneath",
    "Closer",
    "Flatliners",
};


std::vector<std::tuple<std::string, std::string>> movies_actors{
    {"A Few Good Men", "Tom Cruise"},
    {"A Few Good Men", "Kevin Bacon"},
    {"A Few Good Men", "Jack Nicholson"},
    {"What Lies Beneath", "Harrison Ford"},
    {"What Lies Beneath", "Kevin Bacon"},
    {"Top Gun", "Tom  Cruise"},
    {"Top Gun", "Kelly McGillis"},
    {"Witness", "Harrison Ford"},
    {"Witness", "Kelly McGillis"},
    {"Black Swan", "Sebastian Stan"},
    {"Black Swan", "Natalie Portman"},
    {"Black Swan", "Mila Kunis"},
    {"V for Vendetta", "Hugo Weaving"},
    {"V for Vendetta", "Natalie Portman"},
    {"The Matrix", "Carrie-Anne Moss"},
    {"The Matrix", "Keanu Reeves"},
    {"The Matrix", "Hugo Weaving"},
    {"What Lies Beneath", "Michelle Pfeiffer"},
    {"Closer", "Natalie Portman"},
    {"Closer", "Julia Roberts"},
    {"Flatliners", "Kevin Bacon"},
    {"Flatliners", "Julia Roberts"},
};


std::vector<std::tuple<size_t, size_t>> movie_actor_edge_list  {
{     0,     0},
{     0,     1},
{     0,     5},
{     1,     0},
{     1,     6},
{     2,     8},
{     2,     4},
{     2,     9},
{     3,     2},
{     3,     4},
{     4,     3},
{     4,    11},
{     4,     2},
{     5,     7},
{     5,     6},
{     6,     7},
{     6,     1},
{     6,    10},
{     7,     4},
{     7,    12},
{     8,     1},
{     8,    12},
};

  std::vector<std::tuple<size_t, size_t>> actor_movie_edge_list  {
{     0,     0},
{     0,     1},
{     1,     0},
{     1,     6},
{     1,     8},
{     2,     3},
{     2,     4},
{     3,     4},
{     4,     2},
{     4,     3},
{     4,     7},
{     5,     0},
{     6,     1},
{     6,     5},
{     7,     6},
{     7,     5},
{     8,     2},
{     9,     2},
{    10,     6},
{    11,     4},
{    12,     7},
{    12,     8},
  };

std::vector<std::vector<size_t>> movie_actor_index_adjacency_list  {
   /* 0*/ {  0,  1,  5, },
   /* 1*/ {  0,  6,     },
   /* 2*/ {  8,  4,  9, },
   /* 3*/ {  2,  4,     },
   /* 4*/ {  3, 11,  2, },
   /* 5*/ {  7,  6,     },
   /* 6*/ {  7,  1, 10, },
   /* 7*/ {   4, 12,    },
   /* 8*/ {   1, 12,    },
};

std::vector<std::vector<size_t>> actor_movie_index_adjacency_list  {
   /* 0*/ {  0,  1,    },
   /* 1*/ {  0,  6,  8,},
   /* 2*/ {  3,  4,    },
   /* 3*/ {  4,        },
   /* 4*/ {  2,  3,  7,},
   /* 5*/ {  0,        },
   /* 6*/ {  1,  5,    },
   /* 7*/ {  6,  5,    },
   /* 8*/ {  2,        },
   /* 9*/ {  2,        },
  /* 10*/ {  6,        },
  /* 11*/ {  4,        },
  /* 12*/ {  7,  8,    },
};

std::vector<std::tuple<size_t, size_t, size_t>> actor_actor_edge_list {
{  0,  1,  0},
{  0,  5,  0},
{  0,  6,  1},
{  1,  0,  0},
{  1,  5,  0},
{  1,  7,  6},
{  1,    10,  6},
{  1,    12,  8},
{  2,  4,  3},
{  2,  3,  4},
{  2,    11,  4},
{  3,    11,  4},
{  3,  2,  4},
{  4,  8,  2},
{  4,  9,  2},
{  4,  2,  3},
{  4,    12,  7},
{  5,  0,  0},
{  5,  1,  0},
{  6,  0,  1},
{  6,  7,  5},
{  7,  1,  6},
{  7,    10,  6},
{  7,  6,  5},
{  8,  4,  2},
{  8,  9,  2},
{  9,  8,  2},
{  9,  4,  2},
{    10,  7,  6},
{    10,  1,  6},
{    11,  3,  4},
{    11,  2,  4},
{    12,  4,  7},
{    12,  1,  8},
};

std::vector<std::vector<std::tuple<size_t, size_t>>> actor_actor_adjacency_list {
   /* 0*/ { {   1,   0 },{   5,   0 },{   6,   1 },},
   /* 1*/ { {   0,   0 },{   5,   0 },{   7,   6 },{  10,   6 },{  12,   8 },},
   /* 2*/ { {   4,   3 },{   3,   4 },{  11,   4 },},
   /* 3*/ { {  11,   4 },{   2,   4 },},
   /* 4*/ { {   8,   2 },{   9,   2 },{   2,   3 },{  12,   7 },},
   /* 5*/ { {   0,   0 },{   1,   0 },},
   /* 6*/ { {   0,   1 },{   7,   5 },},
   /* 7*/ { {   1,   6 },{  10,   6 },{   6,   5 },},
   /* 8*/ { {   4,   2 },{   9,   2 },},
   /* 9*/ { {   8,   2 },{   4,   2 },},
   /* 10*/ { {   7,   6 },{   1,   6 },},
   /* 11*/ { {   3,   4 },{   2,   4 },},
   /* 12*/ { {   4,   7 },{   1,   8 },},
};

std::vector<std::tuple<size_t, size_t, size_t>> movie_movie_edge_list {
{     0,     1,     0},
{     0,     5,     0},
{     0,     6,     1},
{     1,     0,     0},
{     1,     5,     0},
{     1,     7,     6},
{     1,    10,     6},
{     1,    12,     8},
{     2,     4,     3},
{     2,     3,     4},
{     2,    11,     4},
{     3,    11,     4},
{     3,     2,     4},
{     4,     8,     2},
{     4,     9,     2},
{     4,     2,     3},
{     4,    12,     7},
{     5,     0,     0},
{     5,     1,     0},
{     6,     0,     1},
{     6,     7,     5},
{     7,     1,     6},
{     7,    10,     6},
{     7,     6,     5},
{     8,     4,     2},
{     8,     9,     2},
{     9,     8,     2},
{     9,     4,     2},
{    10,     7,     6},
{    10,     1,     6},
{    11,     3,     4},
{    11,     2,     4},
{    12,     4,     7},
{    12,     1,     8},
};

std::vector<std::vector<std::tuple<size_t, size_t>>> movie_movie_adjacency_list {
   /* 0*/ { {      1,      0 },{      5,      0 },{      6,      1 },},
   /* 1*/ { {      0,      0 },{      5,      0 },{      7,      6 },{     10,      6 },{     12,      8 },},
   /* 2*/ { {      4,      3 },{      3,      4 },{     11,      4 },},
   /* 3*/ { {     11,      4 },{      2,      4 },},
   /* 4*/ { {      8,      2 },{      9,      2 },{      2,      3 },{     12,      7 },},
   /* 5*/ { {      0,      0 },{      1,      0 },},
   /* 6*/ { {      0,      1 },{      7,      5 },},
   /* 7*/ { {      1,      6 },{     10,      6 },{      6,      5 },},
   /* 8*/ { {      4,      2 },{      9,      2 },},
   /* 9*/ { {      8,      2 },{      4,      2 },},
   /* 10*/ { {      7,      6 },{      1,      6 },},
   /* 11*/ { {      3,      4 },{      2,      4 },},
   /* 12*/ { {      4,      7 },{      1,      8 },},
};



/*
Kevin Bacon has a bacon number of 0

Tom Cruise has a bacon number of 1
   Tom Cruise starred with Kevin Bacon in A Few Good Men

Hugo Weaving has a bacon number of 3
   Hugo Weaving starred with Natalie Portman in V for Vendetta
   Natalie Portman starred with Julia Roberts in Closer
   Julia Roberts starred with Kevin Bacon in Flatliners

Carrie-Anne Moss has a bacon number of 4
   Carrie-Anne Moss starred with Hugo Weaving in The Matrix
   Hugo Weaving starred with Natalie Portman in V for Vendetta
   Natalie Portman starred with Julia Roberts in Closer
   Julia Roberts starred with Kevin Bacon in Flatliners

Natalie Portman has a bacon number of 2
   Natalie Portman starred with Julia Roberts in Closer
   Julia Roberts starred with Kevin Bacon in Flatliners

Jack Nicholson has a bacon number of 1
   Jack Nicholson starred with Kevin Bacon in A Few Good Men

Kelly McGillis has a bacon number of 2
   Kelly McGillis starred with Tom Cruise in Top Gun
   Tom Cruise starred with Kevin Bacon in A Few Good Men

Harrison Ford has a bacon number of 1
   Harrison Ford starred with Kevin Bacon in What Lies Beneath

Sebastian Stan has a bacon number of 3
   Sebastian Stan starred with Natalie Portman in Black Swan
   Natalie Portman starred with Julia Roberts in Closer
   Julia Roberts starred with Kevin Bacon in Flatliners

Mila Kunis has a bacon number of 3
   Mila Kunis starred with Natalie Portman in Black Swan
   Natalie Portman starred with Julia Roberts in Closer
   Julia Roberts starred with Kevin Bacon in Flatliners

Michelle Pfeiffer has a bacon number of 1
   Michelle Pfeiffer starred with Kevin Bacon in What Lies Beneath

Keanu Reeves has a bacon number of 4
   Keanu Reeves starred with Hugo Weaving in The Matrix
   Hugo Weaving starred with Natalie Portman in V for Vendetta
   Natalie Portman starred with Julia Roberts in Closer
   Julia Roberts starred with Kevin Bacon in Flatliners

Julia Roberts has a bacon number of 1
   Julia Roberts starred with Kevin Bacon in Flatliners
*/
