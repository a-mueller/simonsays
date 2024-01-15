#include <neotrellis.hpp>

#include "pico/stdlib.h"
#include <stdio.h>


int main() {
  stdio_init_all();

  NeoTrellis trellis(i2c0, 4, 5);
  trellis.init();


  while(true) {
    printf("Running program");
    sleep_ms(2000);
  }
}
