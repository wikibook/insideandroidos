#
# This file should set PRODUCT_MAKEFILES to a list of product makefiles
# to expose to the build system.  LOCAL_DIR will already be set to
# the directory containing this file.
#
# This file may not rely on the value of any variable other than
# LOCAL_DIR; do not use any conditionals, and do not look up the
# value of any variable that isn't set in this file or in a file that
# it includes.
#

COMMON_LUNCH_CHOICES := \
	acme_one-eng \
	acme_one-userdebug \
	acme_one-user

PRODUCT_MAKEFILES := \
	$(LOCAL_DIR)/acme_one.mk

