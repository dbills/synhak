#include <unistd.h>
extern void setup();
extern void loop();

int main() {
  setup();
  while(1) {
    sleep(1);
    loop();
  }
}
