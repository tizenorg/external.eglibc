#include <stdint.h>

#ifdef __i386__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("movl %%gs:0x14, %0" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({							\
     uintptr_t x;					\
     asm ("movl %%gs:%c1, %0" : "=r" (x)		\
	  : "i" (offsetof (tcbhead_t, pointer_guard)));	\
     x;							\
   })
#elif defined __x86_64__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("movq %%fs:0x28, %0" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({ uintptr_t x;						\
     asm ("mov %%fs:%c1, %0" : "=r" (x)				\
	  : "i" (offsetof (tcbhead_t, pointer_guard))); x; })
#elif defined __powerpc64__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ld %0,-28688(13)" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({												\
     uintptr_t x;										\
     asm ("ld %0,%1(13)"									\
	  : "=r" (x)										\
	  : "i" (offsetof (tcbhead_t, pointer_guard) - TLS_TCB_OFFSET - sizeof (tcbhead_t))	\
         );											\
     x;												\
   })
#elif defined __powerpc__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("lwz %0,-28680(2)" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({												\
     uintptr_t x;										\
     asm ("lwz %0,%1(2)"									\
	  : "=r" (x)										\
	  : "i" (offsetof (tcbhead_t, pointer_guard) - TLS_TCB_OFFSET - sizeof (tcbhead_t))	\
         );											\
     x;												\
   })
#elif defined __sparc__ && defined __arch64__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ldx [%%g7+0x28], %0" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({ uintptr_t x; asm ("ldx [%%g7+0x30], %0" : "=r" (x)); x; })
#elif defined __sparc__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ld [%%g7+0x14], %0" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({ uintptr_t x; asm ("ld [%%g7+0x18], %0" : "=r" (x)); x; })
#elif defined __s390x__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ear %0,%%a0; sllg %0,%0,32; ear %0,%%a1; lg %0,0x28(%0)" : "=a" (x)); x; })
/* On s390/s390x there is no unique pointer guard, instead we use the
   same value as the stack guard.  */
#define POINTER_CHK_GUARD \
  ({							\
     uintptr_t x;					\
     asm ("ear %0,%%a0;"				\
	  "sllg %0,%0,32;"				\
	  "ear %0,%%a1;"				\
	  "lg %0,%1(%0)"				\
	 : "=a" (x)					\
	 : "i" (offsetof (tcbhead_t, stack_guard)));	\
     x;							\
   })
#elif defined __s390__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("ear %0,%%a0; l %0,0x14(%0)" : "=a" (x)); x; })
/* On s390/s390x there is no unique pointer guard, instead we use the
   same value as the stack guard.  */
#define POINTER_CHK_GUARD \
  ({							\
     uintptr_t x;					\
     asm ("ear %0,%%a0; l %0,%1(%0)"			\
	  : "=a" (x)					\
	  : "i" (offsetof (tcbhead_t, stack_guard)));	\
     x;							\
   })
#elif defined __ia64__
# define STACK_CHK_GUARD \
  ({ uintptr_t x; asm ("adds %0 = -8, r13;; ld8 %0 = [%0]" : "=r" (x)); x; })
#define POINTER_CHK_GUARD \
  ({ uintptr_t x; asm ("adds %0 = -16, r13;; ld8 %0 = [%0]" : "=r" (x)); x; })
#else
extern uintptr_t __stack_chk_guard;
# define STACK_CHK_GUARD __stack_chk_guard
#ifdef PTRGUARD_LOCAL
extern uintptr_t __pointer_chk_guard_local;
# define POINTER_CHK_GUARD __pointer_chk_guard_local
#else
extern uintptr_t __pointer_chk_guard;
# define POINTER_CHK_GUARD __pointer_chk_guard
#endif
#endif
