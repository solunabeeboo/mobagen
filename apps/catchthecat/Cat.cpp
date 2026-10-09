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
    //get best path
    auto path = generatePath(world);
    if (!path.empty())
        return path.back();

    //cant find path contingency, first avail
    for (int i = 0; i < 6; i++)
    {
        auto p = IfMove(i, world);
        if (!world->getContent(p))
            return p;
    }

    //give up; no moves available
    return world->getCat();
}
