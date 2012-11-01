TEMPLATE = subdirs
SUBDIRS = data

# build must be last:
CONFIG += ordered
SUBDIRS += gui
