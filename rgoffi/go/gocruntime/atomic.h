
// SwapInt32 atomically stores new into *addr and returns the previous *addr value.
int32_t swap_int32(int32_t *addr, int32_t new);

// SwapInt64 atomically stores new into *addr and returns the previous *addr value.
int64_t swap_int64(int64_t *addr, int64_t new);

// SwapUint32 atomically stores new into *addr and returns the previous *addr value.
uint32_t swap_uint32(uint32_t *addr, uint32_t new);

// SwapUint64 atomically stores new into *addr and returns the previous *addr value.
uint64_t swap_uint64(uint64_t *addr, uint64_t new);

// SwapUintptr atomically stores new into *addr and returns the previous *addr value.
uintptr_t swap_uintptr(uintptr_t *addr, uintptr_t new);

// SwapPointer atomically stores new into *addr and returns the previous *addr value.
void* swap_pointer(void **addr , void *new);

// CompareAndSwapInt32 executes the compare-and-swap operation for an int32 value.
uint8_t compare_and_swap_int32(int32_t *addr, int32_t old, int32_t new); 

// CompareAndSwapInt64 executes the compare-and-swap operation for an int64 value.
uint8_t compare_and_swap_int64(int64_t* addr, int64_t old, int64_t new);

// CompareAndSwapUint32 executes the compare-and-swap operation for a uint32 value.
uint8_t compare_and_swap_uint32(uint32_t *addr, uint32_t old, uint32_t new);

// CompareAndSwapUint64 executes the compare-and-swap operation for a uint64 value.
uint8_t compare_and_swap_uint64(uint64_t *addr, uint64_t old, uint64_t new);

// CompareAndSwapUintptr executes the compare-and-swap operation for a uintptr value.
uint8_t compare_and_swap_uintptr(uintptr_t *addr, uintptr_t old, uintptr_t new);

// CompareAndSwapPointer executes the compare-and-swap operation for a unsafe.Pointer value.
uint8_t compare_and_swap_pointer(void **addr , void *old, void *new);

// AddInt32 atomically adds delta to *addr and returns the new value.
int32_t add_int32(int32_t *addr, int32_t delta);

// AddUint32 atomically adds delta to *addr and returns the new value.
// To subtract a signed positive constant value c from x, do AddUint32(&x, ^uint32(c-1)).
// In particular, to decrement x, do AddUint32(&x, ^uint32(0)).
uint32_t add_uint32(uint32_t *addr, uint32_t delta);

// AddInt64 atomically adds delta to *addr and returns the new value.
int64_t add_int64(int64_t *addr, int64_t delta);

// AddUint64 atomically adds delta to *addr and returns the new value.
// To subtract a signed positive constant value c from x, do AddUint64(&x, ^uint64(c-1)).
// In particular, to decrement x, do AddUint64(&x, ^uint64(0)).
uint64_t add_uint64(uint64_t *addr, uint64_t delta);

// AddUintptr atomically adds delta to *addr and returns the new value.
uintptr_t add_uintptr(uintptr_t* addr, uintptr_t delta);

// LoadInt32 atomically loads *addr.
int32_t load_int32(int32_t *addr);

// LoadInt64 atomically loads *addr.
int64_t load_int64(int64_t *addr);

// LoadUint32 atomically loads *addr.
uint32_t load_uint32(uint32_t *addr);

// LoadUint64 atomically loads *addr.
uint64_t load_uint64(uint64_t *addr);

// LoadUintptr atomically loads *addr.
uintptr_t load_uintptr(uintptr_t *addr);

// LoadPointer atomically loads *addr.
void *load_pointer(void **addr);

// StoreInt32 atomically stores val into *addr.
void store_int32(int32_t *addr, int32_t val);

// StoreInt64 atomically stores val into *addr.
void store_int64(int64_t *addr, int64_t val);

// StoreUint32 atomically stores val into *addr.
void store_uint32(uint32_t *addr, uint32_t val);

// StoreUint64 atomically stores val into *addr.
void store_uint64(uint64_t *addr, uint64_t val);

// StoreUintptr atomically stores val into *addr.
void store_uintptr(uintptr_t *addr, uintptr_t val);

// StorePointer atomically stores val into *addr.
void store_pointer(void **addr, void *val);

