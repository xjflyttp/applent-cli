all : hid
	$(CC) hid.cpp -lhidapi-libusb -L/usr/local/lib/ -I/usr/local/include/hidapi/  -o hid 
clean : 
	rm -f *.o
