
# vim: ft=make noexpandtab

OBJECTS := msim.o
OBJECTS_TESTS := check.o check_msim.o

BINDIR_LINUX = ../bin/linux
BINDIR_MSP430G2553 = ../bin/msp430g2553

MD=mkdir -v -p

$(BINDIR_LINUX)/libmsim.a: OBJDIR := ../obj/linux
$(BINDIR_LINUX)/libmsim.a: CFLAGS += \
	-DCHECK \
	-DDEBUG

$(BINDIR_MSP430G2553)/libmsim.a: OBJDIR := ../obj/msp430g2553
$(BINDIR_MSP430G2553)/libmsim.a: AR := msp430-ar
$(BINDIR_MSP430G2553)/libmsim.a: CC := msp430-gcc
$(BINDIR_MSP430G2553)/libmsim.a: CFLAGS += \
	-mmcu=msp430g2553 \
	-g \
	-pg \
	-Os \
	-fno-builtin \
	-ffreestanding \
	-nostdlib

test_msim: LDFLAGS += $(shell pkg-config --libs check) -L. -lmsim
test_msim: CFLAGS += -DCHECK -g -Wall -Werror
test_msim: OBJDIR := ../obj/test_msim

all: $(BINDIR_LINUX)/libmsim.a $(BINDIR_MSP430G2553)/libmsim.a

test_msim: $(BINDIR_LINUX)/libmsim.a $(addprefix ../obj/linux/,$(OBJECTS_TESTS))
	$(CC) -o $@ $^ -L$(BINDIR_LINUX) $(LDFLAGS)

$(BINDIR_LINUX)/libmsim.a: $(addprefix ../obj/linux/,$(OBJECTS))
	$(MD) $(BINDIR_LINUX)
	$(AR) rcs $@ $^

$(BINDIR_MSP430G2553)/libmsim.a: $(addprefix ../obj/msp430g2553/,$(OBJECTS))
	$(MD) $(BINDIR_MSP430G2553)
	$(AR) rcs $@ $^

../obj/linux/check.o: ../tests/check.c
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

../obj/linux/check_msim.o: ../tests/check_msim.c
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

../obj/linux/%.o: $(notdir %.c)
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

../obj/msp430g2553/%.o: $(notdir %.c)
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean docs

docs:
	doxygen doxyfile

clean:
	rm -rf ../obj; \
	rm -f test_msim; \
	rm -rf ../bin; \
	rm -rf ../doc

