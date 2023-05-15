CC      = gcc
CFLAGS  = -Wall -fno-builtin -nostdinc -nostdlib -ggdb3
LD      = ld
DEV1	= /dev/loop18
DEV2	= /dev/loop19
GRUB_PATH = ./grub_distr
IMAGE	= ./hdd.img
IMAGE_SIZE	= 16065
TEMP_DIR = tempdir

OBJFILES = \
	loader.o  \
	common/screen.o  \
	common/sleep.o  \
	kernel.o \
	tetris.o

image:
	@echo "Creating hdd.img..."
	dd if=/dev/zero of=$(IMAGE) bs=512 count=$(IMAGE_SIZE) 1>/dev/null 2>&1

	@echo "Creating bootable first FAT32 partition..."
	sudo losetup $(DEV1) $(IMAGE)
	(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk $(DEV1) 1>/dev/null 2>&1 || true

	@echo "Mounting partition to $(DEV2)..."
	sudo losetup $(DEV2) $(IMAGE) \
    --offset    `echo \`sudo fdisk -lu $(DEV1) | sed -n 9p | awk '{print $$3}'\`*512 | bc` \
    --sizelimit `echo \`sudo fdisk -lu $(DEV1) | sed -n 9p | awk '{print $$4}'\`*512 | bc`
	sudo losetup -d $(DEV1)

	@echo "Format partition..."
	sudo mkdosfs $(DEV2)

	@echo "Copy kernel and grub files on partition..."
	mkdir -p $(TEMP_DIR)
	mount $(DEV2) $(TEMP_DIR)
	mkdir $(TEMP_DIR)/boot
	cp -r grub $(TEMP_DIR)/boot/
	cp kernel.bin $(TEMP_DIR)/
	sleep 1
	umount $(DEV2)
	rm -r $(TEMP_DIR)
	sudo losetup -d $(DEV2)

	@echo "Installing GRUB..."
	echo "device (hd0) hdd.img \n \
	       root (hd0,0)         \n \
	       setup (hd0)          \n \
	       quit\n" | $(GRUB_PATH)/grub/usr/sbin/grub --batch 1>/dev/null
	@echo "Done!"

run:
	qemu-system-i386 -hda $(IMAGE)

rerun: clean all image run

runcgdb: clean all image
	sudo qemu-system-i386 -s -S -hda hdd.img &
	sudo cgdb kernel.bin.dbg

all: kernel.bin

rebuild: clean all

.s.o:
	as -o $@ $< --32

.c.o:
	$(CC) -Iinclude $(CFLAGS) -o $@ -c $< -m32

kernel.bin: $(OBJFILES)
	$(LD) -T linker.ld -o $@.dbg $^ -m elf_i386
	cp $@.dbg $@
	strip $@

clean:
	-sudo umount $(TEMP_DIR)
	-sudo losetup -d $(DEV1)
	-sudo losetup -d $(DEV2)
	rm -f $(OBJFILES) $(IMAGE) kernel.bin