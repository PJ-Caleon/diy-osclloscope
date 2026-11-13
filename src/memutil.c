/* memutil.c - word copy/zero used by Reset_Handler (no libc is linked, so we write our own). */
#include "memutil.h"

/* @TODO(M1-02/1) copy_words: walk dst up to dst_end, copying one word from src each step (walking pointers, like sum_words). [src: M1-02 step 1] */
void copy_words(uint32_t *dst, uint32_t *dst_end, const uint32_t *src)
{
  // copy 1 word from src to dst and move up dst and src
  while(dst < dst_end){
    *dst = *src;
    dst++;
    src++;
  }
}

/* @TODO(M1-02/2) zero_words: walk from start up to end, writing 0 to each word. [src: M1-02 step 2] */
void zero_words(uint32_t *start, uint32_t *end)
{
  while(start < end){
    *start = 0u;
    start++;
  }
}
