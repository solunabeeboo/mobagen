#include "AlignmentRule.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <cmath>

const float zoneRadius = 6.0f;

glm::vec2 AlignmentRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) 
{
  glm::vec2 averageVelocity(0.f);
  // glm::vec2 can be divided by a float, which will divide each component of the vector by that float.

  // begin solution

  //Define a field 
  //Grab velocity of boids in that field
  //[apply rotation force] change velocity using a weighted averagone of me(x) + o1(y) + ...
  //alignment over time. changing velocity will rotate it

  averageVelocity += boid.velocity;

  for (const BoidView& oBoid : neighborhood)
  {
		averageVelocity += oBoid.velocity;	  
  }


  return averageVelocity * (1 / (neighborhood.size() + 1));
  // end solution
}


float getDistance(glm::vec2 a, glm::vec2 b) 
{ 
	return std::sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - b.x) * (b.y - b.x))); 
}