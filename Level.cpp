#include "Level.h"
void Level::init() {
  for (auto &cannon : cannons_) {
    cannon.active = false;
  }
}