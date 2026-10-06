#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) 
{
  auto side = world->getWorldSideSize() / 2;
  auto cat = world->getCat();
  Point2D p;

  //wont ever make a false move
  do 
  {


    p = {Random::Range(-side, side), Random::Range(-side, side)};



  } while (cat == p || world->getContent(p));


  return p;
}
