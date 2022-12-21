#include "asset_pipeline.h"

namespace {
constexpr std::array<glm::u8vec4, 5> pixel_pattern = {
    glm::u8vec4(0x00, 0x00, 0x00, 0xff), // empty
    glm::u8vec4(0xa5, 0x2a, 0x2a, 0xff), // wall
    glm::u8vec4(0x80, 0x80, 0x80, 0xff), // spike
    glm::u8vec4(0xff, 0x00, 0x00, 0xff), // spawn_point
    glm::u8vec4(0x00, 0x00, 0xff, 0xff) // target_point
};
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
    for (uint8_t i = 0; i <= uint8_t(Level::TileType::enum_count); ++i) {
      if (pixel == pixel_pattern[i]) {
        level_chunk.push_back(i);
        break;
      } else if (i == uint8_t(Level::TileType::enum_count)) {
        std::cerr << "Invalid pixel pattern.\n";
        return false;
      }
    }
  }
  write_chunk<uint8_t>("leve", level_chunk, &ofs);
  return true;
}