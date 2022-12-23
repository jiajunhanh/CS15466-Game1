#include "asset_pipeline.h"

namespace {
constexpr auto empty_pattern = glm::u8vec4(0x87, 0xce, 0xeb, 0xff);
constexpr auto wall_pattern = glm::u8vec4(0xa5, 0x2a, 0x2a, 0xff);
constexpr auto spike_pattern = glm::u8vec4(0x80, 0x80, 0x80, 0xff);
constexpr auto artillery_pattern = glm::u8vec4(0x4b, 0x53, 0x20, 0xff);
constexpr auto spawn_point_pattern = glm::u8vec4(0x00, 0xff, 0x00, 0xff);
constexpr auto target_point_pattern = glm::u8vec4(0x00, 0x00, 0xff, 0xff);
}

bool process_assets() {
  glm::uvec2 size;
  std::vector<glm::u8vec4> pixels;
  std::ofstream ofs(data_path("game_data"), std::ios::binary | std::ios::trunc);

  // ---------- process levels ----------
  try {
    load_png(data_path("../assets/levels.png"), &size, &pixels, LowerLeftOrigin);
  } catch (std::exception const &e) {
    std::cerr << e.what() << '\n';
  }
  if (pixels.empty() || size[0] != 32 || size[1] % 30) {
    std::cerr << "Illegal levels.\n";
    return false;
  }
  std::vector<uint8_t> level_chunk;
  for (const auto &pixel : pixels) {
    if (pixel == empty_pattern) {
      level_chunk.push_back(std::uint8_t(Level::TileType::empty));
    } else if (pixel == wall_pattern) {
      level_chunk.push_back(std::uint8_t(Level::TileType::wall));
    } else if (pixel == spike_pattern) {
      level_chunk.push_back(std::uint8_t(Level::TileType::spike));
    } else if (pixel == artillery_pattern) {
      level_chunk.push_back(std::uint8_t(Level::TileType::artillery));
    } else if (pixel == spawn_point_pattern) {
      level_chunk.push_back(std::uint8_t(Level::TileType::spawn_point));
    } else if (pixel == target_point_pattern) {
      level_chunk.push_back(std::uint8_t(Level::TileType::target_point));
    } else {
      std::cerr << "Invalid pattern.\n";
      return false;
    }
  }
  write_chunk<uint8_t>("leve", level_chunk, &ofs);
  return true;
}