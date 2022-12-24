#include "asset_pipeline.h"

namespace {
constexpr glm::uint level_width = 32;
constexpr glm::uint level_height = 30;
constexpr glm::uint sprite_width = 8;
constexpr glm::uint sprite_height = 8;
}

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
  if (pixels.empty() || size[0] != level_width || size[1] % level_height) {
    std::cerr << "Invalid levels.\n";
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
      level_chunk.push_back(std::uint8_t(Level::TileType::cannon));
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

  // ---------- process sprites ----------
  try {
    load_png(data_path("../assets/sprites.png"), &size, &pixels, LowerLeftOrigin);
  } catch (std::exception const &e) {
    std::cerr << e.what() << '\n';
  }
  if (pixels.empty() || size[0] != sprite_width || size[1] % sprite_height) {
    std::cerr << "Invalid sprites.\n";
    return false;
  }
  std::vector<uint8_t> palette_chunk;
  std::vector<uint8_t> tile_chunk;

  for (glm::uint sprite_idx = 0; sprite_idx < size[1] / sprite_height; ++sprite_idx) {
    int n_colors = 1;
    std::array<glm::u8vec4, 4> palette{empty_pattern};
    std::array<uint8_t, sprite_height> bit0{};
    std::array<uint8_t, sprite_height> bit1{};
    for (auto y = sprite_idx * sprite_height; y < (sprite_idx + 1) * sprite_height; ++y) {
      for (glm::uint x = 0; x < sprite_width; ++x) {
        const auto &pixel = pixels[x + y * sprite_width];
        bool in_palette = false;
        for (int i = 0; i < n_colors; ++i) {
          if (pixel == palette[i]) {
            in_palette = true;
            bit0[y % sprite_height] |= (i & 1) << x;
            bit1[y % sprite_height] |= ((i >> 1) & 1) << x;
            break;
          }
        }
        if (!in_palette) {
          if (n_colors == 4) {
            std::cerr << "More than 4 colors in a sprite!\n";
            return false;
          } else {
            palette[n_colors] = pixel;
            bit0[y % sprite_height] |= (n_colors & 1) << x;
            bit1[y % sprite_height] |= ((n_colors >> 1) & 1) << x;
            ++n_colors;
          }
        }
      }
    }
    palette[0] = glm::u8vec4{0};
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        palette_chunk.emplace_back(palette[i][j]);
      }
    }
    for (int i = 0; i < sprite_height; ++i) {
      tile_chunk.emplace_back(bit0[i]);
      tile_chunk.emplace_back(bit1[i]);
    }
  }
  write_chunk<uint8_t>("pale", palette_chunk, &ofs);
  write_chunk<uint8_t>("tile", tile_chunk, &ofs);

  return true;
}