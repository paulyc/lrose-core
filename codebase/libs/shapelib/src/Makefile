#
# Recursive make - makes the subdirectory code
#

include $(LROSE_CORE_DIR)/build/make_include/lrose_make_macros

TARGETS = $(GENERAL_TARGETS)

MODULE_NAME = shapelib

LIBNAME = lib$(MODULE_NAME).a

SUB_DIRS = \
	shapelib

include $(LROSE_CORE_DIR)/build/make_include/lrose_make_recursive_dir_targets

include $(LROSE_CORE_DIR)/build/make_include/lrose_make_inc_targets

include $(LROSE_CORE_DIR)/build/make_include/lrose_make_lib_targets
