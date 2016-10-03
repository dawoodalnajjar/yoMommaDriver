ifeq ($(KERNELRELEASE),)
	
     
    # Assume the source tree is where the running kernel was built
    # You should set KERNELDIR in the environment if it's elsewhere
    KERNELDIR ?= /lib/modules/$(shell uname -r)/build
    # The current directory is passed to sub-makes as argument
    PWD := $(shell pwd)
    
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	@cd $(KERNELDIR)
	@cd ..
	@depmod

modules_uninstall:
	@rm -f /lib/modules/$(shell uname -r)/extra/yoMomma.ko
	@cd $(KERNELDIR)
	@cd ..
	@depmod
	@rmmod ebb.ko

	
clean-dirs:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean-dirs
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

.PHONY: modules modules_install clean modules_uninstall clean-dirs

else
    # called from kernel build system: just declare what our modules are
    obj-m := yoMomma.o
    
endif
