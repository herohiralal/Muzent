#ifndef __cplusplus
    #error "Please use the C bindings.";
#endif

#ifndef MZNT_CXX_MAIN_H
#define MZNT_CXX_MAIN_H


namespace Muzent
{

    // #######################################################################################
    // Collections
    // #######################################################################################

    /**
     * A raw type-unspecific array slice.
     */
    struct _RawArraySlice
    {
       rawptr data;
       i64 count;
    };

    /**
     * UTF-8 string type, with length info (not necessarily null-terminated).
     */
    // typedef ArraySlice<u8> utf8str;

    // #######################################################################################
    // Environment
    // #######################################################################################

    /**
     * Defines the platforms supported by the library.
     */
    enum class _Platform : u8 /* use as value */
    {
        Unknown = 0,
        Windows = 1,
        Linux = 2,
        OSX = 3,
        Android = 4,
        iOS = 5,
        PS5 = 6,
        XBoxSeries = 7,
        Switch = 8,
    };

    /**
     * Defines the architectures supported by the library.
     */
    enum class _Architecture : u8 /* use as value */
    {
        Unknown = 0,
        X64 = 1,
        ARM64 = 2,
    };

    /**
     * Get the current platform.
     */
    _Platform _GetPlatform();

    /**
     * Get the current architecture.
     */
    _Architecture _GetArchitecture();

    // #######################################################################################
    // Runtime
    // #######################################################################################

    /**
     * Defines the source code location for debugging purposes.
     */
    struct _SourceCodeLocation
    {
       utf8str file;
       i32 line;
       i32 column;
       utf8str function;
    };

    // #######################################################################################
    // Sync
    // #######################################################################################

    // Mutex ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * The most basic synchronization primitive.
     */
    struct alignas(8) _Mutex
    {
       u8 buffer[64];
    };

    /**
     * Creates a mutex.
     */
    _Mutex _CreateMutex();

    /**
     * Destroys a mutex.
     */
    void _DestroyMutex(
        _Mutex* mutex
    );

    /**
     * Locks a mutex.
     */
    void _LockMutex(
        _Mutex* mutex
    );

    /**
     * Unlocks a mutex.
     */
    void _UnlockMutex(
        _Mutex* mutex
    );

    /**
     * Tries to lock a mutex.
     * Returns true if the mutex was successfully locked, false otherwise.
     */
    b8 _TryLockMutex(
        _Mutex* mutex
    );

    // Read-Write Mutex ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A read-write mutex.
     * This is a synchronization primitive that allows multiple readers or a single writer.
     * It is useful for scenarios where reads are more frequent than writes.
     */
    struct alignas(8) _RWMutex
    {
       u8 buffer[200];
    };

    /**
     * Creates a read-write mutex.
     */
    _RWMutex _CreateRWMutex();

    /**
     * Destroys a read-write mutex.
     */
    void _DestroyRWMutex(
        _RWMutex* rwmutex
    );

    /**
     * Locks a read-write mutex for reading.
     * Multiple threads can read simultaneously.
     */
    void _LockRWMutexShared(
        _RWMutex* rwmutex
    );

    /**
     * Locks a read-write mutex for writing.
     * Only one thread can write at a time, and no other threads can read while writing.
     */
    void _LockRWMutexExclusive(
        _RWMutex* rwmutex
    );

    /**
     * Unlocks a read-write mutex after reading.
     * Allows other threads to read or write.
     */
    void _UnlockRWMutexShared(
        _RWMutex* rwmutex
    );

    /**
     * Unlocks a read-write mutex after writing.
     * Allows other threads to read or write.
     */
    void _UnlockRWMutexExclusive(
        _RWMutex* rwmutex
    );

    /**
     * Tries to lock a read-write mutex for reading.
     * Returns true if the mutex was successfully locked for reading, false otherwise.
     */
    b8 _TryLockRWMutexShared(
        _RWMutex* rwmutex
    );

    /**
     * Tries to lock a read-write mutex for writing.
     * Returns true if the mutex was successfully locked for writing, false otherwise.
     */
    b8 _TryLockRWMutexExclusive(
        _RWMutex* rwmutex
    );

    // Semaphore ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A semaphore synchronization primitive.
     * It allows a certain number of threads to access a resource concurrently.
     */
    struct alignas(8) _Semaphore
    {
       u8 buffer[32];
    };

    /**
     * Creates a semaphore.
     * The initial count specifies how many threads can access the resource concurrently.
     */
    _Semaphore _CreateSemaphore(
        i32 initialCount
    );

    /**
     * Destroys a semaphore.
     */
    void _DestroySemaphore(
        _Semaphore* semaphore
    );

    /**
     * Waits on a semaphore.
     * The calling thread will block until the semaphore count is greater than zero.
     */
    void _WaitSemaphore(
        _Semaphore* semaphore
    );

    /**
     * Waits on a semaphore with a timeout.
     * The calling thread will block until the semaphore count is greater than zero or the timeout expires.
     * Returns true if the semaphore was acquired, false if the timeout expired.
     */
    b8 _WaitSemaphoreTimeout(
        _Semaphore* semaphore,
        i32 timeoutNs
    );

    /**
     * Signals a semaphore, incrementing its count by a specified amount.
     * If the count was zero, this will wake up one or more waiting threads.
     */
    void _SignalSemaphore(
        _Semaphore* semaphore,
        i32 count
    );

    // Condition Variable ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A condition variable for signaling between threads.
     * It allows threads to wait for a condition to be signaled.
     */
    struct alignas(8) _ConditionVariable
    {
       u8 buffer[48];
    };

    /**
     * Creates a condition variable.
     */
    _ConditionVariable _CreateConditionVariable();

    /**
     * Destroys a condition variable.
     */
    void _DestroyConditionVariable(
        _ConditionVariable* condvar
    );

    /**
     * Waits on a condition variable.
     * The calling thread will block until the condition variable is signaled.
     * The mutex must be locked before calling this function.
     */
    void _WaitConditionVariable(
        _ConditionVariable* condvar,
        _Mutex* mutex
    );

    /**
     * Waits on a condition variable with a timeout.
     * The calling thread will block until the condition variable is signaled or the timeout expires.
     * The mutex must be locked before calling this function.
     * Returns true if the condition variable was signaled, false if the timeout expired.
     */
    b8 _WaitConditionVariableTimeout(
        _ConditionVariable* condvar,
        _Mutex* mutex,
        i32 timeoutNs
    );

    /**
     * Signals a condition variable, waking up one waiting thread.
     * If no threads are waiting, this has no effect.
     */
    void _SignalConditionVariable(
        _ConditionVariable* condvar
    );

    /**
     * Signals a condition variable, waking up all waiting threads.
     * If no threads are waiting, this has no effect.
     */
    void _BroadcastConditionVariable(
        _ConditionVariable* condvar
    );

    // Do Once ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A "do once" primitive.
     * It ensures that a specified initialization function is executed only once, even
     * if called from multiple threads.
     * This is useful for one-time initialization of shared resources.
     */
    struct alignas(8) _DoOnce
    {
       u8 buffer[16];
    };

    /**
     * The callback function type for the "do once" primitive.
     */
    typedef void (*_DoOnceCallback)();

    /*
     * Executing the specified callback function only once.
     * If multiple threads call this function simultaneously, only one will execute.
     */
    void _ExecuteDoOnce(
        _DoOnce* once,
        _DoOnceCallback callback
    );

    // Event ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * An event synchronization primitive.
     * It allows one or more threads to wait until another thread signals a condition.
     */
    struct alignas(8) _Event
    {
       u8 buffer[112];
    };

    /**
     * Creates an event.
     * If manualReset is true, the event must be manually reset after being signaled.
     * If manualReset is false, the event automatically resets after releasing one waiting thread.
     */
    _Event _CreateEvent(
        b8 manualReset
    );

    /**
     * Destroys an event.
     */
    void _DestroyEvent(
        _Event* event
    );

    /**
     * Waits on an event.
     * The calling thread will block until the event is signaled.
     */
    void _WaitEvent(
        _Event* event
    );

    /**
     * Waits on an event with a timeout.
     * The calling thread will block until the event is signaled or the timeout expires.
     * Returns true if the event was signaled, false if the timeout expired.
     */
    b8 _WaitEventTimeout(
        _Event* event,
        i32 timeoutNs
    );

    /**
     * Signals an event.
     * If manualReset is false, wakes up one waiting thread and resets automatically.
     * If manualReset is true, wakes up all waiting threads and remains signaled until reset.
     */
    void _SignalEvent(
        _Event* event
    );

    /**
     * Resets an event, returning it to the unsignaled state.
     * Only meaningful for manual-reset events.
     */
    void _ResetEvent(
        _Event* event
    );

    // #######################################################################################
    // Memory
    // #######################################################################################

    /**
     * Set a block of memory to a specific value.
     */
    void _MemSet(
        rawptr memory,
        i32 value,
        i32 size
    );

    /**
     * Copy a block of memory from source to destination.
     */
    void _MemCopy(
        rawptr destination,
        rawptr source,
        i32 size
    );

    /**
     * Copy a block of memory from source to destination, handling overlapping regions.
     */
    void _MemMove(
        rawptr destination,
        rawptr source,
        i32 size
    );

    // #######################################################################################
    // Allocators
    // #######################################################################################

    // Allocator Declaration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Defines the mode to be used when calling the allocator function.
     */
    enum class _AllocatorMode : u8 /* use as value */
    {
        Allocate = 0,
        Resize = 1,
        Free = 2,
        FreeAll = 3,
        AllocateNoZero = 4,
        ResizeNoZero = 5,
        QueryCapabilities = 255,
    };

    /**
     * Defines the capabilities of an allocator.
     */
    enum class _AllocatorCapability : u64 /* use as flags */
    {
        None = 0,
        ThreadSafe = 1,
        Resize = 2,
        Free = 4,
        FreeAll = 8,
        HintNil = 67108864,
        HintBump = 134217728,
        HintHeap = 268435456,
        HintTemp = 536870912,
        HintDebug = 1073741824,
    };

    /**
     * Defines the error codes that can be returned by the allocator.
     */
    enum class _AllocatorError : u8 /* use as value */
    {
        None = 0,
        OutOfMemory = 1,
        InvalidAlignment = 2,
        InvalidSize = 3,
        InvalidMode = 4,
        Internal = 5,
        OutOfOrderFree = 6,
        DoubleFree = 7,
        CantFreeAll = 8,
    };

    /**
     * Defines the delegate type for the allocator function.
     */
    typedef rawptr (*_AllocatorProcedure)(
        rawptr allocatorData,
        _AllocatorMode mode,
        i32 size,
        i32 alignment,
        rawptr oldMemory,
        i32 oldSize,
        _SourceCodeLocation location,
        _AllocatorError* error
    );

    /**
     * Defines a generic allocator structure that can be used to allocate, resize, and free memory.
     */
    struct _Allocator
    {
       _AllocatorProcedure procedure;
       rawptr data;
    };

