all:
	clang -target x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -nostdlib -I. -c source/bootloader/main.c -o main.o
	lld-link /subsystem:efi_application /entry:efi_main /out:bootx64.efi main.o

run: all
	mkdir -p iso/EFI/BOOT
	cp bootx64.efi iso/EFI/BOOT/
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,readonly=on,file=$(PWD)/firmware/OVMF_CODE.fd \
		-drive format=raw,file=fat:rw:iso \
		-net none

clean:
	rm -rf *.o *.efi iso