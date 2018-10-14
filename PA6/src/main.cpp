#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Must supply .obj file path" << std::endl;
    exit(1);
  }

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("PA7: Solar System", 1080, 1000);
  if(!engine->Initialize(MODEL_DIR + std::string(argv[1])))
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
