CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude
LDLIBS ?= -lcrypto

BATCH30_TEST = aeon_batch30_test
BATCH30_SRC = \
	core/aeon_identity.c \
	core/aeon_identity_crypto.c \
	core/aeon_syscall.c \
	core/aeon_friction.c \
	core/aeon_io_gate.c \
	core/aeon_process_gate.c \
	core/aeon_ebpf_boundary.c \
	core/aeon_kernel_device.c \
	core/aeon_net_policy.c \
	core/aeon_kernel_adapter.c \
	core/aeon_semantic_frame.c \
	core/aeon_semantic_gate.c \
	tests/test_batch30.c

.PHONY: all test batch30-test clean

all: test

test: batch30-test

batch30-test: $(BATCH30_TEST)
	./$(BATCH30_TEST)

$(BATCH30_TEST): $(BATCH30_SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f $(BATCH30_TEST)
