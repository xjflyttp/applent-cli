LDFLAGS += -lhidapi-libusb
LDFLAGS += -L/usr/local/lib/

CPPFLAGS = -I/usr/local/include/hidapi/ -static

#LDFLAGS += -L/usr/local/lib/
#HEADERS += hidapi.h

all : hid
hid : hid.o
clean : 
	rm -f *.o
