#include <iostream>

#include "engine.h"

int main(int argc, char *argv[])
{
  //std::cout << atoi(argv[1]) << std::endl;
  Engine *engine = new Engine("DK Clone", 1080, 920, atoi(argv[1]));
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
