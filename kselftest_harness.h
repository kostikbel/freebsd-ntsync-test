/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2026 The FreeBSD Foundation
 *
 * This software was developed by Konstantin Belousov <kib@FreeBSD.org>
 * under sponsorship from the FreeBSD Foundation.
 */

#ifndef __KSELFTEST_HARNESS_H__
#define	__KSELFTEST_HARNESS_H__

#include <sys/types.h>
#include <sys/queue.h>
#include <errno.h>
#include <pthread.h>
#ifdef __FreeBSD__
#include <pthread_np.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef __linux__
typedef	uint32_t __u32;
typedef	uint64_t __u64;
#endif

#ifdef __FreeBSD__
static inline int
gettid(void)
{
	return (pthread_getthreadid_np());
}
#endif

#define	EXPECT_OP(a, b, op)	do {					\
	if (!((a) op (b))) {						\
		fprintf(stderr, "%s:%d %s %#x != %s %#x\n",		\
		    __FILE__, __LINE__, #a, (a), #b, (b));		\
		/* abort(); */						\
	}								\
} while (0)

#define	EXPECT_EQ(a, b)		EXPECT_OP(a, b, ==)
#define	EXPECT_LE(a, b)		EXPECT_OP(a, b, <=)
#define	ASSERT_LE(a, b)		EXPECT_OP(a, b, <=)	// XXX?

struct test_ctr_tag {
	TAILQ_ENTRY(test_ctr_tag) next;
	void (*test_fun)(void);
	const char *test_name;
};

TAILQ_HEAD(tests_tag, test_ctr_tag);
struct tests_tag tests = TAILQ_HEAD_INITIALIZER(tests);

#define	TEST(name)							\
static void name ## _ctr(void) __attribute((constructor));		\
static void name(void);							\
static void								\
name ## _ctr(void)							\
{									\
	static struct test_ctr_tag tt = {				\
		.test_fun = name,					\
		.test_name = #name,					\
	};								\
									\
	TAILQ_INSERT_TAIL(&tests, &tt, next);				\
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
		printf("%s\n", t->test_name);				\
		t->test_fun();						\
	}								\
}

#endif
