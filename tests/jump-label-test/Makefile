ARCH ?= openrisc
CROSS_COMPILE ?= or1k-none-linux-musl-

KERNEL_SRC ?= $HOME/work/or1k-vm-dir/modules/lib/modules/6.17.0-rc3-00017-gbb2483564597/build

PWD := $(shell pwd)

obj-m := jump_label_test.o

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
