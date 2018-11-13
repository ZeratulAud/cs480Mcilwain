#include <iostream>

#include "engine.h"

int main(int argc, char **argv)
{
  std::string playAgain;
  Engine *engine;

  do
  {
    engine = new Engine("PA10: Pinball", 1080, 920);
    if(!engine->Initialize())
    {
      printf("The engine failed to start.\n");
      delete engine;
      engine = NULL;
      return 1;
    }

    engine->Run();

    std::cout << "Would you like to play again?" << std::endl;
    std::cin >> playAgain;

    delete engine;
    engine = NULL;
  }
  while (playAgain == "yes");

  return 0;
}
