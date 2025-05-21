# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import subprocess
subprocess.call('cd ../.. ; doxygen', shell=True)
subprocess.call('cd ../../bmp ; doxygen', shell=True)
subprocess.call('cd ../../doxybuild ; rm -rf deploy/bmp || true ; cp -a tools/html/* deploy ; cp -a bmp/html deploy/bmp', shell=True)

project = 'spinnaker_tools'
copyright = '%Y, The University of Manchester'
author = 'The University of Manchester'
release = 'latest'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = []

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']
html_extra_path = ['../../doxybuild/deploy']
