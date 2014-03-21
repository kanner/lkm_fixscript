SRC := ${wildcard *.c}
HEADERS := ${wildcard *.h ../include/*.h}

KERNEL_SOURCE_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := test.o
test-objs :=

all:	${SRC} ${HEADERS} Makefile
	${MAKE} modules -C ${KERNEL_SOURCE_DIR} M=${shell pwd}

clean:
	-rm -f *.ko *.o
	-rm -f *.mod.* .*.cmd
	-rm -rf .tmp_versions
	-rm -f Module.symvers Module.markers modules.order

.PHONY: all clean
.DELETE_ON_ERROR:

