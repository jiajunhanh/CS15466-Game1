#pragma once

#include "PPU466.hpp"
#include "Mode.hpp"
#include "Load.hpp"
#include "read_write_chunk.hpp"
#include "data_path.hpp"
#include "Level.h"
#include "asset_pipeline.h"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <fstream>
#include <cstdio>

namespace {
std::vector<Level> levels;
std::vector<std::array<glm::u8vec4, 4>> palettes;
std::vector<uint8_t> bit0;
std::vector<uint8_t> bit1;

[[maybe_unused]] Load<void> _load_helper
    (LoadTag::LoadTagDefault,
     [&levels = levels]() {
       std::vector<uint8_t> data;
       std::ifstream ifs{data_path("game_data"), std::ios::binary};
       if (!ifs) {
         if (!process_assets()) {
           exit(1);
         }
         ifs.open(data_path("game_data"), std::ios::binary);
       }

       // ------------ read levels ------------
       read_chunk<uint8_t>(ifs, "leve", &data);
       std::array<uint8_t, Level::n_tiles> tiles{};
       auto tiles_itr = tiles.begin();
       for (const auto tile : data) {
         *tiles_itr = tile;
         if (++tiles_itr == tiles.end()) {
           levels.emplace_back(tiles);
           tiles_itr = tiles.begin();
         }
       }

       // ------------ read sprites ------------
       read_chunk<uint8_t>(ifs, "pale", &data);
       std::array<glm::u8vec4, 4> palette{};
       auto palette_itr = palette.begin();
       for (size_t i = 0; i < data.size(); i += 4) {
         (*palette_itr) = {data[i], data[i + 1], data[i + 2], data[i + 3]};
         if (++palette_itr == palette.end()) {
           palettes.emplace_back(palette);
           palette_itr = palette.begin();
         }
       }

       read_chunk<uint8_t>(ifs, "tile", &data);
       for (size_t i = 0; i < data.size(); i += 2) {
         bit0.emplace_back(data[i]);
         bit1.emplace_back(data[i + 1]);
       }
     }
    );
}

struct PlayMode : Mode {
  PlayMode();
  virtual ~PlayMode();

  //functions called by main loop:
  virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
  virtual void update(float elapsed) override;
  virtual void draw(glm::uvec2 const &drawable_size) override;

  //helper functions
  void load_level(size_t id);
  //----- game state -----

  static constexpr auto max_velocity = glm::vec2(30.0f, 30.0f);
  static constexpr float gravity = 100.0f;
  static constexpr float launch_interval = 2.0f;
  static constexpr float shoot_interval = 1.0f;
  float time_since_last_launch{};
  float time_since_last_shoot{};
  bool can_shoot{};
  int next_object{};
  int n_activated_cannons{};

  // level index
  size_t level_id{};
  Level *curr_level{};

  //input tracking:
  struct Button {
    uint8_t downs = 0;
    uint8_t pressed = 0;
  } left, right, down, up, shoot;

  //object status:
  enum class ObjectType {
    player,
    shell,
    bullet
  };

  enum class ObjectDirection {
    left,
    right
  };

  struct Object {
    ObjectType type;
    glm::vec2 pos{};
    glm::vec2 v{};
    ObjectDirection direction{};
  };

  std::array<Object, 64> objects{};

  //----- drawing handled by PPU466 -----

  PPU466 ppu;
};
