package Muzent


// #######################################################################################
// Collections
// #######################################################################################

/*
A raw type-unspecific array slice.
*/
_RawArraySlice :: struct  {
	data: rawptr,
	count: i64,
}

// declare []b8

// declare []u8

// declare []u16

// declare []u32

// declare []u64

// declare []i8

// declare []i16

// declare []i32

// declare []i64

// declare []f32

// declare []f64

// declare []#error

/*
UTF-8 string type, with length info (not necessarily null-terminated).
*/
// string :: []u8

// declare []string

// #######################################################################################
// Environment
// #######################################################################################

/*
Defines the platforms supported by the library.
*/
_Platform :: enum u8 {
	Unknown = 0,
	Windows = 1,
	Linux = 2,
	OSX = 3,
	Android = 4,
	iOS = 5,
	PS5 = 6,
	XBoxSeries = 7,
	Switch = 8,
}

/*
Defines the architectures supported by the library.
*/
_Architecture :: enum u8 {
	Unknown = 0,
	X64 = 1,
	ARM64 = 2,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Get the current platform.
	*/
	GetPlatform :: proc "c" () -> _Platform ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Get the current architecture.
	*/
	GetArchitecture :: proc "c" () -> _Architecture ---
}

// #######################################################################################
// Runtime
// #######################################################################################

/*
Defines the source code location for debugging purposes.
*/
_SourceCodeLocation :: struct  {
	file: string,
	line: i32,
	column: i32,
	function: string,
}

// #######################################################################################
// Sync
// #######################################################################################

