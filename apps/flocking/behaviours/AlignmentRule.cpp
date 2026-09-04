#include "AlignmentRule.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <cmath>

const float zoneRadius = 6.0f;

glm::vec2 AlignmentRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) 
{
  // glm::vec2 can be divided by a float, which will divide each component of the vector by that float.

  // begin solution

  //Define a field 
  //Grab velocity of boids in that field
  //[apply rotation force] change velocity using a weighted averagone of me(x) + o1(y) + ...
  //alignment over time. changing velocity will rotate it

  if (neighborhood.empty())
    return glm::vec2(0.0f);



  glm::vec2 averageVelocity(0.0f);

  for (const BoidView& oBoid : neighborhood)
    averageVelocity += oBoid.velocity;

  averageVelocity /= (float)neighborhood.size();



  return averageVelocity - boid.velocity;
}