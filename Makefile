include Makefiles/default_var.mk

NAME := libsdl_rudp.a

CFLAGS += -D_REENTRANT

Darwin_LDFLAGS +=
LDLIBS += $(PACKAGE_MANAGER_LIB)/libSDL2_net.dylib

include Makefiles/lib.mk

$(PACKAGE_MANAGER_LIB)/libSDL2_net.dylib: $(PACKAGE_MANAGER)
	$(PACKAGE_MANAGER) install SDL2_net