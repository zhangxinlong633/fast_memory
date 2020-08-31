#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static inline uint64_t rdtsc(void)
{
	union {
		uint64_t tsc_64;
		struct {
			uint32_t lo_32;
			uint32_t hi_32;
		};
	} tsc;

	asm volatile("rdtsc" :
		     "=a" (tsc.lo_32),
		     "=d" (tsc.hi_32));
	return tsc.tsc_64;
}

int fmemset(void *src, char c, int len)
{
    int unit_size = sizeof(__uint128_t);
    char mem[unit_size];
    __uint128_t *s = src;
    __uint128_t *m = (__uint128_t *)mem;

    int mod = len % unit_size;
    int div = len / unit_size;
    for (int i = 0; i < unit_size; i ++) {
        mem[i] = c;
    }

    for (int i = 0; i < div; i ++) {
        s[i] = *m;
    }

    char *b = src + div * unit_size;
    for (int i = 0; i < mod; i ++) {
        *(char *)((char *)src + div * unit_size + i) = c;
    }

    return 0;
}

int fmemcpy(void *dst, void *src, int len)
{
    int unit_size = sizeof(__uint128_t);
    __uint128_t *d = dst;
    __uint128_t *s = src;
    int mod = len % unit_size;
    int div = len / unit_size;

    for (int i = 0; i < div; i ++) {
        d[i] = s[i];
    }

    char *b = dst + div * unit_size;
    for (int i = 0; i < mod; i ++) {
        *(b + i) = *(char *)((char *)src + div * unit_size + i);
    }

    return 0;
}

int main(int argc, char **argv)
{
    char src[4096] = {0};
    char dst[4096] = {0};
    uint64_t c1 = rdtsc();
    fmemset(src, 'a', 4095);
    uint64_t c2 = rdtsc();
    fmemcpy(dst, src, 4096);
    uint64_t c3 = rdtsc();

    memset(src, 'b', 4095);
    uint64_t c4 = rdtsc();
    memcpy(dst, src, 4096);
    uint64_t c5 = rdtsc();
    printf("fmemset spend: %llu, fmemcpy spend: %llu\n", c2 - c1, c3 - c2);
    printf("memset spend: %llu, memcpy spend: %llu\n", c4 - c3, c5 - c4);

    return 0;
}
