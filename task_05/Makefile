#Makefile

#3.1 documentation/kbuild
#Check if KERNELRELEASE is defined, if it is continue with kbuild
ifdef $(KERNELRELEASE)

obj-m += hello_world.o

#if KERNELRELEASE is not defined, call kernels makefile to build external module
else
#path to build directory (kernel src and makefile)--only take effect if KDIR is not already defined
KDIR ?= /lib/modules/$(shell uname -r)/build

#default build modules in current dir, no target needs specified [target] is modules
default:
	make -C $(KDIR) M=$(PWD)

endif
