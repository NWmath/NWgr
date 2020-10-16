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
copyright = '2020, Andrew Lumsdaine'
author = 'Andrew Lumsdaine'

# The full version, including alpha/beta/rc tags
release = '0.10.15'


# -- General configuration ---------------------------------------------------


sys.path.append(os.path.abspath('_extensions'))

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
#    'me'
    'myst_parser'
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.txt': 'markdown',
    '.md': 'markdown',
}


# Add any paths that contain templates here, relative to this directory.
templates_path = [ '_templates', '_layouts', '_includes']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', '_includes.hyde', '_layouts.hyde', '_templates.sav', 'attic', '_hyde']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx13'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

html_theme_path = [ "_themes" ]

# -- Jinja stuff -------------------------------------------------------------

import json

def jsonize(path):
    return json.loads(path)


def basename(path):
    if path:
      return os.path.basename(path)
    else:
      return ""


def markdownify(path):
    return path


def frob_slash(path):
    return path.replace('/', '2F')


def parent(path):
    (foo, bar) = os.path.split(os.path.dirname(path))
    return bar


def add_jinja_filters(app):
    # Make sure we're outputting HTML                                                                                               
    if app.builder.format != 'html':
        return

    app.builder.templates.environment.add_extension('jinja2_highlight.HighlightExtension')

#    app.builder.templates.environment.filters['basename']    = basename
    app.builder.templates.environment.filters['jsonize']     = jsonize
    app.builder.templates.environment.filters['markdownify'] = markdownify
#    app.builder.templates.environment.filters['frob_slash']  = frob_slash
#    app.builder.templates.environment.filters['parent']      = parent
#    app.builder.templates.environment.filters['siblings']    = siblings



def rstjinja(app, docname, source):
    """                                                                                                                             
    Render our pages as a jinja template for fancy templating goodness.                                                             
    """
    # Make sure we're outputting HTML                                                                                               
    if app.builder.format != 'html':
        return
    src = source[0]
    rendered = app.builder.templates.render_string(
        src, app.config.html_context
    )
    source[0] = rendered


def setup(app):
    '''                                                                                                                             
    Adds extra jinja filters.                                                                                                       
    '''
    app.connect("builder-inited", add_jinja_filters)
    app.connect("source-read", rstjinja)
