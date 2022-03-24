

typedef int lock_rank_t;

enum {
	LOCK_RANK_Dummy = 0,

	// Locks held above sched
	LOCK_RANK_SYSMON,
	LOCK_RANK_SCAVENGE,
	LOCK_RANK_FORCE_GC,
	LOCK_RANK_SWEEP_WAITERS,
	LOCK_RANK_ASSIST_QUEUE,
	LOCK_RANK_CPU_PROF,
	LOCK_RANK_SWEEP,

	LOCK_RANK_POLL_DESC,
	LOCK_RANK_SCHED,
	LOCK_RANK_DEAD_LOCK,
	LOCK_RANK_ALL_G,
	LOCK_RANK_ALL_P,

	LOCK_RANK_TIMERS, // Multiple timers locked simultaneously in destroy()
	LOCK_RANK_ITAB,
	LOCK_RANK_REFLECT_OFFS,
	LOCK_RANK_HCHAN, // Multiple hchans acquired in lock order in syncadjustsudogs()
	LOCK_RANK_TRACE_BUF,
	LOCK_RANK_FIN,
	LOCK_RANK_NOTIFY_LIST,
	LOCK_RANK_TRACE_STRINGS,
	LOCK_RANK_MSPAN_SPECIAL,
	LOCK_RANK_PROF,
	LOCK_RANK_GC_BITS_ARENAS,
	LOCK_RANK_ROOT,
	LOCK_RANK_TRACE,
	LOCK_RANK_TRACE_STACK_TAB,
	LOCK_RANK_NETPOLL_INIT,

	LOCK_RANK_RW_MUTEX_W,
	LOCK_RANK_RW_MUTEX_R,

	LOCK_RANK_SPAN_SET_SPINE,
	LOCK_RANK_GSCAN,
	LOCK_RANK_STACK_POOL,
	LOCK_RANK_STACK_LARGE,
	LOCK_RANK_DEFER,
	LOCK_RANK_SUDOG,

	// Memory-related non-leaf locks
	LOCK_RANK_WBUF_SPANS,
	LOCK_RANK_MHEAP,
	LOCK_RANK_MHEAP_SPECIAL,

	// Memory-related leaf locks
	LOCK_RANK_GLOBAL_ALLOC,
	LOCK_RANK_PAGE_ALLOC_SCAV,

	// Other leaf locks
	LOCK_RANK_GFREE,
	// Generally, hchan must be acquired before gscan. But in one specific
	// case (in syncadjustsudogs from markroot after the g has been suspended
	// by suspendG), we allow gscan to be acquired, and then an hchan lock. To
	// allow this case, we get this lockRankHchanLeaf rank in
	// syncadjustsudogs(), rather than lockRankHchan. By using this special
	// rank, we don't allow any further locks to be acquired other than more
	// hchan locks.
	LOCK_RANK_HCHAN_LEAF,
	LOCK_RANK_PANIC,

	// Leaf locks with no dependencies, so these constants are not actually used anywhere.
	// There are other architecture-dependent leaf locks as well.
	LOCK_RANK_NEW_MHANDOFF,
	LOCK_RANK_DEBUG_PTR_MASK,
	LOCK_RANK_FAKETIME_STATE,
	LOCK_RANK_TICKS,
	LOCK_RANK_RACE_FINI,
	LOCK_RANK_POLL_CACHE,
	LOCK_RANK_DEBUG,
};