    // Allocation ease-of-use functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Allocate memory using the provided allocator.
     */
    rawptr _Allocate(
        _Allocator allocator,
        b8 zeroed,
        i32 size,
        i32 alignment,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Resize memory using the provided allocator.
     */
    rawptr _Resize(
        _Allocator allocator,
        b8 zeroed,
        rawptr oldMemory,
        i32 oldSize,
        i32 newSize,
        i32 alignment,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Fallback resize function that can be used when the allocator does not support resizing.
     */
    rawptr _DefaultResize(
        _Allocator allocator,
        b8 zeroed,
        rawptr oldMemory,
        i32 oldSize,
        i32 newSize,
        i32 alignment,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Free memory using the provided allocator.
     */
    void _Free(
        _Allocator allocator,
        rawptr memory,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Free all memory allocated by the provided allocator.
     */
    void _FreeAll(
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Query the capabilities of the provided allocator.
     */
    u64 _QueryAllocatorCapabilities(
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    // Nil allocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Get the 'nil' allocator. Reports 'out of memory' when requesting memory.
     * Otherwise no-ops all around.
     */
    _Allocator _GetAllocator_Nil();

    // Default Heap Allocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Get the default heap allocator.
     */
    _Allocator _GetAllocator_DefaultHeap();

    /**
     * Main allocator function for the default heap allocator.
     */
    rawptr _AllocatorFn_DefaultHeap(
        rawptr allocatorData,
        _AllocatorMode mode,
        i32 size,
        i32 alignment,
        rawptr oldMemory,
        i32 oldSize,
        _SourceCodeLocation location,
        _AllocatorError* error
    );

    // Arena Alloator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A block of memory used by the arena allocator.
     */
    struct _ArenaAllocatorBlock
    {
       _ArenaAllocatorBlock* previous;
       _Allocator allocator;
       rawptr memory;
       u32 capacity;
       u32 used;
    };

    /**
     * The payload used by the arena allocator.
     */
    struct _ArenaAllocatorPayload
    {
       _Allocator backingAllocator;
       _ArenaAllocatorBlock* currentBlock;
       u32 totalUsed;
       u32 totalCapacity;
       u32 minimumBlockSize;
       u32 numSnapshots;
    };

    /**
     * Create a new arena allocator with the specified backing allocator.
     * The arena allocator will use the backing allocator to allocate its blocks.
     * The arena allocator will not free the backing allocator, so it is the caller's responsibility to
     * free the backing allocator when it is no longer needed.
     */
    _Allocator _NewAllocator_Arena(
        _Allocator backingAllocator,
        u32 pageSize,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Destroy an arena allocator and free all its resources.
     * This does not free the backing allocator, only the arena allocator's own resources.
     */
    void _DestroyAllocator_Arena(
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Main allocator function for the arena allocator.
     */
    rawptr _AllocatorFn_Arena(
        rawptr allocatorData,
        _AllocatorMode mode,
        i32 size,
        i32 alignment,
        rawptr oldMemory,
        i32 oldSize,
        _SourceCodeLocation location,
        _AllocatorError* error
    );

    enum class _ArenaSnapshotError : u8 /* use as value */
    {
        None = 0,
        InvalidData = 1,
        MemoryBlockNotOwned = 2,
        OutOfOrderRestoreUsage = 3,
        DoubleRestoreOrDiscardUsage = 4,
    };

    /**
     * A snapshot of the arena allocator, recording its state at a specific point in time.
     * Can be saved/loaded/discarded as needed.
     */
    struct _ArenaAllocatorSnapshot
    {
       b8 valid;
       _ArenaAllocatorPayload* payload;
       _ArenaAllocatorBlock* block;
       u32 used;
    };

    /**
     * Ensures that the arena allocator has either restored/discarded all the
     * snapshots that were taken.
     */
    b8 _ValidateArenaAllocatorSnapshotState(
        _Allocator allocator
    );

    /**
     * Captures a snapshot of the arena allocator.
     * The returned value can be used to load back the existing state at this point.
     */
    _ArenaAllocatorSnapshot _CaptureArenaAllocatorSnapshot(
        _Allocator allocator
    );

    /**
     * Restores the state of the arena allocator from a snapshot.
     * Upon success, the snapshot is marked as invalid.
     */
    _ArenaSnapshotError _RestoreArenaAllocatorSnapshot(
        _ArenaAllocatorSnapshot* snapshot,
        _SourceCodeLocation loc
    );

    /**
     * Discards a snapshot of the arena allocator.
     */
    _ArenaSnapshotError _DiscardArenaAllocatorSnapshot(
        _ArenaAllocatorSnapshot* snapshot
    );

    // Stack Allocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A page of a stack allocator.
     */
    struct alignas(8) _StackAllocatorPage
    {
       _StackAllocatorPage* previousPage;
       u64 usedBytes;
       u8 buffer[8192];
    };

    /**
     * The header used for every separate stack allocation.
     * This is used to store metadata about the allocation and deallocate appropriately.
     */
    struct _StackAllocationHeader
    {
       _StackAllocatorPage* page;
       i32 size;
       i32 alignment;
       rawptr lastAllocation;
       rawptr lastAllocationHeader;
    };

    /**
     * The payload used by the stack allocator.
     */
    struct _StackAllocatorPayload
    {
       _Allocator backingAllocator;
       _StackAllocatorPage* currentPage;
       rawptr lastAllocation;
       _StackAllocationHeader* lastAllocationHeader;
    };

    /**
     * Create a stack allocator with the specified backing allocator.
     * The stack allocator will use the backing allocator to allocate its pages.
     * The stack allocator will not free the backing allocator, so it is the caller's responsibility to
     * free the backing allocator when it is no longer needed.
     */
    _Allocator _NewAllocator_Stack(
        _Allocator backingAllocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Destroy a stack allocator and free all its resources.
     * This does not free the backing allocator, only the stack allocator's own resources.
     */
    void _DestroyAllocator_Stack(
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Main allocator function for the stack allocator.
     */
    rawptr _AllocatorFn_Stack(
        rawptr allocatorData,
        _AllocatorMode mode,
        i32 size,
        i32 alignment,
        rawptr oldMemory,
        i32 oldSize,
        _SourceCodeLocation location,
        _AllocatorError* error
    );

    // Collections make/free functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Allocate a raw array slice of 'count' elements, each of size 'tySize' and alignment 'tyAlign', using the provided allocator. Optionally zeroed.
     */
    _RawArraySlice _MakeRawSlice(
        i32 tySize,
        i32 tyAlign,
        i64 count,
        b8 zeroed,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Free a raw array slice allocated with `PNSLR_MakeRawSlice`, using the provided allocator.
     */
    void _FreeRawSlice(
        _RawArraySlice* slice,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Resize a raw array slice to one with 'newCount' elements, each of size 'tySize' and alignment 'tyAlign', using the provided allocator. Optionally zeroed.
     */
    void _ResizeRawSlice(
        _RawArraySlice* slice,
        i32 tySize,
        i32 tyAlign,
        i64 newCount,
        b8 zeroed,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Allocate a UTF-8 string of 'count__' characters using the provided allocator. Optionally zeroed.
     */
    utf8str _MakeString(
        i64 count,
        b8 zeroed,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Free a UTF-8 string allocated with `PNSLR_MakeString`, using the provided allocator.
     */
    void _FreeString(
        utf8str str,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Allocate a C-style null-terminated string of 'count__' characters (excluding the null terminator) using the provided allocator. Optionally zeroed.
     */
    cstring _MakeCString(
        i64 count,
        b8 zeroed,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    /**
     * Free a C-style null-terminated string allocated with `PNSLR_MakeCString`, using the provided allocator.
     */
    void _FreeCString(
        cstring str,
        _Allocator allocator,
        _SourceCodeLocation location,
        _AllocatorError* error = { }
    );

    // #######################################################################################
    // Chrono
    // #######################################################################################

    /**
     * Returns the current time in nanoseconds since the Unix epoch (January 1, 1970).
     */
    i64 _NanosecondsSinceUnixEpoch();

    /**
     * Breaks down the given nanoseconds since the Unix epoch into its
     * date and time components.
     */
    b8 _ConvertNanosecondsSinceUnixEpochToDateTime(
        i64 ns,
        i16* outYear,
        u8* outMonth,
        u8* outDay,
        u8* outHour,
        u8* outMinute,
        u8* outSecond
    );

    // #######################################################################################
    // Strings
    // #######################################################################################

    // Basics/Conversions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Returns the length of the given C-style null-terminated string, excluding the null terminator.
     */
    i32 _GetCStringLength(
        cstring str
    );

    /**
     * Clone a C-style string into a new allocated string.
     */
    utf8str _StringFromCString(
        cstring str
    );

    /**
     * Clones a UTF-8 string to a C-style null-terminated string.
     * The returned string is allocated using the specified allocator.
     */
    cstring _CStringFromString(
        utf8str str,
        _Allocator allocator
    );

    /**
     * Clones a UTF-8 string to a new allocated UTF-8 string.
     * The returned string is allocated using the specified allocator.
     */
    utf8str _CloneString(
        utf8str str,
        _Allocator allocator
    );

    // Basic/Manipulation ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Concatenates two UTF-8 strings into a new allocated string.
     * The returned string is allocated using the specified allocator.
     */
    utf8str _ConcatenateStrings(
        utf8str str1,
        utf8str str2,
        _Allocator allocator
    );

    // Casing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Converts a UTF-8 string to uppercase.
     * The returned string is allocated using the specified allocator.
     */
    utf8str _UpperString(
        utf8str str,
        _Allocator allocator
    );

    /**
     * Converts a UTF-8 string to lowercase.
     * The returned string is allocated using the specified allocator.
     */
    utf8str _LowerString(
        utf8str str,
        _Allocator allocator
    );

    // Comparisons ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Represents the type of string comparison to perform.
     */
    enum class _StringComparisonType : u8 /* use as value */
    {
        CaseSensitive = 0,
        CaseInsensitive = 1,
    };

    /**
     * Checks if two UTF-8 strings contain the same data.
     * Returns true if they are equal, false otherwise.
     */
    b8 _AreStringsEqual(
        utf8str str1,
        utf8str str2,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Asymmetric equality-check between a UTF-8 string and a C-style null-terminated string.
     * Returns true if they are equal, false otherwise.
     */
    b8 _AreStringAndCStringEqual(
        utf8str str1,
        cstring str2,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if two C-style null-terminated strings are equal.
     * Returns true if they are equal, false otherwise.
     */
    b8 _AreCStringsEqual(
        cstring str1,
        cstring str2,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a UTF-8 string starts with the specified prefix.
     * Returns true if it does, false otherwise.
     */
    b8 _StringStartsWith(
        utf8str str,
        utf8str prefix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a UTF-8 string ends with the specified suffix.
     * Returns true if it does, false otherwise.
     */
    b8 _StringEndsWith(
        utf8str str,
        utf8str suffix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a C-style null-terminated string starts with the specified prefix.
     */
    b8 _StringStartsWithCString(
        utf8str str,
        cstring prefix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a C-style null-terminated string ends with the specified suffix.
     */
    b8 _StringEndsWithCString(
        utf8str str,
        cstring suffix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Returns the length of the given C-style null-terminated string, excluding the null terminator.
     */
    b8 _CStringStartsWith(
        cstring str,
        utf8str prefix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a C-style null-terminated string ends with the specified UTF-8 suffix.
     */
    b8 _CStringEndsWith(
        cstring str,
        utf8str suffix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a C-style null-terminated string starts with the specified UTF-8 prefix.
     */
    b8 _CStringStartsWithCString(
        utf8str str,
        cstring prefix,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Checks if a C-style null-terminated string ends with the specified UTF-8 suffix.
     */
    b8 _CStringEndsWithCString(
        utf8str str,
        cstring suffix,
        _StringComparisonType comparisonType = { }
    );

    // Advanced comparisons ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Searches for the first occurrence of a substring within a string.
     * Returns the index of the first occurrence, or -1 if not found.
     */
    i32 _SearchFirstIndexInString(
        utf8str str,
        utf8str substring,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Searches for the last occurrence of a substring within a string.
     * Returns the index of the last occurrence, or -1 if not found.
     */
    i32 _SearchLastIndexInString(
        utf8str str,
        utf8str substring,
        _StringComparisonType comparisonType = { }
    );

    /**
     * Replaces all occurrences of a substring within a string with a new value.
     * The returned string is allocated using the specified allocator.
     */
    utf8str _ReplaceInString(
        utf8str str,
        utf8str oldValue,
        utf8str newValue,
        _Allocator allocator,
        _StringComparisonType comparisonType = { }
    );

    // UTF-8 functionalities ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Result structure for UTF-8 rune encoding.
     * Contains the encoded bytes and the number of bytes used.
     */
    struct _EncodedRune
    {
       u8 data[4];
       i32 length;
    };

    /**
     * Result structure for UTF-8 rune decoding.
     * Contains the decoded rune and the number of bytes consumed.
     */
    struct _DecodedRune
    {
       u32 rune;
       i32 length;
    };

    /**
     * Returns the number of bytes required to encode the given rune in UTF-8.
     */
    i32 _GetRuneLength(
        u32 r
    );

    /**
     * Encodes a rune into UTF-8 byte sequence and returns the structure containing encoded bytes/length.
     * Invalid runes or surrogates are replaced with the error rune (U+FFFD).
     */
    _EncodedRune _EncodeRune(
        u32 c
    );

    /**
     * Decodes a UTF-8 byte sequence into a rune and returns the structure containing the rune/length.
     * Returns error rune (U+FFFD) for invalid sequences.
     */
    _DecodedRune _DecodeRune(
        ArraySlice<u8> s
    );

    // Windows-specific bs for UTF-16 conversions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Converts a UTF-8 string to a UTF-16 string.
     * The returned string is allocated using the specified allocator.
     * Only available on Windows. Bad decision to use UTF-16 on Windows, but it's a legacy thing.
     */
    ArraySlice<u16> _UTF16FromUTF8WindowsOnly(
        utf8str str,
        _Allocator allocator
    );

    /**
     * Converts a UTF-16 string to a UTF-8 string.
     * The returned string is allocated using the specified allocator.
     * Only available on Windows. Bad decision to use UTF-16 on Windows, but it's a legacy thing.
     */
    utf8str _UTF8FromUTF16WindowsOnly(
        ArraySlice<u16> utf16str,
        _Allocator allocator
    );

    // String Builder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * A basic string builder. Can accept strings and characters,
     * and build a single string from them.
     *
     * Create by setting the allocator and zeroing the rest of the fields.
     */
    struct _StringBuilder
    {
       _Allocator allocator;
       ArraySlice<u8> buffer;
       i64 writtenSize;
       i64 cursorPos;
    };

    /**
     * Ensure that the string builder has enough space to accommodate additionalSize bytes.
     */
    b8 _ReserveSpaceInStringBuilder(
        _StringBuilder* builder,
        i64 additionalSize
    );

    /**
     * Append a single byte to the string builder. Could be an ANSI/ASCII character,
     * or not. The function does not check for validity.
     */
    b8 _AppendByteToStringBuilder(
        _StringBuilder* builder,
        u8 byte
    );

    /**
     * Append a UTF-8 string to the string builder.
     */
    b8 _AppendStringToStringBuilder(
        _StringBuilder* builder,
        utf8str str
    );

    /**
     * Append a C-style null-terminated string to the string builder.
     */
    b8 _AppendCStringToStringBuilder(
        _StringBuilder* builder,
        cstring str
    );

    /**
     * Append a single character (rune) to the string builder.
     */
    b8 _AppendRuneToStringBuilder(
        _StringBuilder* builder,
        u32 rune
    );

    /**
     * Append an 8-bit boolean value to the string builder.
     */
    b8 _AppendB8ToStringBuilder(
        _StringBuilder* builder,
        b8 value
    );

    /**
     * Append a 32-bit floating-point number to the string builder.
     */
    b8 _AppendF32ToStringBuilder(
        _StringBuilder* builder,
        f32 value,
        i32 decimalPlaces
    );

    /**
     * Append a 64-bit floating point number to the string builder.
     */
    b8 _AppendF64ToStringBuilder(
        _StringBuilder* builder,
        f64 value,
        i32 decimalPlaces
    );

    /**
     * The base to use when appending integer numbers to the string builder.
     */
    enum class _IntegerBase : u8 /* use as value */
    {
        Decimal = 0,
        Binary = 1,
        HexaDecimal = 2,
        Octal = 3,
    };

    /**
     * Append an unsigned 8-bit integer to the string builder.
     */
    b8 _AppendU8ToStringBuilder(
        _StringBuilder* builder,
        u8 value,
        _IntegerBase base = { }
    );

    /**
     * Append an unsigned 16-bit integer to the string builder.
     */
    b8 _AppendU16ToStringBuilder(
        _StringBuilder* builder,
        u16 value,
        _IntegerBase base = { }
    );

    /**
     * Append an unsigned 32-bit integer number to the string builder.
     */
    b8 _AppendU32ToStringBuilder(
        _StringBuilder* builder,
        u32 value,
        _IntegerBase base = { }
    );

    /**
     * Append an unsigned 64-bit integer to the string builder.
     */
    b8 _AppendU64ToStringBuilder(
        _StringBuilder* builder,
        u64 value,
        _IntegerBase base = { }
    );

    /**
     * Append a signed 8-bit integer to the string builder.
     */
    b8 _AppendI8ToStringBuilder(
        _StringBuilder* builder,
        i8 value,
        _IntegerBase base = { }
    );

    /**
     * Append a signed 16-bit integer to the string builder.
     */
    b8 _AppendI16ToStringBuilder(
        _StringBuilder* builder,
        i16 value,
        _IntegerBase base = { }
    );

    /**
     * Append a signed 32-bit integer number to the string builder.
     */
    b8 _AppendI32ToStringBuilder(
        _StringBuilder* builder,
        i32 value,
        _IntegerBase base = { }
    );

    /**
     * Append a signed 64-bit integer to the string builder.
     */
    b8 _AppendI64ToStringBuilder(
        _StringBuilder* builder,
        i64 value,
        _IntegerBase base = { }
    );

    /**
     * Return the string from the string builder.
     */
    utf8str _StringFromStringBuilder(
        _StringBuilder* builder
    );

    /**
     * Reset the string builder, clearing its contents but keeping the allocated buffer.
     */
    void _ResetStringBuilder(
        _StringBuilder* builder
    );

    /**
     * Free the resources used by the string builder.
     */
    void _FreeStringBuilder(
        _StringBuilder* builder
    );

    // String Formatting ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * The possible primitive types that can be formatted.
     */
    enum class _PrimitiveFmtType : u8 /* use as value */
    {
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
    };

    /**
     * The internal encoding of a type-unspecific format specifier.
     * For booleans, valueBufferA is 0 or 1.
     * For floats, valueBufferA is the float value (reinterpret as relevant),
     *     and valueBufferB is the number of decimal places (cast to i32).
     * For integers, valueBufferA is the integer value (reinterpret as relevant),
     *     and the first half of valueBufferB is the base (cast to PNSLR_IntegerBase).
     * For runes, valueBufferA is the rune value (reinterpret as u32).
     * For C-style strings, valueBufferA is the pointer to the string.
     * For UTF-8 strings, valueBufferA is the pointer to the string,
     *     and valueBufferB is the length (reinterpret as i64).
     */
    struct _PrimitiveFmtOptions
    {
       _PrimitiveFmtType type;
       u64 valueBufferA;
       u64 valueBufferB;
    };

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtB8(
        b8 value
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtF32(
        f32 value,
        i32 decimalPlaces
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtF64(
        f64 value,
        i32 decimalPlaces
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtU8(
        u8 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtU16(
        u16 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtU32(
        u32 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtU64(
        u64 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtI8(
        i8 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtI16(
        i16 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtI32(
        i32 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtI64(
        i64 value,
        _IntegerBase base = { }
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtRune(
        u32 value
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtCString(
        cstring value
    );

    /**
     * Use when formatting a string. Pass as one of the varargs.
     */
    _PrimitiveFmtOptions _FmtString(
        utf8str value
    );

    /**
     * Format a string with the given format and arguments, appending the result
     * to the string builder.
     */
    b8 _FormatAndAppendToStringBuilder(
        _StringBuilder* builder,
        utf8str fmtStr,
        ArraySlice<_PrimitiveFmtOptions> args
    );

    /**
     * Format a string with the given format and arguments, returning the result
     * as a new allocated string using the specified allocator.
     */
    utf8str _FormatString(
        utf8str fmtStr,
        ArraySlice<_PrimitiveFmtOptions> args,
        _Allocator allocator
    );

    // Conversions to strings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Convert a boolean value to a string ("true" or "false").
     */
    utf8str _StringFromBoolean(
        b8 value,
        _Allocator allocator
    );

    /**
     * Convert a 32-bit floating-point number to a string with specified decimal places.
     */
    utf8str _StringFromF32(
        f32 value,
        _Allocator allocator,
        i32 decimalPlaces = { }
    );

    /**
     * Convert a 64-bit floating-point number to a string with specified decimal places.
     */
    utf8str _StringFromF64(
        f64 value,
        _Allocator allocator,
        i32 decimalPlaces = { }
    );

    /**
     * Convert an unsigned 8-bit integer to a string in the specified base.
     */
    utf8str _StringFromU8(
        u8 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert an unsigned 16-bit integer to a string in the specified base.
     */
    utf8str _StringFromU16(
        u16 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert an unsigned 32-bit integer to a string in the specified base.
     */
    utf8str _StringFromU32(
        u32 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert an unsigned 64-bit integer to a string in the specified base.
     */
    utf8str _StringFromU64(
        u64 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert a signed 8-bit integer to a string in the specified base.
     */
    utf8str _StringFromI8(
        i8 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert a signed 16-bit integer to a string in the specified base.
     */
    utf8str _StringFromI16(
        i16 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert a signed 32-bit integer to a string in the specified base.
     */
    utf8str _StringFromI32(
        i32 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    /**
     * Convert a signed 64-bit integer to a string in the specified base.
     */
    utf8str _StringFromI64(
        i64 value,
        _Allocator allocator,
        _IntegerBase base = { }
    );

    // Conversions from strings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Convert a validstring (case-insensitive "true" or "false", or "1" or "0") to a boolean.
     */
    b8 _BooleanFromString(
        utf8str str,
        b8* value
    );

    /**
     * Convert a valid string (numbers-only, with zero or one decimal points,
     * optional -/+ sign at the start) to a 32-bit floating-point number.
     */
    b8 _F32FromString(
        utf8str str,
        f32* value
    );

    /**
     * Convert a valid string (numbers-only, with zero or one decimal points,
     * optional -/+ sign at the start) to a 64-bit floating-point number.
     */
    b8 _F64FromString(
        utf8str str,
        f64* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optionally
     * starting with 0b/0o/0x prefix for alternate bases) to an unsigned 8-bit integer.
     * Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
     * By default (no prefix), decimal base is assumed.
     */
    b8 _U8FromString(
        utf8str str,
        u8* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optionally
     * starting with 0b/0o/0x prefix for alternate bases) to an unsigned 16-bit integer.
     * Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
     * By default (no prefix), decimal base is assumed.
     */
    b8 _U16FromString(
        utf8str str,
        u16* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optionally
     * starting with 0b/0o/0x prefix for alternate bases) to an unsigned 32-bit integer.
     * Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
     * By default (no prefix), decimal base is assumed.
     */
    b8 _U32FromString(
        utf8str str,
        u32* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optionally
     * starting with 0b/0o/0x prefix for alternate bases) to an unsigned 64-bit integer.
     * Will be assumed to be hexadecimal if it contains A-F characters but no prefix.
     * By default (no prefix), decimal base is assumed.
     */
    b8 _U64FromString(
        utf8str str,
        u64* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
     * at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
     * a signed 8-bit integer. Will be assumed to be hexadecimal if it contains A-F
     * characters but no prefix. By default (no prefix), decimal base is assumed.
     */
    b8 _I8FromString(
        utf8str str,
        i8* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
     * at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
     * a signed 16-bit integer. Will be assumed to be hexadecimal if it contains A-F
     * characters but no prefix. By default (no prefix), decimal base is assumed.
     */
    b8 _I16FromString(
        utf8str str,
        i16* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
     * at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
     * a signed 32-bit integer. Will be assumed to be hexadecimal if it contains A-F
     * characters but no prefix. By default (no prefix), decimal base is assumed.
     */
    b8 _I32FromString(
        utf8str str,
        i32* value
    );

    /**
     * Convert a valid string (numbers/A-F only, case-insensitive, optional -/+ sign
     * at the start, optionally starting with 0b/0o/0x prefix for alternate bases) to
     * a signed 64-bit integer. Will be assumed to be hexadecimal if it contains A-F
     * characters but no prefix. By default (no prefix), decimal base is assumed.
     */
    b8 _I64FromString(
        utf8str str,
        i64* value
    );

    // #######################################################################################
    // IO
    // #######################################################################################

    /**
     * Represents a normalised path.
     * This is used to ensure that paths are in a consistent format across different platforms.
     * It is a simple wrapper around a UTF-8 string.
     * It's always an absolute path, and the path separators are always forward slashes ('/').
     * For directories, it always ends with a trailing slash.
     */
    struct _Path
    {
       utf8str path;
    };

    /**
     * Represents the type of path normalisation to perform.
     */
    enum class _PathNormalisationType : u8 /* use as value */
    {
        File = 0,
        Directory = 1,
    };

    /**
     * Normalises a path to a consistent format, as specified in `PNSLR_PathNormalisationType`.
     */
    _Path _NormalisePath(
        utf8str path,
        _PathNormalisationType type,
        _Allocator allocator
    );

    /**
     * Split a path into its components. Assumes that the path is a valid normalised path in Panshilar conventions.
     */
    b8 _SplitPath(
        _Path path,
        _Path* parent,
        utf8str* selfNameWithExtension = { },
        utf8str* selfName = { },
        utf8str* extension = { }
    );

    /**
     * Returns a normalised path for a file inside a given directory.
     */
    _Path _GetPathForChildFile(
        _Path dir,
        utf8str fileNameWithExtension,
        _Allocator allocator
    );

    /**
     * Returns a normalised path for a subdirectory inside a given directory.
     */
    _Path _GetPathForSubdirectory(
        _Path dir,
        utf8str dirName,
        _Allocator allocator
    );

    /**
     * The signature of the delegate that's supposed to be called for iterating over a directory.
     */
    typedef b8 (*_DirectoryIterationVisitorDelegate)(
        rawptr payload,
        _Path path,
        b8 isDirectory,
        b8* exploreCurrentDirectory
    );

    /**
     * Iterates over a directory and calls the visitor function for each file/directory found.
     * If `recursive` is true, it will also iterate over subdirectories.
     */
    void _IterateDirectory(
        _Path path,
        b8 recursive,
        rawptr visitorPayload,
        _DirectoryIterationVisitorDelegate visitorFunc
    );

    /**
     * Represents the type of path check to perform when checking if a path exists.
     */
    enum class _PathExistsCheckType : u8 /* use as value */
    {
        Either = 0,
        File = 1,
        Directory = 2,
    };

    /**
     * Checks if a file/directory exists at the specified path.
     */
    b8 _PathExists(
        _Path path,
        _PathExistsCheckType type
    );

    /**
     * Deletes a file/directory at a path, if it exists.
     */
    b8 _DeletePath(
        _Path path
    );

    /**
     * Get the timestamp of a file at the specified path as nanoseconds since unix epoch.
     */
    i64 _GetFileTimestamp(
        _Path path
    );

    /**
     * Gets the size of a file at the specified path in bytes.
     */
    i64 _GetFileSize(
        _Path path
    );

    /**
     * Creates a directory tree, if it doesn't exist.
     * Note that if the path doesn't have a trailing slash, it'll assume it's a file.
     * So, the last component of the path (if is a directory) will not be created.
     */
    b8 _CreateDirectoryTree(
        _Path path
    );

    /**
     * Represents an opened file.
     */
    struct _File
    {
       rawptr handle;
    };

    /**
     * Opens a file for reading, optionally allow writing to the stream.
     * If the file does not exist, this function will fail.
     */
    _File _OpenFileToRead(
        _Path path,
        b8 allowWrite = { }
    );

    /**
     * Opens a file for writing (or appending), optionally allow reading from the stream.
     * If the file does not exist, it will be created.
     */
    _File _OpenFileToWrite(
        _Path path,
        b8 append = { },
        b8 allowRead = { }
    );

    /**
     * Gets the size of an opened file.
     * Returns 0 on error.
     */
    i64 _GetSizeOfFile(
        _File handle
    );

    /**
     * Gets the current position in an opened file.
     * Returns -1 on error.
     */
    i64 _GetCurrentPositionInFile(
        _File handle
    );

    /**
     * Seeks to a specific position in an opened file.
     * If not relative, it's absolute from the start.
     * Returns true on success, false on failure.
     */
    b8 _SeekPositionInFile(
        _File handle,
        i64 newPos,
        b8 relative = { }
    );

    /**
     * Reads data from an opened file at the current position.
     * Optionally stores the number of bytes read.
     * Returns true on success, false on failure.
     */
    b8 _ReadFromFile(
        _File handle,
        ArraySlice<u8> dst,
        i64* readSize = { }
    );

    /**
     * Writes data to an opened file at the current position.
     * Returns true on success, false on failure.
     */
    b8 _WriteToFile(
        _File handle,
        ArraySlice<u8> src
    );

    /**
     * Formats a string with the given format and arguments, writing the
     * result to the file.
     * Returns true on success, false on failure.
     */
    b8 _FormatAndWriteToFile(
        _File handle,
        utf8str fmtStr,
        ArraySlice<_PrimitiveFmtOptions> args
    );

    /**
     * Truncates an opened file to a specific size.
     * Returns true on success, false on failure.
     */
    b8 _TruncateFile(
        _File handle,
        i64 newSize
    );

    /**
     * Flushes any buffered data to the file.
     * Returns true on success, false on failure.
     */
    b8 _FlushFile(
        _File handle
    );

    /**
     * Closes an opened file.
     */
    void _CloseFileHandle(
        _File handle
    );

    /**
     * Reads a file fully end-to-end and stores in a buffer. Won't work if dst is nil.
     * Provided allocator is used for creating the buffer.
     * Returns true on success, false on failure.
     */
    b8 _ReadAllContentsFromFile(
        _Path path,
        ArraySlice<u8>* dst,
        _Allocator allocator
    );

    /**
     * Dump a bunch of data into a file. Optionally append it instead of overwriting.
     * Returns true on success, false on failure.
     */
    b8 _WriteAllContentsToFile(
        _Path path,
        ArraySlice<u8> src,
        b8 append = { }
    );

    /**
     * Copies a file from src to dst. If dst exists, it will be overwritten.
     * Returns true on success, false on failure.
     */
    b8 _CopyFile(
        _Path src,
        _Path dst
    );

    /**
     * Moves a file from src to dst. If dst exists, it will be overwritten.
     * Returns true on success, false on failure.
     */
    b8 _MoveFile(
        _Path src,
        _Path dst
    );

    // #######################################################################################
    // Process
    // #######################################################################################

    /**
     * Exits the current process immediately with the specified exit code.
     */
    void _ExitProcess(
        i32 exitCode
    );

    /**
     * A key-value pair representing an environment variable.
     * The `kvp` field contains the full "KEY=VALUE" string.
     * The 'key' field contains the key part.
     * The 'value' field contains the value part.
     */
    struct _EnvVarKeyValuePair
    {
       utf8str kvp;
       utf8str key;
       utf8str value;
    };

    /**
     * Retrieves all environment variables as an array slice of key-value pairs.
     * The returned array slice is allocated using the provided allocator.
     * The individual strings within the key-value pairs are also allocated using the same allocator.
     * For the key-value pairs, the `kvp` field contains the full "KEY=VALUE" string,
     * while the `key` and `value` fields are just 'views' into that string.
     */
    ArraySlice<_EnvVarKeyValuePair> _GetEnvironmentVariables(
        _Allocator allocator
    );

    /**
     * Opaque handle to a pipe.
     * On Windows, this is a HANDLE.
     * On Unix-like systems, this is a file descriptor (int).
     * This is used for inter-process communication (IPC).
     */
    struct _PipeHandle
    {
       u64 platformHandle;
    };

    /**
     * Creates a pipe and returns the read and write ends.
     * The read end is used for reading data from the pipe.
     * The write end is used for writing data to the pipe.
     */
    b8 _CreatePipe(
        _PipeHandle* outR,
        _PipeHandle* outW
    );

    /**
     * Gets the number of bytes available to read from the read end of the pipe.
     * The size is stored in `outSize`.
     *
     * Note that this function does not block, and the size may change after
     * this function returns.
     */
    b8 _GetRemainingPipeReadSize(
        _PipeHandle pipe,
        i64* outSize
    );

    /**
     * Reads data from the read end of the pipe into the provided buffer.
     * The number of bytes read is stored in `readSize` if it's not null.
     * Returns true on success, false on failure.
     */
    b8 _ReadFromPipe(
        _PipeHandle pipe,
        ArraySlice<u8> dst,
        i64* readSize = { }
    );

    /**
     * Writes data to the write end of the pipe from the provided buffer.
     * Returns true on success, false on failure.
     */
    b8 _WriteToPipe(
        _PipeHandle pipe,
        ArraySlice<u8> src
    );

    /**
     * Closes the pipe handle, releasing any associated resources.
     * After calling this function, the pipe handle should not be used again.
     */
    b8 _ClosePipe(
        _PipeHandle pipe
    );

    /**
     * A handle to a process.
     * The `pid` field is the process ID.
     * On Windows, this is `dwProcessId`.
     * On Unix-like systems, this is the PID.
     * The `handle` field is a platform-specific handle to the process.
     * On Windows, this is a HANDLE.
     * On Unix-like systems, this is pidfd.
     */
    struct _ProcessHandle
    {
       i64 pid;
       u64 handle;
    };

    /**
     * Starts a new process with the specified executable and arguments.
     * Optionally, environment variables, working directory, and pipes for
     * standard output and error can be provided.
     *
     * If not provided, environment variables and working directory are inherited
     * from the current process. If provided, they must be in a 'KEY=VALUE' format.
     *
     * The pipe handles provided must be read ends for stdout and stderr respectively.
     * If null, the respective output is discarded.
     */
    b8 _RunProcess(
        _ProcessHandle* outProcessHandle,
        ArraySlice<utf8str> execAndArgs,
        ArraySlice<utf8str> environmentVariables = { },
        _Path workingDirectory = { },
        _PipeHandle* stdOutPipe = { },
        _PipeHandle* stdErrPipe = { }
    );

    /**
     * Waits for the given process to exit and retrieves its exit code.
     *
     * Returns true if the process exited cleanly or false on failure.
     * The exit code is stored in *outExitCode if provided.
     */
    b8 _WaitForProcess(
        _ProcessHandle* process,
        i32* outExitCode = { }
    );

    /**
     * Kills the given process immediately.
     *
     * Returns true if the signal/termination request succeeded.
     */
    b8 _KillProcess(
        _ProcessHandle* process
    );

    /**
     * Closes the handle associated with the process.
     * Does not terminate or wait for the process.
     */
    void _CloseProcess(
        _ProcessHandle* process
    );

    // #######################################################################################
    // Network
    // #######################################################################################

    /**
     * Represents an IP address in binary form.
     * For IPv4, it's 4 bytes. For IPv6, it's 16 bytes.
     */
    typedef ArraySlice<u8> _IPAddress;

    /**
     * Represents a subnet mask in binary form.
     * For IPv4, it's 4 bytes. For IPv6, it's 16 bytes.
     */
    typedef ArraySlice<u8> _IPMask;

    /**
     * Represents an IP network, consisting of an IP address and a subnet mask.
     */
    struct _IPNetwork
    {
       _IPAddress address;
       _IPMask mask;
    };

    b8 _GetInterfaceIPAddresses(
        ArraySlice<_IPNetwork>* networks,
        _Allocator allocator
    );

    // #######################################################################################
    // Stream
    // #######################################################################################

    // Stream Declaration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Defines the mode to be used when calling the stream function.
     */
    enum class _StreamMode : u8 /* use as value */
    {
        GetSize = 0,
        GetCurrentPos = 1,
        SeekAbsolute = 2,
        SeekRelative = 3,
        Read = 4,
        Write = 5,
        Truncate = 6,
        Flush = 7,
        Close = 8,
    };

    /**
     * Defines the delegate type for the stream function
     */
    typedef b8 (*_StreamProcedure)(
        rawptr streamData,
        _StreamMode mode,
        ArraySlice<u8> data,
        i64 offset,
        i64* extraRet
    );

    /**
     * Defines a generic stream, that can be used for reading/writing data.
     */
    struct _Stream
    {
       _StreamProcedure procedure;
       rawptr data;
    };

    // Stream ease-of-use functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Gets the size of the stream.
     * Returns 0 on error.
     */
    i64 _GetSizeOfStream(
        _Stream stream
    );

    /**
     * Gets the current position in the stream.
     * Returns -1 on error.
     */
    i64 _GetCurrentPositionInStream(
        _Stream stream
    );

    /**
     * Seeks to a new position in the stream.
     * If 'relative' is true, the new position is relative to the current position.
     * If 'relative' is false, the new position is absolute from the start.
     * Returns true on success, false on failure.
     */
    b8 _SeekPositionInStream(
        _Stream stream,
        i64 newPos = { },
        b8 relative = { }
    );

    /**
     * Reads data from the stream into the provided buffer.
     * Optionally stores the number of bytes read.
     * Returns true on success, false on failure.
     */
    b8 _ReadFromStream(
        _Stream stream,
        ArraySlice<u8> dst,
        i64* readSize = { }
    );

    /**
     * Writes data from the provided buffer into the stream.
     * Returns true on success, false on failure.
     */
    b8 _WriteToStream(
        _Stream stream,
        ArraySlice<u8> src
    );

    /**
     * Formats a string and writes it to the stream.
     * Only supports primitives, for obvious reasons.
     * Use with `PNSLR_FmtB8`, `PNSLR_FmtI32`, etc.
     * Returns true on success, false on failure.
     */
    b8 _FormatAndWriteToStream(
        _Stream stream,
        utf8str fmtStr,
        ArraySlice<_PrimitiveFmtOptions> args
    );

    /**
     * Truncates the stream to the specified size.
     * Returns true on success, false on failure.
     */
    b8 _TruncateStream(
        _Stream stream,
        i64 newSize
    );

    /**
     * Flushes any buffered data to the stream.
     * Returns true on success, false on failure.
     */
    b8 _FlushStream(
        _Stream stream
    );

    /**
     * Closes the stream and frees any associated resources.
     */
    void _CloseStream(
        _Stream stream
    );

    // Stream casts ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Creates a stream from a file handle.
     */
    _Stream _StreamFromFile(
        _File file
    );

    /**
     * Creates a stream from a string builder.
     */
    _Stream _StreamFromStringBuilder(
        _StringBuilder* builder
    );

    /**
     * Creates a stream from the standard output pipe.
     */
    _Stream _StreamFromStdOut(
        b8 disableBuffering = { }
    );

    /**
     * Creates a stream from the standard error pipe.
     */
    _Stream _StreamFromStdErr(
        b8 disableBuffering = { }
    );

    // #######################################################################################
    // Logger
    // #######################################################################################

    // Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Defines the logging levels.
     */
    enum class _LoggerLevel : u8 /* use as value */
    {
        Debug = 0,
        Info = 1,
        Warn = 2,
        Error = 3,
        Critical = 4,
    };

    /**
     * Defines options for logging output.
     */
    enum class _LogOption : u8 /* use as flags */
    {
        None = 0,
        IncludeLevel = 1,
        IncludeDate = 2,
        IncludeTime = 4,
        IncludeFile = 8,
        IncludeFn = 16,
        IncludeColours = 32,
    };

    /**
     * Defines the delegate type for the logger function.
     */
    typedef void (*_LoggerProcedure)(
        rawptr loggerData,
        _LoggerLevel level,
        utf8str data,
        _LogOption options,
        _SourceCodeLocation location
    );

    /**
     * Defines a generic logger structure that can be used to log messages.
     */
    struct _Logger
    {
       _LoggerProcedure procedure;
       rawptr data;
       _LoggerLevel minAllowedLvl;
       _LogOption options;
    };

    // Default Logger Control ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Sets the default logger FOR THE CURRENT THREAD.
     * By default, every thread gets a thread-safe default logger that:
     * - logs to stdout on desktop platforms
     * - logs to logcat on Android
     */
    void _SetDefaultLogger(
        _Logger logger
    );

    /**
     * Disables the default logger FOR THE CURRENT THREAD.
     */
    void _DisableDefaultLogger();

    // Default Logger Non-Format Log Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void _LogD(
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogI(
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogW(
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogE(
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogC(
        utf8str msg,
        _SourceCodeLocation loc
    );

    // Default Logger Formatted Log Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void _LogDf(
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogIf(
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogWf(
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogEf(
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogCf(
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogLD(
        _Logger logger,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogLI(
        _Logger logger,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogLW(
        _Logger logger,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogLE(
        _Logger logger,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogLC(
        _Logger logger,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogLDf(
        _Logger logger,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogLIf(
        _Logger logger,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogLWf(
        _Logger logger,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogLEf(
        _Logger logger,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogLCf(
        _Logger logger,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    // Logger functions with explicit level ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void _Log(
        _LoggerLevel level,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _Logf(
        _LoggerLevel level,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    void _LogL(
        _Logger logger,
        _LoggerLevel level,
        utf8str msg,
        _SourceCodeLocation loc
    );

    void _LogLf(
        _Logger logger,
        _LoggerLevel level,
        utf8str fmtMsg,
        ArraySlice<_PrimitiveFmtOptions> args,
        _SourceCodeLocation loc
    );

    // Logger Casts ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Creates a logger that writes to the given file.
     * The file must be opened and valid.
     */
    _Logger _LoggerFromFile(
        _File f,
        _LoggerLevel minAllowedLevel,
        _LogOption options = { }
    );

    /**
     * Creates a logger that uses the default outputs (see `PNSLR_SetDefaultLogger()`).
     * The returned logger is thread-safe and can be used from any thread.
     * This can be used along with `PNSLR_SetDefaultLogger()` to customize
     * the behaviour of the default in-built logger.
     */
    _Logger _GetDefaultLoggerWithOptions(
        _LoggerLevel minAllowedLevel,
        _LogOption options = { }
    );

    /**
     * Creates a nil logger that does nothing.
     * This can be used to disable logging in certain parts of the code.
     */
    _Logger _GetNilLogger();

    // #######################################################################################
    // Threads
    // #######################################################################################

    /**
     * An opaque handle to a thread.
     */
    struct _ThreadHandle
    {
       u64 handle;
    };

    /**
     * Checks if the handle to a thread is valid.
     */
    b8 _IsThreadHandleValid(
        _ThreadHandle handle
    );

    /**
     * Gets a handle to the current thread.
     */
    _ThreadHandle _GetCurrentThreadHandle();

    /**
     * Gets the name of a thread.
     * The returned string is allocated using the provided allocator.
     * If the thread has no name, an empty string is returned.
     */
    utf8str _GetThreadName(
        _ThreadHandle handle,
        _Allocator allocator
    );

    /**
     * Sets the name of a thread.
     * The name is copied, so the provided string does not need to be valid after this call.
     * On some platforms, thread names may be truncated to a certain length.
     *
     * Thread lengths on platforms (excluding null terminator):
     *     Windows/OSX/iOS - 63 characters
     *     Linux/Android   - 15 characters
     */
    void _SetThreadName(
        _ThreadHandle handle,
        utf8str name
    );

    /**
     * Gets the name of the current thread.
     * Read more about `PNSLR_GetThreadName`.
     */
    utf8str _GetCurrentThreadName(
        _Allocator allocator
    );

    /**
     * Sets the name of the current thread.
     * Read more about `PNSLR_SetThreadName`.
     */
    void _SetCurrentThreadName(
        utf8str name
    );

    /**
     * A procedure that can be run on a thread.
     * The `data` parameter is optional user data that can be passed to the thread.
     */
    typedef void (*_ThreadProcedure)(
        rawptr data
    );

    /**
     * Start a new thread with the specified procedure and user data.
     */
    _ThreadHandle _StartThread(
        _ThreadProcedure procedure,
        rawptr data = { },
        utf8str name = { }
    );

    /**
     * Joins a thread, blocking the calling thread until the specified thread has finished.
     */
    void _JoinThread(
        _ThreadHandle handle
    );

    /**
     * Sleeps the current thread for the specified number of milliseconds.
     */
    void _SleepCurrentThread(
        u64 milliseconds
    );

    // #######################################################################################
    // SharedMemoryChannel
    // #######################################################################################

    // Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Opaque handle for a shared memory channel.
     */
    struct _SharedMemoryChannelHandle
    {
       i64 handle;
    };

    /**
     * Platform-specific header for a shared memory channel.
     */
    struct _SharedMemoryChannelPlatformHeader
    {
       u32 magicNum;
    };

    /**
     * Represents the status of a shared memory channel endpoint (reader or writer).
     */
    enum class _SharedMemoryChannelStatus : u8 /* use as value */
    {
        Disconnected = 0,
        Paused = 1,
        Active = 2,
    };

    /**
     * Header for a shared memory channel, containing metadata about the channel.
     */
    struct _SharedMemoryChannelHeader
    {
       u32 magicNum;
       u32 version;
       _SharedMemoryChannelStatus readerStatus;
       _SharedMemoryChannelStatus writerStatus;
       u32 offsetToOsSpecificHeader;
       u32 offsetToMsgQueueHeader;
       u32 offsetToMsgData;
       i64 fullMemRegionSize;
       i64 dataSize;
    };

    /**
     * Header for the message queue within a shared memory channel.
     */
    struct _SharedMemoryChannelMessageQueueHeader
    {
       i64 readCursor;
       u8 padding[56];
       i64 writeCursor;
    };

    /**
     * Represents a reader endpoint for a shared memory channel.
     */
    struct _SharedMemoryChannelReader
    {
       _SharedMemoryChannelHeader* header;
       _SharedMemoryChannelHandle handle;
    };

    /**
     * Represents a writer endpoint for a shared memory channel.
     */
    struct _SharedMemoryChannelWriter
    {
       _SharedMemoryChannelHeader* header;
       _SharedMemoryChannelHandle handle;
    };

    /**
     * Represents a reserved message slot for writing to a shared memory channel.
     */
    struct _SharedMemoryChannelReservedMessage
    {
       _SharedMemoryChannelWriter* channel;
       i64 offset;
       i64 size;
       u8* writePtr;
    };

    /**
     * Represents a message that has been read from a shared memory channel.
     */
    struct _SharedMemoryChannelMessage
    {
       _SharedMemoryChannelReader* channel;
       i64 offset;
       i64 size;
       u8* readPtr;
       i64 readSize;
    };

    // Reader Interface ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Creates a shared memory channel reader with the specified name and size.
     * The reader owns the shared memory segment and other processes can connect as writers.
     */
    b8 _CreateSharedMemoryChannelReader(
        utf8str name,
        i64 size,
        _SharedMemoryChannelReader* reader
    );

    /**
     * Polls for a message from the shared memory channel.
     * Returns true if a message was found, false otherwise.
     * Sets fatalError to true if an unrecoverable error occurred.
     */
    b8 _ReadSharedMemoryChannelMessage(
        _SharedMemoryChannelReader* reader,
        _SharedMemoryChannelMessage* message,
        b8* fatalError = { }
    );

    /**
     * Acknowledges that a message has been processed and advances the read cursor.
     */
    b8 _AcknowledgeSharedMemoryChannelMessage(
        _SharedMemoryChannelMessage* message
    );

    /**
     * Destroys a shared memory channel reader and releases all associated resources.
     */
    b8 _DestroySharedMemoryChannelReader(
        _SharedMemoryChannelReader* reader
    );

    // Writer Interface ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /**
     * Attempts to connect to an existing shared memory channel as a writer.
     * Returns true if successful, false if the channel doesn't exist or connection failed.
     */
    b8 _TryConnectSharedMemoryChannelWriter(
        utf8str name,
        _SharedMemoryChannelWriter* writer
    );

    /**
     * Reserves space for a message in the shared memory channel.
     * Returns true if space was available, false otherwise.
     */
    b8 _PrepareSharedMemoryChannelMessage(
        _SharedMemoryChannelWriter* writer,
        i64 size,
        _SharedMemoryChannelReservedMessage* reservedMessage
    );

    /**
     * Commits a previously reserved message to the shared memory channel.
     */
    b8 _CommitSharedMemoryChannelMessage(
        _SharedMemoryChannelWriter* writer,
        _SharedMemoryChannelReservedMessage reservedMessage
    );

    /**
     * Disconnects from a shared memory channel and releases writer resources.
     */
    b8 _DisconnectSharedMemoryChannelWriter(
        _SharedMemoryChannelWriter* writer
    );

    // #######################################################################################
    // Renderer
    // #######################################################################################

    /**
     * Defines the available Renderer types.
     */
    enum class RendererType : u8 /* use as value */
    {
        Null = 0,
        Vulkan = 1,
        DirectX12 = 2,
        Metal = 3,
    };

    /**
     * Defines the available texture formats.
     */
    enum class TextureFormat : u8 /* use as value */
    {
        Unknown = 0,
        D32_Float = 1,
        B8G8R8A8_UNorm = 2,
        R8G8B8A8_UNorm = 3,
        R16G16B16A16_UNorm = 4,
    };

    /**
     * Represents an opaque handle to the application instance.
     * Matches Dvaarpaal's app handle.
     * - On Windows, this is an HINSTANCE.
     * - On OSX, this is an NSApplication*.
     * - On Android, this is a struct android_app*.
     */
    struct AppHandle
    {
       u64 handle;
    };

    /**
     * Represents an opaque handle to a window.
     * Matches Dvaarpaal's window handle.
     * - On Windows, this is an HWND.
     * - On OSX, this is an NSWindow*.
     * - On Android, this is an ANativeWindow*.
     */
    struct WindowHandle
    {
       u64 handle;
    };

    /**
     * Configuration structure for renderer creation.
     */
    struct RendererConfiguration
    {
       RendererType type;
       AppHandle appHandle;
       _Allocator allocator;
       utf8str appName;
    };

    /**
     * The main renderer interface.
     * Treat as base class for specific renderer implementations.
     */
    struct Renderer
    {
       RendererType type;
       _Allocator allocator;
       AppHandle appHandle;
    };

    /**
     * MAIN_THREAD
     * Creates a renderer instance based on the provided configuration.
     */
    Renderer* CreateRenderer(
        RendererConfiguration config,
        _Allocator tempAllocator
    );

    /**
     * MAIN_THREAD
     * Destroys the given renderer instance and frees associated resources.
     */
    b8 DestroyRenderer(
        Renderer* renderer,
        _Allocator tempAllocator
    );

    /**
     * MAIN_THREAD
     * Halts the thread until all the work that's already submitted to the renderer is done.
     * Return value is insignificant.
     *
     * Warning! - it won't necessarily account for work that gets submitted after this on other threads.
     */
    b8 WaitTillRendererIdle(
        Renderer* renderer
    );

    /**
     * A command buffer for recording rendering commands.
     */
    struct RendererCommandBuffer
    {
       RendererType type;
    };

    /**
     * A surface that can be drawn on top of.
     */
    struct RendererSurface
    {
       RendererType type;
    };

    /**
     * Creates a rendering surface for the given window and application handles.
     * Uses 3 frames in flight.
     */
    RendererSurface* CreateRendererSurfaceFromWindow(
        Renderer* renderer,
        WindowHandle windowHandle,
        _Allocator tempAllocator
    );

    /**
     * Destroys the given rendering surface and frees associated resources.
     */
    b8 DestroyRendererSurface(
        RendererSurface* surface,
        _Allocator tempAllocator
    );

    /**
     * Handles resizing the given rendering surface to the specified dimensions.
     */
    b8 ResizeRendererSurface(
        RendererSurface* surface,
        u16 width,
        u16 height,
        _Allocator tempAllocator
    );

    /**
     * Begins a new frame for the given rendering surface, returning a command buffer for recording commands.
     * The returned command buffer must be ended with MZNT_EndFrame (to submit drawing for the current frame).
     */
    RendererCommandBuffer* BeginFrame(
        RendererSurface* surface,
        f32 r,
        f32 g,
        f32 b,
        f32 a,
        _Allocator tempAllocator
    );

    /**
     * Ends the current frame for the given rendering surface, submitting recorded commands for execution.
     */
    b8 EndFrame(
        RendererSurface* surface,
        _Allocator tempAllocator
    );

    /**
     * Configuration structure for swap-chain.
     */
    struct SwapChainConfiguration
    {
       u16 width;
       u16 height;
       b8 vSync;
       u8 framesInFlight;
    };

    /**
     * A swap-chain corresponding to a window that can be rendered to.
     */
    struct SwapChain
    {
       RendererType type;
    };

    /**
     * MAIN_THREAD
     * Create a swap-chain for the given window, with the given renderer, for a custom number of frames in flight.
     * If width/height are uninitialised, they will be derived from the window.
     */
    SwapChain* CreateSwapChainFromWindow(
        Renderer* renderer,
        WindowHandle windowHandle,
        SwapChainConfiguration cfg,
        _Allocator tempAllocator
    );

    /**
     * MAIN_THREAD
     * Reconfigure a swap-chain with new properties.
     * All configuration values must be initialised appropriately.
     */
    b8 ReconfigureSwapChain(
        SwapChain* swapChain,
        SwapChainConfiguration cfg,
        _Allocator tempAllocator
    );

    /**
     * MAIN_THREAD
     * Destroy the swap-chain, freeing up associated resources.
     */
    b8 DestroySwapChain(
        SwapChain* swapChain,
        _Allocator tempAllocator
    );

    /**
     * MAIN_THREAD
     * Get the texture format of the swap-chain.
     */
    TextureFormat GetSwapChainTextureFormat(
        SwapChain* swapChain
    );

    /**
     * MAIN_THREAD
     * Acquire the next image in the swap-chain, and get its associated command buffer
     * Optionally, also get the image index, to cross-check with your own custom
     * per-frame-in-flight allocated resources.
     *
     * Warning! - the output could be nil, in some cases (like window is minimsed).
     * In this case, image index will be 255.
     */
    RendererCommandBuffer* IterateSwapChain(
        SwapChain* swapChain,
        u8* outImgIdx,
        _Allocator tempAllocator
    );

    /**
     * RENDER_THREAD
     * Wrap up recording commands for the swap-chain and submit the current image
     * for presenting.
     */
    b8 PresentSwapChain(
        SwapChain* swapChain,
        _Allocator tempAllocator
    );

} // namespace end

#endif//MZNT_CXX_MAIN_H
#ifdef PNSLR_CXX_IMPL

#if (_MSC_VER)
    #define PNSLR_STRUCT_OFFSET(type, member) ((u64)&reinterpret_cast<char const volatile&>((((type*)0)->member)))
#elif (__clang__) || (__GNUC__)
    #define PNSLR_STRUCT_OFFSET(type, member) ((u64) offsetof(type, member))
#else
    #error "UNSUPPORTED COMPILER!";
#endif

struct PNSLR_RawArraySlice
{
   rawptr data;
   i64 count;
};
static_assert(sizeof(PNSLR_RawArraySlice) == sizeof(Muzent::_RawArraySlice), "size mismatch");
static_assert(alignof(PNSLR_RawArraySlice) == alignof(Muzent::_RawArraySlice), "align mismatch");
PNSLR_RawArraySlice* PNSLR_Bindings_Convert(Muzent::_RawArraySlice* x) { return reinterpret_cast<PNSLR_RawArraySlice*>(x); }
Muzent::_RawArraySlice* PNSLR_Bindings_Convert(PNSLR_RawArraySlice* x) { return reinterpret_cast<Muzent::_RawArraySlice*>(x); }
PNSLR_RawArraySlice& PNSLR_Bindings_Convert(Muzent::_RawArraySlice& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_RawArraySlice& PNSLR_Bindings_Convert(PNSLR_RawArraySlice& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_RawArraySlice, data) == PNSLR_STRUCT_OFFSET(Muzent::_RawArraySlice, data), "data offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_RawArraySlice, count) == PNSLR_STRUCT_OFFSET(Muzent::_RawArraySlice, count), "count offset mismatch");

typedef struct { b8* data; i64 count; } PNSLR_ArraySlice_b8;
static_assert(sizeof(PNSLR_ArraySlice_b8) == sizeof(ArraySlice<b8>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_b8) == alignof(ArraySlice<b8>), "align mismatch");
PNSLR_ArraySlice_b8* PNSLR_Bindings_Convert(ArraySlice<b8>* x) { return reinterpret_cast<PNSLR_ArraySlice_b8*>(x); }
ArraySlice<b8>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_b8* x) { return reinterpret_cast<ArraySlice<b8>*>(x); }
PNSLR_ArraySlice_b8& PNSLR_Bindings_Convert(ArraySlice<b8>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<b8>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_b8& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_b8, count) == PNSLR_STRUCT_OFFSET(ArraySlice<b8>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_b8, data) == PNSLR_STRUCT_OFFSET(ArraySlice<b8>, data), "data offset mismatch");

typedef struct { u8* data; i64 count; } PNSLR_ArraySlice_u8;
static_assert(sizeof(PNSLR_ArraySlice_u8) == sizeof(ArraySlice<u8>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_u8) == alignof(ArraySlice<u8>), "align mismatch");
PNSLR_ArraySlice_u8* PNSLR_Bindings_Convert(ArraySlice<u8>* x) { return reinterpret_cast<PNSLR_ArraySlice_u8*>(x); }
ArraySlice<u8>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_u8* x) { return reinterpret_cast<ArraySlice<u8>*>(x); }
PNSLR_ArraySlice_u8& PNSLR_Bindings_Convert(ArraySlice<u8>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<u8>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_u8& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u8, count) == PNSLR_STRUCT_OFFSET(ArraySlice<u8>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u8, data) == PNSLR_STRUCT_OFFSET(ArraySlice<u8>, data), "data offset mismatch");

typedef struct { u16* data; i64 count; } PNSLR_ArraySlice_u16;
static_assert(sizeof(PNSLR_ArraySlice_u16) == sizeof(ArraySlice<u16>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_u16) == alignof(ArraySlice<u16>), "align mismatch");
PNSLR_ArraySlice_u16* PNSLR_Bindings_Convert(ArraySlice<u16>* x) { return reinterpret_cast<PNSLR_ArraySlice_u16*>(x); }
ArraySlice<u16>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_u16* x) { return reinterpret_cast<ArraySlice<u16>*>(x); }
PNSLR_ArraySlice_u16& PNSLR_Bindings_Convert(ArraySlice<u16>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<u16>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_u16& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u16, count) == PNSLR_STRUCT_OFFSET(ArraySlice<u16>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u16, data) == PNSLR_STRUCT_OFFSET(ArraySlice<u16>, data), "data offset mismatch");

typedef struct { u32* data; i64 count; } PNSLR_ArraySlice_u32;
static_assert(sizeof(PNSLR_ArraySlice_u32) == sizeof(ArraySlice<u32>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_u32) == alignof(ArraySlice<u32>), "align mismatch");
PNSLR_ArraySlice_u32* PNSLR_Bindings_Convert(ArraySlice<u32>* x) { return reinterpret_cast<PNSLR_ArraySlice_u32*>(x); }
ArraySlice<u32>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_u32* x) { return reinterpret_cast<ArraySlice<u32>*>(x); }
PNSLR_ArraySlice_u32& PNSLR_Bindings_Convert(ArraySlice<u32>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<u32>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_u32& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u32, count) == PNSLR_STRUCT_OFFSET(ArraySlice<u32>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u32, data) == PNSLR_STRUCT_OFFSET(ArraySlice<u32>, data), "data offset mismatch");

typedef struct { u64* data; i64 count; } PNSLR_ArraySlice_u64;
static_assert(sizeof(PNSLR_ArraySlice_u64) == sizeof(ArraySlice<u64>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_u64) == alignof(ArraySlice<u64>), "align mismatch");
PNSLR_ArraySlice_u64* PNSLR_Bindings_Convert(ArraySlice<u64>* x) { return reinterpret_cast<PNSLR_ArraySlice_u64*>(x); }
ArraySlice<u64>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_u64* x) { return reinterpret_cast<ArraySlice<u64>*>(x); }
PNSLR_ArraySlice_u64& PNSLR_Bindings_Convert(ArraySlice<u64>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<u64>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_u64& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u64, count) == PNSLR_STRUCT_OFFSET(ArraySlice<u64>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_u64, data) == PNSLR_STRUCT_OFFSET(ArraySlice<u64>, data), "data offset mismatch");

typedef struct { i8* data; i64 count; } PNSLR_ArraySlice_i8;
static_assert(sizeof(PNSLR_ArraySlice_i8) == sizeof(ArraySlice<i8>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_i8) == alignof(ArraySlice<i8>), "align mismatch");
PNSLR_ArraySlice_i8* PNSLR_Bindings_Convert(ArraySlice<i8>* x) { return reinterpret_cast<PNSLR_ArraySlice_i8*>(x); }
ArraySlice<i8>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_i8* x) { return reinterpret_cast<ArraySlice<i8>*>(x); }
PNSLR_ArraySlice_i8& PNSLR_Bindings_Convert(ArraySlice<i8>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<i8>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_i8& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i8, count) == PNSLR_STRUCT_OFFSET(ArraySlice<i8>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i8, data) == PNSLR_STRUCT_OFFSET(ArraySlice<i8>, data), "data offset mismatch");

typedef struct { i16* data; i64 count; } PNSLR_ArraySlice_i16;
static_assert(sizeof(PNSLR_ArraySlice_i16) == sizeof(ArraySlice<i16>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_i16) == alignof(ArraySlice<i16>), "align mismatch");
PNSLR_ArraySlice_i16* PNSLR_Bindings_Convert(ArraySlice<i16>* x) { return reinterpret_cast<PNSLR_ArraySlice_i16*>(x); }
ArraySlice<i16>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_i16* x) { return reinterpret_cast<ArraySlice<i16>*>(x); }
PNSLR_ArraySlice_i16& PNSLR_Bindings_Convert(ArraySlice<i16>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<i16>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_i16& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i16, count) == PNSLR_STRUCT_OFFSET(ArraySlice<i16>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i16, data) == PNSLR_STRUCT_OFFSET(ArraySlice<i16>, data), "data offset mismatch");

typedef struct { i32* data; i64 count; } PNSLR_ArraySlice_i32;
static_assert(sizeof(PNSLR_ArraySlice_i32) == sizeof(ArraySlice<i32>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_i32) == alignof(ArraySlice<i32>), "align mismatch");
PNSLR_ArraySlice_i32* PNSLR_Bindings_Convert(ArraySlice<i32>* x) { return reinterpret_cast<PNSLR_ArraySlice_i32*>(x); }
ArraySlice<i32>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_i32* x) { return reinterpret_cast<ArraySlice<i32>*>(x); }
PNSLR_ArraySlice_i32& PNSLR_Bindings_Convert(ArraySlice<i32>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<i32>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_i32& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i32, count) == PNSLR_STRUCT_OFFSET(ArraySlice<i32>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i32, data) == PNSLR_STRUCT_OFFSET(ArraySlice<i32>, data), "data offset mismatch");

typedef struct { i64* data; i64 count; } PNSLR_ArraySlice_i64;
static_assert(sizeof(PNSLR_ArraySlice_i64) == sizeof(ArraySlice<i64>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_i64) == alignof(ArraySlice<i64>), "align mismatch");
PNSLR_ArraySlice_i64* PNSLR_Bindings_Convert(ArraySlice<i64>* x) { return reinterpret_cast<PNSLR_ArraySlice_i64*>(x); }
ArraySlice<i64>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_i64* x) { return reinterpret_cast<ArraySlice<i64>*>(x); }
PNSLR_ArraySlice_i64& PNSLR_Bindings_Convert(ArraySlice<i64>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<i64>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_i64& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i64, count) == PNSLR_STRUCT_OFFSET(ArraySlice<i64>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_i64, data) == PNSLR_STRUCT_OFFSET(ArraySlice<i64>, data), "data offset mismatch");

typedef struct { f32* data; i64 count; } PNSLR_ArraySlice_f32;
static_assert(sizeof(PNSLR_ArraySlice_f32) == sizeof(ArraySlice<f32>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_f32) == alignof(ArraySlice<f32>), "align mismatch");
PNSLR_ArraySlice_f32* PNSLR_Bindings_Convert(ArraySlice<f32>* x) { return reinterpret_cast<PNSLR_ArraySlice_f32*>(x); }
ArraySlice<f32>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_f32* x) { return reinterpret_cast<ArraySlice<f32>*>(x); }
PNSLR_ArraySlice_f32& PNSLR_Bindings_Convert(ArraySlice<f32>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<f32>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_f32& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_f32, count) == PNSLR_STRUCT_OFFSET(ArraySlice<f32>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_f32, data) == PNSLR_STRUCT_OFFSET(ArraySlice<f32>, data), "data offset mismatch");

typedef struct { f64* data; i64 count; } PNSLR_ArraySlice_f64;
static_assert(sizeof(PNSLR_ArraySlice_f64) == sizeof(ArraySlice<f64>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_f64) == alignof(ArraySlice<f64>), "align mismatch");
PNSLR_ArraySlice_f64* PNSLR_Bindings_Convert(ArraySlice<f64>* x) { return reinterpret_cast<PNSLR_ArraySlice_f64*>(x); }
ArraySlice<f64>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_f64* x) { return reinterpret_cast<ArraySlice<f64>*>(x); }
PNSLR_ArraySlice_f64& PNSLR_Bindings_Convert(ArraySlice<f64>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<f64>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_f64& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_f64, count) == PNSLR_STRUCT_OFFSET(ArraySlice<f64>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_f64, data) == PNSLR_STRUCT_OFFSET(ArraySlice<f64>, data), "data offset mismatch");

typedef struct { char* data; i64 count; } PNSLR_ArraySlice_char;
static_assert(sizeof(PNSLR_ArraySlice_char) == sizeof(ArraySlice<char>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_char) == alignof(ArraySlice<char>), "align mismatch");
PNSLR_ArraySlice_char* PNSLR_Bindings_Convert(ArraySlice<char>* x) { return reinterpret_cast<PNSLR_ArraySlice_char*>(x); }
ArraySlice<char>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_char* x) { return reinterpret_cast<ArraySlice<char>*>(x); }
PNSLR_ArraySlice_char& PNSLR_Bindings_Convert(ArraySlice<char>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<char>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_char& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_char, count) == PNSLR_STRUCT_OFFSET(ArraySlice<char>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_char, data) == PNSLR_STRUCT_OFFSET(ArraySlice<char>, data), "data offset mismatch");

typedef PNSLR_ArraySlice_u8 PNSLR_UTF8STR;

typedef struct { PNSLR_UTF8STR* data; i64 count; } PNSLR_ArraySlice_PNSLR_UTF8STR;
static_assert(sizeof(PNSLR_ArraySlice_PNSLR_UTF8STR) == sizeof(ArraySlice<utf8str>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_PNSLR_UTF8STR) == alignof(ArraySlice<utf8str>), "align mismatch");
PNSLR_ArraySlice_PNSLR_UTF8STR* PNSLR_Bindings_Convert(ArraySlice<utf8str>* x) { return reinterpret_cast<PNSLR_ArraySlice_PNSLR_UTF8STR*>(x); }
ArraySlice<utf8str>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_UTF8STR* x) { return reinterpret_cast<ArraySlice<utf8str>*>(x); }
PNSLR_ArraySlice_PNSLR_UTF8STR& PNSLR_Bindings_Convert(ArraySlice<utf8str>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<utf8str>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_UTF8STR& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_UTF8STR, count) == PNSLR_STRUCT_OFFSET(ArraySlice<utf8str>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_UTF8STR, data) == PNSLR_STRUCT_OFFSET(ArraySlice<utf8str>, data), "data offset mismatch");

enum class PNSLR_Platform : u8 { };
static_assert(sizeof(PNSLR_Platform) == sizeof(Muzent::_Platform), "size mismatch");
static_assert(alignof(PNSLR_Platform) == alignof(Muzent::_Platform), "align mismatch");
PNSLR_Platform* PNSLR_Bindings_Convert(Muzent::_Platform* x) { return reinterpret_cast<PNSLR_Platform*>(x); }
Muzent::_Platform* PNSLR_Bindings_Convert(PNSLR_Platform* x) { return reinterpret_cast<Muzent::_Platform*>(x); }
PNSLR_Platform& PNSLR_Bindings_Convert(Muzent::_Platform& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Platform& PNSLR_Bindings_Convert(PNSLR_Platform& x) { return *PNSLR_Bindings_Convert(&x); }

enum class PNSLR_Architecture : u8 { };
static_assert(sizeof(PNSLR_Architecture) == sizeof(Muzent::_Architecture), "size mismatch");
static_assert(alignof(PNSLR_Architecture) == alignof(Muzent::_Architecture), "align mismatch");
PNSLR_Architecture* PNSLR_Bindings_Convert(Muzent::_Architecture* x) { return reinterpret_cast<PNSLR_Architecture*>(x); }
Muzent::_Architecture* PNSLR_Bindings_Convert(PNSLR_Architecture* x) { return reinterpret_cast<Muzent::_Architecture*>(x); }
PNSLR_Architecture& PNSLR_Bindings_Convert(Muzent::_Architecture& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Architecture& PNSLR_Bindings_Convert(PNSLR_Architecture& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" PNSLR_Platform PNSLR_GetPlatform();
Muzent::_Platform Muzent::_GetPlatform()
{
    PNSLR_Platform zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetPlatform(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Architecture PNSLR_GetArchitecture();
Muzent::_Architecture Muzent::_GetArchitecture()
{
    PNSLR_Architecture zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetArchitecture(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_SourceCodeLocation
{
   PNSLR_UTF8STR file;
   i32 line;
   i32 column;
   PNSLR_UTF8STR function;
};
static_assert(sizeof(PNSLR_SourceCodeLocation) == sizeof(Muzent::_SourceCodeLocation), "size mismatch");
static_assert(alignof(PNSLR_SourceCodeLocation) == alignof(Muzent::_SourceCodeLocation), "align mismatch");
PNSLR_SourceCodeLocation* PNSLR_Bindings_Convert(Muzent::_SourceCodeLocation* x) { return reinterpret_cast<PNSLR_SourceCodeLocation*>(x); }
Muzent::_SourceCodeLocation* PNSLR_Bindings_Convert(PNSLR_SourceCodeLocation* x) { return reinterpret_cast<Muzent::_SourceCodeLocation*>(x); }
PNSLR_SourceCodeLocation& PNSLR_Bindings_Convert(Muzent::_SourceCodeLocation& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SourceCodeLocation& PNSLR_Bindings_Convert(PNSLR_SourceCodeLocation& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SourceCodeLocation, file) == PNSLR_STRUCT_OFFSET(Muzent::_SourceCodeLocation, file), "file offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SourceCodeLocation, line) == PNSLR_STRUCT_OFFSET(Muzent::_SourceCodeLocation, line), "line offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SourceCodeLocation, column) == PNSLR_STRUCT_OFFSET(Muzent::_SourceCodeLocation, column), "column offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SourceCodeLocation, function) == PNSLR_STRUCT_OFFSET(Muzent::_SourceCodeLocation, function), "function offset mismatch");

struct alignas(8) PNSLR_Mutex
{
   u8 buffer[64];
};
static_assert(sizeof(PNSLR_Mutex) == sizeof(Muzent::_Mutex), "size mismatch");
static_assert(alignof(PNSLR_Mutex) == alignof(Muzent::_Mutex), "align mismatch");
PNSLR_Mutex* PNSLR_Bindings_Convert(Muzent::_Mutex* x) { return reinterpret_cast<PNSLR_Mutex*>(x); }
Muzent::_Mutex* PNSLR_Bindings_Convert(PNSLR_Mutex* x) { return reinterpret_cast<Muzent::_Mutex*>(x); }
PNSLR_Mutex& PNSLR_Bindings_Convert(Muzent::_Mutex& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Mutex& PNSLR_Bindings_Convert(PNSLR_Mutex& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Mutex, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_Mutex, buffer), "buffer offset mismatch");

extern "C" PNSLR_Mutex PNSLR_CreateMutex();
Muzent::_Mutex Muzent::_CreateMutex()
{
    PNSLR_Mutex zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateMutex(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroyMutex(PNSLR_Mutex* mutex);
void Muzent::_DestroyMutex(Muzent::_Mutex* mutex)
{
    PNSLR_DestroyMutex(PNSLR_Bindings_Convert(mutex));
}

extern "C" void PNSLR_LockMutex(PNSLR_Mutex* mutex);
void Muzent::_LockMutex(Muzent::_Mutex* mutex)
{
    PNSLR_LockMutex(PNSLR_Bindings_Convert(mutex));
}

extern "C" void PNSLR_UnlockMutex(PNSLR_Mutex* mutex);
void Muzent::_UnlockMutex(Muzent::_Mutex* mutex)
{
    PNSLR_UnlockMutex(PNSLR_Bindings_Convert(mutex));
}

extern "C" b8 PNSLR_TryLockMutex(PNSLR_Mutex* mutex);
b8 Muzent::_TryLockMutex(Muzent::_Mutex* mutex)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_TryLockMutex(PNSLR_Bindings_Convert(mutex)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct alignas(8) PNSLR_RWMutex
{
   u8 buffer[200];
};
static_assert(sizeof(PNSLR_RWMutex) == sizeof(Muzent::_RWMutex), "size mismatch");
static_assert(alignof(PNSLR_RWMutex) == alignof(Muzent::_RWMutex), "align mismatch");
PNSLR_RWMutex* PNSLR_Bindings_Convert(Muzent::_RWMutex* x) { return reinterpret_cast<PNSLR_RWMutex*>(x); }
Muzent::_RWMutex* PNSLR_Bindings_Convert(PNSLR_RWMutex* x) { return reinterpret_cast<Muzent::_RWMutex*>(x); }
PNSLR_RWMutex& PNSLR_Bindings_Convert(Muzent::_RWMutex& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_RWMutex& PNSLR_Bindings_Convert(PNSLR_RWMutex& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_RWMutex, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_RWMutex, buffer), "buffer offset mismatch");

extern "C" PNSLR_RWMutex PNSLR_CreateRWMutex();
Muzent::_RWMutex Muzent::_CreateRWMutex()
{
    PNSLR_RWMutex zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateRWMutex(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroyRWMutex(PNSLR_RWMutex* rwmutex);
void Muzent::_DestroyRWMutex(Muzent::_RWMutex* rwmutex)
{
    PNSLR_DestroyRWMutex(PNSLR_Bindings_Convert(rwmutex));
}

extern "C" void PNSLR_LockRWMutexShared(PNSLR_RWMutex* rwmutex);
void Muzent::_LockRWMutexShared(Muzent::_RWMutex* rwmutex)
{
    PNSLR_LockRWMutexShared(PNSLR_Bindings_Convert(rwmutex));
}

extern "C" void PNSLR_LockRWMutexExclusive(PNSLR_RWMutex* rwmutex);
void Muzent::_LockRWMutexExclusive(Muzent::_RWMutex* rwmutex)
{
    PNSLR_LockRWMutexExclusive(PNSLR_Bindings_Convert(rwmutex));
}

extern "C" void PNSLR_UnlockRWMutexShared(PNSLR_RWMutex* rwmutex);
void Muzent::_UnlockRWMutexShared(Muzent::_RWMutex* rwmutex)
{
    PNSLR_UnlockRWMutexShared(PNSLR_Bindings_Convert(rwmutex));
}

extern "C" void PNSLR_UnlockRWMutexExclusive(PNSLR_RWMutex* rwmutex);
void Muzent::_UnlockRWMutexExclusive(Muzent::_RWMutex* rwmutex)
{
    PNSLR_UnlockRWMutexExclusive(PNSLR_Bindings_Convert(rwmutex));
}

extern "C" b8 PNSLR_TryLockRWMutexShared(PNSLR_RWMutex* rwmutex);
b8 Muzent::_TryLockRWMutexShared(Muzent::_RWMutex* rwmutex)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_TryLockRWMutexShared(PNSLR_Bindings_Convert(rwmutex)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_TryLockRWMutexExclusive(PNSLR_RWMutex* rwmutex);
b8 Muzent::_TryLockRWMutexExclusive(Muzent::_RWMutex* rwmutex)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_TryLockRWMutexExclusive(PNSLR_Bindings_Convert(rwmutex)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct alignas(8) PNSLR_Semaphore
{
   u8 buffer[32];
};
static_assert(sizeof(PNSLR_Semaphore) == sizeof(Muzent::_Semaphore), "size mismatch");
static_assert(alignof(PNSLR_Semaphore) == alignof(Muzent::_Semaphore), "align mismatch");
PNSLR_Semaphore* PNSLR_Bindings_Convert(Muzent::_Semaphore* x) { return reinterpret_cast<PNSLR_Semaphore*>(x); }
Muzent::_Semaphore* PNSLR_Bindings_Convert(PNSLR_Semaphore* x) { return reinterpret_cast<Muzent::_Semaphore*>(x); }
PNSLR_Semaphore& PNSLR_Bindings_Convert(Muzent::_Semaphore& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Semaphore& PNSLR_Bindings_Convert(PNSLR_Semaphore& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Semaphore, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_Semaphore, buffer), "buffer offset mismatch");

extern "C" PNSLR_Semaphore PNSLR_CreateSemaphore(i32 initialCount);
Muzent::_Semaphore Muzent::_CreateSemaphore(i32 initialCount)
{
    PNSLR_Semaphore zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateSemaphore(PNSLR_Bindings_Convert(initialCount)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroySemaphore(PNSLR_Semaphore* semaphore);
void Muzent::_DestroySemaphore(Muzent::_Semaphore* semaphore)
{
    PNSLR_DestroySemaphore(PNSLR_Bindings_Convert(semaphore));
}

extern "C" void PNSLR_WaitSemaphore(PNSLR_Semaphore* semaphore);
void Muzent::_WaitSemaphore(Muzent::_Semaphore* semaphore)
{
    PNSLR_WaitSemaphore(PNSLR_Bindings_Convert(semaphore));
}

extern "C" b8 PNSLR_WaitSemaphoreTimeout(PNSLR_Semaphore* semaphore, i32 timeoutNs);
b8 Muzent::_WaitSemaphoreTimeout(Muzent::_Semaphore* semaphore, i32 timeoutNs)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WaitSemaphoreTimeout(PNSLR_Bindings_Convert(semaphore), PNSLR_Bindings_Convert(timeoutNs)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_SignalSemaphore(PNSLR_Semaphore* semaphore, i32 count);
void Muzent::_SignalSemaphore(Muzent::_Semaphore* semaphore, i32 count)
{
    PNSLR_SignalSemaphore(PNSLR_Bindings_Convert(semaphore), PNSLR_Bindings_Convert(count));
}

struct alignas(8) PNSLR_ConditionVariable
{
   u8 buffer[48];
};
static_assert(sizeof(PNSLR_ConditionVariable) == sizeof(Muzent::_ConditionVariable), "size mismatch");
static_assert(alignof(PNSLR_ConditionVariable) == alignof(Muzent::_ConditionVariable), "align mismatch");
PNSLR_ConditionVariable* PNSLR_Bindings_Convert(Muzent::_ConditionVariable* x) { return reinterpret_cast<PNSLR_ConditionVariable*>(x); }
Muzent::_ConditionVariable* PNSLR_Bindings_Convert(PNSLR_ConditionVariable* x) { return reinterpret_cast<Muzent::_ConditionVariable*>(x); }
PNSLR_ConditionVariable& PNSLR_Bindings_Convert(Muzent::_ConditionVariable& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ConditionVariable& PNSLR_Bindings_Convert(PNSLR_ConditionVariable& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ConditionVariable, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_ConditionVariable, buffer), "buffer offset mismatch");

extern "C" PNSLR_ConditionVariable PNSLR_CreateConditionVariable();
Muzent::_ConditionVariable Muzent::_CreateConditionVariable()
{
    PNSLR_ConditionVariable zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateConditionVariable(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroyConditionVariable(PNSLR_ConditionVariable* condvar);
void Muzent::_DestroyConditionVariable(Muzent::_ConditionVariable* condvar)
{
    PNSLR_DestroyConditionVariable(PNSLR_Bindings_Convert(condvar));
}

extern "C" void PNSLR_WaitConditionVariable(PNSLR_ConditionVariable* condvar, PNSLR_Mutex* mutex);
void Muzent::_WaitConditionVariable(Muzent::_ConditionVariable* condvar, Muzent::_Mutex* mutex)
{
    PNSLR_WaitConditionVariable(PNSLR_Bindings_Convert(condvar), PNSLR_Bindings_Convert(mutex));
}

extern "C" b8 PNSLR_WaitConditionVariableTimeout(PNSLR_ConditionVariable* condvar, PNSLR_Mutex* mutex, i32 timeoutNs);
b8 Muzent::_WaitConditionVariableTimeout(Muzent::_ConditionVariable* condvar, Muzent::_Mutex* mutex, i32 timeoutNs)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WaitConditionVariableTimeout(PNSLR_Bindings_Convert(condvar), PNSLR_Bindings_Convert(mutex), PNSLR_Bindings_Convert(timeoutNs)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_SignalConditionVariable(PNSLR_ConditionVariable* condvar);
void Muzent::_SignalConditionVariable(Muzent::_ConditionVariable* condvar)
{
    PNSLR_SignalConditionVariable(PNSLR_Bindings_Convert(condvar));
}

extern "C" void PNSLR_BroadcastConditionVariable(PNSLR_ConditionVariable* condvar);
void Muzent::_BroadcastConditionVariable(Muzent::_ConditionVariable* condvar)
{
    PNSLR_BroadcastConditionVariable(PNSLR_Bindings_Convert(condvar));
}

struct alignas(8) PNSLR_DoOnce
{
   u8 buffer[16];
};
static_assert(sizeof(PNSLR_DoOnce) == sizeof(Muzent::_DoOnce), "size mismatch");
static_assert(alignof(PNSLR_DoOnce) == alignof(Muzent::_DoOnce), "align mismatch");
PNSLR_DoOnce* PNSLR_Bindings_Convert(Muzent::_DoOnce* x) { return reinterpret_cast<PNSLR_DoOnce*>(x); }
Muzent::_DoOnce* PNSLR_Bindings_Convert(PNSLR_DoOnce* x) { return reinterpret_cast<Muzent::_DoOnce*>(x); }
PNSLR_DoOnce& PNSLR_Bindings_Convert(Muzent::_DoOnce& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_DoOnce& PNSLR_Bindings_Convert(PNSLR_DoOnce& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_DoOnce, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_DoOnce, buffer), "buffer offset mismatch");

extern "C" typedef void (*PNSLR_DoOnceCallback)();
static_assert(sizeof(PNSLR_DoOnceCallback) == sizeof(Muzent::_DoOnceCallback), "size mismatch");
static_assert(alignof(PNSLR_DoOnceCallback) == alignof(Muzent::_DoOnceCallback), "align mismatch");
PNSLR_DoOnceCallback* PNSLR_Bindings_Convert(Muzent::_DoOnceCallback* x) { return reinterpret_cast<PNSLR_DoOnceCallback*>(x); }
Muzent::_DoOnceCallback* PNSLR_Bindings_Convert(PNSLR_DoOnceCallback* x) { return reinterpret_cast<Muzent::_DoOnceCallback*>(x); }
PNSLR_DoOnceCallback& PNSLR_Bindings_Convert(Muzent::_DoOnceCallback& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_DoOnceCallback& PNSLR_Bindings_Convert(PNSLR_DoOnceCallback& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" void PNSLR_ExecuteDoOnce(PNSLR_DoOnce* once, PNSLR_DoOnceCallback callback);
void Muzent::_ExecuteDoOnce(Muzent::_DoOnce* once, Muzent::_DoOnceCallback callback)
{
    PNSLR_ExecuteDoOnce(PNSLR_Bindings_Convert(once), PNSLR_Bindings_Convert(callback));
}

struct alignas(8) PNSLR_Event
{
   u8 buffer[112];
};
static_assert(sizeof(PNSLR_Event) == sizeof(Muzent::_Event), "size mismatch");
static_assert(alignof(PNSLR_Event) == alignof(Muzent::_Event), "align mismatch");
PNSLR_Event* PNSLR_Bindings_Convert(Muzent::_Event* x) { return reinterpret_cast<PNSLR_Event*>(x); }
Muzent::_Event* PNSLR_Bindings_Convert(PNSLR_Event* x) { return reinterpret_cast<Muzent::_Event*>(x); }
PNSLR_Event& PNSLR_Bindings_Convert(Muzent::_Event& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Event& PNSLR_Bindings_Convert(PNSLR_Event& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Event, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_Event, buffer), "buffer offset mismatch");

extern "C" PNSLR_Event PNSLR_CreateEvent(b8 manualReset);
Muzent::_Event Muzent::_CreateEvent(b8 manualReset)
{
    PNSLR_Event zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateEvent(PNSLR_Bindings_Convert(manualReset)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroyEvent(PNSLR_Event* event);
void Muzent::_DestroyEvent(Muzent::_Event* event)
{
    PNSLR_DestroyEvent(PNSLR_Bindings_Convert(event));
}

extern "C" void PNSLR_WaitEvent(PNSLR_Event* event);
void Muzent::_WaitEvent(Muzent::_Event* event)
{
    PNSLR_WaitEvent(PNSLR_Bindings_Convert(event));
}

extern "C" b8 PNSLR_WaitEventTimeout(PNSLR_Event* event, i32 timeoutNs);
b8 Muzent::_WaitEventTimeout(Muzent::_Event* event, i32 timeoutNs)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WaitEventTimeout(PNSLR_Bindings_Convert(event), PNSLR_Bindings_Convert(timeoutNs)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_SignalEvent(PNSLR_Event* event);
void Muzent::_SignalEvent(Muzent::_Event* event)
{
    PNSLR_SignalEvent(PNSLR_Bindings_Convert(event));
}

extern "C" void PNSLR_ResetEvent(PNSLR_Event* event);
void Muzent::_ResetEvent(Muzent::_Event* event)
{
    PNSLR_ResetEvent(PNSLR_Bindings_Convert(event));
}

extern "C" void PNSLR_MemSet(rawptr memory, i32 value, i32 size);
void Muzent::_MemSet(rawptr memory, i32 value, i32 size)
{
    PNSLR_MemSet(PNSLR_Bindings_Convert(memory), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(size));
}

extern "C" void PNSLR_MemCopy(rawptr destination, rawptr source, i32 size);
void Muzent::_MemCopy(rawptr destination, rawptr source, i32 size)
{
    PNSLR_MemCopy(PNSLR_Bindings_Convert(destination), PNSLR_Bindings_Convert(source), PNSLR_Bindings_Convert(size));
}

extern "C" void PNSLR_MemMove(rawptr destination, rawptr source, i32 size);
void Muzent::_MemMove(rawptr destination, rawptr source, i32 size)
{
    PNSLR_MemMove(PNSLR_Bindings_Convert(destination), PNSLR_Bindings_Convert(source), PNSLR_Bindings_Convert(size));
}

enum class PNSLR_AllocatorMode : u8 { };
static_assert(sizeof(PNSLR_AllocatorMode) == sizeof(Muzent::_AllocatorMode), "size mismatch");
static_assert(alignof(PNSLR_AllocatorMode) == alignof(Muzent::_AllocatorMode), "align mismatch");
PNSLR_AllocatorMode* PNSLR_Bindings_Convert(Muzent::_AllocatorMode* x) { return reinterpret_cast<PNSLR_AllocatorMode*>(x); }
Muzent::_AllocatorMode* PNSLR_Bindings_Convert(PNSLR_AllocatorMode* x) { return reinterpret_cast<Muzent::_AllocatorMode*>(x); }
PNSLR_AllocatorMode& PNSLR_Bindings_Convert(Muzent::_AllocatorMode& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_AllocatorMode& PNSLR_Bindings_Convert(PNSLR_AllocatorMode& x) { return *PNSLR_Bindings_Convert(&x); }

enum class PNSLR_AllocatorCapability : u64 { };
static_assert(sizeof(PNSLR_AllocatorCapability) == sizeof(Muzent::_AllocatorCapability), "size mismatch");
static_assert(alignof(PNSLR_AllocatorCapability) == alignof(Muzent::_AllocatorCapability), "align mismatch");
PNSLR_AllocatorCapability* PNSLR_Bindings_Convert(Muzent::_AllocatorCapability* x) { return reinterpret_cast<PNSLR_AllocatorCapability*>(x); }
Muzent::_AllocatorCapability* PNSLR_Bindings_Convert(PNSLR_AllocatorCapability* x) { return reinterpret_cast<Muzent::_AllocatorCapability*>(x); }
PNSLR_AllocatorCapability& PNSLR_Bindings_Convert(Muzent::_AllocatorCapability& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_AllocatorCapability& PNSLR_Bindings_Convert(PNSLR_AllocatorCapability& x) { return *PNSLR_Bindings_Convert(&x); }

enum class PNSLR_AllocatorError : u8 { };
static_assert(sizeof(PNSLR_AllocatorError) == sizeof(Muzent::_AllocatorError), "size mismatch");
static_assert(alignof(PNSLR_AllocatorError) == alignof(Muzent::_AllocatorError), "align mismatch");
PNSLR_AllocatorError* PNSLR_Bindings_Convert(Muzent::_AllocatorError* x) { return reinterpret_cast<PNSLR_AllocatorError*>(x); }
Muzent::_AllocatorError* PNSLR_Bindings_Convert(PNSLR_AllocatorError* x) { return reinterpret_cast<Muzent::_AllocatorError*>(x); }
PNSLR_AllocatorError& PNSLR_Bindings_Convert(Muzent::_AllocatorError& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_AllocatorError& PNSLR_Bindings_Convert(PNSLR_AllocatorError& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" typedef rawptr (*PNSLR_AllocatorProcedure)(rawptr allocatorData, PNSLR_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
static_assert(sizeof(PNSLR_AllocatorProcedure) == sizeof(Muzent::_AllocatorProcedure), "size mismatch");
static_assert(alignof(PNSLR_AllocatorProcedure) == alignof(Muzent::_AllocatorProcedure), "align mismatch");
PNSLR_AllocatorProcedure* PNSLR_Bindings_Convert(Muzent::_AllocatorProcedure* x) { return reinterpret_cast<PNSLR_AllocatorProcedure*>(x); }
Muzent::_AllocatorProcedure* PNSLR_Bindings_Convert(PNSLR_AllocatorProcedure* x) { return reinterpret_cast<Muzent::_AllocatorProcedure*>(x); }
PNSLR_AllocatorProcedure& PNSLR_Bindings_Convert(Muzent::_AllocatorProcedure& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_AllocatorProcedure& PNSLR_Bindings_Convert(PNSLR_AllocatorProcedure& x) { return *PNSLR_Bindings_Convert(&x); }

struct PNSLR_Allocator
{
   PNSLR_AllocatorProcedure procedure;
   rawptr data;
};
static_assert(sizeof(PNSLR_Allocator) == sizeof(Muzent::_Allocator), "size mismatch");
static_assert(alignof(PNSLR_Allocator) == alignof(Muzent::_Allocator), "align mismatch");
PNSLR_Allocator* PNSLR_Bindings_Convert(Muzent::_Allocator* x) { return reinterpret_cast<PNSLR_Allocator*>(x); }
Muzent::_Allocator* PNSLR_Bindings_Convert(PNSLR_Allocator* x) { return reinterpret_cast<Muzent::_Allocator*>(x); }
PNSLR_Allocator& PNSLR_Bindings_Convert(Muzent::_Allocator& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Allocator& PNSLR_Bindings_Convert(PNSLR_Allocator& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Allocator, procedure) == PNSLR_STRUCT_OFFSET(Muzent::_Allocator, procedure), "procedure offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Allocator, data) == PNSLR_STRUCT_OFFSET(Muzent::_Allocator, data), "data offset mismatch");

typedef struct { PNSLR_Allocator* data; i64 count; } PNSLR_ArraySlice_PNSLR_Allocator;
static_assert(sizeof(PNSLR_ArraySlice_PNSLR_Allocator) == sizeof(ArraySlice<Muzent::_Allocator>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_PNSLR_Allocator) == alignof(ArraySlice<Muzent::_Allocator>), "align mismatch");
PNSLR_ArraySlice_PNSLR_Allocator* PNSLR_Bindings_Convert(ArraySlice<Muzent::_Allocator>* x) { return reinterpret_cast<PNSLR_ArraySlice_PNSLR_Allocator*>(x); }
ArraySlice<Muzent::_Allocator>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_Allocator* x) { return reinterpret_cast<ArraySlice<Muzent::_Allocator>*>(x); }
PNSLR_ArraySlice_PNSLR_Allocator& PNSLR_Bindings_Convert(ArraySlice<Muzent::_Allocator>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<Muzent::_Allocator>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_Allocator& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_Allocator, count) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_Allocator>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_Allocator, data) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_Allocator>, data), "data offset mismatch");

extern "C" rawptr PNSLR_Allocate(PNSLR_Allocator allocator, b8 zeroed, i32 size, i32 alignment, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
rawptr Muzent::_Allocate(Muzent::_Allocator allocator, b8 zeroed, i32 size, i32 alignment, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    rawptr zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_Allocate(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(size), PNSLR_Bindings_Convert(alignment), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" rawptr PNSLR_Resize(PNSLR_Allocator allocator, b8 zeroed, rawptr oldMemory, i32 oldSize, i32 newSize, i32 alignment, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
rawptr Muzent::_Resize(Muzent::_Allocator allocator, b8 zeroed, rawptr oldMemory, i32 oldSize, i32 newSize, i32 alignment, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    rawptr zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_Resize(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(oldMemory), PNSLR_Bindings_Convert(oldSize), PNSLR_Bindings_Convert(newSize), PNSLR_Bindings_Convert(alignment), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" rawptr PNSLR_DefaultResize(PNSLR_Allocator allocator, b8 zeroed, rawptr oldMemory, i32 oldSize, i32 newSize, i32 alignment, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
rawptr Muzent::_DefaultResize(Muzent::_Allocator allocator, b8 zeroed, rawptr oldMemory, i32 oldSize, i32 newSize, i32 alignment, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    rawptr zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_DefaultResize(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(oldMemory), PNSLR_Bindings_Convert(oldSize), PNSLR_Bindings_Convert(newSize), PNSLR_Bindings_Convert(alignment), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_Free(PNSLR_Allocator allocator, rawptr memory, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_Free(Muzent::_Allocator allocator, rawptr memory, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_Free(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(memory), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" void PNSLR_FreeAll(PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_FreeAll(Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_FreeAll(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" u64 PNSLR_QueryAllocatorCapabilities(PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
u64 Muzent::_QueryAllocatorCapabilities(Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    u64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_QueryAllocatorCapabilities(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Allocator PNSLR_GetAllocator_Nil();
Muzent::_Allocator Muzent::_GetAllocator_Nil()
{
    PNSLR_Allocator zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetAllocator_Nil(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Allocator PNSLR_GetAllocator_DefaultHeap();
Muzent::_Allocator Muzent::_GetAllocator_DefaultHeap()
{
    PNSLR_Allocator zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetAllocator_DefaultHeap(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" rawptr PNSLR_AllocatorFn_DefaultHeap(rawptr allocatorData, PNSLR_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
rawptr Muzent::_AllocatorFn_DefaultHeap(rawptr allocatorData, Muzent::_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    rawptr zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AllocatorFn_DefaultHeap(PNSLR_Bindings_Convert(allocatorData), PNSLR_Bindings_Convert(mode), PNSLR_Bindings_Convert(size), PNSLR_Bindings_Convert(alignment), PNSLR_Bindings_Convert(oldMemory), PNSLR_Bindings_Convert(oldSize), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_ArenaAllocatorBlock
{
   PNSLR_ArenaAllocatorBlock* previous;
   PNSLR_Allocator allocator;
   rawptr memory;
   u32 capacity;
   u32 used;
};
static_assert(sizeof(PNSLR_ArenaAllocatorBlock) == sizeof(Muzent::_ArenaAllocatorBlock), "size mismatch");
static_assert(alignof(PNSLR_ArenaAllocatorBlock) == alignof(Muzent::_ArenaAllocatorBlock), "align mismatch");
PNSLR_ArenaAllocatorBlock* PNSLR_Bindings_Convert(Muzent::_ArenaAllocatorBlock* x) { return reinterpret_cast<PNSLR_ArenaAllocatorBlock*>(x); }
Muzent::_ArenaAllocatorBlock* PNSLR_Bindings_Convert(PNSLR_ArenaAllocatorBlock* x) { return reinterpret_cast<Muzent::_ArenaAllocatorBlock*>(x); }
PNSLR_ArenaAllocatorBlock& PNSLR_Bindings_Convert(Muzent::_ArenaAllocatorBlock& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ArenaAllocatorBlock& PNSLR_Bindings_Convert(PNSLR_ArenaAllocatorBlock& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorBlock, previous) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorBlock, previous), "previous offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorBlock, allocator) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorBlock, allocator), "allocator offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorBlock, memory) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorBlock, memory), "memory offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorBlock, capacity) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorBlock, capacity), "capacity offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorBlock, used) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorBlock, used), "used offset mismatch");

struct PNSLR_ArenaAllocatorPayload
{
   PNSLR_Allocator backingAllocator;
   PNSLR_ArenaAllocatorBlock* currentBlock;
   u32 totalUsed;
   u32 totalCapacity;
   u32 minimumBlockSize;
   u32 numSnapshots;
};
static_assert(sizeof(PNSLR_ArenaAllocatorPayload) == sizeof(Muzent::_ArenaAllocatorPayload), "size mismatch");
static_assert(alignof(PNSLR_ArenaAllocatorPayload) == alignof(Muzent::_ArenaAllocatorPayload), "align mismatch");
PNSLR_ArenaAllocatorPayload* PNSLR_Bindings_Convert(Muzent::_ArenaAllocatorPayload* x) { return reinterpret_cast<PNSLR_ArenaAllocatorPayload*>(x); }
Muzent::_ArenaAllocatorPayload* PNSLR_Bindings_Convert(PNSLR_ArenaAllocatorPayload* x) { return reinterpret_cast<Muzent::_ArenaAllocatorPayload*>(x); }
PNSLR_ArenaAllocatorPayload& PNSLR_Bindings_Convert(Muzent::_ArenaAllocatorPayload& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ArenaAllocatorPayload& PNSLR_Bindings_Convert(PNSLR_ArenaAllocatorPayload& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorPayload, backingAllocator) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorPayload, backingAllocator), "backingAllocator offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorPayload, currentBlock) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorPayload, currentBlock), "currentBlock offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorPayload, totalUsed) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorPayload, totalUsed), "totalUsed offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorPayload, totalCapacity) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorPayload, totalCapacity), "totalCapacity offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorPayload, minimumBlockSize) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorPayload, minimumBlockSize), "minimumBlockSize offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorPayload, numSnapshots) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorPayload, numSnapshots), "numSnapshots offset mismatch");

extern "C" PNSLR_Allocator PNSLR_NewAllocator_Arena(PNSLR_Allocator backingAllocator, u32 pageSize, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
Muzent::_Allocator Muzent::_NewAllocator_Arena(Muzent::_Allocator backingAllocator, u32 pageSize, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_Allocator zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_NewAllocator_Arena(PNSLR_Bindings_Convert(backingAllocator), PNSLR_Bindings_Convert(pageSize), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroyAllocator_Arena(PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_DestroyAllocator_Arena(Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_DestroyAllocator_Arena(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" rawptr PNSLR_AllocatorFn_Arena(rawptr allocatorData, PNSLR_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
rawptr Muzent::_AllocatorFn_Arena(rawptr allocatorData, Muzent::_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    rawptr zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AllocatorFn_Arena(PNSLR_Bindings_Convert(allocatorData), PNSLR_Bindings_Convert(mode), PNSLR_Bindings_Convert(size), PNSLR_Bindings_Convert(alignment), PNSLR_Bindings_Convert(oldMemory), PNSLR_Bindings_Convert(oldSize), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

enum class PNSLR_ArenaSnapshotError : u8 { };
static_assert(sizeof(PNSLR_ArenaSnapshotError) == sizeof(Muzent::_ArenaSnapshotError), "size mismatch");
static_assert(alignof(PNSLR_ArenaSnapshotError) == alignof(Muzent::_ArenaSnapshotError), "align mismatch");
PNSLR_ArenaSnapshotError* PNSLR_Bindings_Convert(Muzent::_ArenaSnapshotError* x) { return reinterpret_cast<PNSLR_ArenaSnapshotError*>(x); }
Muzent::_ArenaSnapshotError* PNSLR_Bindings_Convert(PNSLR_ArenaSnapshotError* x) { return reinterpret_cast<Muzent::_ArenaSnapshotError*>(x); }
PNSLR_ArenaSnapshotError& PNSLR_Bindings_Convert(Muzent::_ArenaSnapshotError& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ArenaSnapshotError& PNSLR_Bindings_Convert(PNSLR_ArenaSnapshotError& x) { return *PNSLR_Bindings_Convert(&x); }

struct PNSLR_ArenaAllocatorSnapshot
{
   b8 valid;
   PNSLR_ArenaAllocatorPayload* payload;
   PNSLR_ArenaAllocatorBlock* block;
   u32 used;
};
static_assert(sizeof(PNSLR_ArenaAllocatorSnapshot) == sizeof(Muzent::_ArenaAllocatorSnapshot), "size mismatch");
static_assert(alignof(PNSLR_ArenaAllocatorSnapshot) == alignof(Muzent::_ArenaAllocatorSnapshot), "align mismatch");
PNSLR_ArenaAllocatorSnapshot* PNSLR_Bindings_Convert(Muzent::_ArenaAllocatorSnapshot* x) { return reinterpret_cast<PNSLR_ArenaAllocatorSnapshot*>(x); }
Muzent::_ArenaAllocatorSnapshot* PNSLR_Bindings_Convert(PNSLR_ArenaAllocatorSnapshot* x) { return reinterpret_cast<Muzent::_ArenaAllocatorSnapshot*>(x); }
PNSLR_ArenaAllocatorSnapshot& PNSLR_Bindings_Convert(Muzent::_ArenaAllocatorSnapshot& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ArenaAllocatorSnapshot& PNSLR_Bindings_Convert(PNSLR_ArenaAllocatorSnapshot& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorSnapshot, valid) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorSnapshot, valid), "valid offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorSnapshot, payload) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorSnapshot, payload), "payload offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorSnapshot, block) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorSnapshot, block), "block offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArenaAllocatorSnapshot, used) == PNSLR_STRUCT_OFFSET(Muzent::_ArenaAllocatorSnapshot, used), "used offset mismatch");

extern "C" b8 PNSLR_ValidateArenaAllocatorSnapshotState(PNSLR_Allocator allocator);
b8 Muzent::_ValidateArenaAllocatorSnapshotState(Muzent::_Allocator allocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ValidateArenaAllocatorSnapshotState(PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_ArenaAllocatorSnapshot PNSLR_CaptureArenaAllocatorSnapshot(PNSLR_Allocator allocator);
Muzent::_ArenaAllocatorSnapshot Muzent::_CaptureArenaAllocatorSnapshot(Muzent::_Allocator allocator)
{
    PNSLR_ArenaAllocatorSnapshot zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CaptureArenaAllocatorSnapshot(PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_ArenaSnapshotError PNSLR_RestoreArenaAllocatorSnapshot(PNSLR_ArenaAllocatorSnapshot* snapshot, PNSLR_SourceCodeLocation loc);
Muzent::_ArenaSnapshotError Muzent::_RestoreArenaAllocatorSnapshot(Muzent::_ArenaAllocatorSnapshot* snapshot, Muzent::_SourceCodeLocation loc)
{
    PNSLR_ArenaSnapshotError zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_RestoreArenaAllocatorSnapshot(PNSLR_Bindings_Convert(snapshot), PNSLR_Bindings_Convert(loc)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_ArenaSnapshotError PNSLR_DiscardArenaAllocatorSnapshot(PNSLR_ArenaAllocatorSnapshot* snapshot);
Muzent::_ArenaSnapshotError Muzent::_DiscardArenaAllocatorSnapshot(Muzent::_ArenaAllocatorSnapshot* snapshot)
{
    PNSLR_ArenaSnapshotError zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_DiscardArenaAllocatorSnapshot(PNSLR_Bindings_Convert(snapshot)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct alignas(8) PNSLR_StackAllocatorPage
{
   PNSLR_StackAllocatorPage* previousPage;
   u64 usedBytes;
   u8 buffer[8192];
};
static_assert(sizeof(PNSLR_StackAllocatorPage) == sizeof(Muzent::_StackAllocatorPage), "size mismatch");
static_assert(alignof(PNSLR_StackAllocatorPage) == alignof(Muzent::_StackAllocatorPage), "align mismatch");
PNSLR_StackAllocatorPage* PNSLR_Bindings_Convert(Muzent::_StackAllocatorPage* x) { return reinterpret_cast<PNSLR_StackAllocatorPage*>(x); }
Muzent::_StackAllocatorPage* PNSLR_Bindings_Convert(PNSLR_StackAllocatorPage* x) { return reinterpret_cast<Muzent::_StackAllocatorPage*>(x); }
PNSLR_StackAllocatorPage& PNSLR_Bindings_Convert(Muzent::_StackAllocatorPage& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StackAllocatorPage& PNSLR_Bindings_Convert(PNSLR_StackAllocatorPage& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPage, previousPage) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPage, previousPage), "previousPage offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPage, usedBytes) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPage, usedBytes), "usedBytes offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPage, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPage, buffer), "buffer offset mismatch");

struct PNSLR_StackAllocationHeader
{
   PNSLR_StackAllocatorPage* page;
   i32 size;
   i32 alignment;
   rawptr lastAllocation;
   rawptr lastAllocationHeader;
};
static_assert(sizeof(PNSLR_StackAllocationHeader) == sizeof(Muzent::_StackAllocationHeader), "size mismatch");
static_assert(alignof(PNSLR_StackAllocationHeader) == alignof(Muzent::_StackAllocationHeader), "align mismatch");
PNSLR_StackAllocationHeader* PNSLR_Bindings_Convert(Muzent::_StackAllocationHeader* x) { return reinterpret_cast<PNSLR_StackAllocationHeader*>(x); }
Muzent::_StackAllocationHeader* PNSLR_Bindings_Convert(PNSLR_StackAllocationHeader* x) { return reinterpret_cast<Muzent::_StackAllocationHeader*>(x); }
PNSLR_StackAllocationHeader& PNSLR_Bindings_Convert(Muzent::_StackAllocationHeader& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StackAllocationHeader& PNSLR_Bindings_Convert(PNSLR_StackAllocationHeader& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocationHeader, page) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocationHeader, page), "page offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocationHeader, size) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocationHeader, size), "size offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocationHeader, alignment) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocationHeader, alignment), "alignment offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocationHeader, lastAllocation) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocationHeader, lastAllocation), "lastAllocation offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocationHeader, lastAllocationHeader) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocationHeader, lastAllocationHeader), "lastAllocationHeader offset mismatch");

struct PNSLR_StackAllocatorPayload
{
   PNSLR_Allocator backingAllocator;
   PNSLR_StackAllocatorPage* currentPage;
   rawptr lastAllocation;
   PNSLR_StackAllocationHeader* lastAllocationHeader;
};
static_assert(sizeof(PNSLR_StackAllocatorPayload) == sizeof(Muzent::_StackAllocatorPayload), "size mismatch");
static_assert(alignof(PNSLR_StackAllocatorPayload) == alignof(Muzent::_StackAllocatorPayload), "align mismatch");
PNSLR_StackAllocatorPayload* PNSLR_Bindings_Convert(Muzent::_StackAllocatorPayload* x) { return reinterpret_cast<PNSLR_StackAllocatorPayload*>(x); }
Muzent::_StackAllocatorPayload* PNSLR_Bindings_Convert(PNSLR_StackAllocatorPayload* x) { return reinterpret_cast<Muzent::_StackAllocatorPayload*>(x); }
PNSLR_StackAllocatorPayload& PNSLR_Bindings_Convert(Muzent::_StackAllocatorPayload& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StackAllocatorPayload& PNSLR_Bindings_Convert(PNSLR_StackAllocatorPayload& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPayload, backingAllocator) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPayload, backingAllocator), "backingAllocator offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPayload, currentPage) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPayload, currentPage), "currentPage offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPayload, lastAllocation) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPayload, lastAllocation), "lastAllocation offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StackAllocatorPayload, lastAllocationHeader) == PNSLR_STRUCT_OFFSET(Muzent::_StackAllocatorPayload, lastAllocationHeader), "lastAllocationHeader offset mismatch");

extern "C" PNSLR_Allocator PNSLR_NewAllocator_Stack(PNSLR_Allocator backingAllocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
Muzent::_Allocator Muzent::_NewAllocator_Stack(Muzent::_Allocator backingAllocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_Allocator zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_NewAllocator_Stack(PNSLR_Bindings_Convert(backingAllocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_DestroyAllocator_Stack(PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_DestroyAllocator_Stack(Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_DestroyAllocator_Stack(PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" rawptr PNSLR_AllocatorFn_Stack(rawptr allocatorData, PNSLR_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
rawptr Muzent::_AllocatorFn_Stack(rawptr allocatorData, Muzent::_AllocatorMode mode, i32 size, i32 alignment, rawptr oldMemory, i32 oldSize, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    rawptr zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AllocatorFn_Stack(PNSLR_Bindings_Convert(allocatorData), PNSLR_Bindings_Convert(mode), PNSLR_Bindings_Convert(size), PNSLR_Bindings_Convert(alignment), PNSLR_Bindings_Convert(oldMemory), PNSLR_Bindings_Convert(oldSize), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_RawArraySlice PNSLR_MakeRawSlice(i32 tySize, i32 tyAlign, i64 count, b8 zeroed, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
Muzent::_RawArraySlice Muzent::_MakeRawSlice(i32 tySize, i32 tyAlign, i64 count, b8 zeroed, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_RawArraySlice zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_MakeRawSlice(PNSLR_Bindings_Convert(tySize), PNSLR_Bindings_Convert(tyAlign), PNSLR_Bindings_Convert(count), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_FreeRawSlice(PNSLR_RawArraySlice* slice, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_FreeRawSlice(Muzent::_RawArraySlice* slice, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_FreeRawSlice(PNSLR_Bindings_Convert(slice), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" void PNSLR_ResizeRawSlice(PNSLR_RawArraySlice* slice, i32 tySize, i32 tyAlign, i64 newCount, b8 zeroed, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_ResizeRawSlice(Muzent::_RawArraySlice* slice, i32 tySize, i32 tyAlign, i64 newCount, b8 zeroed, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_ResizeRawSlice(PNSLR_Bindings_Convert(slice), PNSLR_Bindings_Convert(tySize), PNSLR_Bindings_Convert(tyAlign), PNSLR_Bindings_Convert(newCount), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" PNSLR_UTF8STR PNSLR_MakeString(i64 count, b8 zeroed, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
utf8str Muzent::_MakeString(i64 count, b8 zeroed, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_MakeString(PNSLR_Bindings_Convert(count), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_FreeString(PNSLR_UTF8STR str, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_FreeString(utf8str str, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_FreeString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" cstring PNSLR_MakeCString(i64 count, b8 zeroed, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
cstring Muzent::_MakeCString(i64 count, b8 zeroed, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    cstring zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_MakeCString(PNSLR_Bindings_Convert(count), PNSLR_Bindings_Convert(zeroed), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_FreeCString(cstring str, PNSLR_Allocator allocator, PNSLR_SourceCodeLocation location, PNSLR_AllocatorError* error);
void Muzent::_FreeCString(cstring str, Muzent::_Allocator allocator, Muzent::_SourceCodeLocation location, Muzent::_AllocatorError* error)
{
    PNSLR_FreeCString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(location), PNSLR_Bindings_Convert(error));
}

extern "C" i64 PNSLR_NanosecondsSinceUnixEpoch();
i64 Muzent::_NanosecondsSinceUnixEpoch()
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_NanosecondsSinceUnixEpoch(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_ConvertNanosecondsSinceUnixEpochToDateTime(i64 ns, i16* outYear, u8* outMonth, u8* outDay, u8* outHour, u8* outMinute, u8* outSecond);
b8 Muzent::_ConvertNanosecondsSinceUnixEpochToDateTime(i64 ns, i16* outYear, u8* outMonth, u8* outDay, u8* outHour, u8* outMinute, u8* outSecond)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ConvertNanosecondsSinceUnixEpochToDateTime(PNSLR_Bindings_Convert(ns), PNSLR_Bindings_Convert(outYear), PNSLR_Bindings_Convert(outMonth), PNSLR_Bindings_Convert(outDay), PNSLR_Bindings_Convert(outHour), PNSLR_Bindings_Convert(outMinute), PNSLR_Bindings_Convert(outSecond)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i32 PNSLR_GetCStringLength(cstring str);
i32 Muzent::_GetCStringLength(cstring str)
{
    i32 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetCStringLength(PNSLR_Bindings_Convert(str)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromCString(cstring str);
utf8str Muzent::_StringFromCString(cstring str)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromCString(PNSLR_Bindings_Convert(str)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" cstring PNSLR_CStringFromString(PNSLR_UTF8STR str, PNSLR_Allocator allocator);
cstring Muzent::_CStringFromString(utf8str str, Muzent::_Allocator allocator)
{
    cstring zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CStringFromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_CloneString(PNSLR_UTF8STR str, PNSLR_Allocator allocator);
utf8str Muzent::_CloneString(utf8str str, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CloneString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_ConcatenateStrings(PNSLR_UTF8STR str1, PNSLR_UTF8STR str2, PNSLR_Allocator allocator);
utf8str Muzent::_ConcatenateStrings(utf8str str1, utf8str str2, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ConcatenateStrings(PNSLR_Bindings_Convert(str1), PNSLR_Bindings_Convert(str2), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_UpperString(PNSLR_UTF8STR str, PNSLR_Allocator allocator);
utf8str Muzent::_UpperString(utf8str str, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_UpperString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_LowerString(PNSLR_UTF8STR str, PNSLR_Allocator allocator);
utf8str Muzent::_LowerString(utf8str str, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_LowerString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

enum class PNSLR_StringComparisonType : u8 { };
static_assert(sizeof(PNSLR_StringComparisonType) == sizeof(Muzent::_StringComparisonType), "size mismatch");
static_assert(alignof(PNSLR_StringComparisonType) == alignof(Muzent::_StringComparisonType), "align mismatch");
PNSLR_StringComparisonType* PNSLR_Bindings_Convert(Muzent::_StringComparisonType* x) { return reinterpret_cast<PNSLR_StringComparisonType*>(x); }
Muzent::_StringComparisonType* PNSLR_Bindings_Convert(PNSLR_StringComparisonType* x) { return reinterpret_cast<Muzent::_StringComparisonType*>(x); }
PNSLR_StringComparisonType& PNSLR_Bindings_Convert(Muzent::_StringComparisonType& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StringComparisonType& PNSLR_Bindings_Convert(PNSLR_StringComparisonType& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" b8 PNSLR_AreStringsEqual(PNSLR_UTF8STR str1, PNSLR_UTF8STR str2, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_AreStringsEqual(utf8str str1, utf8str str2, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AreStringsEqual(PNSLR_Bindings_Convert(str1), PNSLR_Bindings_Convert(str2), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AreStringAndCStringEqual(PNSLR_UTF8STR str1, cstring str2, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_AreStringAndCStringEqual(utf8str str1, cstring str2, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AreStringAndCStringEqual(PNSLR_Bindings_Convert(str1), PNSLR_Bindings_Convert(str2), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AreCStringsEqual(cstring str1, cstring str2, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_AreCStringsEqual(cstring str1, cstring str2, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AreCStringsEqual(PNSLR_Bindings_Convert(str1), PNSLR_Bindings_Convert(str2), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_StringStartsWith(PNSLR_UTF8STR str, PNSLR_UTF8STR prefix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_StringStartsWith(utf8str str, utf8str prefix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringStartsWith(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(prefix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_StringEndsWith(PNSLR_UTF8STR str, PNSLR_UTF8STR suffix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_StringEndsWith(utf8str str, utf8str suffix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringEndsWith(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(suffix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_StringStartsWithCString(PNSLR_UTF8STR str, cstring prefix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_StringStartsWithCString(utf8str str, cstring prefix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringStartsWithCString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(prefix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_StringEndsWithCString(PNSLR_UTF8STR str, cstring suffix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_StringEndsWithCString(utf8str str, cstring suffix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringEndsWithCString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(suffix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CStringStartsWith(cstring str, PNSLR_UTF8STR prefix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_CStringStartsWith(cstring str, utf8str prefix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CStringStartsWith(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(prefix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CStringEndsWith(cstring str, PNSLR_UTF8STR suffix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_CStringEndsWith(cstring str, utf8str suffix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CStringEndsWith(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(suffix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CStringStartsWithCString(PNSLR_UTF8STR str, cstring prefix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_CStringStartsWithCString(utf8str str, cstring prefix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CStringStartsWithCString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(prefix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CStringEndsWithCString(PNSLR_UTF8STR str, cstring suffix, PNSLR_StringComparisonType comparisonType);
b8 Muzent::_CStringEndsWithCString(utf8str str, cstring suffix, Muzent::_StringComparisonType comparisonType)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CStringEndsWithCString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(suffix), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i32 PNSLR_SearchFirstIndexInString(PNSLR_UTF8STR str, PNSLR_UTF8STR substring, PNSLR_StringComparisonType comparisonType);
i32 Muzent::_SearchFirstIndexInString(utf8str str, utf8str substring, Muzent::_StringComparisonType comparisonType)
{
    i32 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_SearchFirstIndexInString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(substring), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i32 PNSLR_SearchLastIndexInString(PNSLR_UTF8STR str, PNSLR_UTF8STR substring, PNSLR_StringComparisonType comparisonType);
i32 Muzent::_SearchLastIndexInString(utf8str str, utf8str substring, Muzent::_StringComparisonType comparisonType)
{
    i32 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_SearchLastIndexInString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(substring), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_ReplaceInString(PNSLR_UTF8STR str, PNSLR_UTF8STR oldValue, PNSLR_UTF8STR newValue, PNSLR_Allocator allocator, PNSLR_StringComparisonType comparisonType);
utf8str Muzent::_ReplaceInString(utf8str str, utf8str oldValue, utf8str newValue, Muzent::_Allocator allocator, Muzent::_StringComparisonType comparisonType)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReplaceInString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(oldValue), PNSLR_Bindings_Convert(newValue), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(comparisonType)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_EncodedRune
{
   u8 data[4];
   i32 length;
};
static_assert(sizeof(PNSLR_EncodedRune) == sizeof(Muzent::_EncodedRune), "size mismatch");
static_assert(alignof(PNSLR_EncodedRune) == alignof(Muzent::_EncodedRune), "align mismatch");
PNSLR_EncodedRune* PNSLR_Bindings_Convert(Muzent::_EncodedRune* x) { return reinterpret_cast<PNSLR_EncodedRune*>(x); }
Muzent::_EncodedRune* PNSLR_Bindings_Convert(PNSLR_EncodedRune* x) { return reinterpret_cast<Muzent::_EncodedRune*>(x); }
PNSLR_EncodedRune& PNSLR_Bindings_Convert(Muzent::_EncodedRune& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_EncodedRune& PNSLR_Bindings_Convert(PNSLR_EncodedRune& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_EncodedRune, data) == PNSLR_STRUCT_OFFSET(Muzent::_EncodedRune, data), "data offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_EncodedRune, length) == PNSLR_STRUCT_OFFSET(Muzent::_EncodedRune, length), "length offset mismatch");

struct PNSLR_DecodedRune
{
   u32 rune;
   i32 length;
};
static_assert(sizeof(PNSLR_DecodedRune) == sizeof(Muzent::_DecodedRune), "size mismatch");
static_assert(alignof(PNSLR_DecodedRune) == alignof(Muzent::_DecodedRune), "align mismatch");
PNSLR_DecodedRune* PNSLR_Bindings_Convert(Muzent::_DecodedRune* x) { return reinterpret_cast<PNSLR_DecodedRune*>(x); }
Muzent::_DecodedRune* PNSLR_Bindings_Convert(PNSLR_DecodedRune* x) { return reinterpret_cast<Muzent::_DecodedRune*>(x); }
PNSLR_DecodedRune& PNSLR_Bindings_Convert(Muzent::_DecodedRune& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_DecodedRune& PNSLR_Bindings_Convert(PNSLR_DecodedRune& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_DecodedRune, rune) == PNSLR_STRUCT_OFFSET(Muzent::_DecodedRune, rune), "rune offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_DecodedRune, length) == PNSLR_STRUCT_OFFSET(Muzent::_DecodedRune, length), "length offset mismatch");

extern "C" i32 PNSLR_GetRuneLength(u32 r);
i32 Muzent::_GetRuneLength(u32 r)
{
    i32 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetRuneLength(PNSLR_Bindings_Convert(r)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_EncodedRune PNSLR_EncodeRune(u32 c);
Muzent::_EncodedRune Muzent::_EncodeRune(u32 c)
{
    PNSLR_EncodedRune zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_EncodeRune(PNSLR_Bindings_Convert(c)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_DecodedRune PNSLR_DecodeRune(PNSLR_ArraySlice_u8 s);
Muzent::_DecodedRune Muzent::_DecodeRune(ArraySlice<u8> s)
{
    PNSLR_DecodedRune zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_DecodeRune(PNSLR_Bindings_Convert(s)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_ArraySlice_u16 PNSLR_UTF16FromUTF8WindowsOnly(PNSLR_UTF8STR str, PNSLR_Allocator allocator);
ArraySlice<u16> Muzent::_UTF16FromUTF8WindowsOnly(utf8str str, Muzent::_Allocator allocator)
{
    PNSLR_ArraySlice_u16 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_UTF16FromUTF8WindowsOnly(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_UTF8FromUTF16WindowsOnly(PNSLR_ArraySlice_u16 utf16str, PNSLR_Allocator allocator);
utf8str Muzent::_UTF8FromUTF16WindowsOnly(ArraySlice<u16> utf16str, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_UTF8FromUTF16WindowsOnly(PNSLR_Bindings_Convert(utf16str), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_StringBuilder
{
   PNSLR_Allocator allocator;
   PNSLR_ArraySlice_u8 buffer;
   i64 writtenSize;
   i64 cursorPos;
};
static_assert(sizeof(PNSLR_StringBuilder) == sizeof(Muzent::_StringBuilder), "size mismatch");
static_assert(alignof(PNSLR_StringBuilder) == alignof(Muzent::_StringBuilder), "align mismatch");
PNSLR_StringBuilder* PNSLR_Bindings_Convert(Muzent::_StringBuilder* x) { return reinterpret_cast<PNSLR_StringBuilder*>(x); }
Muzent::_StringBuilder* PNSLR_Bindings_Convert(PNSLR_StringBuilder* x) { return reinterpret_cast<Muzent::_StringBuilder*>(x); }
PNSLR_StringBuilder& PNSLR_Bindings_Convert(Muzent::_StringBuilder& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StringBuilder& PNSLR_Bindings_Convert(PNSLR_StringBuilder& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StringBuilder, allocator) == PNSLR_STRUCT_OFFSET(Muzent::_StringBuilder, allocator), "allocator offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StringBuilder, buffer) == PNSLR_STRUCT_OFFSET(Muzent::_StringBuilder, buffer), "buffer offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StringBuilder, writtenSize) == PNSLR_STRUCT_OFFSET(Muzent::_StringBuilder, writtenSize), "writtenSize offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_StringBuilder, cursorPos) == PNSLR_STRUCT_OFFSET(Muzent::_StringBuilder, cursorPos), "cursorPos offset mismatch");

extern "C" b8 PNSLR_ReserveSpaceInStringBuilder(PNSLR_StringBuilder* builder, i64 additionalSize);
b8 Muzent::_ReserveSpaceInStringBuilder(Muzent::_StringBuilder* builder, i64 additionalSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReserveSpaceInStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(additionalSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendByteToStringBuilder(PNSLR_StringBuilder* builder, u8 byte);
b8 Muzent::_AppendByteToStringBuilder(Muzent::_StringBuilder* builder, u8 byte)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendByteToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(byte)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendStringToStringBuilder(PNSLR_StringBuilder* builder, PNSLR_UTF8STR str);
b8 Muzent::_AppendStringToStringBuilder(Muzent::_StringBuilder* builder, utf8str str)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendStringToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(str)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendCStringToStringBuilder(PNSLR_StringBuilder* builder, cstring str);
b8 Muzent::_AppendCStringToStringBuilder(Muzent::_StringBuilder* builder, cstring str)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendCStringToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(str)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendRuneToStringBuilder(PNSLR_StringBuilder* builder, u32 rune);
b8 Muzent::_AppendRuneToStringBuilder(Muzent::_StringBuilder* builder, u32 rune)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendRuneToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(rune)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendB8ToStringBuilder(PNSLR_StringBuilder* builder, b8 value);
b8 Muzent::_AppendB8ToStringBuilder(Muzent::_StringBuilder* builder, b8 value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendB8ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendF32ToStringBuilder(PNSLR_StringBuilder* builder, f32 value, i32 decimalPlaces);
b8 Muzent::_AppendF32ToStringBuilder(Muzent::_StringBuilder* builder, f32 value, i32 decimalPlaces)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendF32ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(decimalPlaces)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendF64ToStringBuilder(PNSLR_StringBuilder* builder, f64 value, i32 decimalPlaces);
b8 Muzent::_AppendF64ToStringBuilder(Muzent::_StringBuilder* builder, f64 value, i32 decimalPlaces)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendF64ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(decimalPlaces)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

enum class PNSLR_IntegerBase : u8 { };
static_assert(sizeof(PNSLR_IntegerBase) == sizeof(Muzent::_IntegerBase), "size mismatch");
static_assert(alignof(PNSLR_IntegerBase) == alignof(Muzent::_IntegerBase), "align mismatch");
PNSLR_IntegerBase* PNSLR_Bindings_Convert(Muzent::_IntegerBase* x) { return reinterpret_cast<PNSLR_IntegerBase*>(x); }
Muzent::_IntegerBase* PNSLR_Bindings_Convert(PNSLR_IntegerBase* x) { return reinterpret_cast<Muzent::_IntegerBase*>(x); }
PNSLR_IntegerBase& PNSLR_Bindings_Convert(Muzent::_IntegerBase& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_IntegerBase& PNSLR_Bindings_Convert(PNSLR_IntegerBase& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" b8 PNSLR_AppendU8ToStringBuilder(PNSLR_StringBuilder* builder, u8 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendU8ToStringBuilder(Muzent::_StringBuilder* builder, u8 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendU8ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendU16ToStringBuilder(PNSLR_StringBuilder* builder, u16 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendU16ToStringBuilder(Muzent::_StringBuilder* builder, u16 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendU16ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendU32ToStringBuilder(PNSLR_StringBuilder* builder, u32 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendU32ToStringBuilder(Muzent::_StringBuilder* builder, u32 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendU32ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendU64ToStringBuilder(PNSLR_StringBuilder* builder, u64 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendU64ToStringBuilder(Muzent::_StringBuilder* builder, u64 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendU64ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendI8ToStringBuilder(PNSLR_StringBuilder* builder, i8 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendI8ToStringBuilder(Muzent::_StringBuilder* builder, i8 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendI8ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendI16ToStringBuilder(PNSLR_StringBuilder* builder, i16 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendI16ToStringBuilder(Muzent::_StringBuilder* builder, i16 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendI16ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendI32ToStringBuilder(PNSLR_StringBuilder* builder, i32 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendI32ToStringBuilder(Muzent::_StringBuilder* builder, i32 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendI32ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AppendI64ToStringBuilder(PNSLR_StringBuilder* builder, i64 value, PNSLR_IntegerBase base);
b8 Muzent::_AppendI64ToStringBuilder(Muzent::_StringBuilder* builder, i64 value, Muzent::_IntegerBase base)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AppendI64ToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromStringBuilder(PNSLR_StringBuilder* builder);
utf8str Muzent::_StringFromStringBuilder(Muzent::_StringBuilder* builder)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromStringBuilder(PNSLR_Bindings_Convert(builder)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_ResetStringBuilder(PNSLR_StringBuilder* builder);
void Muzent::_ResetStringBuilder(Muzent::_StringBuilder* builder)
{
    PNSLR_ResetStringBuilder(PNSLR_Bindings_Convert(builder));
}

extern "C" void PNSLR_FreeStringBuilder(PNSLR_StringBuilder* builder);
void Muzent::_FreeStringBuilder(Muzent::_StringBuilder* builder)
{
    PNSLR_FreeStringBuilder(PNSLR_Bindings_Convert(builder));
}

enum class PNSLR_PrimitiveFmtType : u8 { };
static_assert(sizeof(PNSLR_PrimitiveFmtType) == sizeof(Muzent::_PrimitiveFmtType), "size mismatch");
static_assert(alignof(PNSLR_PrimitiveFmtType) == alignof(Muzent::_PrimitiveFmtType), "align mismatch");
PNSLR_PrimitiveFmtType* PNSLR_Bindings_Convert(Muzent::_PrimitiveFmtType* x) { return reinterpret_cast<PNSLR_PrimitiveFmtType*>(x); }
Muzent::_PrimitiveFmtType* PNSLR_Bindings_Convert(PNSLR_PrimitiveFmtType* x) { return reinterpret_cast<Muzent::_PrimitiveFmtType*>(x); }
PNSLR_PrimitiveFmtType& PNSLR_Bindings_Convert(Muzent::_PrimitiveFmtType& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_PrimitiveFmtType& PNSLR_Bindings_Convert(PNSLR_PrimitiveFmtType& x) { return *PNSLR_Bindings_Convert(&x); }

struct PNSLR_PrimitiveFmtOptions
{
   PNSLR_PrimitiveFmtType type;
   u64 valueBufferA;
   u64 valueBufferB;
};
static_assert(sizeof(PNSLR_PrimitiveFmtOptions) == sizeof(Muzent::_PrimitiveFmtOptions), "size mismatch");
static_assert(alignof(PNSLR_PrimitiveFmtOptions) == alignof(Muzent::_PrimitiveFmtOptions), "align mismatch");
PNSLR_PrimitiveFmtOptions* PNSLR_Bindings_Convert(Muzent::_PrimitiveFmtOptions* x) { return reinterpret_cast<PNSLR_PrimitiveFmtOptions*>(x); }
Muzent::_PrimitiveFmtOptions* PNSLR_Bindings_Convert(PNSLR_PrimitiveFmtOptions* x) { return reinterpret_cast<Muzent::_PrimitiveFmtOptions*>(x); }
PNSLR_PrimitiveFmtOptions& PNSLR_Bindings_Convert(Muzent::_PrimitiveFmtOptions& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_PrimitiveFmtOptions& PNSLR_Bindings_Convert(PNSLR_PrimitiveFmtOptions& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_PrimitiveFmtOptions, type) == PNSLR_STRUCT_OFFSET(Muzent::_PrimitiveFmtOptions, type), "type offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_PrimitiveFmtOptions, valueBufferA) == PNSLR_STRUCT_OFFSET(Muzent::_PrimitiveFmtOptions, valueBufferA), "valueBufferA offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_PrimitiveFmtOptions, valueBufferB) == PNSLR_STRUCT_OFFSET(Muzent::_PrimitiveFmtOptions, valueBufferB), "valueBufferB offset mismatch");

typedef struct { PNSLR_PrimitiveFmtOptions* data; i64 count; } PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions;
static_assert(sizeof(PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions) == sizeof(ArraySlice<Muzent::_PrimitiveFmtOptions>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions) == alignof(ArraySlice<Muzent::_PrimitiveFmtOptions>), "align mismatch");
PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions* PNSLR_Bindings_Convert(ArraySlice<Muzent::_PrimitiveFmtOptions>* x) { return reinterpret_cast<PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions*>(x); }
ArraySlice<Muzent::_PrimitiveFmtOptions>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions* x) { return reinterpret_cast<ArraySlice<Muzent::_PrimitiveFmtOptions>*>(x); }
PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions& PNSLR_Bindings_Convert(ArraySlice<Muzent::_PrimitiveFmtOptions>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<Muzent::_PrimitiveFmtOptions>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions, count) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_PrimitiveFmtOptions>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions, data) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_PrimitiveFmtOptions>, data), "data offset mismatch");

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtB8(b8 value);
Muzent::_PrimitiveFmtOptions Muzent::_FmtB8(b8 value)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtB8(PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtF32(f32 value, i32 decimalPlaces);
Muzent::_PrimitiveFmtOptions Muzent::_FmtF32(f32 value, i32 decimalPlaces)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtF32(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(decimalPlaces)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtF64(f64 value, i32 decimalPlaces);
Muzent::_PrimitiveFmtOptions Muzent::_FmtF64(f64 value, i32 decimalPlaces)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtF64(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(decimalPlaces)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtU8(u8 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtU8(u8 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtU8(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtU16(u16 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtU16(u16 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtU16(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtU32(u32 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtU32(u32 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtU32(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtU64(u64 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtU64(u64 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtU64(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtI8(i8 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtI8(i8 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtI8(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtI16(i16 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtI16(i16 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtI16(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtI32(i32 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtI32(i32 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtI32(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtI64(i64 value, PNSLR_IntegerBase base);
Muzent::_PrimitiveFmtOptions Muzent::_FmtI64(i64 value, Muzent::_IntegerBase base)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtI64(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtRune(u32 value);
Muzent::_PrimitiveFmtOptions Muzent::_FmtRune(u32 value)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtRune(PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtCString(cstring value);
Muzent::_PrimitiveFmtOptions Muzent::_FmtCString(cstring value)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtCString(PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_PrimitiveFmtOptions PNSLR_FmtString(PNSLR_UTF8STR value);
Muzent::_PrimitiveFmtOptions Muzent::_FmtString(utf8str value)
{
    PNSLR_PrimitiveFmtOptions zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FmtString(PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_FormatAndAppendToStringBuilder(PNSLR_StringBuilder* builder, PNSLR_UTF8STR fmtStr, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args);
b8 Muzent::_FormatAndAppendToStringBuilder(Muzent::_StringBuilder* builder, utf8str fmtStr, ArraySlice<Muzent::_PrimitiveFmtOptions> args)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FormatAndAppendToStringBuilder(PNSLR_Bindings_Convert(builder), PNSLR_Bindings_Convert(fmtStr), PNSLR_Bindings_Convert(args)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_FormatString(PNSLR_UTF8STR fmtStr, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_Allocator allocator);
utf8str Muzent::_FormatString(utf8str fmtStr, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FormatString(PNSLR_Bindings_Convert(fmtStr), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromBoolean(b8 value, PNSLR_Allocator allocator);
utf8str Muzent::_StringFromBoolean(b8 value, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromBoolean(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromF32(f32 value, PNSLR_Allocator allocator, i32 decimalPlaces);
utf8str Muzent::_StringFromF32(f32 value, Muzent::_Allocator allocator, i32 decimalPlaces)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromF32(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(decimalPlaces)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromF64(f64 value, PNSLR_Allocator allocator, i32 decimalPlaces);
utf8str Muzent::_StringFromF64(f64 value, Muzent::_Allocator allocator, i32 decimalPlaces)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromF64(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(decimalPlaces)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromU8(u8 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromU8(u8 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromU8(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromU16(u16 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromU16(u16 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromU16(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromU32(u32 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromU32(u32 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromU32(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromU64(u64 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromU64(u64 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromU64(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromI8(i8 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromI8(i8 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromI8(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromI16(i16 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromI16(i16 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromI16(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromI32(i32 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromI32(i32 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromI32(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_StringFromI64(i64 value, PNSLR_Allocator allocator, PNSLR_IntegerBase base);
utf8str Muzent::_StringFromI64(i64 value, Muzent::_Allocator allocator, Muzent::_IntegerBase base)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StringFromI64(PNSLR_Bindings_Convert(value), PNSLR_Bindings_Convert(allocator), PNSLR_Bindings_Convert(base)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_BooleanFromString(PNSLR_UTF8STR str, b8* value);
b8 Muzent::_BooleanFromString(utf8str str, b8* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_BooleanFromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_F32FromString(PNSLR_UTF8STR str, f32* value);
b8 Muzent::_F32FromString(utf8str str, f32* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_F32FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_F64FromString(PNSLR_UTF8STR str, f64* value);
b8 Muzent::_F64FromString(utf8str str, f64* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_F64FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_U8FromString(PNSLR_UTF8STR str, u8* value);
b8 Muzent::_U8FromString(utf8str str, u8* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_U8FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_U16FromString(PNSLR_UTF8STR str, u16* value);
b8 Muzent::_U16FromString(utf8str str, u16* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_U16FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_U32FromString(PNSLR_UTF8STR str, u32* value);
b8 Muzent::_U32FromString(utf8str str, u32* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_U32FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_U64FromString(PNSLR_UTF8STR str, u64* value);
b8 Muzent::_U64FromString(utf8str str, u64* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_U64FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_I8FromString(PNSLR_UTF8STR str, i8* value);
b8 Muzent::_I8FromString(utf8str str, i8* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_I8FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_I16FromString(PNSLR_UTF8STR str, i16* value);
b8 Muzent::_I16FromString(utf8str str, i16* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_I16FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_I32FromString(PNSLR_UTF8STR str, i32* value);
b8 Muzent::_I32FromString(utf8str str, i32* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_I32FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_I64FromString(PNSLR_UTF8STR str, i64* value);
b8 Muzent::_I64FromString(utf8str str, i64* value)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_I64FromString(PNSLR_Bindings_Convert(str), PNSLR_Bindings_Convert(value)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_Path
{
   PNSLR_UTF8STR path;
};
static_assert(sizeof(PNSLR_Path) == sizeof(Muzent::_Path), "size mismatch");
static_assert(alignof(PNSLR_Path) == alignof(Muzent::_Path), "align mismatch");
PNSLR_Path* PNSLR_Bindings_Convert(Muzent::_Path* x) { return reinterpret_cast<PNSLR_Path*>(x); }
Muzent::_Path* PNSLR_Bindings_Convert(PNSLR_Path* x) { return reinterpret_cast<Muzent::_Path*>(x); }
PNSLR_Path& PNSLR_Bindings_Convert(Muzent::_Path& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Path& PNSLR_Bindings_Convert(PNSLR_Path& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Path, path) == PNSLR_STRUCT_OFFSET(Muzent::_Path, path), "path offset mismatch");

enum class PNSLR_PathNormalisationType : u8 { };
static_assert(sizeof(PNSLR_PathNormalisationType) == sizeof(Muzent::_PathNormalisationType), "size mismatch");
static_assert(alignof(PNSLR_PathNormalisationType) == alignof(Muzent::_PathNormalisationType), "align mismatch");
PNSLR_PathNormalisationType* PNSLR_Bindings_Convert(Muzent::_PathNormalisationType* x) { return reinterpret_cast<PNSLR_PathNormalisationType*>(x); }
Muzent::_PathNormalisationType* PNSLR_Bindings_Convert(PNSLR_PathNormalisationType* x) { return reinterpret_cast<Muzent::_PathNormalisationType*>(x); }
PNSLR_PathNormalisationType& PNSLR_Bindings_Convert(Muzent::_PathNormalisationType& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_PathNormalisationType& PNSLR_Bindings_Convert(PNSLR_PathNormalisationType& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" PNSLR_Path PNSLR_NormalisePath(PNSLR_UTF8STR path, PNSLR_PathNormalisationType type, PNSLR_Allocator allocator);
Muzent::_Path Muzent::_NormalisePath(utf8str path, Muzent::_PathNormalisationType type, Muzent::_Allocator allocator)
{
    PNSLR_Path zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_NormalisePath(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(type), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_SplitPath(PNSLR_Path path, PNSLR_Path* parent, PNSLR_UTF8STR* selfNameWithExtension, PNSLR_UTF8STR* selfName, PNSLR_UTF8STR* extension);
b8 Muzent::_SplitPath(Muzent::_Path path, Muzent::_Path* parent, utf8str* selfNameWithExtension, utf8str* selfName, utf8str* extension)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_SplitPath(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(parent), PNSLR_Bindings_Convert(selfNameWithExtension), PNSLR_Bindings_Convert(selfName), PNSLR_Bindings_Convert(extension)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Path PNSLR_GetPathForChildFile(PNSLR_Path dir, PNSLR_UTF8STR fileNameWithExtension, PNSLR_Allocator allocator);
Muzent::_Path Muzent::_GetPathForChildFile(Muzent::_Path dir, utf8str fileNameWithExtension, Muzent::_Allocator allocator)
{
    PNSLR_Path zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetPathForChildFile(PNSLR_Bindings_Convert(dir), PNSLR_Bindings_Convert(fileNameWithExtension), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Path PNSLR_GetPathForSubdirectory(PNSLR_Path dir, PNSLR_UTF8STR dirName, PNSLR_Allocator allocator);
Muzent::_Path Muzent::_GetPathForSubdirectory(Muzent::_Path dir, utf8str dirName, Muzent::_Allocator allocator)
{
    PNSLR_Path zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetPathForSubdirectory(PNSLR_Bindings_Convert(dir), PNSLR_Bindings_Convert(dirName), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" typedef b8 (*PNSLR_DirectoryIterationVisitorDelegate)(rawptr payload, PNSLR_Path path, b8 isDirectory, b8* exploreCurrentDirectory);
static_assert(sizeof(PNSLR_DirectoryIterationVisitorDelegate) == sizeof(Muzent::_DirectoryIterationVisitorDelegate), "size mismatch");
static_assert(alignof(PNSLR_DirectoryIterationVisitorDelegate) == alignof(Muzent::_DirectoryIterationVisitorDelegate), "align mismatch");
PNSLR_DirectoryIterationVisitorDelegate* PNSLR_Bindings_Convert(Muzent::_DirectoryIterationVisitorDelegate* x) { return reinterpret_cast<PNSLR_DirectoryIterationVisitorDelegate*>(x); }
Muzent::_DirectoryIterationVisitorDelegate* PNSLR_Bindings_Convert(PNSLR_DirectoryIterationVisitorDelegate* x) { return reinterpret_cast<Muzent::_DirectoryIterationVisitorDelegate*>(x); }
PNSLR_DirectoryIterationVisitorDelegate& PNSLR_Bindings_Convert(Muzent::_DirectoryIterationVisitorDelegate& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_DirectoryIterationVisitorDelegate& PNSLR_Bindings_Convert(PNSLR_DirectoryIterationVisitorDelegate& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" void PNSLR_IterateDirectory(PNSLR_Path path, b8 recursive, rawptr visitorPayload, PNSLR_DirectoryIterationVisitorDelegate visitorFunc);
void Muzent::_IterateDirectory(Muzent::_Path path, b8 recursive, rawptr visitorPayload, Muzent::_DirectoryIterationVisitorDelegate visitorFunc)
{
    PNSLR_IterateDirectory(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(recursive), PNSLR_Bindings_Convert(visitorPayload), PNSLR_Bindings_Convert(visitorFunc));
}

enum class PNSLR_PathExistsCheckType : u8 { };
static_assert(sizeof(PNSLR_PathExistsCheckType) == sizeof(Muzent::_PathExistsCheckType), "size mismatch");
static_assert(alignof(PNSLR_PathExistsCheckType) == alignof(Muzent::_PathExistsCheckType), "align mismatch");
PNSLR_PathExistsCheckType* PNSLR_Bindings_Convert(Muzent::_PathExistsCheckType* x) { return reinterpret_cast<PNSLR_PathExistsCheckType*>(x); }
Muzent::_PathExistsCheckType* PNSLR_Bindings_Convert(PNSLR_PathExistsCheckType* x) { return reinterpret_cast<Muzent::_PathExistsCheckType*>(x); }
PNSLR_PathExistsCheckType& PNSLR_Bindings_Convert(Muzent::_PathExistsCheckType& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_PathExistsCheckType& PNSLR_Bindings_Convert(PNSLR_PathExistsCheckType& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" b8 PNSLR_PathExists(PNSLR_Path path, PNSLR_PathExistsCheckType type);
b8 Muzent::_PathExists(Muzent::_Path path, Muzent::_PathExistsCheckType type)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_PathExists(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(type)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_DeletePath(PNSLR_Path path);
b8 Muzent::_DeletePath(Muzent::_Path path)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_DeletePath(PNSLR_Bindings_Convert(path)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i64 PNSLR_GetFileTimestamp(PNSLR_Path path);
i64 Muzent::_GetFileTimestamp(Muzent::_Path path)
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetFileTimestamp(PNSLR_Bindings_Convert(path)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i64 PNSLR_GetFileSize(PNSLR_Path path);
i64 Muzent::_GetFileSize(Muzent::_Path path)
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetFileSize(PNSLR_Bindings_Convert(path)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CreateDirectoryTree(PNSLR_Path path);
b8 Muzent::_CreateDirectoryTree(Muzent::_Path path)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateDirectoryTree(PNSLR_Bindings_Convert(path)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_File
{
   rawptr handle;
};
static_assert(sizeof(PNSLR_File) == sizeof(Muzent::_File), "size mismatch");
static_assert(alignof(PNSLR_File) == alignof(Muzent::_File), "align mismatch");
PNSLR_File* PNSLR_Bindings_Convert(Muzent::_File* x) { return reinterpret_cast<PNSLR_File*>(x); }
Muzent::_File* PNSLR_Bindings_Convert(PNSLR_File* x) { return reinterpret_cast<Muzent::_File*>(x); }
PNSLR_File& PNSLR_Bindings_Convert(Muzent::_File& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_File& PNSLR_Bindings_Convert(PNSLR_File& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_File, handle) == PNSLR_STRUCT_OFFSET(Muzent::_File, handle), "handle offset mismatch");

extern "C" PNSLR_File PNSLR_OpenFileToRead(PNSLR_Path path, b8 allowWrite);
Muzent::_File Muzent::_OpenFileToRead(Muzent::_Path path, b8 allowWrite)
{
    PNSLR_File zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_OpenFileToRead(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(allowWrite)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_File PNSLR_OpenFileToWrite(PNSLR_Path path, b8 append, b8 allowRead);
Muzent::_File Muzent::_OpenFileToWrite(Muzent::_Path path, b8 append, b8 allowRead)
{
    PNSLR_File zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_OpenFileToWrite(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(append), PNSLR_Bindings_Convert(allowRead)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i64 PNSLR_GetSizeOfFile(PNSLR_File handle);
i64 Muzent::_GetSizeOfFile(Muzent::_File handle)
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetSizeOfFile(PNSLR_Bindings_Convert(handle)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i64 PNSLR_GetCurrentPositionInFile(PNSLR_File handle);
i64 Muzent::_GetCurrentPositionInFile(Muzent::_File handle)
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetCurrentPositionInFile(PNSLR_Bindings_Convert(handle)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_SeekPositionInFile(PNSLR_File handle, i64 newPos, b8 relative);
b8 Muzent::_SeekPositionInFile(Muzent::_File handle, i64 newPos, b8 relative)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_SeekPositionInFile(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(newPos), PNSLR_Bindings_Convert(relative)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_ReadFromFile(PNSLR_File handle, PNSLR_ArraySlice_u8 dst, i64* readSize);
b8 Muzent::_ReadFromFile(Muzent::_File handle, ArraySlice<u8> dst, i64* readSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReadFromFile(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(dst), PNSLR_Bindings_Convert(readSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_WriteToFile(PNSLR_File handle, PNSLR_ArraySlice_u8 src);
b8 Muzent::_WriteToFile(Muzent::_File handle, ArraySlice<u8> src)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WriteToFile(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(src)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_FormatAndWriteToFile(PNSLR_File handle, PNSLR_UTF8STR fmtStr, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args);
b8 Muzent::_FormatAndWriteToFile(Muzent::_File handle, utf8str fmtStr, ArraySlice<Muzent::_PrimitiveFmtOptions> args)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FormatAndWriteToFile(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(fmtStr), PNSLR_Bindings_Convert(args)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_TruncateFile(PNSLR_File handle, i64 newSize);
b8 Muzent::_TruncateFile(Muzent::_File handle, i64 newSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_TruncateFile(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(newSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_FlushFile(PNSLR_File handle);
b8 Muzent::_FlushFile(Muzent::_File handle)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FlushFile(PNSLR_Bindings_Convert(handle)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_CloseFileHandle(PNSLR_File handle);
void Muzent::_CloseFileHandle(Muzent::_File handle)
{
    PNSLR_CloseFileHandle(PNSLR_Bindings_Convert(handle));
}

extern "C" b8 PNSLR_ReadAllContentsFromFile(PNSLR_Path path, PNSLR_ArraySlice_u8* dst, PNSLR_Allocator allocator);
b8 Muzent::_ReadAllContentsFromFile(Muzent::_Path path, ArraySlice<u8>* dst, Muzent::_Allocator allocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReadAllContentsFromFile(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(dst), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_WriteAllContentsToFile(PNSLR_Path path, PNSLR_ArraySlice_u8 src, b8 append);
b8 Muzent::_WriteAllContentsToFile(Muzent::_Path path, ArraySlice<u8> src, b8 append)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WriteAllContentsToFile(PNSLR_Bindings_Convert(path), PNSLR_Bindings_Convert(src), PNSLR_Bindings_Convert(append)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CopyFile(PNSLR_Path src, PNSLR_Path dst);
b8 Muzent::_CopyFile(Muzent::_Path src, Muzent::_Path dst)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CopyFile(PNSLR_Bindings_Convert(src), PNSLR_Bindings_Convert(dst)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_MoveFile(PNSLR_Path src, PNSLR_Path dst);
b8 Muzent::_MoveFile(Muzent::_Path src, Muzent::_Path dst)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_MoveFile(PNSLR_Bindings_Convert(src), PNSLR_Bindings_Convert(dst)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_ExitProcess(i32 exitCode);
void Muzent::_ExitProcess(i32 exitCode)
{
    PNSLR_ExitProcess(PNSLR_Bindings_Convert(exitCode));
}

struct PNSLR_EnvVarKeyValuePair
{
   PNSLR_UTF8STR kvp;
   PNSLR_UTF8STR key;
   PNSLR_UTF8STR value;
};
static_assert(sizeof(PNSLR_EnvVarKeyValuePair) == sizeof(Muzent::_EnvVarKeyValuePair), "size mismatch");
static_assert(alignof(PNSLR_EnvVarKeyValuePair) == alignof(Muzent::_EnvVarKeyValuePair), "align mismatch");
PNSLR_EnvVarKeyValuePair* PNSLR_Bindings_Convert(Muzent::_EnvVarKeyValuePair* x) { return reinterpret_cast<PNSLR_EnvVarKeyValuePair*>(x); }
Muzent::_EnvVarKeyValuePair* PNSLR_Bindings_Convert(PNSLR_EnvVarKeyValuePair* x) { return reinterpret_cast<Muzent::_EnvVarKeyValuePair*>(x); }
PNSLR_EnvVarKeyValuePair& PNSLR_Bindings_Convert(Muzent::_EnvVarKeyValuePair& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_EnvVarKeyValuePair& PNSLR_Bindings_Convert(PNSLR_EnvVarKeyValuePair& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_EnvVarKeyValuePair, kvp) == PNSLR_STRUCT_OFFSET(Muzent::_EnvVarKeyValuePair, kvp), "kvp offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_EnvVarKeyValuePair, key) == PNSLR_STRUCT_OFFSET(Muzent::_EnvVarKeyValuePair, key), "key offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_EnvVarKeyValuePair, value) == PNSLR_STRUCT_OFFSET(Muzent::_EnvVarKeyValuePair, value), "value offset mismatch");

typedef struct { PNSLR_EnvVarKeyValuePair* data; i64 count; } PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair;
static_assert(sizeof(PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair) == sizeof(ArraySlice<Muzent::_EnvVarKeyValuePair>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair) == alignof(ArraySlice<Muzent::_EnvVarKeyValuePair>), "align mismatch");
PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair* PNSLR_Bindings_Convert(ArraySlice<Muzent::_EnvVarKeyValuePair>* x) { return reinterpret_cast<PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair*>(x); }
ArraySlice<Muzent::_EnvVarKeyValuePair>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair* x) { return reinterpret_cast<ArraySlice<Muzent::_EnvVarKeyValuePair>*>(x); }
PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair& PNSLR_Bindings_Convert(ArraySlice<Muzent::_EnvVarKeyValuePair>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<Muzent::_EnvVarKeyValuePair>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair, count) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_EnvVarKeyValuePair>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair, data) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_EnvVarKeyValuePair>, data), "data offset mismatch");

extern "C" PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair PNSLR_GetEnvironmentVariables(PNSLR_Allocator allocator);
ArraySlice<Muzent::_EnvVarKeyValuePair> Muzent::_GetEnvironmentVariables(Muzent::_Allocator allocator)
{
    PNSLR_ArraySlice_PNSLR_EnvVarKeyValuePair zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetEnvironmentVariables(PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_PipeHandle
{
   u64 platformHandle;
};
static_assert(sizeof(PNSLR_PipeHandle) == sizeof(Muzent::_PipeHandle), "size mismatch");
static_assert(alignof(PNSLR_PipeHandle) == alignof(Muzent::_PipeHandle), "align mismatch");
PNSLR_PipeHandle* PNSLR_Bindings_Convert(Muzent::_PipeHandle* x) { return reinterpret_cast<PNSLR_PipeHandle*>(x); }
Muzent::_PipeHandle* PNSLR_Bindings_Convert(PNSLR_PipeHandle* x) { return reinterpret_cast<Muzent::_PipeHandle*>(x); }
PNSLR_PipeHandle& PNSLR_Bindings_Convert(Muzent::_PipeHandle& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_PipeHandle& PNSLR_Bindings_Convert(PNSLR_PipeHandle& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_PipeHandle, platformHandle) == PNSLR_STRUCT_OFFSET(Muzent::_PipeHandle, platformHandle), "platformHandle offset mismatch");

extern "C" b8 PNSLR_CreatePipe(PNSLR_PipeHandle* outR, PNSLR_PipeHandle* outW);
b8 Muzent::_CreatePipe(Muzent::_PipeHandle* outR, Muzent::_PipeHandle* outW)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreatePipe(PNSLR_Bindings_Convert(outR), PNSLR_Bindings_Convert(outW)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_GetRemainingPipeReadSize(PNSLR_PipeHandle pipe, i64* outSize);
b8 Muzent::_GetRemainingPipeReadSize(Muzent::_PipeHandle pipe, i64* outSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetRemainingPipeReadSize(PNSLR_Bindings_Convert(pipe), PNSLR_Bindings_Convert(outSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_ReadFromPipe(PNSLR_PipeHandle pipe, PNSLR_ArraySlice_u8 dst, i64* readSize);
b8 Muzent::_ReadFromPipe(Muzent::_PipeHandle pipe, ArraySlice<u8> dst, i64* readSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReadFromPipe(PNSLR_Bindings_Convert(pipe), PNSLR_Bindings_Convert(dst), PNSLR_Bindings_Convert(readSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_WriteToPipe(PNSLR_PipeHandle pipe, PNSLR_ArraySlice_u8 src);
b8 Muzent::_WriteToPipe(Muzent::_PipeHandle pipe, ArraySlice<u8> src)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WriteToPipe(PNSLR_Bindings_Convert(pipe), PNSLR_Bindings_Convert(src)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_ClosePipe(PNSLR_PipeHandle pipe);
b8 Muzent::_ClosePipe(Muzent::_PipeHandle pipe)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ClosePipe(PNSLR_Bindings_Convert(pipe)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_ProcessHandle
{
   i64 pid;
   u64 handle;
};
static_assert(sizeof(PNSLR_ProcessHandle) == sizeof(Muzent::_ProcessHandle), "size mismatch");
static_assert(alignof(PNSLR_ProcessHandle) == alignof(Muzent::_ProcessHandle), "align mismatch");
PNSLR_ProcessHandle* PNSLR_Bindings_Convert(Muzent::_ProcessHandle* x) { return reinterpret_cast<PNSLR_ProcessHandle*>(x); }
Muzent::_ProcessHandle* PNSLR_Bindings_Convert(PNSLR_ProcessHandle* x) { return reinterpret_cast<Muzent::_ProcessHandle*>(x); }
PNSLR_ProcessHandle& PNSLR_Bindings_Convert(Muzent::_ProcessHandle& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ProcessHandle& PNSLR_Bindings_Convert(PNSLR_ProcessHandle& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ProcessHandle, pid) == PNSLR_STRUCT_OFFSET(Muzent::_ProcessHandle, pid), "pid offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ProcessHandle, handle) == PNSLR_STRUCT_OFFSET(Muzent::_ProcessHandle, handle), "handle offset mismatch");

extern "C" b8 PNSLR_RunProcess(PNSLR_ProcessHandle* outProcessHandle, PNSLR_ArraySlice_PNSLR_UTF8STR execAndArgs, PNSLR_ArraySlice_PNSLR_UTF8STR environmentVariables, PNSLR_Path workingDirectory, PNSLR_PipeHandle* stdOutPipe, PNSLR_PipeHandle* stdErrPipe);
b8 Muzent::_RunProcess(Muzent::_ProcessHandle* outProcessHandle, ArraySlice<utf8str> execAndArgs, ArraySlice<utf8str> environmentVariables, Muzent::_Path workingDirectory, Muzent::_PipeHandle* stdOutPipe, Muzent::_PipeHandle* stdErrPipe)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_RunProcess(PNSLR_Bindings_Convert(outProcessHandle), PNSLR_Bindings_Convert(execAndArgs), PNSLR_Bindings_Convert(environmentVariables), PNSLR_Bindings_Convert(workingDirectory), PNSLR_Bindings_Convert(stdOutPipe), PNSLR_Bindings_Convert(stdErrPipe)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_WaitForProcess(PNSLR_ProcessHandle* process, i32* outExitCode);
b8 Muzent::_WaitForProcess(Muzent::_ProcessHandle* process, i32* outExitCode)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WaitForProcess(PNSLR_Bindings_Convert(process), PNSLR_Bindings_Convert(outExitCode)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_KillProcess(PNSLR_ProcessHandle* process);
b8 Muzent::_KillProcess(Muzent::_ProcessHandle* process)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_KillProcess(PNSLR_Bindings_Convert(process)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_CloseProcess(PNSLR_ProcessHandle* process);
void Muzent::_CloseProcess(Muzent::_ProcessHandle* process)
{
    PNSLR_CloseProcess(PNSLR_Bindings_Convert(process));
}

typedef PNSLR_ArraySlice_u8 PNSLR_IPAddress;

typedef PNSLR_ArraySlice_u8 PNSLR_IPMask;

struct PNSLR_IPNetwork
{
   PNSLR_IPAddress address;
   PNSLR_IPMask mask;
};
static_assert(sizeof(PNSLR_IPNetwork) == sizeof(Muzent::_IPNetwork), "size mismatch");
static_assert(alignof(PNSLR_IPNetwork) == alignof(Muzent::_IPNetwork), "align mismatch");
PNSLR_IPNetwork* PNSLR_Bindings_Convert(Muzent::_IPNetwork* x) { return reinterpret_cast<PNSLR_IPNetwork*>(x); }
Muzent::_IPNetwork* PNSLR_Bindings_Convert(PNSLR_IPNetwork* x) { return reinterpret_cast<Muzent::_IPNetwork*>(x); }
PNSLR_IPNetwork& PNSLR_Bindings_Convert(Muzent::_IPNetwork& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_IPNetwork& PNSLR_Bindings_Convert(PNSLR_IPNetwork& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_IPNetwork, address) == PNSLR_STRUCT_OFFSET(Muzent::_IPNetwork, address), "address offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_IPNetwork, mask) == PNSLR_STRUCT_OFFSET(Muzent::_IPNetwork, mask), "mask offset mismatch");

typedef struct { PNSLR_IPNetwork* data; i64 count; } PNSLR_ArraySlice_PNSLR_IPNetwork;
static_assert(sizeof(PNSLR_ArraySlice_PNSLR_IPNetwork) == sizeof(ArraySlice<Muzent::_IPNetwork>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_PNSLR_IPNetwork) == alignof(ArraySlice<Muzent::_IPNetwork>), "align mismatch");
PNSLR_ArraySlice_PNSLR_IPNetwork* PNSLR_Bindings_Convert(ArraySlice<Muzent::_IPNetwork>* x) { return reinterpret_cast<PNSLR_ArraySlice_PNSLR_IPNetwork*>(x); }
ArraySlice<Muzent::_IPNetwork>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_IPNetwork* x) { return reinterpret_cast<ArraySlice<Muzent::_IPNetwork>*>(x); }
PNSLR_ArraySlice_PNSLR_IPNetwork& PNSLR_Bindings_Convert(ArraySlice<Muzent::_IPNetwork>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<Muzent::_IPNetwork>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_IPNetwork& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_IPNetwork, count) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_IPNetwork>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_IPNetwork, data) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_IPNetwork>, data), "data offset mismatch");

extern "C" b8 PNSLR_GetInterfaceIPAddresses(PNSLR_ArraySlice_PNSLR_IPNetwork* networks, PNSLR_Allocator allocator);
b8 Muzent::_GetInterfaceIPAddresses(ArraySlice<Muzent::_IPNetwork>* networks, Muzent::_Allocator allocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetInterfaceIPAddresses(PNSLR_Bindings_Convert(networks), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

enum class PNSLR_StreamMode : u8 { };
static_assert(sizeof(PNSLR_StreamMode) == sizeof(Muzent::_StreamMode), "size mismatch");
static_assert(alignof(PNSLR_StreamMode) == alignof(Muzent::_StreamMode), "align mismatch");
PNSLR_StreamMode* PNSLR_Bindings_Convert(Muzent::_StreamMode* x) { return reinterpret_cast<PNSLR_StreamMode*>(x); }
Muzent::_StreamMode* PNSLR_Bindings_Convert(PNSLR_StreamMode* x) { return reinterpret_cast<Muzent::_StreamMode*>(x); }
PNSLR_StreamMode& PNSLR_Bindings_Convert(Muzent::_StreamMode& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StreamMode& PNSLR_Bindings_Convert(PNSLR_StreamMode& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" typedef b8 (*PNSLR_StreamProcedure)(rawptr streamData, PNSLR_StreamMode mode, PNSLR_ArraySlice_u8 data, i64 offset, i64* extraRet);
static_assert(sizeof(PNSLR_StreamProcedure) == sizeof(Muzent::_StreamProcedure), "size mismatch");
static_assert(alignof(PNSLR_StreamProcedure) == alignof(Muzent::_StreamProcedure), "align mismatch");
PNSLR_StreamProcedure* PNSLR_Bindings_Convert(Muzent::_StreamProcedure* x) { return reinterpret_cast<PNSLR_StreamProcedure*>(x); }
Muzent::_StreamProcedure* PNSLR_Bindings_Convert(PNSLR_StreamProcedure* x) { return reinterpret_cast<Muzent::_StreamProcedure*>(x); }
PNSLR_StreamProcedure& PNSLR_Bindings_Convert(Muzent::_StreamProcedure& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_StreamProcedure& PNSLR_Bindings_Convert(PNSLR_StreamProcedure& x) { return *PNSLR_Bindings_Convert(&x); }

struct PNSLR_Stream
{
   PNSLR_StreamProcedure procedure;
   rawptr data;
};
static_assert(sizeof(PNSLR_Stream) == sizeof(Muzent::_Stream), "size mismatch");
static_assert(alignof(PNSLR_Stream) == alignof(Muzent::_Stream), "align mismatch");
PNSLR_Stream* PNSLR_Bindings_Convert(Muzent::_Stream* x) { return reinterpret_cast<PNSLR_Stream*>(x); }
Muzent::_Stream* PNSLR_Bindings_Convert(PNSLR_Stream* x) { return reinterpret_cast<Muzent::_Stream*>(x); }
PNSLR_Stream& PNSLR_Bindings_Convert(Muzent::_Stream& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Stream& PNSLR_Bindings_Convert(PNSLR_Stream& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Stream, procedure) == PNSLR_STRUCT_OFFSET(Muzent::_Stream, procedure), "procedure offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Stream, data) == PNSLR_STRUCT_OFFSET(Muzent::_Stream, data), "data offset mismatch");

typedef struct { PNSLR_Stream* data; i64 count; } PNSLR_ArraySlice_PNSLR_Stream;
static_assert(sizeof(PNSLR_ArraySlice_PNSLR_Stream) == sizeof(ArraySlice<Muzent::_Stream>), "size mismatch");
static_assert(alignof(PNSLR_ArraySlice_PNSLR_Stream) == alignof(ArraySlice<Muzent::_Stream>), "align mismatch");
PNSLR_ArraySlice_PNSLR_Stream* PNSLR_Bindings_Convert(ArraySlice<Muzent::_Stream>* x) { return reinterpret_cast<PNSLR_ArraySlice_PNSLR_Stream*>(x); }
ArraySlice<Muzent::_Stream>* PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_Stream* x) { return reinterpret_cast<ArraySlice<Muzent::_Stream>*>(x); }
PNSLR_ArraySlice_PNSLR_Stream& PNSLR_Bindings_Convert(ArraySlice<Muzent::_Stream>& x) { return *PNSLR_Bindings_Convert(&x); }
ArraySlice<Muzent::_Stream>& PNSLR_Bindings_Convert(PNSLR_ArraySlice_PNSLR_Stream& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_Stream, count) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_Stream>, count), "count offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ArraySlice_PNSLR_Stream, data) == PNSLR_STRUCT_OFFSET(ArraySlice<Muzent::_Stream>, data), "data offset mismatch");

extern "C" i64 PNSLR_GetSizeOfStream(PNSLR_Stream stream);
i64 Muzent::_GetSizeOfStream(Muzent::_Stream stream)
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetSizeOfStream(PNSLR_Bindings_Convert(stream)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" i64 PNSLR_GetCurrentPositionInStream(PNSLR_Stream stream);
i64 Muzent::_GetCurrentPositionInStream(Muzent::_Stream stream)
{
    i64 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetCurrentPositionInStream(PNSLR_Bindings_Convert(stream)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_SeekPositionInStream(PNSLR_Stream stream, i64 newPos, b8 relative);
b8 Muzent::_SeekPositionInStream(Muzent::_Stream stream, i64 newPos, b8 relative)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_SeekPositionInStream(PNSLR_Bindings_Convert(stream), PNSLR_Bindings_Convert(newPos), PNSLR_Bindings_Convert(relative)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_ReadFromStream(PNSLR_Stream stream, PNSLR_ArraySlice_u8 dst, i64* readSize);
b8 Muzent::_ReadFromStream(Muzent::_Stream stream, ArraySlice<u8> dst, i64* readSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReadFromStream(PNSLR_Bindings_Convert(stream), PNSLR_Bindings_Convert(dst), PNSLR_Bindings_Convert(readSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_WriteToStream(PNSLR_Stream stream, PNSLR_ArraySlice_u8 src);
b8 Muzent::_WriteToStream(Muzent::_Stream stream, ArraySlice<u8> src)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_WriteToStream(PNSLR_Bindings_Convert(stream), PNSLR_Bindings_Convert(src)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_FormatAndWriteToStream(PNSLR_Stream stream, PNSLR_UTF8STR fmtStr, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args);
b8 Muzent::_FormatAndWriteToStream(Muzent::_Stream stream, utf8str fmtStr, ArraySlice<Muzent::_PrimitiveFmtOptions> args)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FormatAndWriteToStream(PNSLR_Bindings_Convert(stream), PNSLR_Bindings_Convert(fmtStr), PNSLR_Bindings_Convert(args)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_TruncateStream(PNSLR_Stream stream, i64 newSize);
b8 Muzent::_TruncateStream(Muzent::_Stream stream, i64 newSize)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_TruncateStream(PNSLR_Bindings_Convert(stream), PNSLR_Bindings_Convert(newSize)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_FlushStream(PNSLR_Stream stream);
b8 Muzent::_FlushStream(Muzent::_Stream stream)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_FlushStream(PNSLR_Bindings_Convert(stream)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_CloseStream(PNSLR_Stream stream);
void Muzent::_CloseStream(Muzent::_Stream stream)
{
    PNSLR_CloseStream(PNSLR_Bindings_Convert(stream));
}

extern "C" PNSLR_Stream PNSLR_StreamFromFile(PNSLR_File file);
Muzent::_Stream Muzent::_StreamFromFile(Muzent::_File file)
{
    PNSLR_Stream zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StreamFromFile(PNSLR_Bindings_Convert(file)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Stream PNSLR_StreamFromStringBuilder(PNSLR_StringBuilder* builder);
Muzent::_Stream Muzent::_StreamFromStringBuilder(Muzent::_StringBuilder* builder)
{
    PNSLR_Stream zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StreamFromStringBuilder(PNSLR_Bindings_Convert(builder)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Stream PNSLR_StreamFromStdOut(b8 disableBuffering);
Muzent::_Stream Muzent::_StreamFromStdOut(b8 disableBuffering)
{
    PNSLR_Stream zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StreamFromStdOut(PNSLR_Bindings_Convert(disableBuffering)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Stream PNSLR_StreamFromStdErr(b8 disableBuffering);
Muzent::_Stream Muzent::_StreamFromStdErr(b8 disableBuffering)
{
    PNSLR_Stream zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StreamFromStdErr(PNSLR_Bindings_Convert(disableBuffering)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

enum class PNSLR_LoggerLevel : u8 { };
static_assert(sizeof(PNSLR_LoggerLevel) == sizeof(Muzent::_LoggerLevel), "size mismatch");
static_assert(alignof(PNSLR_LoggerLevel) == alignof(Muzent::_LoggerLevel), "align mismatch");
PNSLR_LoggerLevel* PNSLR_Bindings_Convert(Muzent::_LoggerLevel* x) { return reinterpret_cast<PNSLR_LoggerLevel*>(x); }
Muzent::_LoggerLevel* PNSLR_Bindings_Convert(PNSLR_LoggerLevel* x) { return reinterpret_cast<Muzent::_LoggerLevel*>(x); }
PNSLR_LoggerLevel& PNSLR_Bindings_Convert(Muzent::_LoggerLevel& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_LoggerLevel& PNSLR_Bindings_Convert(PNSLR_LoggerLevel& x) { return *PNSLR_Bindings_Convert(&x); }

enum class PNSLR_LogOption : u8 { };
static_assert(sizeof(PNSLR_LogOption) == sizeof(Muzent::_LogOption), "size mismatch");
static_assert(alignof(PNSLR_LogOption) == alignof(Muzent::_LogOption), "align mismatch");
PNSLR_LogOption* PNSLR_Bindings_Convert(Muzent::_LogOption* x) { return reinterpret_cast<PNSLR_LogOption*>(x); }
Muzent::_LogOption* PNSLR_Bindings_Convert(PNSLR_LogOption* x) { return reinterpret_cast<Muzent::_LogOption*>(x); }
PNSLR_LogOption& PNSLR_Bindings_Convert(Muzent::_LogOption& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_LogOption& PNSLR_Bindings_Convert(PNSLR_LogOption& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" typedef void (*PNSLR_LoggerProcedure)(rawptr loggerData, PNSLR_LoggerLevel level, PNSLR_UTF8STR data, PNSLR_LogOption options, PNSLR_SourceCodeLocation location);
static_assert(sizeof(PNSLR_LoggerProcedure) == sizeof(Muzent::_LoggerProcedure), "size mismatch");
static_assert(alignof(PNSLR_LoggerProcedure) == alignof(Muzent::_LoggerProcedure), "align mismatch");
PNSLR_LoggerProcedure* PNSLR_Bindings_Convert(Muzent::_LoggerProcedure* x) { return reinterpret_cast<PNSLR_LoggerProcedure*>(x); }
Muzent::_LoggerProcedure* PNSLR_Bindings_Convert(PNSLR_LoggerProcedure* x) { return reinterpret_cast<Muzent::_LoggerProcedure*>(x); }
PNSLR_LoggerProcedure& PNSLR_Bindings_Convert(Muzent::_LoggerProcedure& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_LoggerProcedure& PNSLR_Bindings_Convert(PNSLR_LoggerProcedure& x) { return *PNSLR_Bindings_Convert(&x); }

struct PNSLR_Logger
{
   PNSLR_LoggerProcedure procedure;
   rawptr data;
   PNSLR_LoggerLevel minAllowedLvl;
   PNSLR_LogOption options;
};
static_assert(sizeof(PNSLR_Logger) == sizeof(Muzent::_Logger), "size mismatch");
static_assert(alignof(PNSLR_Logger) == alignof(Muzent::_Logger), "align mismatch");
PNSLR_Logger* PNSLR_Bindings_Convert(Muzent::_Logger* x) { return reinterpret_cast<PNSLR_Logger*>(x); }
Muzent::_Logger* PNSLR_Bindings_Convert(PNSLR_Logger* x) { return reinterpret_cast<Muzent::_Logger*>(x); }
PNSLR_Logger& PNSLR_Bindings_Convert(Muzent::_Logger& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_Logger& PNSLR_Bindings_Convert(PNSLR_Logger& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Logger, procedure) == PNSLR_STRUCT_OFFSET(Muzent::_Logger, procedure), "procedure offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Logger, data) == PNSLR_STRUCT_OFFSET(Muzent::_Logger, data), "data offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Logger, minAllowedLvl) == PNSLR_STRUCT_OFFSET(Muzent::_Logger, minAllowedLvl), "minAllowedLvl offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_Logger, options) == PNSLR_STRUCT_OFFSET(Muzent::_Logger, options), "options offset mismatch");

extern "C" void PNSLR_SetDefaultLogger(PNSLR_Logger logger);
void Muzent::_SetDefaultLogger(Muzent::_Logger logger)
{
    PNSLR_SetDefaultLogger(PNSLR_Bindings_Convert(logger));
}

extern "C" void PNSLR_DisableDefaultLogger();
void Muzent::_DisableDefaultLogger()
{
    PNSLR_DisableDefaultLogger();
}

extern "C" void PNSLR_LogD(PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogD(utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogD(PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogI(PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogI(utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogI(PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogW(PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogW(utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogW(PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogE(PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogE(utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogE(PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogC(PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogC(utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogC(PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogDf(PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogDf(utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogDf(PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogIf(PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogIf(utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogIf(PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogWf(PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogWf(utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogWf(PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogEf(PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogEf(utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogEf(PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogCf(PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogCf(utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogCf(PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLD(PNSLR_Logger logger, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLD(Muzent::_Logger logger, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLD(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLI(PNSLR_Logger logger, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLI(Muzent::_Logger logger, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLI(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLW(PNSLR_Logger logger, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLW(Muzent::_Logger logger, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLW(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLE(PNSLR_Logger logger, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLE(Muzent::_Logger logger, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLE(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLC(PNSLR_Logger logger, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLC(Muzent::_Logger logger, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLC(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLDf(PNSLR_Logger logger, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLDf(Muzent::_Logger logger, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLDf(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLIf(PNSLR_Logger logger, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLIf(Muzent::_Logger logger, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLIf(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLWf(PNSLR_Logger logger, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLWf(Muzent::_Logger logger, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLWf(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLEf(PNSLR_Logger logger, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLEf(Muzent::_Logger logger, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLEf(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLCf(PNSLR_Logger logger, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLCf(Muzent::_Logger logger, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLCf(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_Log(PNSLR_LoggerLevel level, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_Log(Muzent::_LoggerLevel level, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_Log(PNSLR_Bindings_Convert(level), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_Logf(PNSLR_LoggerLevel level, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_Logf(Muzent::_LoggerLevel level, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_Logf(PNSLR_Bindings_Convert(level), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogL(PNSLR_Logger logger, PNSLR_LoggerLevel level, PNSLR_UTF8STR msg, PNSLR_SourceCodeLocation loc);
void Muzent::_LogL(Muzent::_Logger logger, Muzent::_LoggerLevel level, utf8str msg, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogL(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(level), PNSLR_Bindings_Convert(msg), PNSLR_Bindings_Convert(loc));
}

extern "C" void PNSLR_LogLf(PNSLR_Logger logger, PNSLR_LoggerLevel level, PNSLR_UTF8STR fmtMsg, PNSLR_ArraySlice_PNSLR_PrimitiveFmtOptions args, PNSLR_SourceCodeLocation loc);
void Muzent::_LogLf(Muzent::_Logger logger, Muzent::_LoggerLevel level, utf8str fmtMsg, ArraySlice<Muzent::_PrimitiveFmtOptions> args, Muzent::_SourceCodeLocation loc)
{
    PNSLR_LogLf(PNSLR_Bindings_Convert(logger), PNSLR_Bindings_Convert(level), PNSLR_Bindings_Convert(fmtMsg), PNSLR_Bindings_Convert(args), PNSLR_Bindings_Convert(loc));
}

extern "C" PNSLR_Logger PNSLR_LoggerFromFile(PNSLR_File f, PNSLR_LoggerLevel minAllowedLevel, PNSLR_LogOption options);
Muzent::_Logger Muzent::_LoggerFromFile(Muzent::_File f, Muzent::_LoggerLevel minAllowedLevel, Muzent::_LogOption options)
{
    PNSLR_Logger zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_LoggerFromFile(PNSLR_Bindings_Convert(f), PNSLR_Bindings_Convert(minAllowedLevel), PNSLR_Bindings_Convert(options)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Logger PNSLR_GetDefaultLoggerWithOptions(PNSLR_LoggerLevel minAllowedLevel, PNSLR_LogOption options);
Muzent::_Logger Muzent::_GetDefaultLoggerWithOptions(Muzent::_LoggerLevel minAllowedLevel, Muzent::_LogOption options)
{
    PNSLR_Logger zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetDefaultLoggerWithOptions(PNSLR_Bindings_Convert(minAllowedLevel), PNSLR_Bindings_Convert(options)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_Logger PNSLR_GetNilLogger();
Muzent::_Logger Muzent::_GetNilLogger()
{
    PNSLR_Logger zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetNilLogger(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct PNSLR_ThreadHandle
{
   u64 handle;
};
static_assert(sizeof(PNSLR_ThreadHandle) == sizeof(Muzent::_ThreadHandle), "size mismatch");
static_assert(alignof(PNSLR_ThreadHandle) == alignof(Muzent::_ThreadHandle), "align mismatch");
PNSLR_ThreadHandle* PNSLR_Bindings_Convert(Muzent::_ThreadHandle* x) { return reinterpret_cast<PNSLR_ThreadHandle*>(x); }
Muzent::_ThreadHandle* PNSLR_Bindings_Convert(PNSLR_ThreadHandle* x) { return reinterpret_cast<Muzent::_ThreadHandle*>(x); }
PNSLR_ThreadHandle& PNSLR_Bindings_Convert(Muzent::_ThreadHandle& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ThreadHandle& PNSLR_Bindings_Convert(PNSLR_ThreadHandle& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_ThreadHandle, handle) == PNSLR_STRUCT_OFFSET(Muzent::_ThreadHandle, handle), "handle offset mismatch");

extern "C" b8 PNSLR_IsThreadHandleValid(PNSLR_ThreadHandle handle);
b8 Muzent::_IsThreadHandleValid(Muzent::_ThreadHandle handle)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_IsThreadHandleValid(PNSLR_Bindings_Convert(handle)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_ThreadHandle PNSLR_GetCurrentThreadHandle();
Muzent::_ThreadHandle Muzent::_GetCurrentThreadHandle()
{
    PNSLR_ThreadHandle zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetCurrentThreadHandle(); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" PNSLR_UTF8STR PNSLR_GetThreadName(PNSLR_ThreadHandle handle, PNSLR_Allocator allocator);
utf8str Muzent::_GetThreadName(Muzent::_ThreadHandle handle, Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetThreadName(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_SetThreadName(PNSLR_ThreadHandle handle, PNSLR_UTF8STR name);
void Muzent::_SetThreadName(Muzent::_ThreadHandle handle, utf8str name)
{
    PNSLR_SetThreadName(PNSLR_Bindings_Convert(handle), PNSLR_Bindings_Convert(name));
}

extern "C" PNSLR_UTF8STR PNSLR_GetCurrentThreadName(PNSLR_Allocator allocator);
utf8str Muzent::_GetCurrentThreadName(Muzent::_Allocator allocator)
{
    PNSLR_UTF8STR zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_GetCurrentThreadName(PNSLR_Bindings_Convert(allocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_SetCurrentThreadName(PNSLR_UTF8STR name);
void Muzent::_SetCurrentThreadName(utf8str name)
{
    PNSLR_SetCurrentThreadName(PNSLR_Bindings_Convert(name));
}

extern "C" typedef void (*PNSLR_ThreadProcedure)(rawptr data);
static_assert(sizeof(PNSLR_ThreadProcedure) == sizeof(Muzent::_ThreadProcedure), "size mismatch");
static_assert(alignof(PNSLR_ThreadProcedure) == alignof(Muzent::_ThreadProcedure), "align mismatch");
PNSLR_ThreadProcedure* PNSLR_Bindings_Convert(Muzent::_ThreadProcedure* x) { return reinterpret_cast<PNSLR_ThreadProcedure*>(x); }
Muzent::_ThreadProcedure* PNSLR_Bindings_Convert(PNSLR_ThreadProcedure* x) { return reinterpret_cast<Muzent::_ThreadProcedure*>(x); }
PNSLR_ThreadProcedure& PNSLR_Bindings_Convert(Muzent::_ThreadProcedure& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_ThreadProcedure& PNSLR_Bindings_Convert(PNSLR_ThreadProcedure& x) { return *PNSLR_Bindings_Convert(&x); }

extern "C" PNSLR_ThreadHandle PNSLR_StartThread(PNSLR_ThreadProcedure procedure, rawptr data, PNSLR_UTF8STR name);
Muzent::_ThreadHandle Muzent::_StartThread(Muzent::_ThreadProcedure procedure, rawptr data, utf8str name)
{
    PNSLR_ThreadHandle zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_StartThread(PNSLR_Bindings_Convert(procedure), PNSLR_Bindings_Convert(data), PNSLR_Bindings_Convert(name)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" void PNSLR_JoinThread(PNSLR_ThreadHandle handle);
void Muzent::_JoinThread(Muzent::_ThreadHandle handle)
{
    PNSLR_JoinThread(PNSLR_Bindings_Convert(handle));
}

extern "C" void PNSLR_SleepCurrentThread(u64 milliseconds);
void Muzent::_SleepCurrentThread(u64 milliseconds)
{
    PNSLR_SleepCurrentThread(PNSLR_Bindings_Convert(milliseconds));
}

struct PNSLR_SharedMemoryChannelHandle
{
   i64 handle;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelHandle) == sizeof(Muzent::_SharedMemoryChannelHandle), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelHandle) == alignof(Muzent::_SharedMemoryChannelHandle), "align mismatch");
PNSLR_SharedMemoryChannelHandle* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelHandle* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelHandle*>(x); }
Muzent::_SharedMemoryChannelHandle* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelHandle* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelHandle*>(x); }
PNSLR_SharedMemoryChannelHandle& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelHandle& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelHandle& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelHandle& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHandle, handle) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHandle, handle), "handle offset mismatch");

struct PNSLR_SharedMemoryChannelPlatformHeader
{
   u32 magicNum;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelPlatformHeader) == sizeof(Muzent::_SharedMemoryChannelPlatformHeader), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelPlatformHeader) == alignof(Muzent::_SharedMemoryChannelPlatformHeader), "align mismatch");
PNSLR_SharedMemoryChannelPlatformHeader* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelPlatformHeader* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelPlatformHeader*>(x); }
Muzent::_SharedMemoryChannelPlatformHeader* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelPlatformHeader* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelPlatformHeader*>(x); }
PNSLR_SharedMemoryChannelPlatformHeader& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelPlatformHeader& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelPlatformHeader& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelPlatformHeader& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelPlatformHeader, magicNum) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelPlatformHeader, magicNum), "magicNum offset mismatch");

enum class PNSLR_SharedMemoryChannelStatus : u8 { };
static_assert(sizeof(PNSLR_SharedMemoryChannelStatus) == sizeof(Muzent::_SharedMemoryChannelStatus), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelStatus) == alignof(Muzent::_SharedMemoryChannelStatus), "align mismatch");
PNSLR_SharedMemoryChannelStatus* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelStatus* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelStatus*>(x); }
Muzent::_SharedMemoryChannelStatus* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelStatus* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelStatus*>(x); }
PNSLR_SharedMemoryChannelStatus& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelStatus& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelStatus& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelStatus& x) { return *PNSLR_Bindings_Convert(&x); }

struct PNSLR_SharedMemoryChannelHeader
{
   u32 magicNum;
   u32 version;
   PNSLR_SharedMemoryChannelStatus readerStatus;
   PNSLR_SharedMemoryChannelStatus writerStatus;
   u32 offsetToOsSpecificHeader;
   u32 offsetToMsgQueueHeader;
   u32 offsetToMsgData;
   i64 fullMemRegionSize;
   i64 dataSize;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelHeader) == sizeof(Muzent::_SharedMemoryChannelHeader), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelHeader) == alignof(Muzent::_SharedMemoryChannelHeader), "align mismatch");
PNSLR_SharedMemoryChannelHeader* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelHeader* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelHeader*>(x); }
Muzent::_SharedMemoryChannelHeader* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelHeader* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelHeader*>(x); }
PNSLR_SharedMemoryChannelHeader& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelHeader& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelHeader& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelHeader& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, magicNum) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, magicNum), "magicNum offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, version) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, version), "version offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, readerStatus) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, readerStatus), "readerStatus offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, writerStatus) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, writerStatus), "writerStatus offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, offsetToOsSpecificHeader) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, offsetToOsSpecificHeader), "offsetToOsSpecificHeader offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, offsetToMsgQueueHeader) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, offsetToMsgQueueHeader), "offsetToMsgQueueHeader offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, offsetToMsgData) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, offsetToMsgData), "offsetToMsgData offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, fullMemRegionSize) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, fullMemRegionSize), "fullMemRegionSize offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelHeader, dataSize) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelHeader, dataSize), "dataSize offset mismatch");

struct PNSLR_SharedMemoryChannelMessageQueueHeader
{
   i64 readCursor;
   u8 padding[56];
   i64 writeCursor;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelMessageQueueHeader) == sizeof(Muzent::_SharedMemoryChannelMessageQueueHeader), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelMessageQueueHeader) == alignof(Muzent::_SharedMemoryChannelMessageQueueHeader), "align mismatch");
PNSLR_SharedMemoryChannelMessageQueueHeader* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelMessageQueueHeader* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelMessageQueueHeader*>(x); }
Muzent::_SharedMemoryChannelMessageQueueHeader* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelMessageQueueHeader* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelMessageQueueHeader*>(x); }
PNSLR_SharedMemoryChannelMessageQueueHeader& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelMessageQueueHeader& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelMessageQueueHeader& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelMessageQueueHeader& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessageQueueHeader, readCursor) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessageQueueHeader, readCursor), "readCursor offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessageQueueHeader, padding) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessageQueueHeader, padding), "padding offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessageQueueHeader, writeCursor) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessageQueueHeader, writeCursor), "writeCursor offset mismatch");

struct PNSLR_SharedMemoryChannelReader
{
   PNSLR_SharedMemoryChannelHeader* header;
   PNSLR_SharedMemoryChannelHandle handle;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelReader) == sizeof(Muzent::_SharedMemoryChannelReader), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelReader) == alignof(Muzent::_SharedMemoryChannelReader), "align mismatch");
PNSLR_SharedMemoryChannelReader* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelReader* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelReader*>(x); }
Muzent::_SharedMemoryChannelReader* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelReader* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelReader*>(x); }
PNSLR_SharedMemoryChannelReader& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelReader& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelReader& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelReader& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelReader, header) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelReader, header), "header offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelReader, handle) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelReader, handle), "handle offset mismatch");

struct PNSLR_SharedMemoryChannelWriter
{
   PNSLR_SharedMemoryChannelHeader* header;
   PNSLR_SharedMemoryChannelHandle handle;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelWriter) == sizeof(Muzent::_SharedMemoryChannelWriter), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelWriter) == alignof(Muzent::_SharedMemoryChannelWriter), "align mismatch");
PNSLR_SharedMemoryChannelWriter* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelWriter* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelWriter*>(x); }
Muzent::_SharedMemoryChannelWriter* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelWriter* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelWriter*>(x); }
PNSLR_SharedMemoryChannelWriter& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelWriter& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelWriter& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelWriter& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelWriter, header) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelWriter, header), "header offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelWriter, handle) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelWriter, handle), "handle offset mismatch");

struct PNSLR_SharedMemoryChannelReservedMessage
{
   PNSLR_SharedMemoryChannelWriter* channel;
   i64 offset;
   i64 size;
   u8* writePtr;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelReservedMessage) == sizeof(Muzent::_SharedMemoryChannelReservedMessage), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelReservedMessage) == alignof(Muzent::_SharedMemoryChannelReservedMessage), "align mismatch");
PNSLR_SharedMemoryChannelReservedMessage* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelReservedMessage* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelReservedMessage*>(x); }
Muzent::_SharedMemoryChannelReservedMessage* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelReservedMessage* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelReservedMessage*>(x); }
PNSLR_SharedMemoryChannelReservedMessage& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelReservedMessage& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelReservedMessage& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelReservedMessage& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelReservedMessage, channel) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelReservedMessage, channel), "channel offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelReservedMessage, offset) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelReservedMessage, offset), "offset offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelReservedMessage, size) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelReservedMessage, size), "size offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelReservedMessage, writePtr) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelReservedMessage, writePtr), "writePtr offset mismatch");

struct PNSLR_SharedMemoryChannelMessage
{
   PNSLR_SharedMemoryChannelReader* channel;
   i64 offset;
   i64 size;
   u8* readPtr;
   i64 readSize;
};
static_assert(sizeof(PNSLR_SharedMemoryChannelMessage) == sizeof(Muzent::_SharedMemoryChannelMessage), "size mismatch");
static_assert(alignof(PNSLR_SharedMemoryChannelMessage) == alignof(Muzent::_SharedMemoryChannelMessage), "align mismatch");
PNSLR_SharedMemoryChannelMessage* PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelMessage* x) { return reinterpret_cast<PNSLR_SharedMemoryChannelMessage*>(x); }
Muzent::_SharedMemoryChannelMessage* PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelMessage* x) { return reinterpret_cast<Muzent::_SharedMemoryChannelMessage*>(x); }
PNSLR_SharedMemoryChannelMessage& PNSLR_Bindings_Convert(Muzent::_SharedMemoryChannelMessage& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::_SharedMemoryChannelMessage& PNSLR_Bindings_Convert(PNSLR_SharedMemoryChannelMessage& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessage, channel) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessage, channel), "channel offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessage, offset) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessage, offset), "offset offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessage, size) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessage, size), "size offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessage, readPtr) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessage, readPtr), "readPtr offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(PNSLR_SharedMemoryChannelMessage, readSize) == PNSLR_STRUCT_OFFSET(Muzent::_SharedMemoryChannelMessage, readSize), "readSize offset mismatch");

extern "C" b8 PNSLR_CreateSharedMemoryChannelReader(PNSLR_UTF8STR name, i64 size, PNSLR_SharedMemoryChannelReader* reader);
b8 Muzent::_CreateSharedMemoryChannelReader(utf8str name, i64 size, Muzent::_SharedMemoryChannelReader* reader)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CreateSharedMemoryChannelReader(PNSLR_Bindings_Convert(name), PNSLR_Bindings_Convert(size), PNSLR_Bindings_Convert(reader)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_ReadSharedMemoryChannelMessage(PNSLR_SharedMemoryChannelReader* reader, PNSLR_SharedMemoryChannelMessage* message, b8* fatalError);
b8 Muzent::_ReadSharedMemoryChannelMessage(Muzent::_SharedMemoryChannelReader* reader, Muzent::_SharedMemoryChannelMessage* message, b8* fatalError)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_ReadSharedMemoryChannelMessage(PNSLR_Bindings_Convert(reader), PNSLR_Bindings_Convert(message), PNSLR_Bindings_Convert(fatalError)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_AcknowledgeSharedMemoryChannelMessage(PNSLR_SharedMemoryChannelMessage* message);
b8 Muzent::_AcknowledgeSharedMemoryChannelMessage(Muzent::_SharedMemoryChannelMessage* message)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_AcknowledgeSharedMemoryChannelMessage(PNSLR_Bindings_Convert(message)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_DestroySharedMemoryChannelReader(PNSLR_SharedMemoryChannelReader* reader);
b8 Muzent::_DestroySharedMemoryChannelReader(Muzent::_SharedMemoryChannelReader* reader)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_DestroySharedMemoryChannelReader(PNSLR_Bindings_Convert(reader)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_TryConnectSharedMemoryChannelWriter(PNSLR_UTF8STR name, PNSLR_SharedMemoryChannelWriter* writer);
b8 Muzent::_TryConnectSharedMemoryChannelWriter(utf8str name, Muzent::_SharedMemoryChannelWriter* writer)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_TryConnectSharedMemoryChannelWriter(PNSLR_Bindings_Convert(name), PNSLR_Bindings_Convert(writer)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_PrepareSharedMemoryChannelMessage(PNSLR_SharedMemoryChannelWriter* writer, i64 size, PNSLR_SharedMemoryChannelReservedMessage* reservedMessage);
b8 Muzent::_PrepareSharedMemoryChannelMessage(Muzent::_SharedMemoryChannelWriter* writer, i64 size, Muzent::_SharedMemoryChannelReservedMessage* reservedMessage)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_PrepareSharedMemoryChannelMessage(PNSLR_Bindings_Convert(writer), PNSLR_Bindings_Convert(size), PNSLR_Bindings_Convert(reservedMessage)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_CommitSharedMemoryChannelMessage(PNSLR_SharedMemoryChannelWriter* writer, PNSLR_SharedMemoryChannelReservedMessage reservedMessage);
b8 Muzent::_CommitSharedMemoryChannelMessage(Muzent::_SharedMemoryChannelWriter* writer, Muzent::_SharedMemoryChannelReservedMessage reservedMessage)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_CommitSharedMemoryChannelMessage(PNSLR_Bindings_Convert(writer), PNSLR_Bindings_Convert(reservedMessage)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 PNSLR_DisconnectSharedMemoryChannelWriter(PNSLR_SharedMemoryChannelWriter* writer);
b8 Muzent::_DisconnectSharedMemoryChannelWriter(Muzent::_SharedMemoryChannelWriter* writer)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = PNSLR_DisconnectSharedMemoryChannelWriter(PNSLR_Bindings_Convert(writer)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

enum class MZNT_RendererType : u8 { };
static_assert(sizeof(MZNT_RendererType) == sizeof(Muzent::RendererType), "size mismatch");
static_assert(alignof(MZNT_RendererType) == alignof(Muzent::RendererType), "align mismatch");
MZNT_RendererType* PNSLR_Bindings_Convert(Muzent::RendererType* x) { return reinterpret_cast<MZNT_RendererType*>(x); }
Muzent::RendererType* PNSLR_Bindings_Convert(MZNT_RendererType* x) { return reinterpret_cast<Muzent::RendererType*>(x); }
MZNT_RendererType& PNSLR_Bindings_Convert(Muzent::RendererType& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::RendererType& PNSLR_Bindings_Convert(MZNT_RendererType& x) { return *PNSLR_Bindings_Convert(&x); }

enum class MZNT_TextureFormat : u8 { };
static_assert(sizeof(MZNT_TextureFormat) == sizeof(Muzent::TextureFormat), "size mismatch");
static_assert(alignof(MZNT_TextureFormat) == alignof(Muzent::TextureFormat), "align mismatch");
MZNT_TextureFormat* PNSLR_Bindings_Convert(Muzent::TextureFormat* x) { return reinterpret_cast<MZNT_TextureFormat*>(x); }
Muzent::TextureFormat* PNSLR_Bindings_Convert(MZNT_TextureFormat* x) { return reinterpret_cast<Muzent::TextureFormat*>(x); }
MZNT_TextureFormat& PNSLR_Bindings_Convert(Muzent::TextureFormat& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::TextureFormat& PNSLR_Bindings_Convert(MZNT_TextureFormat& x) { return *PNSLR_Bindings_Convert(&x); }

struct MZNT_AppHandle
{
   u64 handle;
};
static_assert(sizeof(MZNT_AppHandle) == sizeof(Muzent::AppHandle), "size mismatch");
static_assert(alignof(MZNT_AppHandle) == alignof(Muzent::AppHandle), "align mismatch");
MZNT_AppHandle* PNSLR_Bindings_Convert(Muzent::AppHandle* x) { return reinterpret_cast<MZNT_AppHandle*>(x); }
Muzent::AppHandle* PNSLR_Bindings_Convert(MZNT_AppHandle* x) { return reinterpret_cast<Muzent::AppHandle*>(x); }
MZNT_AppHandle& PNSLR_Bindings_Convert(Muzent::AppHandle& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::AppHandle& PNSLR_Bindings_Convert(MZNT_AppHandle& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_AppHandle, handle) == PNSLR_STRUCT_OFFSET(Muzent::AppHandle, handle), "handle offset mismatch");

struct MZNT_WindowHandle
{
   u64 handle;
};
static_assert(sizeof(MZNT_WindowHandle) == sizeof(Muzent::WindowHandle), "size mismatch");
static_assert(alignof(MZNT_WindowHandle) == alignof(Muzent::WindowHandle), "align mismatch");
MZNT_WindowHandle* PNSLR_Bindings_Convert(Muzent::WindowHandle* x) { return reinterpret_cast<MZNT_WindowHandle*>(x); }
Muzent::WindowHandle* PNSLR_Bindings_Convert(MZNT_WindowHandle* x) { return reinterpret_cast<Muzent::WindowHandle*>(x); }
MZNT_WindowHandle& PNSLR_Bindings_Convert(Muzent::WindowHandle& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::WindowHandle& PNSLR_Bindings_Convert(MZNT_WindowHandle& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_WindowHandle, handle) == PNSLR_STRUCT_OFFSET(Muzent::WindowHandle, handle), "handle offset mismatch");

struct MZNT_RendererConfiguration
{
   MZNT_RendererType type;
   MZNT_AppHandle appHandle;
   PNSLR_Allocator allocator;
   PNSLR_UTF8STR appName;
};
static_assert(sizeof(MZNT_RendererConfiguration) == sizeof(Muzent::RendererConfiguration), "size mismatch");
static_assert(alignof(MZNT_RendererConfiguration) == alignof(Muzent::RendererConfiguration), "align mismatch");
MZNT_RendererConfiguration* PNSLR_Bindings_Convert(Muzent::RendererConfiguration* x) { return reinterpret_cast<MZNT_RendererConfiguration*>(x); }
Muzent::RendererConfiguration* PNSLR_Bindings_Convert(MZNT_RendererConfiguration* x) { return reinterpret_cast<Muzent::RendererConfiguration*>(x); }
MZNT_RendererConfiguration& PNSLR_Bindings_Convert(Muzent::RendererConfiguration& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::RendererConfiguration& PNSLR_Bindings_Convert(MZNT_RendererConfiguration& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_RendererConfiguration, type) == PNSLR_STRUCT_OFFSET(Muzent::RendererConfiguration, type), "type offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_RendererConfiguration, appHandle) == PNSLR_STRUCT_OFFSET(Muzent::RendererConfiguration, appHandle), "appHandle offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_RendererConfiguration, allocator) == PNSLR_STRUCT_OFFSET(Muzent::RendererConfiguration, allocator), "allocator offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_RendererConfiguration, appName) == PNSLR_STRUCT_OFFSET(Muzent::RendererConfiguration, appName), "appName offset mismatch");

struct MZNT_Renderer
{
   MZNT_RendererType type;
   PNSLR_Allocator allocator;
   MZNT_AppHandle appHandle;
};
static_assert(sizeof(MZNT_Renderer) == sizeof(Muzent::Renderer), "size mismatch");
static_assert(alignof(MZNT_Renderer) == alignof(Muzent::Renderer), "align mismatch");
MZNT_Renderer* PNSLR_Bindings_Convert(Muzent::Renderer* x) { return reinterpret_cast<MZNT_Renderer*>(x); }
Muzent::Renderer* PNSLR_Bindings_Convert(MZNT_Renderer* x) { return reinterpret_cast<Muzent::Renderer*>(x); }
MZNT_Renderer& PNSLR_Bindings_Convert(Muzent::Renderer& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::Renderer& PNSLR_Bindings_Convert(MZNT_Renderer& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_Renderer, type) == PNSLR_STRUCT_OFFSET(Muzent::Renderer, type), "type offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_Renderer, allocator) == PNSLR_STRUCT_OFFSET(Muzent::Renderer, allocator), "allocator offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_Renderer, appHandle) == PNSLR_STRUCT_OFFSET(Muzent::Renderer, appHandle), "appHandle offset mismatch");

extern "C" MZNT_Renderer* MZNT_CreateRenderer(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator);
Muzent::Renderer* Muzent::CreateRenderer(Muzent::RendererConfiguration config, Muzent::_Allocator tempAllocator)
{
    MZNT_Renderer* zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_CreateRenderer(PNSLR_Bindings_Convert(config), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_DestroyRenderer(MZNT_Renderer* renderer, PNSLR_Allocator tempAllocator);
b8 Muzent::DestroyRenderer(Muzent::Renderer* renderer, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_DestroyRenderer(PNSLR_Bindings_Convert(renderer), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_WaitTillRendererIdle(MZNT_Renderer* renderer);
b8 Muzent::WaitTillRendererIdle(Muzent::Renderer* renderer)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_WaitTillRendererIdle(PNSLR_Bindings_Convert(renderer)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct MZNT_RendererCommandBuffer
{
   MZNT_RendererType type;
};
static_assert(sizeof(MZNT_RendererCommandBuffer) == sizeof(Muzent::RendererCommandBuffer), "size mismatch");
static_assert(alignof(MZNT_RendererCommandBuffer) == alignof(Muzent::RendererCommandBuffer), "align mismatch");
MZNT_RendererCommandBuffer* PNSLR_Bindings_Convert(Muzent::RendererCommandBuffer* x) { return reinterpret_cast<MZNT_RendererCommandBuffer*>(x); }
Muzent::RendererCommandBuffer* PNSLR_Bindings_Convert(MZNT_RendererCommandBuffer* x) { return reinterpret_cast<Muzent::RendererCommandBuffer*>(x); }
MZNT_RendererCommandBuffer& PNSLR_Bindings_Convert(Muzent::RendererCommandBuffer& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::RendererCommandBuffer& PNSLR_Bindings_Convert(MZNT_RendererCommandBuffer& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_RendererCommandBuffer, type) == PNSLR_STRUCT_OFFSET(Muzent::RendererCommandBuffer, type), "type offset mismatch");

struct MZNT_RendererSurface
{
   MZNT_RendererType type;
};
static_assert(sizeof(MZNT_RendererSurface) == sizeof(Muzent::RendererSurface), "size mismatch");
static_assert(alignof(MZNT_RendererSurface) == alignof(Muzent::RendererSurface), "align mismatch");
MZNT_RendererSurface* PNSLR_Bindings_Convert(Muzent::RendererSurface* x) { return reinterpret_cast<MZNT_RendererSurface*>(x); }
Muzent::RendererSurface* PNSLR_Bindings_Convert(MZNT_RendererSurface* x) { return reinterpret_cast<Muzent::RendererSurface*>(x); }
MZNT_RendererSurface& PNSLR_Bindings_Convert(Muzent::RendererSurface& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::RendererSurface& PNSLR_Bindings_Convert(MZNT_RendererSurface& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_RendererSurface, type) == PNSLR_STRUCT_OFFSET(Muzent::RendererSurface, type), "type offset mismatch");

extern "C" MZNT_RendererSurface* MZNT_CreateRendererSurfaceFromWindow(MZNT_Renderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator);
Muzent::RendererSurface* Muzent::CreateRendererSurfaceFromWindow(Muzent::Renderer* renderer, Muzent::WindowHandle windowHandle, Muzent::_Allocator tempAllocator)
{
    MZNT_RendererSurface* zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_CreateRendererSurfaceFromWindow(PNSLR_Bindings_Convert(renderer), PNSLR_Bindings_Convert(windowHandle), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_DestroyRendererSurface(MZNT_RendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 Muzent::DestroyRendererSurface(Muzent::RendererSurface* surface, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_DestroyRendererSurface(PNSLR_Bindings_Convert(surface), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_ResizeRendererSurface(MZNT_RendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator);
b8 Muzent::ResizeRendererSurface(Muzent::RendererSurface* surface, u16 width, u16 height, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_ResizeRendererSurface(PNSLR_Bindings_Convert(surface), PNSLR_Bindings_Convert(width), PNSLR_Bindings_Convert(height), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" MZNT_RendererCommandBuffer* MZNT_BeginFrame(MZNT_RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator);
Muzent::RendererCommandBuffer* Muzent::BeginFrame(Muzent::RendererSurface* surface, f32 r, f32 g, f32 b, f32 a, Muzent::_Allocator tempAllocator)
{
    MZNT_RendererCommandBuffer* zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_BeginFrame(PNSLR_Bindings_Convert(surface), PNSLR_Bindings_Convert(r), PNSLR_Bindings_Convert(g), PNSLR_Bindings_Convert(b), PNSLR_Bindings_Convert(a), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_EndFrame(MZNT_RendererSurface* surface, PNSLR_Allocator tempAllocator);
b8 Muzent::EndFrame(Muzent::RendererSurface* surface, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_EndFrame(PNSLR_Bindings_Convert(surface), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

struct MZNT_SwapChainConfiguration
{
   u16 width;
   u16 height;
   b8 vSync;
   u8 framesInFlight;
};
static_assert(sizeof(MZNT_SwapChainConfiguration) == sizeof(Muzent::SwapChainConfiguration), "size mismatch");
static_assert(alignof(MZNT_SwapChainConfiguration) == alignof(Muzent::SwapChainConfiguration), "align mismatch");
MZNT_SwapChainConfiguration* PNSLR_Bindings_Convert(Muzent::SwapChainConfiguration* x) { return reinterpret_cast<MZNT_SwapChainConfiguration*>(x); }
Muzent::SwapChainConfiguration* PNSLR_Bindings_Convert(MZNT_SwapChainConfiguration* x) { return reinterpret_cast<Muzent::SwapChainConfiguration*>(x); }
MZNT_SwapChainConfiguration& PNSLR_Bindings_Convert(Muzent::SwapChainConfiguration& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::SwapChainConfiguration& PNSLR_Bindings_Convert(MZNT_SwapChainConfiguration& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_SwapChainConfiguration, width) == PNSLR_STRUCT_OFFSET(Muzent::SwapChainConfiguration, width), "width offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_SwapChainConfiguration, height) == PNSLR_STRUCT_OFFSET(Muzent::SwapChainConfiguration, height), "height offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_SwapChainConfiguration, vSync) == PNSLR_STRUCT_OFFSET(Muzent::SwapChainConfiguration, vSync), "vSync offset mismatch");
static_assert(PNSLR_STRUCT_OFFSET(MZNT_SwapChainConfiguration, framesInFlight) == PNSLR_STRUCT_OFFSET(Muzent::SwapChainConfiguration, framesInFlight), "framesInFlight offset mismatch");

struct MZNT_SwapChain
{
   MZNT_RendererType type;
};
static_assert(sizeof(MZNT_SwapChain) == sizeof(Muzent::SwapChain), "size mismatch");
static_assert(alignof(MZNT_SwapChain) == alignof(Muzent::SwapChain), "align mismatch");
MZNT_SwapChain* PNSLR_Bindings_Convert(Muzent::SwapChain* x) { return reinterpret_cast<MZNT_SwapChain*>(x); }
Muzent::SwapChain* PNSLR_Bindings_Convert(MZNT_SwapChain* x) { return reinterpret_cast<Muzent::SwapChain*>(x); }
MZNT_SwapChain& PNSLR_Bindings_Convert(Muzent::SwapChain& x) { return *PNSLR_Bindings_Convert(&x); }
Muzent::SwapChain& PNSLR_Bindings_Convert(MZNT_SwapChain& x) { return *PNSLR_Bindings_Convert(&x); }
static_assert(PNSLR_STRUCT_OFFSET(MZNT_SwapChain, type) == PNSLR_STRUCT_OFFSET(Muzent::SwapChain, type), "type offset mismatch");

extern "C" MZNT_SwapChain* MZNT_CreateSwapChainFromWindow(MZNT_Renderer* renderer, MZNT_WindowHandle windowHandle, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator);
Muzent::SwapChain* Muzent::CreateSwapChainFromWindow(Muzent::Renderer* renderer, Muzent::WindowHandle windowHandle, Muzent::SwapChainConfiguration cfg, Muzent::_Allocator tempAllocator)
{
    MZNT_SwapChain* zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_CreateSwapChainFromWindow(PNSLR_Bindings_Convert(renderer), PNSLR_Bindings_Convert(windowHandle), PNSLR_Bindings_Convert(cfg), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_ReconfigureSwapChain(MZNT_SwapChain* swapChain, MZNT_SwapChainConfiguration cfg, PNSLR_Allocator tempAllocator);
b8 Muzent::ReconfigureSwapChain(Muzent::SwapChain* swapChain, Muzent::SwapChainConfiguration cfg, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_ReconfigureSwapChain(PNSLR_Bindings_Convert(swapChain), PNSLR_Bindings_Convert(cfg), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_DestroySwapChain(MZNT_SwapChain* swapChain, PNSLR_Allocator tempAllocator);
b8 Muzent::DestroySwapChain(Muzent::SwapChain* swapChain, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_DestroySwapChain(PNSLR_Bindings_Convert(swapChain), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" MZNT_TextureFormat MZNT_GetSwapChainTextureFormat(MZNT_SwapChain* swapChain);
Muzent::TextureFormat Muzent::GetSwapChainTextureFormat(Muzent::SwapChain* swapChain)
{
    MZNT_TextureFormat zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_GetSwapChainTextureFormat(PNSLR_Bindings_Convert(swapChain)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" MZNT_RendererCommandBuffer* MZNT_IterateSwapChain(MZNT_SwapChain* swapChain, u8* outImgIdx, PNSLR_Allocator tempAllocator);
Muzent::RendererCommandBuffer* Muzent::IterateSwapChain(Muzent::SwapChain* swapChain, u8* outImgIdx, Muzent::_Allocator tempAllocator)
{
    MZNT_RendererCommandBuffer* zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_IterateSwapChain(PNSLR_Bindings_Convert(swapChain), PNSLR_Bindings_Convert(outImgIdx), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

extern "C" b8 MZNT_PresentSwapChain(MZNT_SwapChain* swapChain, PNSLR_Allocator tempAllocator);
b8 Muzent::PresentSwapChain(Muzent::SwapChain* swapChain, Muzent::_Allocator tempAllocator)
{
    b8 zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW = MZNT_PresentSwapChain(PNSLR_Bindings_Convert(swapChain), PNSLR_Bindings_Convert(tempAllocator)); return PNSLR_Bindings_Convert(zzzz_RetValXYZABCDEFGHIJKLMNOPQRSTUVW);
}

#undef PNSLR_STRUCT_OFFSET

#endif//PNSLR_CXX_IMPL

