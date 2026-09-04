#include "CohesionRule.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.

  // begin solution

  if (neighborhood.empty())
    return cohesionForce;

  glm::vec2 centerOfMass(0.0f);
  for (const BoidView& oBoid : neighborhood)
    centerOfMass += oBoid.position;

  centerOfMass /= (float)neighborhood.size();


  glm::vec2 direction = centerOfMass - boid.position;

  if (glm::length(direction) > 0.f)
    cohesionForce = glm::normalize(direction);

  // end solution
  return cohesionForce;
}
