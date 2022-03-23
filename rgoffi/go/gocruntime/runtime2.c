#include <stdint.h>


static const int tls_slots = 6;

typedef uintptr_t guintptr_t;
typedef uintptr_t muintptr_t;
typedef uintptr_t puintptr_t; 
typedef uintptr_t tracebufptr_t;
typedef int gc_mark_worker_mode_t;

// G status
enum {
	_Gidle = 0,
};


// TODO(threadedstream): to be moved to mgc
enum {
	// GC_MARK_WORKER_NOT_WORKER indicates that the next scheduled G is not
	// starting work and the mode should be ignored.
	GC_MARK_WORKER_NOT_WORKER = 0,

	// GC_MARK_WORKER_DEDICATED_MODE indicates that the P of a mark
	// worker is dedicated to running that mark worker. The mark
	// worker should run without preemption.
	GC_MARK_WORKER_DEDICATED_MODE,

	// GC_MARK_WORKER_FRACTIONAL_MODE indicates that a P is currently
	// running the "fractional" mark worker. The fractional worker
	// is necessary when GOMAXPROCS*gcBackgroundUtilization is not
	// an integer and using only dedicated workers would result in
	// utilization too far from the target of gcBackgroundUtilization.
	// The fractional worker should run until it is preempted and
	// will be scheduled to pick up the fractional part of
	// GOMAXPROCS*gcBackgroundUtilization.
	GC_MARK_WORKER_FRACTIONAL_MODE,

	// GC_MARK_WORKER_IDLE_MODE indicates that a P is running the mark
	// worker because it has nothing else to do. The idle worker
	// should run until it is preempted and account its time
	// against gcController.idleMarkTime.
	GC_MARK_WORKER_IDLE_MODE,
};


struct stack;
struct _panic;
struct _defer;
struct gobuf;
struct wait_reason;
struct ancestor_info;
struct sudog;
struct timer;
struct m;
struct g;
struct p;
struct glist;
// TODO(threadedstream): to be moved to histogram
struct time_histogram;
// TODO(threadedstream): to be moved to proc
struct sysmontick;
struct mcache;
struct persistent_alloc;
struct gc_work;
struct wbbuf;
struct mutex;
struct gqueue;
struct page_cache;
struct gsignalstack;
// TODO(threadedstream): to be moved to mheap
struct mspan;
// TODO(threadedstream): to be moved to os_linux_generic
struct sigset;
struct dlog_per_m;
// TODO(threadedstream): to be moved to os_linux
struct m_os;
struct held_lock_info;
struct func_info;
struct bitvector;
struct cgocallers;
struct note;
struct libcall;

typedef struct p p_t;
typedef struct g g_t;
typedef struct gqueue gqueue_t;
typedef struct gsignalstack gsignalstack_t;
typedef struct sigset sigset_t;
typedef struct dlog_per_m dlog_per_m_t;
typedef struct persistent_alloc persistent_alloc_t;
typedef struct mcache mcache_t;
typedef struct page_cache page_cache_t;
typedef struct gc_work gc_work_t;
typedef struct wbbuf wbbuf_t;
typedef struct mutex mutex_t;
typedef struct m_os m_os_t; 
typedef struct held_lock_info held_lock_info_t;
typedef struct cgocallers cgocallers_t;
typedef struct note note_t;
typedef struct libcall libcall_t;
typedef struct m m_t;
typedef struct stack stack_t;
typedef struct _panic _panic_t;
typedef struct bitvector bitvector_t;
typedef struct func_info func_info_t;
typedef struct _defer _defer_t;
typedef struct gobuf gobuf_t;
typedef struct wait_reason wait_reason_t;
typedef struct ancestor_info ancestor_info_t;
typedef struct sudog sudog_t;
typedef struct timer timer_t;
typedef struct glist glist_t;
typedef struct mspan mspan_t;
typedef struct sysmontick sysmontick_t;
typedef struct time_histogram time_histogram_t;

