# Makefile - host unit tests (given) + firmware build (yours, from M1-02).
# Recipe lines MUST start with a TAB character, not spaces.

# ---------------- host unit tests (given) ----------------
HOSTCC  ?= gcc
HFLAGS   = -std=gnu11 -Wall -Wextra -g -O0 -DUNIT_TEST -Iinclude -Ithird_party/unity
HOST_SRC = $(filter-out src/startup.c src/main.c,$(wildcard src/*.c))
SUPPORT  = test/fakes.c test/fake_i2c.c third_party/unity/unity.c

build/test_%: test/test_%.c $(HOST_SRC) $(SUPPORT) $(wildcard include/*.h test/*.h)
	@mkdir -p build
	@$(HOSTCC) $(HFLAGS) -o $@ test/test_$*.c $(HOST_SRC) $(SUPPORT) -lm

# One test, full output:  make t T=m1_01
t:
	@$(MAKE) --no-print-directory build/test_$(T) && ./build/test_$(T)

# Runs exactly the tests named in tests.active (one name per line).
test-active:
	@set -e; for t in $$(grep -v '^[[:space:]]*#' tests.active | grep -v '^[[:space:]]*$$'); do \
	  $(MAKE) --no-print-directory build/$$t; ./build/$$t; done

# Runs every test file (red for issues you have not done yet).
test-all:
	@fail=0; for f in test/test_*.c; do t=$$(basename $$f .c); \
	  $(MAKE) --no-print-directory build/$$t >/dev/null || { echo "BUILD FAILED: $$t"; fail=1; continue; }; \
	  ./build/$$t | grep -E 'FAIL|Tests' ; ./build/$$t >/dev/null || fail=1; done; exit $$fail

# ---------------- firmware (M1-02 and M1-03) ----------------
# @TODO(M1-02/10) Fill the 5 variables: CROSS = arm-none-eabi-  ; CPU = -mcpu=cortex-m3 -mthumb ; CFLAGS (CPU, -Os -g -std=gnu11 -Wall -Wextra -ffreestanding -fno-tree-loop-distribute-patterns -ffunction-sections -fdata-sections -Iinclude) ; LDFLAGS (CPU, -nostdlib -T ld/stm32f103c6.ld -Wl,-Map=build/firmware.map -Wl,--gc-sections) ; FW_SRC = every .c file in src/. [src: M1-02 step 10]
CROSS = arm-none-eabi-
CPU = -mcpu=cortex-m3 -mthumb
CFLAGS = $(CPU) -Os -g -std=gnu11 -Wall -Wextra -ffreestanding -fno-tree-loop-distribute-patterns -ffunction-sections -fdata-sections -Iinclude
LDFLAGS = $(CPU) -nostdlib -T ld/stm32f103c6.ld -Wl,-Map=build/firmware.map -Wl,--gc-sections
FW_SRC = $(wildcard src/*.c)

# @TODO(M1-02/11) Write the rule for build/firmware.elf (compile all FW_SRC and link; end the link line with -lgcc), the rule for build/firmware.bin (objcopy -O binary), and the phony targets `firmware` and `size`. [src: M1-02 step 11]
build/firmware.elf: $(FW_SRC) $(wildcard include/*.h) ld/stm32f103c6.ld 
	@mkdir -p build
	$(CROSS)gcc $(CFLAGS) $(FW_SRC) $(LDFLAGS) -lgcc -o $@

build/firmware.bin: build/firmware.elf 
	$(CROSS)objcopy -O binary $< $@

firmware: build/firmware.elf build/firmware.bin
size: build/firmware.elf
	$(CROSS)size $<
# @TODO(M1-03/2) Add a `flash` target that depends on build/firmware.elf and runs OpenOCD: openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/firmware.elf verify reset exit" [src: M1-03 step 2]

.PHONY: t test-active test-all firmware size flash clean
clean:
	rm -rf build
