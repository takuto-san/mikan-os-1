/**
 * @file newlib_support.c
 * 
 * OSがC標準ライブラリの機能（例えばmallocによるメモリ確保など）を
 * 利用できるようにする
 */
// day05c, day05e
#include <errno.h>
#include <sys/types.h>

void _exit(void) {
  while (1) __asm__("hlt");
}

caddr_t sbrk(int incr) {
  errno = ENOMEM;
  return (caddr_t)-1;
}

// day06c
int getpid(void) {
  return 1;
}

int kill(int pid, int sig) {
  errno = EINVAL;
  return -1;
}