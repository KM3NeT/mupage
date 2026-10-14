#include "Random.hh"

Random::Random(std::mt19937::result_type seed) : mt_rand(seed), dis(0.0, 1.0) {}

double Random::Rndm() { return dis(mt_rand); }

double Random::Exp(double tau) {
  return -tau * log(Rndm());  // from TRandom::Exp()
}
