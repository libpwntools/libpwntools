#include <iostream>
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <vector>
#include "../src/pwntools.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

General general;
int main() {

 Struct st;
 std::vector<int32_t> L_ROP, L_ROP2;
 L_ROP = {
  0x41414141,
  0x42424242,
  0x43434343,
  0x44444444
 };
 L_ROP2 = {
  0x34343434,
  0x33333333,
  0x32323232,
  0x31313131
 };
 Char *ropchain = st.flat(L_ROP);
 Char *ropchain2 = st.flat(L_ROP2);
 return 0;
}
