

// compare and set
extern uint8_t _cas(int32_t *ptr, int32_t old, int32_t new);


extern uint8_t _casint32()

// compare and set (pointers)
extern uint8_t _casp1(void **ptr, void* old, void* new);


// eXchange add
extern uint32_t _xadd(uint32_t volatile *val, int32_t delta);


// 64-bit eXchange add 
extern uint64_t _xadd64(uint64_t volatile* val, int64_t delta);