struct _defer {
	uint8_t started;
	uint8_t heap;
	// openDefer indicates that this _defer is for a frame with open-coded
	// defers. We have only one defer record for the entire frame (which may
	// currently have 0, 1, or more defers active).
	uint8_t open_defer; 
	uintptr_t sp;         // sp at time of defer
	uintptr_t pc;         // pc at time of defer
	void (*fn)();          // can be nil for open-coded defers
	_panic_t *_panic;     // panic that is running defer
	_defer_t *link;       // next defer on G; can point to either heap or stack!

	// If openDefer is true, the fields below record values about the stack
	// frame and associated function that has the open-coded defer(s). sp
	// above will be the sp for the frame, and pc will be address of the
	// deferreturn call in the function.
	void *fd;   // funcdata for the function associated with the frame
	uintptr_t varp;         // value of varp for the stack frame
	// framepc is the current pc associated with the stack frame. Together,
	// with sp above (which is the sp associated with the stack frame),
	// framepc/sp can be used as pc/sp pair to continue a stack trace via
	// gentraceback().
	uintptr_t framepc; 
};

struct _panic {
	void *argp; // pointer to arguments of deferred call run during panic; cannot move - known to liblink
	// in original source file, arg is annotated with type any
	void *arg;                   // argument to panic
	_panic_t *link;              // link to earlier panic
	uintptr_t pc;                // where to return to in runtime if this panic is bypassed
	void *sp; // where to return to in runtime if this panic is bypassed
	uint8_t recovered;            // whether this panic is over
	uint8_t aborted;              // the panic was aborted
	uint8_t goexit;    
};

// stack traces
struct stkframe {
	func_info_t fn;   // function being run
	uintptr_t pc;          // program counter within fn
	uintptr_t continpc;    // program counter where execution can continue, or 0 if not
	uintptr_t lr;    // program counter at caller aka link register
	uintptr_t sp;    // stack pointer at pc
	uintptr_t fp;   // stack pointer at caller aka frame pointer
	uintptr_t varp;    // top of local variables
	uintptr_t argp;    // pointer to function arguments
	uintptr_t arglen;    // number of bytes at argp
	bitvector_t *argmap;    // force use of this argmap
};

struct stack {};
struct gqueue{};
struct gobuf{};
struct wait_reason{};
struct gsignalstack{};
struct sigset{};
struct time_histogram{};
struct note{};
struct libcall{};
struct dlog_per_m{};
struct glist {};
struct mutex {};
struct m_os{};
struct held_lock_info{};
struct sysmontick {};
struct page_cache {};
struct wbbuf {};
struct mcache {};
struct persistent_alloc {};
struct gc_work{};

struct g{
		// Stack parameters.
	// stack describes the actual stack memory: [stack.lo, stack.hi).
	// stackguard0 is the stack pointer compared in the Go stack growth prologue.
	// It is stack.lo+StackGuard normally, but can be StackPreempt to trigger a preemption.
	// stackguard1 is the stack pointer compared in the C stack growth prologue.
	// It is stack.lo+StackGuard on g0 and gsignal stacks.
	// It is ~0 on other goroutine stacks, to trigger a call to morestackc (and crash).
	stack_t stack;   // offset known to runtime/cgo
	uintptr_t stackguard0; // offset known to liblink
	uintptr_t stackguard1; // offset known to liblink

