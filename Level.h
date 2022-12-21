#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <array>

class Level {
 public:
  static constexpr int width = 32;
  static constexpr int height = 30;
  static constexpr int n_tiles = width * height;
  enum class TileType {
    empty = 0,
    wall = 1,
    spike = 2,
    spawn_point = 3,
    target_point = 4,
    enum_count = 5
  };
 private:
  std::array<uint8_t, width * height> tiles_{};
 public:
  Level() : tiles_{} {}
  explicit Level(const std::array<uint8_t, n_tiles> &data) : tiles_{data} {}
  [[maybe_unused]] auto &tiles() {return tiles_;}
  [[maybe_unused]] [[nodiscard]] const auto &tiles() const {return tiles_;}
};
