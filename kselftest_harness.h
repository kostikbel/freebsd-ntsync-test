#ifndef __KSELFTEST_HARNESS_H__
#define	__KSELFTEST_HARNESS_H__

#include <sys/types.h>
#include <sys/queue.h>
#include <errno.h>
#include <pthread.h>
#include <pthread_np.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef	uint32_t __u32;
typedef	uint64_t __u64;

static inline int
gettid(void)
{
	return (pthread_getthreadid_np());
}

#define	EXPECT_OP(a, b, op)	do {					\
	if (!((a) op (b))) {						\
		fprintf(stderr, "%s:%d a %#x != b %#x",			\
		    __FILE__, __LINE__,					\
		    (a), (b));						\
		abort();						\
	}								\
} while (0)

#define	EXPECT_EQ(a, b)		EXPECT_OP(a, b, ==)
#define	EXPECT_LE(a, b)		EXPECT_OP(a, b, <=)
#define	ASSERT_LE(a, b)		EXPECT_OP(a, b, <=)	// XXX?

struct test_ctr_tag {
	TAILQ_ENTRY(test_ctr_tag) next;
	void (*test_fun)(void);
};

TAILQ_HEAD(tests_tag, test_ctr_tag);
struct tests_tag tests;

#define	TEST(name)							\
static void name ## _ctr(void) __attribute((constructor));		\
static void name(void);							\
static void								\
name ## _ctr(void)							\
{									\
	static struct test_ctr_tag tt = {				\
		.test_fun = name,					\
	};								\
									\
	TAILQ_INSERT_HEAD(&tests, &tt, next);				\
}									\
static									\
void name(void)

#define	TEST_HARNESS_MAIN						\
struct tests_tag tests;							\
int									\
main(void)								\
{									\
	struct test_ctr_tag *t;						\
									\
	TAILQ_FOREACH(t, &tests, next) {				\
		t->test_fun();						\
	}								\
}

#endif
