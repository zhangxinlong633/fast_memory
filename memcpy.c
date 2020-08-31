#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

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
    fmemset(src, 'a', 4095);

    fmemcpy(dst, src, 4096);
    printf("dst: %s\n", dst);
}
