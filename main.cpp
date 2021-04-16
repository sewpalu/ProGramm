#include <iostream>

#include "ArgumentsDispatcher.hpp"

int main(int argc, char** argv)
{
  auto cli = ArgumentsDispatcher{argc, argv};
  cli.dispatch();
}
