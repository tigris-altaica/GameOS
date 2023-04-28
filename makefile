CC      = gcc
CFLAGS  = -Wall -fno-builtin -nostdinc -nostdlib
LD      = ld
DEV1	= /dev/loop18
DEV2	= /dev/loop19
GRUB_PATH = ./grub_distr

OBJFILES = \
	loader.o  \
	common/printf.o  \
	common/screen.o  \
	kernel.o

image:
	@echo "Creating hdd.img..."
	dd if=/dev/zero of=./hdd.img bs=512 count=16065 1>/dev/null 2>&1

	@echo "Creating bootable first FAT32 partition..."
	losetup $(DEV1) ./hdd.img
	(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk $(DEV1) 1>/dev/null 2>&1 || true

	@echo "Mounting partition to $(DEV2)..."
	losetup $(DEV2) ./hdd.img \
    --offset    `echo \`fdisk -lu $(DEV1) | sed -n 9p | awk '{print $$3}'\`*512 | bc` \
    --sizelimit `echo \`fdisk -lu $(DEV1) | sed -n 9p | awk '{print $$4}'\`*512 | bc`
	losetup -d $(DEV1)

	@echo "Format partition..."
	mkdosfs $(DEV2)

	@echo "Copy kernel and grub files on partition..."
	mkdir -p tempdir
	mount $(DEV2) tempdir
	mkdir tempdir/boot
	cp -r grub tempdir/boot/
	cp kernel.bin tempdir/
	sleep 1
	umount $(DEV2)
	rm -r tempdir
	losetup -d $(DEV2)

	@echo "Installing GRUB..."
	echo "device (hd0) hdd.img \n \
	       root (hd0,0)         \n \
	       setup (hd0)          \n \
	       quit\n" | $(GRUB_PATH)/grub/usr/sbin/grub --batch 1>/dev/null
	@echo "Done!"

run: clean all image
	qemu-system-i386 -hda hdd.img
all: kernel.bin image
rebuild: clean all image
.s.o:
	as -o $@ $< --32
.c.o:
	$(CC) -Iinclude $(CFLAGS) -o $@ -c $< -m32
kernel.bin: $(OBJFILES)
	$(LD) -T linker.ld -o $@ $^ -m elf_i386
clean:
	rm -f $(OBJFILES) hdd.img kernel.bin