	_panic_t *_panic; // innermost panic - offset known to liblink
	_defer_t    *_defer; // innermost defer
	m_t         *m; // current m; offset known to arm liblink
	gobuf_t sched;
	uintptr_t syscallsp; // if status==Gsyscall, syscallsp = sched.sp to use during gc
	uintptr_t syscallpc; // if status==Gsyscall, syscallpc = sched.pc to use during gc
	uintptr_t stktopsp; // expected sp at top of stack, to check in traceback
	// param is a generic pointer parameter field used to pass
	// values in particular contexts where other storage for the
	// parameter would be difficult to find. It is currently used
	// in three ways:
	// 1. When a channel operation wakes up a blocked goroutine, it sets param to
	//    point to the sudog of the completed blocking operation.
	// 2. By gcAssistAlloc1 to signal back to its caller that the goroutine completed
	//    the GC cycle. It is unsafe to do so in any other way, because the goroutine's
	//    stack may have moved in the meantime.
	// 3. By debugCallWrap to pass parameters to a new goroutine because allocating a
	//    closure in the runtime is forbidden.
	void *param;
	uint32_t atomicstatus;
	uint32_t stack_lock; // sigprof/scang lock; TODO: fold in to atomicstatus
	int64_t goid;
	guintptr_t schedlink;
	int64_t waitsince;      // approx time when the g become blocked
	wait_reason_t waitreason; // if status==Gwaiting

	uint8_t preempt; // preemption signal, duplicates stackguard0 = stackpreempt
	uint8_t preempt_stop; // transition to _Gpreempted on preemption; otherwise, just deschedule
	uint8_t preempt_shrink; // shrink stack at synchronous safe point

	// asyncSafePoint is set if g is stopped at an asynchronous
	// safe point. This means there are frames on the stack
	// without precise pointer information.
	uint8_t async_safe_point;

	uint8_t paniconfault; // panic (instead of crash) on unexpected fault address
	uint8_t gcscandone; // g has scanned stack; protected by _Gscan bit in status
	uint8_t throwsplit; // must not split stack
	// activeStackChans indicates that there are unlocked channels
	// pointing into this goroutine's stack. If true, stack
	// copying needs to acquire channel locks to protect these
	// areas of the stack.
	uint8_t active_stack_chans;
	// parkingOnChan indicates that the goroutine is about to
	// park on a chansend or chanrecv. Used to signal an unsafe point
	// for stack shrinking. It's a boolean value, but is updated atomically.
	uint8_t parking_on_chan;

	int8_t raceignore;     // ignore race detection events
	uint8_t sysblocktraced;     // StartTrace has emitted EvGoInSyscall about this goroutine
	uint8_t tracking;     // whether we're tracking this G for sched latency statistics
	uint8_t trackingSeq;    // used to decide whether to track this G
	int64_t runnableStamp;    // timestamp of when the G last became runnable, only used when tracking
	int64_t runnableTime;    // the amount of time spent runnable, cleared when running, only used when tracking
	int64_t sysexitticks;    // cputicks when syscall has returned (for tracing)
	uint64_t traceseq;   // trace event sequencer
	puintptr_t tracelastp; // last P emitted an event for this goroutine
	muintptr_t lockedm;
	uint32_t sig;
	uint8_t *writebuf;
	uintptr_t sigcode0;
	uintptr_t sigcode1;
	uintptr_t sigpc;
	uintptr_t gopc;         // pc of go statement that created this goroutine
	ancestor_info_t **ancestors; // ancestor information goroutine(s) that created this goroutine (only used if debug.tracebackancestors)
	uintptr_t startpc;         // pc of goroutine function
	uintptr_t racectx;
	sudog_t* waiting;         // sudog structures this g is waiting on (that have a valid elem ptr); in lock order
	uintptr_t *cgo_ctxt;      // cgo traceback context
	void *labels; // profiler labels
	timer_t *timer;         // cached timer for time.Sleep
	uint32_t select_done;         // are we participating in a select and did someone win the race?

	// Per-G GC state

	// gcAssistBytes is this G's GC assist credit in terms of
	// bytes allocated. If this is positive, then the G has credit
	// to allocate gcAssistBytes bytes without assisting. If this
	// is negative, then the G must correct this by performing
	// scan work. We track this in bytes to make it fast to update
	// and check for debt in the malloc hot path. The assist ratio
	// determines how this corresponds to scan work debt.
	int64_t gc_assist_bytes;
};

struct m {
	g_t *g0;     // goroutine with scheduling stack
	gobuf_t morebuf;  // gobuf arg to morestack
	uint32_t divmod; // div/mod denominator for arm - known to liblink

