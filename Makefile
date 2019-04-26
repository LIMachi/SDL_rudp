include Makefiles/default_var.mk

DEBUG := 1
#SANITIZE := 1

NAME := libsdl_rudp.a

CFLAGS += -D_REENTRANT

Darwin_LDFLAGS +=
LDLIBS += $(PACKAGE_MANAGER_LIB)/libSDL2_net.dylib $(PACKAGE_MANAGER_LIB)/libSDL2.dylib

include Makefiles/lib.mk

$(PACKAGE_MANAGER_LIB)/libSDL2_net.dylib: $(PACKAGE_MANAGER)
	$(PACKAGE_MANAGER) install SDL2_net
$(PACKAGE_MANAGER_LIB)/libSDL2.dylib: $(PACKAGE_MANAGER)
	$(PACKAGE_MANAGER) install SDL2
