PKG_CONFIG_PATH:=$(shell pwd):$(PKG_CONFIG_PATH)
PKGS+=loop

include platform/makefile.in