	// Fields not known to debuggers.
	uint64_t procid;            // for debuggers, but offset not hard-coded
	g_t *gsignal;                // signal-handling g
	gsignalstack_t gosigstack;      // Go-allocated signal handling stack
	sigset_t sigmask;                   // storage for saved signal mask
	uintptr_t tls[tls_slots];            // thread-local storage (for x86 extern register)
	void (*mstartfn)();      
	g_t *curg;       // current running goroutine
	guintptr_t caughtsig;      // goroutine running during fatal signal
	puintptr_t p;              // attached p for executing go code (nil if not executing go code)
	puintptr_t nextp;         
	puintptr_t oldp; // the p that was attached before executing a syscall
	int64_t id;            
	int32_t mallocing;     
	int32_t throwing;      
	// TODO(threadedstream): should be heap-allocated?
	const char* preemptoff;     // if != "", keep curg running on this m
	int32_t locks;        
	int32_t dying;         
	int32_t profilehz;     
	uint8_t spinning;       // m is out of work and is actively looking for work
	uint8_t blocked; // m is blocked on a note
	uint8_t new_sigstack; // minit on C thread called sigaltstack
	int8_t printlock;
	uint8_t incgo;   // m is executing a cgo call
	uint32_t free_wait;       // if == 0, safe to free g0 and delete m (atomic)
	uint64_t fastrand;      
	uint8_t needextram;    
	uint8_t traceback;     
	uint64_t ncgocall;            // number of cgo calls in total
	int32_t ncgo;                 // number of cgo calls currently in progress
	uint32_t cgo_callers_use;       // if non-zero, cgoCallers in use temporarily
	cgocallers_t *cgo_callers;    // cgo traceback if crashing in cgo call
	note_t park;          
	m_t *alllink;        // on allm
	muintptr_t schedlink;     
	guintptr_t lockedg;       
	uintptr_t createstack[32];    // stack that created this thread.
	uint32_t locked_ext;           // tracking for external LockOSThread
	uint32_t locked_int;           // tracking for internal lockOSThread
	muintptr_t nextwaitm;         // next m waiting for lock
	uint8_t (*waitunlockf) (g_t *, void*);
	void *waitlock;
	uint8_t waittraceev;   
	int waittraceskip; 
	uint8_t startingtrace; 
	uint32_t syscalltick;   
	m_t *freelink;       // on sched.freem

	// these are here because they are too large to be on the stack
	// of low-level NOSPLIT functions.
	libcall_t libcall;   
	uintptr_t libcallpc;  // for cpu profiler
	uintptr_t libcallsp; 
	guintptr_t libcallg;  
	libcall_t syscall;    // stores syscall parameters on windows

	uintptr_t vdso_sp;  // SP for traceback while in VDSO call (0 if not in call)
	uintptr_t vdso_pc; // PC for traceback while in VDSO call

	// preemptGen counts the number of completed preemption
	// signals. This is used to detect when a preemption is
	// requested, but fails. Accessed atomically.
	uint32_t preempt_gen; 

	// Whether this is a pending preemption signal on this M.
	// Accessed atomically.
	uint32_t signal_pending; 

	dlog_per_m_t _;

	m_os_t os;

	// Up to 10 locks held by this m, maintained by the lock ranking code.
	int locks_held_len;
	held_lock_info_t locks_held[10];    
};


struct p {
	int32_t id;    
	uint32_t status;       // one of pidle/prunning/...
	puintptr_t link;        
	uint32_t schedtick;        // incremented on every scheduler call
	uint32_t syscalltick;      // incremented on every system call
	sysmontick_t sysmontick;   // last tick observed by sysmon
	muintptr_t m;              // back-link to associated m (nil if idle)
	mcache_t *mcache;      
	page_cache_t pcache;      
	uintptr_t raceprocctx; 

	_defer_t **deferpool;     // pool of available defer structs (see panic.go)
	_defer_t *deferpoolbuf[32]; 

