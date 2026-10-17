CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude
LDLIBS ?= -lcrypto
all: test
test: aeon_batch31_test aeon_batch32_test
	./aeon_batch31_test
	./aeon_batch32_test
aeon_batch31_test: core/aeon_identity.c core/aeon_identity_crypto.c \
	core/aeon_syscall.c core/aeon_friction.c core/aeon_io_gate.c \
	core/aeon_process_gate.c core/aeon_ebpf_boundary.c \
	core/aeon_kernel_device.c core/aeon_net_policy.c \
	core/aeon_kernel_adapter.c core/aeon_semantic_frame.c core/aeon_semantic_gate.c tests/test_batch31.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
aeon_batch32_test: core/aeon_identity.c core/aeon_identity_crypto.c \
	core/aeon_syscall.c core/aeon_friction.c core/aeon_io_gate.c \
	core/aeon_process_gate.c core/aeon_ebpf_boundary.c \
	core/aeon_kernel_device.c core/aeon_net_policy.c \
	core/aeon_kernel_adapter.c core/aeon_semantic_frame.c core/aeon_semantic_gate.c \
	core/aeon_semantic_wire.c tests/test_batch32.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
clean:
	rm -f aeon_batch31_test aeon_batch32_test
