#include <iostream>

#include "engine.h"

int main(int argc, char **argv)
{
  Engine *engine = new Engine("PA10: Pinball", 1080, 920);
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }

  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
