CFLAGS += $(MIPS16_OPT) $(COPTS) -I$(TOP)/ipeth/libimobiledevice/include -I$(TOP)/ipeth/libplist/include -DNEED_PRINTF -ffunction-sections -fdata-sections -Wl,--gc-sections 
LDLIBS += $(TOP)/ipeth/libimobiledevice/src/.libs/libimobiledevice.a -L$(TOP)/openssl -lssl -lcrypto -L$(TOP)/ipeth/libplist/src -lplist $(TOP)/ipeth/libusbmuxd/libusbmuxd/liblibusbmuxd-static.a $(TOP)/ipeth/libxml2/.libs/libxml2.a -lm -lpthread -ldl  -ffunction-sections -fdata-sections -Wl,--gc-sections 

all: ipheth-pair ipheth-loop

ipheth-pair: ipheth-pair.o

ipheth-loop: ipheth-loop.o
	$(CC) -o ipheth-loop  -fdata-sections -Wl,--gc-sections ipheth-loop.o

install:
	install -m 0755 ipheth-pair /lib/udev/
	install -m 0644 90-iphone-tether.rules /etc/udev/rules.d
	/sbin/udevadm control --reload-rules

clean:
	rm -f *.o ipheth-pair ipheth-loop
