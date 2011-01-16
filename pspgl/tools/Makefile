CC = gcc
RM = rm -f
CFLAGS = -g -O0 -Wall

TARGETS = decode_ge_dump decode_vram_dump

all: $(TARGETS)

decode_ge_dump: decode_ge_dump.c
	$(CC) $(CFLAGS) $< -o $@

decode_vram_dump: decode_vram_dump.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGETS)