	// Cache of goroutine ids, amortizes accesses to runtime·sched.goidgen.
	uint64_t goidcache;
	uint64_t goidcacheend; 

	// Queue of runnable goroutines. Accessed without lock.
	uint32_t runqhead;
	uint32_t runqtail;
	guintptr_t runq[256];
	// runnext, if non-nil, is a runnable G that was ready'd by
	// the current G and should be run next instead of what's in
	// runq if there's time remaining in the running G's time
	// slice. It will inherit the time left in the current time
	// slice. If a set of goroutines is locked in a
	// communicate-and-wait pattern, this schedules that set as a
	// unit and eliminates the (potentially large) scheduling
	// latency that otherwise arises from adding the ready'd
	// goroutines to the end of the run queue.
	//
	// Note that while other P's may atomically CAS this to zero,
	// only the owner P can CAS it to a valid G.
	guintptr_t runnext; 

	// Available G's (status == Gdead)
	struct {
		glist_t list;
		int32_t n; 
	} gfree; 

	sudog_t **sudogcache; 
	sudog_t *sudogbuf[128];

	// Cache of mspan objects from the heap.
	struct {
		// We need an explicit length here because this field is used
		// in allocation codepaths where write barriers are not allowed,
		// and eliminating the write barrier/keeping it eliminated from
		// slice updates is tricky, moreso than just managing the length
		// ourselves.
		int len; 
		mspan_t *buf[128];
	} mspancache; 

	tracebufptr_t tracebuf; 

	// traceSweep indicates the sweep events should be traced.
	// This is used to defer the sweep start event until a span
	// has actually been swept.
	uint8_t trace_sweep; 
	// traceSwept and traceReclaimed track the number of bytes
	// swept and reclaimed by sweeping in the current sweep loop.
	uintptr_t trace_swept, trace_reclaimed;

	persistent_alloc_t palloc;  // per-P to avoid mutex

	uint32_t _;  // Alignment for atomic fields below

	// The when field of the first entry on the timer heap.
	// This is updated using atomic functions.
	// This is 0 if the timer heap is empty.
	uint64_t timer0_when; 

	// The earliest known nextwhen field of a timer with
	// timerModifiedEarlier status. Because the timer may have been
	// modified again, there need not be any timer with this value.
	// This is updated using atomic functions.
	// This is 0 if there are no timerModifiedEarlier timers.
	uint64_t timer_modified_earliest; 

	// Per-P GC state
	int64_t gcAssistTime;          // Nanoseconds in assistAlloc
	int64_t gcFractionalMarkTime;  // Nanoseconds in fractional mark worker (atomic)

	// gcMarkWorkerMode is the mode for the next mark worker to run in.
	// That is, this is used to communicate with the worker goroutine
	// selected for immediate execution by
	// gcController.findRunnableGCWorker. When scheduling other goroutines,
	// this field must be set to gcMarkWorkerNotWorker.
	gc_mark_worker_mode_t gc_mark_worker_mode; 
	// gcMarkWorkerStartTime is the nanotime() at which the most recent
	// mark worker started.
	int64_t gc_mark_worker_start_time;

	// gcw is this P's GC work buffer cache. The work buffer is
	// filled by write barriers, drained by mutator assists, and
	// disposed on certain GC state transitions.
	gc_work_t gcw; 

	// wbBuf is this P's GC write barrier buffer.
	//
	// TODO: Consider caching this in the running G.
	wbbuf_t wbbuf;

	uint32_t run_safe_point_fn;  // if 1, run sched.safePointFn at next safe point

	// statsSeq is a counter indicating whether this P is currently
	// writing any stats. Its value is even when not, odd when it is.
	uint32_t stats_seq; 

	// Lock for timers. We normally access the timers while running
	// on this P, but the scheduler can also do it from a different P.
	mutex_t timersLock; 

	// Actions to take at some time. This is used to implement the
	// standard library's time package.
	// Must hold timersLock to access.
	timer_t **timers; 

