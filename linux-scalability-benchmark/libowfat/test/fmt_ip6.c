#include "ip6.h"
#include <assert.h>

int main() {
  char buf[100];
  int i;
  buf[i=fmt_ip6(buf,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00")]=0; assert(i==2 && !strcmp(buf,"::"));
  buf[i=fmt_ip6(buf,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01")]=0; assert(i==3 && !strcmp(buf,"::1"));
  buf[i=fmt_ip6(buf,"\xfe\xc0\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x01")]=0;
  assert(i==16 && !strcmp(buf,"fec0:0:0:ffff::1"));
  buf[i=fmt_ip6(buf,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x7f\x00\x00\x01")]=0;
  assert(i==16 && !strcmp(buf,"::ffff:127.0.0.1"));
  return 0;
}