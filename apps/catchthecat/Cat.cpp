#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::IfMove(int direction, CatWorld* world)
{
  auto pos = world->getCat();

  switch (direction) {
    case 0:
      return CatWorld::NE(pos);
    case 1:
      return CatWorld::NW(pos);
    case 2:
      return CatWorld::E(pos);
    case 3:
      return CatWorld::W(pos);
    case 4:
      return CatWorld::SW(pos);
    case 5:
      return CatWorld::SE(pos);
    default:
      throw std::runtime_error("random out of range");
  }
}

Point2D Cat::Move(CatWorld* world) 
{
  auto pos = world->getCat();
  Point2D prosPos;
  auto rand = 0;

  //wont ever lost to a bad move
  do
  {
    rand = Random::Range(0, 5);
    prosPos = IfMove(rand, world);

  } while (pos == prosPos || world->getContent(prosPos));

  return prosPos;
}
