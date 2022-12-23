#pragma once

#include "PPU466.hpp"
#include "Mode.hpp"
#include "Load.hpp"
#include "read_write_chunk.hpp"
#include "data_path.hpp"
#include "Level.h"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <fstream>
#include <cstdio>

namespace {
std::vector<Level> levels;

[[maybe_unused]] Load<void> _load_helper
    (LoadTag::LoadTagDefault,
     [&levels = levels]() {
       std::vector<uint8_t> data;
       std::ifstream ifs(data_path("game_data"), std::ios::binary);

       // ------------ read levels ------------
       read_chunk<uint8_t>(ifs, "leve", &data);
       std::array<uint8_t, Level::n_tiles> tiles{};
       auto itr = tiles.begin();
       for (const auto tile : data) {
         *itr = tile;
         if (++itr == tiles.end()) {
           levels.emplace_back(tiles);
           itr = tiles.begin();
         }
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

  // level index
  size_t level_id{};

  //input tracking:
  struct Button {
    uint8_t downs = 0;
    uint8_t pressed = 0;
  } left, right, down, up;

  //object status:
  enum class ObjectType {
    player,
    shell
  };

  struct Object {
    glm::vec2 pos{};
    glm::vec2 v{};
    ObjectType type{};
  };

  std::array<Object, 64> objects{};

  //----- drawing handled by PPU466 -----

  PPU466 ppu;
};
