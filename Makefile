AS_SOURCES = $(wildcard **/*.s)
C_SOURCES  = $(wildcard **/*.c)
C_HEADERS  = $(wildcard **/*.h)
OBJECTS    = ${C_SOURCES:.c=.o} ${AS_SOURCES:.s=.o}

CC = x86_64-elf-gcc
AS = nasm
LD = x86_64-elf-ld

CFLAGS 	= -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -Iinclude/
LDFLAGS = -T link.ld -n -melf_x86_64
ASFLAGS = -f elf64

all: mink.iso

run: mink.iso
	@qemu-system-x86_64 -hda mink.iso -m 4096

mink.iso: kernel.elf
	@mkdir -p build/boot/grub/
	@cp tools/grub/grub.cfg build/boot/grub/
	@cp kernel.elf build/boot/
	@grub-mkrescue -o mink.iso build/

kernel.elf: $(OBJECTS)
	@$(LD) $(LDFLAGS) $(OBJECTS) -o kernel.elf

%.o: %.c $(C_HEADERS)
	@$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	@find . -name '*.o' -type f -delete
	@find . -name '*.elf' -type f -delete
	@find . -name '*.iso' -type f -delete
	@rm -rf build/
