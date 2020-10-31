# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'NW Graph'
copyright = '2020, PNNL, UW'
author = 'Andrew Lumsdaine'

# The full version, including alpha/beta/rc tags
release = '0.10.15'


# -- General configuration ---------------------------------------------------

sys.path.append(os.path.abspath('_extensions'))

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.mathjax',
    'sphinxcontrib.bibtex',
    'myst_parser',
    'sphinx_copybutton', 
    'hydeme'
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}


# Add any paths that contain templates here, relative to this directory.
templates_path = [ '_templates', '_layouts', '_includes']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', '_includes.hyde', '_layouts.hyde', '_templates.sav', 'attic', '_hyde']


# -- Options for MathJax -----------------------------------------------------

mathjax_options = {
}
mathjax_config = {
    'TeX': {
    'Macros': {
      'RR': "{\\mathbb R}",
      'Real': "{\\mathbb R}",
      'Complex': "{\\mathbb C}",        
      'mat' :["{\\mathbf{#1}}",1],
      'vec' :["{\\mathbf{#1}}",1],       
      'bold': ["{\\bf #1}",1],
        'Spc': ["\\mathbb{#1}",1],
        'norm': ["|| #1 ||",1]        
    }
  }
}



# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme_path = [ "_themes" ]
html_theme = 'sphinx13'


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

html_css_files = [
    'css/hyde_light.css'
]


# -- Jinja stuff -------------------------------------------------------------

import hydeme

html_context = hydeme.hydeme_context()
