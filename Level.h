#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <cassert>
#include <array>

class Level {
 public:
  static constexpr int width = 32;
  static constexpr int height = 30;
  static constexpr int n_tiles = width * height;
  struct Cannon {
    int pos{};
    bool active{};
  };
  enum class TileType {
    empty = 0,
    wall = 1,
    spike = 2,
    cannon = 3,
    spawn_point = 4,
    target_point = 5
  };
 private:
  glm::vec2 spawn_pos_{};
  glm::vec2 target_pos_{};
  std::vector<Cannon> cannons_;
  std::array<uint8_t, width * height> tiles_{};
 public:
  Level() : tiles_{} {}
  explicit Level(const std::array<uint8_t, n_tiles> &data) : tiles_{data} {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        if (tiles_[x + y * width] == std::uint8_t(TileType::spawn_point)) {
          spawn_pos_.x = float(x) * 8.0f;
          spawn_pos_.y = float(y) * 8.0f;
        } else if (tiles_[x + y * width] == std::uint8_t(TileType::target_point)) {
          target_pos_.x = float(x) * 8.0f;
          target_pos_.y = float(y) * 8.0f;
        } else if (tiles_[x + y * width] == std::uint8_t(TileType::cannon)) {
          cannons_.emplace_back(Cannon{x + y * width, false});
        }
      }
    }
  }
  void init();
  [[maybe_unused]] auto &tiles() { return tiles_; }
  [[maybe_unused]] [[nodiscard]] const auto &tiles() const { return tiles_; }
  [[maybe_unused]] auto &spawn_pos() { return spawn_pos_; }
  [[maybe_unused]] [[nodiscard]] const auto &spawn_pos() const { return spawn_pos_; }
  [[maybe_unused]] auto &target_pos() { return target_pos_; }
  [[maybe_unused]] [[nodiscard]] const auto &target_pos() const { return target_pos_; }
  [[maybe_unused]] auto &cannons() { return cannons_; }
  [[maybe_unused]] [[nodiscard]] const auto &cannons() const { return cannons_; }
};
