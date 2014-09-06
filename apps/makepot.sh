#!/bin/sh
# This script file processes the source files looking for strings
# marked as translatable and outputs them to a template (.pot) file.
# That template is then merged into existing translation-specific .po
# files, removing obsolete and adding new messages to those .po files.
#
xgettext -C -d tqslapp -o tqslapp.pot  -k_  -k__ -ki18narg -E  *.cpp *.h