#include <iostream>

#include "engine.h"

int main(int argc, char *argv[])
{
  int level = 0;
  if (argc>1){
    level = atoi(argv[1]);
  }
  Engine *engine = new Engine("DK Clone", 1080, 920, level);
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
