CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude
LDLIBS ?= -lcrypto
all: test
test: aeon_batch31_test aeon_batch32_test aeon_batch33_test aeon_batch34_test aeon_batch35_test aeon_batch36_test aeon_batch37_test
	./aeon_batch31_test
	./aeon_batch32_test
	./aeon_batch33_test
	./aeon_batch34_test
	./aeon_batch35_test
	./aeon_batch36_test
	./aeon_batch37_test

aeon_batch31_test: core/aeon_identity.c core/aeon_identity_crypto.c core/aeon_syscall.c core/aeon_friction.c core/aeon_io_gate.c core/aeon_process_gate.c core/aeon_ebpf_boundary.c core/aeon_kernel_device.c core/aeon_net_policy.c core/aeon_kernel_adapter.c core/aeon_semantic_frame.c core/aeon_semantic_gate.c tests/test_batch31.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch32_test: core/aeon_identity.c core/aeon_identity_crypto.c core/aeon_syscall.c core/aeon_friction.c core/aeon_io_gate.c core/aeon_process_gate.c core/aeon_ebpf_boundary.c core/aeon_kernel_device.c core/aeon_net_policy.c core/aeon_kernel_adapter.c core/aeon_semantic_frame.c core/aeon_semantic_gate.c core/aeon_semantic_wire.c tests/test_batch32.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch33_test: core/aeon_runtime_memory.c tests/test_batch33.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch34_test: core/aeon_runtime_memory.c core/aeon_tensor.c tests/test_batch34.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch35_test: core/aeon_runtime_memory.c core/aeon_tensor.c core/aeon_tensor_view.c tests/test_batch35.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch36_test: core/aeon_model_stream.c tests/test_batch36.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch37_test: core/aeon_model_stream.c core/aeon_model_storage.c tests/test_batch37.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f aeon_batch31_test aeon_batch32_test aeon_batch33_test aeon_batch34_test aeon_batch35_test aeon_batch36_test aeon_batch37_test
