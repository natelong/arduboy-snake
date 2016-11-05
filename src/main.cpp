#include <Arduino.h>

#include "lib/common.h"
#include "lib/sys.h"

#include "game.h"

int main(void) {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    SysInit();


    Game game;

    for (;;) {
      if(SysLoop() == FALSE){
        continue;
      }

      game.update();
      game.draw();

      SysLoopEnd();
      if (serialEventRun) serialEventRun();
    }

    return 0;
}

