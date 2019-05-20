PKGS := gstreamer-video-1.0 gstreamer-base-1.0 gstreamer-net-1.0 gstreamer-1.0 glib-2.0
CFLAGS := $(shell pkg-config --cflags $(PKGS))  -Wall
LDFLAGS := $(shell pkg-config --libs $(PKGS)) -lm

TARGETS = gst-launch-remote
CC=gcc
LIBTOOL := libtool --tag=CC --mode=link

all: $(TARGETS)

gst-launch-remote: main.c ../gst-launch-remote/gst-launch-remote.c ../gst-launch-remote/gst-launch-remote.h
		$(LIBTOOL) $(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

clean:
		rm -f $(TARGETS)
