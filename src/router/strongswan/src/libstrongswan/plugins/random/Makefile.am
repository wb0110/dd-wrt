
INCLUDES = -I$(top_srcdir)/src/libstrongswan

AM_CFLAGS = -rdynamic \
-DDEV_RANDOM=\"${random_device}\" \
-DDEV_URANDOM=\"${urandom_device}\"

if MONOLITHIC
noinst_LTLIBRARIES = libstrongswan-random.la
else
plugin_LTLIBRARIES = libstrongswan-random.la
endif

libstrongswan_random_la_SOURCES = \
	random_plugin.h random_plugin.c \
	random_rng.c random_rng.h

libstrongswan_random_la_LDFLAGS = -module -avoid-version
