#include <Arduino.h>

#include "lib/common.h"
#include "lib/sys.h"
#include "lib/oled.h"

int main(void) {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    SysInit();

    for (;;) {
      if(SysLoop() == FALSE){
        continue;
      }

      OledDrawStr(0, 0, "Hey Stephanie!");
      SysLoopEnd();
      if (serialEventRun) serialEventRun();
    }

    return 0;
}