// Mutex ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
The most basic synchronization primitive.
*/
_Mutex :: struct #align(8)  {
	buffer: [64]u8,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a mutex.
	*/
	CreateMutex :: proc "c" () -> _Mutex ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys a mutex.
	*/
	DestroyMutex :: proc "c" (
		mutex: ^_Mutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Locks a mutex.
	*/
	LockMutex :: proc "c" (
		mutex: ^_Mutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Unlocks a mutex.
	*/
	UnlockMutex :: proc "c" (
		mutex: ^_Mutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Tries to lock a mutex.
	Returns true if the mutex was successfully locked, false otherwise.
	*/
	TryLockMutex :: proc "c" (
		mutex: ^_Mutex,
	) -> b8 ---
}

// Read-Write Mutex ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A read-write mutex.
This is a synchronization primitive that allows multiple readers or a single writer.
It is useful for scenarios where reads are more frequent than writes.
*/
_RWMutex :: struct #align(8)  {
	buffer: [200]u8,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a read-write mutex.
	*/
	CreateRWMutex :: proc "c" () -> _RWMutex ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys a read-write mutex.
	*/
	DestroyRWMutex :: proc "c" (
		rwmutex: ^_RWMutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Locks a read-write mutex for reading.
	Multiple threads can read simultaneously.
	*/
	LockRWMutexShared :: proc "c" (
		rwmutex: ^_RWMutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Locks a read-write mutex for writing.
	Only one thread can write at a time, and no other threads can read while writing.
	*/
	LockRWMutexExclusive :: proc "c" (
		rwmutex: ^_RWMutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Unlocks a read-write mutex after reading.
	Allows other threads to read or write.
	*/
	UnlockRWMutexShared :: proc "c" (
		rwmutex: ^_RWMutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Unlocks a read-write mutex after writing.
	Allows other threads to read or write.
	*/
	UnlockRWMutexExclusive :: proc "c" (
		rwmutex: ^_RWMutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Tries to lock a read-write mutex for reading.
	Returns true if the mutex was successfully locked for reading, false otherwise.
	*/
	TryLockRWMutexShared :: proc "c" (
		rwmutex: ^_RWMutex,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Tries to lock a read-write mutex for writing.
	Returns true if the mutex was successfully locked for writing, false otherwise.
	*/
	TryLockRWMutexExclusive :: proc "c" (
		rwmutex: ^_RWMutex,
	) -> b8 ---
}

// Semaphore ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A semaphore synchronization primitive.
It allows a certain number of threads to access a resource concurrently.
*/
_Semaphore :: struct #align(8)  {
	buffer: [32]u8,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a semaphore.
	The initial count specifies how many threads can access the resource concurrently.
	*/
	CreateSemaphore :: proc "c" (
		initialCount: i32,
	) -> _Semaphore ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys a semaphore.
	*/
	DestroySemaphore :: proc "c" (
		semaphore: ^_Semaphore,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits on a semaphore.
	The calling thread will block until the semaphore count is greater than zero.
	*/
	WaitSemaphore :: proc "c" (
		semaphore: ^_Semaphore,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits on a semaphore with a timeout.
	The calling thread will block until the semaphore count is greater than zero or the timeout expires.
	Returns true if the semaphore was acquired, false if the timeout expired.
	*/
	WaitSemaphoreTimeout :: proc "c" (
		semaphore: ^_Semaphore,
		timeoutNs: i32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Signals a semaphore, incrementing its count by a specified amount.
	If the count was zero, this will wake up one or more waiting threads.
	*/
	SignalSemaphore :: proc "c" (
		semaphore: ^_Semaphore,
		count: i32,
	) ---
}

// Condition Variable ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A condition variable for signaling between threads.
It allows threads to wait for a condition to be signaled.
*/
_ConditionVariable :: struct #align(8)  {
	buffer: [48]u8,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a condition variable.
	*/
	CreateConditionVariable :: proc "c" () -> _ConditionVariable ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys a condition variable.
	*/
	DestroyConditionVariable :: proc "c" (
		condvar: ^_ConditionVariable,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits on a condition variable.
	The calling thread will block until the condition variable is signaled.
	The mutex must be locked before calling this function.
	*/
	WaitConditionVariable :: proc "c" (
		condvar: ^_ConditionVariable,
		mutex: ^_Mutex,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits on a condition variable with a timeout.
	The calling thread will block until the condition variable is signaled or the timeout expires.
	The mutex must be locked before calling this function.
	Returns true if the condition variable was signaled, false if the timeout expired.
	*/
	WaitConditionVariableTimeout :: proc "c" (
		condvar: ^_ConditionVariable,
		mutex: ^_Mutex,
		timeoutNs: i32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Signals a condition variable, waking up one waiting thread.
	If no threads are waiting, this has no effect.
	*/
	SignalConditionVariable :: proc "c" (
		condvar: ^_ConditionVariable,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Signals a condition variable, waking up all waiting threads.
	If no threads are waiting, this has no effect.
	*/
	BroadcastConditionVariable :: proc "c" (
		condvar: ^_ConditionVariable,
	) ---
}

// Do Once ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A "do once" primitive.
It ensures that a specified initialization function is executed only once, even
if called from multiple threads.
This is useful for one-time initialization of shared resources.
*/
_DoOnce :: struct #align(8)  {
	buffer: [16]u8,
}

/*
The callback function type for the "do once" primitive.
*/
_DoOnceCallback :: #type proc "c" ()

@(link_prefix="PNSLR_")
foreign {
	/*
	Executing the specified callback function only once.
	If multiple threads call this function simultaneously, only one will execute.
	*/
	ExecuteDoOnce :: proc "c" (
		once: ^_DoOnce,
		callback: _DoOnceCallback,
	) ---
}

// Event ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
An event synchronization primitive.
It allows one or more threads to wait until another thread signals a condition.
*/
_Event :: struct #align(8)  {
	buffer: [112]u8,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates an event.
	If manualReset is true, the event must be manually reset after being signaled.
	If manualReset is false, the event automatically resets after releasing one waiting thread.
	*/
	CreateEvent :: proc "c" (
		manualReset: b8,
	) -> _Event ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys an event.
	*/
	DestroyEvent :: proc "c" (
		event: ^_Event,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits on an event.
	The calling thread will block until the event is signaled.
	*/
	WaitEvent :: proc "c" (
		event: ^_Event,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits on an event with a timeout.
	The calling thread will block until the event is signaled or the timeout expires.
	Returns true if the event was signaled, false if the timeout expired.
	*/
	WaitEventTimeout :: proc "c" (
		event: ^_Event,
		timeoutNs: i32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Signals an event.
	If manualReset is false, wakes up one waiting thread and resets automatically.
	If manualReset is true, wakes up all waiting threads and remains signaled until reset.
	*/
	SignalEvent :: proc "c" (
		event: ^_Event,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Resets an event, returning it to the unsignaled state.
	Only meaningful for manual-reset events.
	*/
	ResetEvent :: proc "c" (
		event: ^_Event,
	) ---
}

// #######################################################################################
// Memory
// #######################################################################################

@(link_prefix="PNSLR_")
foreign {
	/*
	Set a block of memory to a specific value.
	*/
	MemSet :: proc "c" (
		memory: rawptr,
		value: i32,
		size: i32,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Copy a block of memory from source to destination.
	*/
	MemCopy :: proc "c" (
		destination: rawptr,
		source: rawptr,
		size: i32,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Copy a block of memory from source to destination, handling overlapping regions.
	*/
	MemMove :: proc "c" (
		destination: rawptr,
		source: rawptr,
		size: i32,
	) ---
}

// #######################################################################################
// Allocators
// #######################################################################################

// Allocator Declaration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Defines the mode to be used when calling the allocator function.
*/
_AllocatorMode :: enum u8 {
	Allocate = 0,
	Resize = 1,
	Free = 2,
	FreeAll = 3,
	AllocateNoZero = 4,
	ResizeNoZero = 5,
	QueryCapabilities = 255,
}

/*
Defines the capabilities of an allocator.
*/
_AllocatorCapability :: distinct bit_set[_AllocatorCapabilityValues; u64]

_AllocatorCapabilityValues :: enum u8 {
	ThreadSafe = 0,
	Resize = 1,
	Free = 2,
	FreeAll = 3,
	HintNil = 26,
	HintBump = 27,
	HintHeap = 28,
	HintTemp = 29,
	HintDebug = 30,
}

/*
Defines the error codes that can be returned by the allocator.
*/
_AllocatorError :: enum u8 {
	None = 0,
	OutOfMemory = 1,
	InvalidAlignment = 2,
	InvalidSize = 3,
	InvalidMode = 4,
	Internal = 5,
	OutOfOrderFree = 6,
	DoubleFree = 7,
	CantFreeAll = 8,
}

/*
Defines the delegate type for the allocator function.
*/
_AllocatorProcedure :: #type proc "c" (
	allocatorData: rawptr,
	mode: _AllocatorMode,
	size: i32,
	alignment: i32,
	oldMemory: rawptr,
	oldSize: i32,
	location: _SourceCodeLocation,
	error: ^_AllocatorError,
) -> rawptr

/*
Defines a generic allocator structure that can be used to allocate, resize, and free memory.
*/
_Allocator :: struct  {
	procedure: _AllocatorProcedure,
	data: rawptr,
}

// declare []_Allocator

// Allocation ease-of-use functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Allocate memory using the provided allocator.
	*/
	Allocate :: proc "c" (
		allocator: _Allocator,
		zeroed: b8,
		size: i32,
		alignment: i32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> rawptr ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Resize memory using the provided allocator.
	*/
	Resize :: proc "c" (
		allocator: _Allocator,
		zeroed: b8,
		oldMemory: rawptr,
		oldSize: i32,
		newSize: i32,
		alignment: i32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> rawptr ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Fallback resize function that can be used when the allocator does not support resizing.
	*/
	DefaultResize :: proc "c" (
		allocator: _Allocator,
		zeroed: b8,
		oldMemory: rawptr,
		oldSize: i32,
		newSize: i32,
		alignment: i32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> rawptr ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Free memory using the provided allocator.
	*/
	Free :: proc "c" (
		allocator: _Allocator,
		memory: rawptr,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Free all memory allocated by the provided allocator.
	*/
	FreeAll :: proc "c" (
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Query the capabilities of the provided allocator.
	*/
	QueryAllocatorCapabilities :: proc "c" (
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> u64 ---
}

// Nil allocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Get the 'nil' allocator. Reports 'out of memory' when requesting memory.
	Otherwise no-ops all around.
	*/
	GetAllocator_Nil :: proc "c" () -> _Allocator ---
}

// Default Heap Allocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Get the default heap allocator.
	*/
	GetAllocator_DefaultHeap :: proc "c" () -> _Allocator ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Main allocator function for the default heap allocator.
	*/
	AllocatorFn_DefaultHeap :: proc "c" (
		allocatorData: rawptr,
		mode: _AllocatorMode,
		size: i32,
		alignment: i32,
		oldMemory: rawptr,
		oldSize: i32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError,
	) -> rawptr ---
}

// Arena Alloator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A block of memory used by the arena allocator.
*/
_ArenaAllocatorBlock :: struct  {
	previous: ^_ArenaAllocatorBlock,
	allocator: _Allocator,
	memory: rawptr,
	capacity: u32,
	used: u32,
}

/*
The payload used by the arena allocator.
*/
_ArenaAllocatorPayload :: struct  {
	backingAllocator: _Allocator,
	currentBlock: ^_ArenaAllocatorBlock,
	totalUsed: u32,
	totalCapacity: u32,
	minimumBlockSize: u32,
	numSnapshots: u32,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Create a new arena allocator with the specified backing allocator.
	The arena allocator will use the backing allocator to allocate its blocks.
	The arena allocator will not free the backing allocator, so it is the caller's responsibility to
	free the backing allocator when it is no longer needed.
	*/
	NewAllocator_Arena :: proc "c" (
		backingAllocator: _Allocator,
		pageSize: u32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> _Allocator ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroy an arena allocator and free all its resources.
	This does not free the backing allocator, only the arena allocator's own resources.
	*/
	DestroyAllocator_Arena :: proc "c" (
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Main allocator function for the arena allocator.
	*/
	AllocatorFn_Arena :: proc "c" (
		allocatorData: rawptr,
		mode: _AllocatorMode,
		size: i32,
		alignment: i32,
		oldMemory: rawptr,
		oldSize: i32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError,
	) -> rawptr ---
}

_ArenaSnapshotError :: enum u8 {
	None = 0,
	InvalidData = 1,
	MemoryBlockNotOwned = 2,
	OutOfOrderRestoreUsage = 3,
	DoubleRestoreOrDiscardUsage = 4,
}

/*
A snapshot of the arena allocator, recording its state at a specific point in time.
Can be saved/loaded/discarded as needed.
*/
_ArenaAllocatorSnapshot :: struct  {
	valid: b8,
	payload: ^_ArenaAllocatorPayload,
	block: ^_ArenaAllocatorBlock,
	used: u32,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Ensures that the arena allocator has either restored/discarded all the
	snapshots that were taken.
	*/
	ValidateArenaAllocatorSnapshotState :: proc "c" (
		allocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Captures a snapshot of the arena allocator.
	The returned value can be used to load back the existing state at this point.
	*/
	CaptureArenaAllocatorSnapshot :: proc "c" (
		allocator: _Allocator,
	) -> _ArenaAllocatorSnapshot ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Restores the state of the arena allocator from a snapshot.
	Upon success, the snapshot is marked as invalid.
	*/
	RestoreArenaAllocatorSnapshot :: proc "c" (
		snapshot: ^_ArenaAllocatorSnapshot,
		loc: _SourceCodeLocation,
	) -> _ArenaSnapshotError ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Discards a snapshot of the arena allocator.
	*/
	DiscardArenaAllocatorSnapshot :: proc "c" (
		snapshot: ^_ArenaAllocatorSnapshot,
	) -> _ArenaSnapshotError ---
}

// Stack Allocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A page of a stack allocator.
*/
_StackAllocatorPage :: struct #align(8)  {
	previousPage: ^_StackAllocatorPage,
	usedBytes: u64,
	buffer: [8192]u8,
}

/*
The header used for every separate stack allocation.
This is used to store metadata about the allocation and deallocate appropriately.
*/
_StackAllocationHeader :: struct  {
	page: ^_StackAllocatorPage,
	size: i32,
	alignment: i32,
	lastAllocation: rawptr,
	lastAllocationHeader: rawptr,
}

/*
The payload used by the stack allocator.
*/
_StackAllocatorPayload :: struct  {
	backingAllocator: _Allocator,
	currentPage: ^_StackAllocatorPage,
	lastAllocation: rawptr,
	lastAllocationHeader: ^_StackAllocationHeader,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Create a stack allocator with the specified backing allocator.
	The stack allocator will use the backing allocator to allocate its pages.
	The stack allocator will not free the backing allocator, so it is the caller's responsibility to
	free the backing allocator when it is no longer needed.
	*/
	NewAllocator_Stack :: proc "c" (
		backingAllocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> _Allocator ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroy a stack allocator and free all its resources.
	This does not free the backing allocator, only the stack allocator's own resources.
	*/
	DestroyAllocator_Stack :: proc "c" (
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Main allocator function for the stack allocator.
	*/
	AllocatorFn_Stack :: proc "c" (
		allocatorData: rawptr,
		mode: _AllocatorMode,
		size: i32,
		alignment: i32,
		oldMemory: rawptr,
		oldSize: i32,
		location: _SourceCodeLocation,
		error: ^_AllocatorError,
	) -> rawptr ---
}

// Collections make/free functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Allocate a raw array slice of 'count' elements, each of size 'tySize' and alignment 'tyAlign', using the provided allocator. Optionally zeroed.
	*/
	MakeRawSlice :: proc "c" (
		tySize: i32,
		tyAlign: i32,
		count: i64,
		zeroed: b8,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> _RawArraySlice ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Free a raw array slice allocated with `PNSLR_MakeRawSlice`, using the provided allocator.
	*/
	FreeRawSlice :: proc "c" (
		slice: ^_RawArraySlice,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Resize a raw array slice to one with 'newCount' elements, each of size 'tySize' and alignment 'tyAlign', using the provided allocator. Optionally zeroed.
	*/
	ResizeRawSlice :: proc "c" (
		slice: ^_RawArraySlice,
		tySize: i32,
		tyAlign: i32,
		newCount: i64,
		zeroed: b8,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Allocate a UTF-8 string of 'count__' characters using the provided allocator. Optionally zeroed.
	*/
	MakeString :: proc "c" (
		count: i64,
		zeroed: b8,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Free a UTF-8 string allocated with `PNSLR_MakeString`, using the provided allocator.
	*/
	FreeString :: proc "c" (
		str: string,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Allocate a C-style null-terminated string of 'count__' characters (excluding the null terminator) using the provided allocator. Optionally zeroed.
	*/
	MakeCString :: proc "c" (
		count: i64,
		zeroed: b8,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) -> cstring ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Free a C-style null-terminated string allocated with `PNSLR_MakeCString`, using the provided allocator.
	*/
	FreeCString :: proc "c" (
		str: cstring,
		allocator: _Allocator,
		location: _SourceCodeLocation,
		error: ^_AllocatorError = { },
	) ---
}

// #######################################################################################
// Chrono
// #######################################################################################

@(link_prefix="PNSLR_")
foreign {
	/*
	Returns the current time in nanoseconds since the Unix epoch (January 1, 1970).
	*/
	NanosecondsSinceUnixEpoch :: proc "c" () -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Breaks down the given nanoseconds since the Unix epoch into its
	date and time components.
	*/
	ConvertNanosecondsSinceUnixEpochToDateTime :: proc "c" (
		ns: i64,
		outYear: ^i16,
		outMonth: ^u8,
		outDay: ^u8,
		outHour: ^u8,
		outMinute: ^u8,
		outSecond: ^u8,
	) -> b8 ---
}

// #######################################################################################
// Strings
// #######################################################################################

// Basics/Conversions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Returns the length of the given C-style null-terminated string, excluding the null terminator.
	*/
	GetCStringLength :: proc "c" (
		str: cstring,
	) -> i32 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Clone a C-style string into a new allocated string.
	*/
	StringFromCString :: proc "c" (
		str: cstring,
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Clones a UTF-8 string to a C-style null-terminated string.
	The returned string is allocated using the specified allocator.
	*/
	CStringFromString :: proc "c" (
		str: string,
		allocator: _Allocator,
	) -> cstring ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Clones a UTF-8 string to a new allocated UTF-8 string.
	The returned string is allocated using the specified allocator.
	*/
	CloneString :: proc "c" (
		str: string,
		allocator: _Allocator,
	) -> string ---
}

// Basic/Manipulation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Concatenates two UTF-8 strings into a new allocated string.
	The returned string is allocated using the specified allocator.
	*/
	ConcatenateStrings :: proc "c" (
		str1: string,
		str2: string,
		allocator: _Allocator,
	) -> string ---
}

// Casing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Converts a UTF-8 string to uppercase.
	The returned string is allocated using the specified allocator.
	*/
	UpperString :: proc "c" (
		str: string,
		allocator: _Allocator,
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Converts a UTF-8 string to lowercase.
	The returned string is allocated using the specified allocator.
	*/
	LowerString :: proc "c" (
		str: string,
		allocator: _Allocator,
	) -> string ---
}

// Comparisons ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Represents the type of string comparison to perform.
*/
_StringComparisonType :: enum u8 {
	CaseSensitive = 0,
	CaseInsensitive = 1,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if two UTF-8 strings contain the same data.
	Returns true if they are equal, false otherwise.
	*/
	AreStringsEqual :: proc "c" (
		str1: string,
		str2: string,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Asymmetric equality-check between a UTF-8 string and a C-style null-terminated string.
	Returns true if they are equal, false otherwise.
	*/
	AreStringAndCStringEqual :: proc "c" (
		str1: string,
		str2: cstring,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if two C-style null-terminated strings are equal.
	Returns true if they are equal, false otherwise.
	*/
	AreCStringsEqual :: proc "c" (
		str1: cstring,
		str2: cstring,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a UTF-8 string starts with the specified prefix.
	Returns true if it does, false otherwise.
	*/
	StringStartsWith :: proc "c" (
		str: string,
		prefix: string,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a UTF-8 string ends with the specified suffix.
	Returns true if it does, false otherwise.
	*/
	StringEndsWith :: proc "c" (
		str: string,
		suffix: string,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a C-style null-terminated string starts with the specified prefix.
	*/
	StringStartsWithCString :: proc "c" (
		str: string,
		prefix: cstring,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a C-style null-terminated string ends with the specified suffix.
	*/
	StringEndsWithCString :: proc "c" (
		str: string,
		suffix: cstring,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Returns the length of the given C-style null-terminated string, excluding the null terminator.
	*/
	CStringStartsWith :: proc "c" (
		str: cstring,
		prefix: string,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a C-style null-terminated string ends with the specified UTF-8 suffix.
	*/
	CStringEndsWith :: proc "c" (
		str: cstring,
		suffix: string,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a C-style null-terminated string starts with the specified UTF-8 prefix.
	*/
	CStringStartsWithCString :: proc "c" (
		str: string,
		prefix: cstring,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a C-style null-terminated string ends with the specified UTF-8 suffix.
	*/
	CStringEndsWithCString :: proc "c" (
		str: string,
		suffix: cstring,
		comparisonType: _StringComparisonType = { },
	) -> b8 ---
}

// Advanced comparisons ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Searches for the first occurrence of a substring within a string.
	Returns the index of the first occurrence, or -1 if not found.
	*/
	SearchFirstIndexInString :: proc "c" (
		str: string,
		substring: string,
		comparisonType: _StringComparisonType = { },
	) -> i32 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Searches for the last occurrence of a substring within a string.
	Returns the index of the last occurrence, or -1 if not found.
	*/
	SearchLastIndexInString :: proc "c" (
		str: string,
		substring: string,
		comparisonType: _StringComparisonType = { },
	) -> i32 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Replaces all occurrences of a substring within a string with a new value.
	The returned string is allocated using the specified allocator.
	*/
	ReplaceInString :: proc "c" (
		str: string,
		oldValue: string,
		newValue: string,
		allocator: _Allocator,
		comparisonType: _StringComparisonType = { },
	) -> string ---
}

// UTF-8 functionalities ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Result structure for UTF-8 rune encoding.
Contains the encoded bytes and the number of bytes used.
*/
_EncodedRune :: struct  {
	data: [4]u8,
	length: i32,
}

/*
Result structure for UTF-8 rune decoding.
Contains the decoded rune and the number of bytes consumed.
*/
_DecodedRune :: struct  {
	rune: u32,
	length: i32,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Returns the number of bytes required to encode the given rune in UTF-8.
	*/
	GetRuneLength :: proc "c" (
		r: u32,
	) -> i32 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Encodes a rune into UTF-8 byte sequence and returns the structure containing encoded bytes/length.
	Invalid runes or surrogates are replaced with the error rune (U+FFFD).
	*/
	EncodeRune :: proc "c" (
		c: u32,
	) -> _EncodedRune ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Decodes a UTF-8 byte sequence into a rune and returns the structure containing the rune/length.
	Returns error rune (U+FFFD) for invalid sequences.
	*/
	DecodeRune :: proc "c" (
		s: []u8,
	) -> _DecodedRune ---
}

// Windows-specific bs for UTF-16 conversions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Converts a UTF-8 string to a UTF-16 string.
	The returned string is allocated using the specified allocator.
	Only available on Windows. Bad decision to use UTF-16 on Windows, but it's a legacy thing.
	*/
	UTF16FromUTF8WindowsOnly :: proc "c" (
		str: string,
		allocator: _Allocator,
	) -> []u16 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Converts a UTF-16 string to a UTF-8 string.
	The returned string is allocated using the specified allocator.
	Only available on Windows. Bad decision to use UTF-16 on Windows, but it's a legacy thing.
	*/
	UTF8FromUTF16WindowsOnly :: proc "c" (
		utf16str: []u16,
		allocator: _Allocator,
	) -> string ---
}

// String Builder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
A basic string builder. Can accept strings and characters,
and build a single string from them.
 *
Create by setting the allocator and zeroing the rest of the fields.
*/
_StringBuilder :: struct  {
	allocator: _Allocator,
	buffer: []u8,
	writtenSize: i64,
	cursorPos: i64,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Ensure that the string builder has enough space to accommodate additionalSize bytes.
	*/
	ReserveSpaceInStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		additionalSize: i64,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a single byte to the string builder. Could be an ANSI/ASCII character,
	or not. The function does not check for validity.
	*/
	AppendByteToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		byte: u8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a UTF-8 string to the string builder.
	*/
	AppendStringToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		str: string,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a C-style null-terminated string to the string builder.
	*/
	AppendCStringToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		str: cstring,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a single character (rune) to the string builder.
	*/
	AppendRuneToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		rune: u32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append an 8-bit boolean value to the string builder.
	*/
	AppendB8ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: b8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a 32-bit floating-point number to the string builder.
	*/
	AppendF32ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: f32,
		decimalPlaces: i32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a 64-bit floating point number to the string builder.
	*/
	AppendF64ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: f64,
		decimalPlaces: i32,
	) -> b8 ---
}

/*
The base to use when appending integer numbers to the string builder.
*/
_IntegerBase :: enum u8 {
	Decimal = 0,
	Binary = 1,
	HexaDecimal = 2,
	Octal = 3,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append an unsigned 8-bit integer to the string builder.
	*/
	AppendU8ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: u8,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append an unsigned 16-bit integer to the string builder.
	*/
	AppendU16ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: u16,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append an unsigned 32-bit integer number to the string builder.
	*/
	AppendU32ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: u32,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append an unsigned 64-bit integer to the string builder.
	*/
	AppendU64ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: u64,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a signed 8-bit integer to the string builder.
	*/
	AppendI8ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: i8,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a signed 16-bit integer to the string builder.
	*/
	AppendI16ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: i16,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a signed 32-bit integer number to the string builder.
	*/
	AppendI32ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: i32,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Append a signed 64-bit integer to the string builder.
	*/
	AppendI64ToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		value: i64,
		base: _IntegerBase = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Return the string from the string builder.
	*/
	StringFromStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Reset the string builder, clearing its contents but keeping the allocated buffer.
	*/
	ResetStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Free the resources used by the string builder.
	*/
	FreeStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
	) ---
}

// String Formatting ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
The possible primitive types that can be formatted.
*/
_PrimitiveFmtType :: enum u8 {
	B8 = 0,
	F32 = 1,
	F64 = 2,
	U8 = 3,
	U16 = 4,
	U32 = 5,
	U64 = 6,
	I8 = 7,
	I16 = 8,
	I32 = 9,
	I64 = 10,
	Rune = 11,
	CString = 12,
	String = 13,
}

/*
The internal encoding of a type-unspecific format specifier.
For booleans, valueBufferA is 0 or 1.
For floats, valueBufferA is the float value (reinterpret as relevant),
    and valueBufferB is the number of decimal places (cast to i32).
For integers, valueBufferA is the integer value (reinterpret as relevant),
    and the first half of valueBufferB is the base (cast to PNSLR_IntegerBase).
For runes, valueBufferA is the rune value (reinterpret as u32).
For C-style strings, valueBufferA is the pointer to the string.
For UTF-8 strings, valueBufferA is the pointer to the string,
    and valueBufferB is the length (reinterpret as i64).
*/
_PrimitiveFmtOptions :: struct  {
	type: _PrimitiveFmtType,
	valueBufferA: u64,
	valueBufferB: u64,
}

// declare []_PrimitiveFmtOptions

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtB8 :: proc "c" (
		value: b8,
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtF32 :: proc "c" (
		value: f32,
		decimalPlaces: i32,
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtF64 :: proc "c" (
		value: f64,
		decimalPlaces: i32,
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtU8 :: proc "c" (
		value: u8,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtU16 :: proc "c" (
		value: u16,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtU32 :: proc "c" (
		value: u32,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtU64 :: proc "c" (
		value: u64,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtI8 :: proc "c" (
		value: i8,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtI16 :: proc "c" (
		value: i16,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtI32 :: proc "c" (
		value: i32,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtI64 :: proc "c" (
		value: i64,
		base: _IntegerBase = { },
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtRune :: proc "c" (
		value: u32,
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtCString :: proc "c" (
		value: cstring,
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Use when formatting a string. Pass as one of the varargs.
	*/
	FmtString :: proc "c" (
		value: string,
	) -> _PrimitiveFmtOptions ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Format a string with the given format and arguments, appending the result
	to the string builder.
	*/
	FormatAndAppendToStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
		fmtStr: string,
		args: []_PrimitiveFmtOptions,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Format a string with the given format and arguments, returning the result
	as a new allocated string using the specified allocator.
	*/
	FormatString :: proc "c" (
		fmtStr: string,
		args: []_PrimitiveFmtOptions,
		allocator: _Allocator,
	) -> string ---
}

// Conversions to strings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a boolean value to a string ("true" or "false").
	*/
	StringFromBoolean :: proc "c" (
		value: b8,
		allocator: _Allocator,
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a 32-bit floating-point number to a string with specified decimal places.
	*/
	StringFromF32 :: proc "c" (
		value: f32,
		allocator: _Allocator,
		decimalPlaces: i32 = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a 64-bit floating-point number to a string with specified decimal places.
	*/
	StringFromF64 :: proc "c" (
		value: f64,
		allocator: _Allocator,
		decimalPlaces: i32 = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert an unsigned 8-bit integer to a string in the specified base.
	*/
	StringFromU8 :: proc "c" (
		value: u8,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert an unsigned 16-bit integer to a string in the specified base.
	*/
	StringFromU16 :: proc "c" (
		value: u16,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert an unsigned 32-bit integer to a string in the specified base.
	*/
	StringFromU32 :: proc "c" (
		value: u32,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert an unsigned 64-bit integer to a string in the specified base.
	*/
	StringFromU64 :: proc "c" (
		value: u64,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a signed 8-bit integer to a string in the specified base.
	*/
	StringFromI8 :: proc "c" (
		value: i8,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a signed 16-bit integer to a string in the specified base.
	*/
	StringFromI16 :: proc "c" (
		value: i16,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a signed 32-bit integer to a string in the specified base.
	*/
	StringFromI32 :: proc "c" (
		value: i32,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a signed 64-bit integer to a string in the specified base.
	*/
	StringFromI64 :: proc "c" (
		value: i64,
		allocator: _Allocator,
		base: _IntegerBase = { },
	) -> string ---
}

// Conversions from strings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a validstring (case-insensitive "true" or "false", or "1" or "0") to a boolean.
	*/
	BooleanFromString :: proc "c" (
		str: string,
		value: ^b8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers-only, with zero or one decimal points,
	optional -/+ sign at the start) to a 32-bit floating-point number.
	*/
	F32FromString :: proc "c" (
		str: string,
		value: ^f32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers-only, with zero or one decimal points,
	optional -/+ sign at the start) to a 64-bit floating-point number.
	*/
	F64FromString :: proc "c" (
		str: string,
		value: ^f64,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optionally
	starting with 0b/0o/0x prefix for alternate bases) to an unsigned 8-bit integer.
	Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
	By default (no prefix), decimal base is assumed.
	*/
	U8FromString :: proc "c" (
		str: string,
		value: ^u8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optionally
	starting with 0b/0o/0x prefix for alternate bases) to an unsigned 16-bit integer.
	Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
	By default (no prefix), decimal base is assumed.
	*/
	U16FromString :: proc "c" (
		str: string,
		value: ^u16,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optionally
	starting with 0b/0o/0x prefix for alternate bases) to an unsigned 32-bit integer.
	Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
	By default (no prefix), decimal base is assumed.
	*/
	U32FromString :: proc "c" (
		str: string,
		value: ^u32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optionally
	starting with 0b/0o/0x prefix for alternate bases) to an unsigned 64-bit integer.
	Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
	By default (no prefix), decimal base is assumed.
	*/
	U64FromString :: proc "c" (
		str: string,
		value: ^u64,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
	at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
	a signed 8-bit integer. Will be assumed to be hexadecimal if it contains A-F
	characters but no prefix. By default (no prefix), decimal base is assumed.
	*/
	I8FromString :: proc "c" (
		str: string,
		value: ^i8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
	at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
	a signed 16-bit integer. Will be assumed to be hexadecimal if it contains A-F
	characters but no prefix. By default (no prefix), decimal base is assumed.
	*/
	I16FromString :: proc "c" (
		str: string,
		value: ^i16,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
	at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
	a signed 32-bit integer. Will be assumed to be hexadecimal if it contains A-F
	characters but no prefix. By default (no prefix), decimal base is assumed.
	*/
	I32FromString :: proc "c" (
		str: string,
		value: ^i32,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
	at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
	a signed 64-bit integer. Will be assumed to be hexadecimal if it contains A-F
	characters but no prefix. By default (no prefix), decimal base is assumed.
	*/
	I64FromString :: proc "c" (
		str: string,
		value: ^i64,
	) -> b8 ---
}

// #######################################################################################
// IO
// #######################################################################################

/*
Represents a normalised path.
This is used to ensure that paths are in a consistent format across different platforms.
It is a simple wrapper around a UTF-8 string.
It's always an absolute path, and the path separators are always forward slashes ('/').
For directories, it always ends with a trailing slash.
*/
_Path :: struct  {
	path: string,
}

/*
Represents the type of path normalisation to perform.
*/
_PathNormalisationType :: enum u8 {
	File = 0,
	Directory = 1,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Normalises a path to a consistent format, as specified in `PNSLR_PathNormalisationType`.
	*/
	NormalisePath :: proc "c" (
		path: string,
		type: _PathNormalisationType,
		allocator: _Allocator,
	) -> _Path ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Split a path into its components. Assumes that the path is a valid normalised path in Panshilar conventions.
	*/
	SplitPath :: proc "c" (
		path: _Path,
		parent: ^_Path,
		selfNameWithExtension: ^string = { },
		selfName: ^string = { },
		extension: ^string = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Returns a normalised path for a file inside a given directory.
	*/
	GetPathForChildFile :: proc "c" (
		dir: _Path,
		fileNameWithExtension: string,
		allocator: _Allocator,
	) -> _Path ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Returns a normalised path for a subdirectory inside a given directory.
	*/
	GetPathForSubdirectory :: proc "c" (
		dir: _Path,
		dirName: string,
		allocator: _Allocator,
	) -> _Path ---
}

/*
The signature of the delegate that's supposed to be called for iterating over a directory.
*/
_DirectoryIterationVisitorDelegate :: #type proc "c" (
	payload: rawptr,
	path: _Path,
	isDirectory: b8,
	exploreCurrentDirectory: ^b8,
) -> b8

@(link_prefix="PNSLR_")
foreign {
	/*
	Iterates over a directory and calls the visitor function for each file/directory found.
	If `recursive` is true, it will also iterate over subdirectories.
	*/
	IterateDirectory :: proc "c" (
		path: _Path,
		recursive: b8,
		visitorPayload: rawptr,
		visitorFunc: _DirectoryIterationVisitorDelegate,
	) ---
}

/*
Represents the type of path check to perform when checking if a path exists.
*/
_PathExistsCheckType :: enum u8 {
	Either = 0,
	File = 1,
	Directory = 2,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if a file/directory exists at the specified path.
	*/
	PathExists :: proc "c" (
		path: _Path,
		type: _PathExistsCheckType,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Deletes a file/directory at a path, if it exists.
	*/
	DeletePath :: proc "c" (
		path: _Path,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Get the timestamp of a file at the specified path as nanoseconds since unix epoch.
	*/
	GetFileTimestamp :: proc "c" (
		path: _Path,
	) -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the size of a file at the specified path in bytes.
	*/
	GetFileSize :: proc "c" (
		path: _Path,
	) -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a directory tree, if it doesn't exist.
	Note that if the path doesn't have a trailing slash, it'll assume it's a file.
	So, the last component of the path (if is a directory) will not be created.
	*/
	CreateDirectoryTree :: proc "c" (
		path: _Path,
	) -> b8 ---
}

/*
Represents an opened file.
*/
_File :: struct  {
	handle: rawptr,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Opens a file for reading, optionally allow writing to the stream.
	If the file does not exist, this function will fail.
	*/
	OpenFileToRead :: proc "c" (
		path: _Path,
		allowWrite: b8 = { },
	) -> _File ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Opens a file for writing (or appending), optionally allow reading from the stream.
	If the file does not exist, it will be created.
	*/
	OpenFileToWrite :: proc "c" (
		path: _Path,
		append: b8 = { },
		allowRead: b8 = { },
	) -> _File ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the size of an opened file.
	Returns 0 on error.
	*/
	GetSizeOfFile :: proc "c" (
		handle: _File,
	) -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the current position in an opened file.
	Returns -1 on error.
	*/
	GetCurrentPositionInFile :: proc "c" (
		handle: _File,
	) -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Seeks to a specific position in an opened file.
	If not relative, it's absolute from the start.
	Returns true on success, false on failure.
	*/
	SeekPositionInFile :: proc "c" (
		handle: _File,
		newPos: i64,
		relative: b8 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Reads data from an opened file at the current position.
	Optionally stores the number of bytes read.
	Returns true on success, false on failure.
	*/
	ReadFromFile :: proc "c" (
		handle: _File,
		dst: []u8,
		readSize: ^i64 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Writes data to an opened file at the current position.
	Returns true on success, false on failure.
	*/
	WriteToFile :: proc "c" (
		handle: _File,
		src: []u8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Formats a string with the given format and arguments, writing the
	result to the file.
	Returns true on success, false on failure.
	*/
	FormatAndWriteToFile :: proc "c" (
		handle: _File,
		fmtStr: string,
		args: []_PrimitiveFmtOptions,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Truncates an opened file to a specific size.
	Returns true on success, false on failure.
	*/
	TruncateFile :: proc "c" (
		handle: _File,
		newSize: i64,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Flushes any buffered data to the file.
	Returns true on success, false on failure.
	*/
	FlushFile :: proc "c" (
		handle: _File,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Closes an opened file.
	*/
	CloseFileHandle :: proc "c" (
		handle: _File,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Reads a file fully end-to-end and stores in a buffer. Won't work if dst is nil.
	Provided allocator is used for creating the buffer.
	Returns true on success, false on failure.
	*/
	ReadAllContentsFromFile :: proc "c" (
		path: _Path,
		dst: ^[]u8,
		allocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Dump a bunch of data into a file. Optionally append it instead of overwriting.
	Returns true on success, false on failure.
	*/
	WriteAllContentsToFile :: proc "c" (
		path: _Path,
		src: []u8,
		append: b8 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Copies a file from src to dst. If dst exists, it will be overwritten.
	Returns true on success, false on failure.
	*/
	CopyFile :: proc "c" (
		src: _Path,
		dst: _Path,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Moves a file from src to dst. If dst exists, it will be overwritten.
	Returns true on success, false on failure.
	*/
	MoveFile :: proc "c" (
		src: _Path,
		dst: _Path,
	) -> b8 ---
}

// #######################################################################################
// Process
// #######################################################################################

@(link_prefix="PNSLR_")
foreign {
	/*
	Exits the current process immediately with the specified exit code.
	*/
	ExitProcess :: proc "c" (
		exitCode: i32,
	) ---
}

/*
A key-value pair representing an environment variable.
The `kvp` field contains the full "KEY=VALUE" string.
The 'key' field contains the key part.
The 'value' field contains the value part.
*/
_EnvVarKeyValuePair :: struct  {
	kvp: string,
	key: string,
	value: string,
}

// declare []_EnvVarKeyValuePair

@(link_prefix="PNSLR_")
foreign {
	/*
	Retrieves all environment variables as an array slice of key-value pairs.
	The returned array slice is allocated using the provided allocator.
	The individual strings within the key-value pairs are also allocated using the same allocator.
	For the key-value pairs, the `kvp` field contains the full "KEY=VALUE" string,
	while the `key` and `value` fields are just 'views' into that string.
	*/
	GetEnvironmentVariables :: proc "c" (
		allocator: _Allocator,
	) -> []_EnvVarKeyValuePair ---
}

/*
Opaque handle to a pipe.
On Windows, this is a HANDLE.
On Unix-like systems, this is a file descriptor (int).
This is used for inter-process communication (IPC).
*/
_PipeHandle :: struct  {
	platformHandle: u64,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a pipe and returns the read and write ends.
	The read end is used for reading data from the pipe.
	The write end is used for writing data to the pipe.
	*/
	CreatePipe :: proc "c" (
		outR: ^_PipeHandle,
		outW: ^_PipeHandle,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the number of bytes available to read from the read end of the pipe.
	The size is stored in `outSize`.
	 *
	Note that this function does not block, and the size may change after
	this function returns.
	*/
	GetRemainingPipeReadSize :: proc "c" (
		pipe: _PipeHandle,
		outSize: ^i64,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Reads data from the read end of the pipe into the provided buffer.
	The number of bytes read is stored in `readSize` if it's not null.
	Returns true on success, false on failure.
	*/
	ReadFromPipe :: proc "c" (
		pipe: _PipeHandle,
		dst: []u8,
		readSize: ^i64 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Writes data to the write end of the pipe from the provided buffer.
	Returns true on success, false on failure.
	*/
	WriteToPipe :: proc "c" (
		pipe: _PipeHandle,
		src: []u8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Closes the pipe handle, releasing any associated resources.
	After calling this function, the pipe handle should not be used again.
	*/
	ClosePipe :: proc "c" (
		pipe: _PipeHandle,
	) -> b8 ---
}

/*
A handle to a process.
The `pid` field is the process ID.
On Windows, this is `dwProcessId`.
On Unix-like systems, this is the PID.
The `handle` field is a platform-specific handle to the process.
On Windows, this is a HANDLE.
On Unix-like systems, this is pidfd.
*/
_ProcessHandle :: struct  {
	pid: i64,
	handle: u64,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Starts a new process with the specified executable and arguments.
	Optionally, environment variables, working directory, and pipes for
	standard output and error can be provided.
	 *
	If not provided, environment variables and working directory are inherited
	from the current process. If provided, they must be in a 'KEY=VALUE' format.
	 *
	The pipe handles provided must be read ends for stdout and stderr respectively.
	If null, the respective output is discarded.
	*/
	RunProcess :: proc "c" (
		outProcessHandle: ^_ProcessHandle,
		execAndArgs: []string,
		environmentVariables: []string = { },
		workingDirectory: _Path = { },
		stdOutPipe: ^_PipeHandle = { },
		stdErrPipe: ^_PipeHandle = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Waits for the given process to exit and retrieves its exit code.
	 *
	Returns true if the process exited cleanly or false on failure.
	The exit code is stored in *outExitCode if provided.
	*/
	WaitForProcess :: proc "c" (
		process: ^_ProcessHandle,
		outExitCode: ^i32 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Kills the given process immediately.
	 *
	Returns true if the signal/termination request succeeded.
	*/
	KillProcess :: proc "c" (
		process: ^_ProcessHandle,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Closes the handle associated with the process.
	Does not terminate or wait for the process.
	*/
	CloseProcess :: proc "c" (
		process: ^_ProcessHandle,
	) ---
}

// #######################################################################################
// Network
// #######################################################################################

/*
Represents an IP address in binary form.
For IPv4, it's 4 bytes. For IPv6, it's 16 bytes.
*/
_IPAddress :: []u8

/*
Represents a subnet mask in binary form.
For IPv4, it's 4 bytes. For IPv6, it's 16 bytes.
*/
_IPMask :: []u8

/*
Represents an IP network, consisting of an IP address and a subnet mask.
*/
_IPNetwork :: struct  {
	address: _IPAddress,
	mask: _IPMask,
}

// declare []_IPNetwork

@(link_prefix="PNSLR_")
foreign {
	GetInterfaceIPAddresses :: proc "c" (
		networks: ^[]_IPNetwork,
		allocator: _Allocator,
	) -> b8 ---
}

// #######################################################################################
// Stream
// #######################################################################################

// Stream Declaration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Defines the mode to be used when calling the stream function.
*/
_StreamMode :: enum u8 {
	GetSize = 0,
	GetCurrentPos = 1,
	SeekAbsolute = 2,
	SeekRelative = 3,
	Read = 4,
	Write = 5,
	Truncate = 6,
	Flush = 7,
	Close = 8,
}

/*
Defines the delegate type for the stream function
*/
_StreamProcedure :: #type proc "c" (
	streamData: rawptr,
	mode: _StreamMode,
	data: []u8,
	offset: i64,
	extraRet: ^i64,
) -> b8

/*
Defines a generic stream, that can be used for reading/writing data.
*/
_Stream :: struct  {
	procedure: _StreamProcedure,
	data: rawptr,
}

// declare []_Stream

// Stream ease-of-use functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the size of the stream.
	Returns 0 on error.
	*/
	GetSizeOfStream :: proc "c" (
		stream: _Stream,
	) -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the current position in the stream.
	Returns -1 on error.
	*/
	GetCurrentPositionInStream :: proc "c" (
		stream: _Stream,
	) -> i64 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Seeks to a new position in the stream.
	If 'relative' is true, the new position is relative to the current position.
	If 'relative' is false, the new position is absolute from the start.
	Returns true on success, false on failure.
	*/
	SeekPositionInStream :: proc "c" (
		stream: _Stream,
		newPos: i64 = { },
		relative: b8 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Reads data from the stream into the provided buffer.
	Optionally stores the number of bytes read.
	Returns true on success, false on failure.
	*/
	ReadFromStream :: proc "c" (
		stream: _Stream,
		dst: []u8,
		readSize: ^i64 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Writes data from the provided buffer into the stream.
	Returns true on success, false on failure.
	*/
	WriteToStream :: proc "c" (
		stream: _Stream,
		src: []u8,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Formats a string and writes it to the stream.
	Only supports primitives, for obvious reasons.
	Use with `PNSLR_FmtB8`, `PNSLR_FmtI32`, etc.
	Returns true on success, false on failure.
	*/
	FormatAndWriteToStream :: proc "c" (
		stream: _Stream,
		fmtStr: string,
		args: []_PrimitiveFmtOptions,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Truncates the stream to the specified size.
	Returns true on success, false on failure.
	*/
	TruncateStream :: proc "c" (
		stream: _Stream,
		newSize: i64,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Flushes any buffered data to the stream.
	Returns true on success, false on failure.
	*/
	FlushStream :: proc "c" (
		stream: _Stream,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Closes the stream and frees any associated resources.
	*/
	CloseStream :: proc "c" (
		stream: _Stream,
	) ---
}

// Stream casts ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a stream from a file handle.
	*/
	StreamFromFile :: proc "c" (
		file: _File,
	) -> _Stream ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a stream from a string builder.
	*/
	StreamFromStringBuilder :: proc "c" (
		builder: ^_StringBuilder,
	) -> _Stream ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a stream from the standard output pipe.
	*/
	StreamFromStdOut :: proc "c" (
		disableBuffering: b8 = { },
	) -> _Stream ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a stream from the standard error pipe.
	*/
	StreamFromStdErr :: proc "c" (
		disableBuffering: b8 = { },
	) -> _Stream ---
}

// #######################################################################################
// Logger
// #######################################################################################

// Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Defines the logging levels.
*/
_LoggerLevel :: enum u8 {
	Debug = 0,
	Info = 1,
	Warn = 2,
	Error = 3,
	Critical = 4,
}

/*
Defines options for logging output.
*/
_LogOption :: distinct bit_set[_LogOptionValues; u8]

_LogOptionValues :: enum u8 {
	IncludeLevel = 0,
	IncludeDate = 1,
	IncludeTime = 2,
	IncludeFile = 3,
	IncludeFn = 4,
	IncludeColours = 5,
}

/*
Defines the delegate type for the logger function.
*/
_LoggerProcedure :: #type proc "c" (
	loggerData: rawptr,
	level: _LoggerLevel,
	data: string,
	options: _LogOption,
	location: _SourceCodeLocation,
)

/*
Defines a generic logger structure that can be used to log messages.
*/
_Logger :: struct  {
	procedure: _LoggerProcedure,
	data: rawptr,
	minAllowedLvl: _LoggerLevel,
	options: _LogOption,
}

// Default Logger Control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Sets the default logger FOR THE CURRENT THREAD.
	By default, every thread gets a thread-safe default logger that:
	- logs to stdout on desktop platforms
	- logs to logcat on Android
	*/
	SetDefaultLogger :: proc "c" (
		logger: _Logger,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Disables the default logger FOR THE CURRENT THREAD.
	*/
	DisableDefaultLogger :: proc "c" () ---
}

// Default Logger Non-Format Log Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	LogD :: proc "c" (
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogI :: proc "c" (
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogW :: proc "c" (
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogE :: proc "c" (
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogC :: proc "c" (
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

// Default Logger Formatted Log Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	LogDf :: proc "c" (
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogIf :: proc "c" (
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogWf :: proc "c" (
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogEf :: proc "c" (
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogCf :: proc "c" (
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLD :: proc "c" (
		logger: _Logger,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLI :: proc "c" (
		logger: _Logger,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLW :: proc "c" (
		logger: _Logger,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLE :: proc "c" (
		logger: _Logger,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLC :: proc "c" (
		logger: _Logger,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLDf :: proc "c" (
		logger: _Logger,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLIf :: proc "c" (
		logger: _Logger,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLWf :: proc "c" (
		logger: _Logger,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLEf :: proc "c" (
		logger: _Logger,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLCf :: proc "c" (
		logger: _Logger,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

// Logger functions with explicit level ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	Log :: proc "c" (
		level: _LoggerLevel,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	Logf :: proc "c" (
		level: _LoggerLevel,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogL :: proc "c" (
		logger: _Logger,
		level: _LoggerLevel,
		msg: string,
		loc: _SourceCodeLocation,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	LogLf :: proc "c" (
		logger: _Logger,
		level: _LoggerLevel,
		fmtMsg: string,
		args: []_PrimitiveFmtOptions,
		loc: _SourceCodeLocation,
	) ---
}

// Logger Casts ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a logger that writes to the given file.
	The file must be opened and valid.
	*/
	LoggerFromFile :: proc "c" (
		f: _File,
		minAllowedLevel: _LoggerLevel,
		options: _LogOption = { },
	) -> _Logger ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a logger that uses the default outputs (see `PNSLR_SetDefaultLogger()`).
	The returned logger is thread-safe and can be used from any thread.
	This can be used along with `PNSLR_SetDefaultLogger()` to customize
	the behaviour of the default in-built logger.
	*/
	GetDefaultLoggerWithOptions :: proc "c" (
		minAllowedLevel: _LoggerLevel,
		options: _LogOption = { },
	) -> _Logger ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a nil logger that does nothing.
	This can be used to disable logging in certain parts of the code.
	*/
	GetNilLogger :: proc "c" () -> _Logger ---
}

// #######################################################################################
// Threads
// #######################################################################################

/*
An opaque handle to a thread.
*/
_ThreadHandle :: struct  {
	handle: u64,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Checks if the handle to a thread is valid.
	*/
	IsThreadHandleValid :: proc "c" (
		handle: _ThreadHandle,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets a handle to the current thread.
	*/
	GetCurrentThreadHandle :: proc "c" () -> _ThreadHandle ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the name of a thread.
	The returned string is allocated using the provided allocator.
	If the thread has no name, an empty string is returned.
	*/
	GetThreadName :: proc "c" (
		handle: _ThreadHandle,
		allocator: _Allocator,
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Sets the name of a thread.
	The name is copied, so the provided string does not need to be valid after this call.
	On some platforms, thread names may be truncated to a certain length.
	 *
	Thread lengths on platforms (excluding null terminator):
	    Windows/OSX/iOS - 63 characters
	    Linux/Android   - 15 characters
	*/
	SetThreadName :: proc "c" (
		handle: _ThreadHandle,
		name: string,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Gets the name of the current thread.
	Read more about `PNSLR_GetThreadName`.
	*/
	GetCurrentThreadName :: proc "c" (
		allocator: _Allocator,
	) -> string ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Sets the name of the current thread.
	Read more about `PNSLR_SetThreadName`.
	*/
	SetCurrentThreadName :: proc "c" (
		name: string,
	) ---
}

/*
A procedure that can be run on a thread.
The `data` parameter is optional user data that can be passed to the thread.
*/
_ThreadProcedure :: #type proc "c" (
	data: rawptr,
)

@(link_prefix="PNSLR_")
foreign {
	/*
	Start a new thread with the specified procedure and user data.
	*/
	StartThread :: proc "c" (
		procedure: _ThreadProcedure,
		data: rawptr = { },
		name: string = { },
	) -> _ThreadHandle ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Joins a thread, blocking the calling thread until the specified thread has finished.
	*/
	JoinThread :: proc "c" (
		handle: _ThreadHandle,
	) ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Sleeps the current thread for the specified number of milliseconds.
	*/
	SleepCurrentThread :: proc "c" (
		milliseconds: u64,
	) ---
}

// #######################################################################################
// SharedMemoryChannel
// #######################################################################################

// Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
Opaque handle for a shared memory channel.
*/
_SharedMemoryChannelHandle :: struct  {
	handle: i64,
}

/*
Platform-specific header for a shared memory channel.
*/
_SharedMemoryChannelPlatformHeader :: struct  {
	magicNum: u32,
}

/*
Represents the status of a shared memory channel endpoint (reader or writer).
*/
_SharedMemoryChannelStatus :: enum u8 {
	Disconnected = 0,
	Paused = 1,
	Active = 2,
}

/*
Header for a shared memory channel, containing metadata about the channel.
*/
_SharedMemoryChannelHeader :: struct  {
	magicNum: u32,
	version: u32,
	readerStatus: _SharedMemoryChannelStatus,
	writerStatus: _SharedMemoryChannelStatus,
	offsetToOsSpecificHeader: u32,
	offsetToMsgQueueHeader: u32,
	offsetToMsgData: u32,
	fullMemRegionSize: i64,
	dataSize: i64,
}

/*
Header for the message queue within a shared memory channel.
*/
_SharedMemoryChannelMessageQueueHeader :: struct  {
	readCursor: i64,
	padding: [56]u8,
	writeCursor: i64,
}

/*
Represents a reader endpoint for a shared memory channel.
*/
_SharedMemoryChannelReader :: struct  {
	header: ^_SharedMemoryChannelHeader,
	handle: _SharedMemoryChannelHandle,
}

/*
Represents a writer endpoint for a shared memory channel.
*/
_SharedMemoryChannelWriter :: struct  {
	header: ^_SharedMemoryChannelHeader,
	handle: _SharedMemoryChannelHandle,
}

/*
Represents a reserved message slot for writing to a shared memory channel.
*/
_SharedMemoryChannelReservedMessage :: struct  {
	channel: ^_SharedMemoryChannelWriter,
	offset: i64,
	size: i64,
	writePtr: ^u8,
}

/*
Represents a message that has been read from a shared memory channel.
*/
_SharedMemoryChannelMessage :: struct  {
	channel: ^_SharedMemoryChannelReader,
	offset: i64,
	size: i64,
	readPtr: ^u8,
	readSize: i64,
}

// Reader Interface ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a shared memory channel reader with the specified name and size.
	The reader owns the shared memory segment and other processes can connect as writers.
	*/
	CreateSharedMemoryChannelReader :: proc "c" (
		name: string,
		size: i64,
		reader: ^_SharedMemoryChannelReader,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Polls for a message from the shared memory channel.
	Returns true if a message was found, false otherwise.
	Sets fatalError to true if an unrecoverable error occurred.
	*/
	ReadSharedMemoryChannelMessage :: proc "c" (
		reader: ^_SharedMemoryChannelReader,
		message: ^_SharedMemoryChannelMessage,
		fatalError: ^b8 = { },
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Acknowledges that a message has been processed and advances the read cursor.
	*/
	AcknowledgeSharedMemoryChannelMessage :: proc "c" (
		message: ^_SharedMemoryChannelMessage,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys a shared memory channel reader and releases all associated resources.
	*/
	DestroySharedMemoryChannelReader :: proc "c" (
		reader: ^_SharedMemoryChannelReader,
	) -> b8 ---
}

// Writer Interface ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

@(link_prefix="PNSLR_")
foreign {
	/*
	Attempts to connect to an existing shared memory channel as a writer.
	Returns true if successful, false if the channel doesn't exist or connection failed.
	*/
	TryConnectSharedMemoryChannelWriter :: proc "c" (
		name: string,
		writer: ^_SharedMemoryChannelWriter,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Reserves space for a message in the shared memory channel.
	Returns true if space was available, false otherwise.
	*/
	PrepareSharedMemoryChannelMessage :: proc "c" (
		writer: ^_SharedMemoryChannelWriter,
		size: i64,
		reservedMessage: ^_SharedMemoryChannelReservedMessage,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Commits a previously reserved message to the shared memory channel.
	*/
	CommitSharedMemoryChannelMessage :: proc "c" (
		writer: ^_SharedMemoryChannelWriter,
		reservedMessage: _SharedMemoryChannelReservedMessage,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Disconnects from a shared memory channel and releases writer resources.
	*/
	DisconnectSharedMemoryChannelWriter :: proc "c" (
		writer: ^_SharedMemoryChannelWriter,
	) -> b8 ---
}

// #######################################################################################
// Renderer
// #######################################################################################

/*
Defines the available Renderer types.
*/
RendererType :: enum u8 {
	Null = 0,
	Vulkan = 1,
	DirectX12 = 2,
	Metal = 3,
}

/*
Defines the available texture formats.
*/
TextureFormat :: enum u8 {
	Unknown = 0,
	D32_Float = 1,
	B8G8R8A8_UNorm = 2,
	R8G8B8A8_UNorm = 3,
	R16G16B16A16_UNorm = 4,
}

/*
Represents an opaque handle to the application instance.
Matches Dvaarpaal's app handle.
- On Windows, this is an HINSTANCE.
- On OSX, this is an NSApplication*.
- On Android, this is a struct android_app*.
*/
AppHandle :: struct  {
	handle: u64,
}

/*
Represents an opaque handle to a window.
Matches Dvaarpaal's window handle.
- On Windows, this is an HWND.
- On OSX, this is an NSWindow*.
- On Android, this is an ANativeWindow*.
*/
WindowHandle :: struct  {
	handle: u64,
}

/*
Configuration structure for renderer creation.
*/
RendererConfiguration :: struct  {
	type: RendererType,
	appHandle: AppHandle,
	allocator: _Allocator,
	appName: string,
}

/*
The main renderer interface.
Treat as base class for specific renderer implementations.
*/
Renderer :: struct  {
	type: RendererType,
	allocator: _Allocator,
	appHandle: AppHandle,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Creates a renderer instance based on the provided configuration.
	*/
	reateRenderer :: proc "c" (
		config: RendererConfiguration,
		tempAllocator: _Allocator,
	) -> ^Renderer ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Destroys the given renderer instance and frees associated resources.
	*/
	estroyRenderer :: proc "c" (
		renderer: ^Renderer,
		tempAllocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Halts the thread until all the work that's already submitted to the renderer is done.
	Return value is insignificant.
	 *
	Warning! - it won't necessarily account for work that gets submitted after this on other threads.
	*/
	aitTillRendererIdle :: proc "c" (
		renderer: ^Renderer,
	) -> b8 ---
}

/*
A command buffer for recording rendering commands.
*/
RendererCommandBuffer :: struct  {
	type: RendererType,
}

/*
A surface that can be drawn on top of.
*/
RendererSurface :: struct  {
	type: RendererType,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Creates a rendering surface for the given window and application handles.
	Uses 3 frames in flight.
	*/
	reateRendererSurfaceFromWindow :: proc "c" (
		renderer: ^Renderer,
		windowHandle: WindowHandle,
		tempAllocator: _Allocator,
	) -> ^RendererSurface ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Destroys the given rendering surface and frees associated resources.
	*/
	estroyRendererSurface :: proc "c" (
		surface: ^RendererSurface,
		tempAllocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Handles resizing the given rendering surface to the specified dimensions.
	*/
	esizeRendererSurface :: proc "c" (
		surface: ^RendererSurface,
		width: u16,
		height: u16,
		tempAllocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Begins a new frame for the given rendering surface, returning a command buffer for recording commands.
	The returned command buffer must be ended with MZNT_EndFrame (to submit drawing for the current frame).
	*/
	eginFrame :: proc "c" (
		surface: ^RendererSurface,
		r: f32,
		g: f32,
		b: f32,
		a: f32,
		tempAllocator: _Allocator,
	) -> ^RendererCommandBuffer ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	Ends the current frame for the given rendering surface, submitting recorded commands for execution.
	*/
	ndFrame :: proc "c" (
		surface: ^RendererSurface,
		tempAllocator: _Allocator,
	) -> b8 ---
}

/*
Configuration structure for swap-chain.
*/
SwapChainConfiguration :: struct  {
	width: u16,
	height: u16,
	vSync: b8,
	framesInFlight: u8,
}

/*
A swap-chain corresponding to a window that can be rendered to.
*/
SwapChain :: struct  {
	type: RendererType,
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Create a swap-chain for the given window, with the given renderer, for a custom number of frames in flight.
	If width/height are uninitialised, they will be derived from the window.
	*/
	reateSwapChainFromWindow :: proc "c" (
		renderer: ^Renderer,
		windowHandle: WindowHandle,
		cfg: SwapChainConfiguration,
		tempAllocator: _Allocator,
	) -> ^SwapChain ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Reconfigure a swap-chain with new properties.
	All configuration values must be initialised appropriately.
	*/
	econfigureSwapChain :: proc "c" (
		swapChain: ^SwapChain,
		cfg: SwapChainConfiguration,
		tempAllocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Destroy the swap-chain, freeing up associated resources.
	*/
	estroySwapChain :: proc "c" (
		swapChain: ^SwapChain,
		tempAllocator: _Allocator,
	) -> b8 ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Get the texture format of the swap-chain.
	*/
	etSwapChainTextureFormat :: proc "c" (
		swapChain: ^SwapChain,
	) -> TextureFormat ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	MAIN_THREAD
	Acquire the next image in the swap-chain, and get its associated command buffer
	Optionally, also get the image index, to cross-check with your own custom
	per-frame-in-flight allocated resources.
	 *
	Warning! - the output could be nil, in some cases (like window is minimsed).
	In this case, image index will be 255.
	*/
	terateSwapChain :: proc "c" (
		swapChain: ^SwapChain,
		outImgIdx: ^u8,
		tempAllocator: _Allocator,
	) -> ^RendererCommandBuffer ---
}

@(link_prefix="PNSLR_")
foreign {
	/*
	RENDER_THREAD
	Wrap up recording commands for the swap-chain and submit the current image
	for presenting.
	*/
	resentSwapChain :: proc "c" (
		swapChain: ^SwapChain,
		tempAllocator: _Allocator,
	) -> b8 ---
}