	// Number of timers in P's heap.
	// Modified using atomic instructions.
	uint32_t num_timers; 

	// Number of timerDeleted timers in P's heap.
	// Modified using atomic instructions.
	uint32_t deleted_timers; 

	// Race context used while executing timer functions.
	uintptr_t timer_race_ctx; 

	// scannableStackSizeDelta accumulates the amount of stack space held by
	// live goroutines (i.e. those eligible for stack scanning).
	// Flushed to gcController.scannableStackSize once scannableStackSizeSlack
	// or -scannableStackSizeSlack is reached.
	int64_t scannable_stack_size_delta; 

	// preempt is set to indicate that this P should be enter the
	// scheduler ASAP (regardless of what G is running on it).
	uint8_t preempt; 

	// Padding is no longer needed. False sharing is now not a worry because p is large enough
	// that its size class is an integer multiple of the cache line size (for any of our architectures).
};

struct schedt {
	// accessed atomically. keep at top to ensure alignment on 32-bit systems.
	uint64_t goid_gen; 
	uint64_t last_poll;   // time of last network poll, 0 if currently polling
	uint64_t poll_until;  // time to which current poll is sleeping

	mutex_t lock; 

	// When increasing nmidle, nmidlelocked, nmsys, or nmfreed, be
	// sure to call checkdead().

	muintptr_t midle;         // idle m's waiting for work
	int32_t nmidle;           // number of idle m's waiting for work
	int32_t nmidlelocked;     // number of locked m's waiting for work
	int64_t mnext;            // number of m's that have been created and next M ID
	int32_t maxmcount;        // maximum number of m's allowed (or die)
	int32_t nmsys;            // number of system m's not counted for deadlock
	int64_t nmfreed;          // cumulative number of freed m's

	uint32_t ngsys;  // number of system goroutines; updated atomically

	puintptr_t pidle;       // idle p's
	uint32_t npidle;     
	uint32_t nmspinning;  // See "Worker thread parking/unparking" comment in proc.go.

	// Global runnable queue.
	gqueue_t runq;     
	int32_t runqsize; 

	// disable controls selective disabling of the scheduler.
	//
	// Use schedEnableUser to control this.
	//
	// disable is protected by sched.lock.
	struct {
		// user disables scheduling of user goroutines.
		uint8_t user;
		gqueue_t runnable;  // pending runnable Gs
		int32_t n;          // length of runnable
	} disable;

	// Global cache of dead G's.
	struct {
		mutex_t lock;   
		glist_t stack;    // Gs with stacks
		glist_t no_stack;  // Gs without stacks
		int32_t n;       
	} gFree;

	// Central cache of sudog structs.
	mutex_t sudoglock;
	sudog_t *sudogcache;

	// Central pool of available defer structs.
	mutex_t deferlock; 
	_defer_t *deferpool; 

	// freem is the list of m's waiting to be freed when their
	// m.exited is set. Linked through m.freelink.
	m_t *freem;

	uint32_t gcwaiting;   // gc is waiting to run
	int32_t stopwait;   
	note_t stopnote;   
	uint32_t sysmonwait; 
	note_t sysmonnote; 

	// safepointFn should be called on each P at the next GC
	// safepoint if p.runSafePointFn is set.
	void (*safe_point_fn)(p_t*);	   
	int32_t safe_point_wait; 
	note_t safe_point_note; 

	int32_t profilehz;  // cpu profiling rate

	int64_t procresizetime;  // nanotime() of last change to gomaxprocs
	int64_t totaltime;       // ∫gomaxprocs dt up to procresizetime

	// sysmonlock protects sysmon's actions on the runtime.
	//
	// Acquire and hold this mutex to block sysmon from interacting
	// with the rest of the runtime.
	mutex_t sysmonlock;

	// timeToRun is a distribution of scheduling latencies, defined
	// as the sum of time a G spends in the _Grunnable state before
	// it transitions to _Grunning.
	//
	// timeToRun is protected by sched.lock.
	time_histogram_t timeToRun;
};