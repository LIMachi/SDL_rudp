include ../Makefiles/default_var.mk

DEBUG := 1
#SANITIZE := 1

NAME := libsdl_rudp.a

CFLAGS += -D_REENTRANT -O0 -g

Darwin_LDFLAGS +=
Linux_LDLIBS += $(PACKAGE_MANAGER_LIB)/libSDL2_net.so $(PACKAGE_MANAGER_LIB)/libSDL2.so
Darwin_LDLIBS += $(PACKAGE_MANAGER_LIB)/libSDL2_net.dylib $(PACKAGE_MANAGER_LIB)/libSDL2.dylib

include ../Makefiles/lib.mk

ifeq ($(UNAME), Linux)

$(PACKAGE_MANAGER_LIB)/libSDL2_net.so:
	$(PACKAGE_MANAGER) $(PACKAGE_MANAGER_INSTALL_ARGUMENT) $(SDL2_NAME)
$(PACKAGE_MANAGER_LIB)/libSDL2.so:
	$(PACKAGE_MANAGER) $(PACKAGE_MANAGER_INSTALL_ARGUMENT) $(SDL2_NET_NAME)

else
ifeq ($(UNAME), Darwin)

$(PACKAGE_MANAGER_LIB)/libSDL2_net.dylib:
	$(PACKAGE_MANAGER) $(PACKAGE_MANAGER_INSTALL_ARGUMENT) $(SDL2_NAME)
$(PACKAGE_MANAGER_LIB)/libSDL2.dylib:
	$(PACKAGE_MANAGER) $(PACKAGE_MANAGER_INSTALL_ARGUMENT) $(SDL2_NET_NAME)

else


endif
endif
