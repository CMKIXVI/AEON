CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude
LDLIBS ?= -lcrypto

all: test

test: aeon_batch31_test aeon_batch32_test aeon_batch33_test aeon_batch34_test aeon_batch35_test aeon_batch36_test aeon_batch37_test aeon_batch38_test aeon_batch39_test aeon_batch40_test aeon_batch41_test aeon_batch42_test aeon_batch43_test aeon_batch44_test aeon_batch45_test aeon_batch46_test aeon_batch47_test aeon_batch49_test aeon_batch50_test aeon_batch51_test aeon_batch52_test aeon_batch53_test aeon_batch55_test aeon_batch56_test aeon_batch57_test aeon_batch58_test aeon_batch59_test aeon_batch60_test
	./aeon_batch31_test
	./aeon_batch32_test
	./aeon_batch33_test
	./aeon_batch34_test
	./aeon_batch35_test
	./aeon_batch36_test
	./aeon_batch37_test
	./aeon_batch38_test
	./aeon_batch39_test
	./aeon_batch40_test
	./aeon_batch41_test
	./aeon_batch42_test
	./aeon_batch43_test
	./aeon_batch44_test
	./aeon_batch45_test
	./aeon_batch46_test
	./aeon_batch47_test
	./aeon_batch49_test
	./aeon_batch50_test
	./aeon_batch51_test
	./aeon_batch52_test
	./aeon_batch53_test
	./aeon_batch55_test
	./aeon_batch56_test
	./aeon_batch57_test
	./aeon_batch58_test
	./aeon_batch59_test
	./aeon_batch60_test


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

aeon_batch38_test: core/aeon_model_stream.c core/aeon_nvme_storage.c tests/test_batch38.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch39_test: core/aeon_model_stream.c core/aeon_nvme_queue_read.c tests/test_batch39.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch40_test: core/aeon_model_stream.c core/aeon_nvme_queue_read.c tests/test_batch40.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch41_test: core/aeon_model_stream.c core/aeon_nvme_queue_read.c tests/test_batch41.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch42_test: core/aeon_model_index.c tests/test_batch42.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch43_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_model_manifest.c tests/test_batch43.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch44_test: core/aeon_tensor_layout.c tests/test_batch44.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch45_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c tests/test_batch45.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch46_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c tests/test_batch46.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch47_test: core/aeon_tensor_layout.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c tests/test_batch47.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch49_test: core/aeon_tensor_layout.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c tests/test_batch49.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch50_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c tests/test_batch50.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch51_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c core/aeon_tensor_cache_materialization.c tests/test_batch51.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch52_test: core/aeon_tensor_layout.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c tests/test_batch52.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch53_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c core/aeon_tensor_cache_materialization.c tests/test_batch53.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch55_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c core/aeon_tensor_cache_materialization.c tests/test_batch55.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch56_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c core/aeon_tensor_cache_materialization.c tests/test_batch56.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch57_test: core/aeon_tensor_cache.c tests/test_batch57.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch58_test: core/aeon_tensor_cache.c tests/test_batch58.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch59_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c core/aeon_tensor_cache_materialization.c tests/test_batch59.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

aeon_batch60_test: core/aeon_model_index.c core/aeon_model_stream.c core/aeon_tensor_layout.c core/aeon_tensor_binding.c core/aeon_tensor_materialization.c core/aeon_tensor_cache.c core/aeon_tensor_cache_materialization.c tests/test_batch60.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f aeon_batch31_test aeon_batch32_test aeon_batch33_test aeon_batch34_test aeon_batch35_test aeon_batch36_test aeon_batch37_test aeon_batch38_test aeon_batch39_test aeon_batch40_test aeon_batch41_test aeon_batch42_test aeon_batch43_test aeon_batch44_test aeon_batch45_test aeon_batch46_test aeon_batch47_test aeon_batch49_test aeon_batch50_test aeon_batch51_test aeon_batch52_test aeon_batch53_test aeon_batch55_test aeon_batch56_test aeon_batch57_test aeon_batch58_test aeon_batch59_test aeon_batch60_test
