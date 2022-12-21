#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <iostream>

namespace {
constexpr auto empty_idx = int(Level::TileType::empty);
constexpr auto wall_idx = int(Level::TileType::wall);
constexpr auto spike_idx = int(Level::TileType::spike);
constexpr auto spawn_point_idx = int(Level::TileType::spawn_point);
constexpr auto target_point_idx = int(Level::TileType::target_point);
}

PlayMode::PlayMode() {


  ppu.tile_table[empty_idx].bit0 = {};
  ppu.tile_table[empty_idx].bit1 = {};

  ppu.tile_table[wall_idx].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[wall_idx].bit1 = {};

  ppu.tile_table[spike_idx].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[spike_idx].bit1 = {};

  ppu.tile_table[spawn_point_idx].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[spawn_point_idx].bit1 = {};

  ppu.tile_table[target_point_idx].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[target_point_idx].bit1 = {};

  //use sprite 32 as a "player":
  ppu.tile_table[32].bit0 = {
      0b01111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
  };
  ppu.tile_table[32].bit1 = {
      0b00000000,
      0b00000000,
      0b00011000,
      0b00100100,
      0b00000000,
      0b00100100,
      0b00000000,
      0b00000000,
  };

  ppu.palette_table[empty_idx] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[wall_idx] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0xa5, 0x2a, 0x2a, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[spike_idx] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x80, 0x80, 0x80, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[spawn_point_idx] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0xff, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[target_point_idx] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x00, 0x00, 0xff, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  //used for the player:
  ppu.palette_table[7] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0xff, 0xff, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

  if (evt.type == SDL_KEYDOWN) {
    if (evt.key.keysym.sym == SDLK_LEFT) {
      left.downs += 1;
      left.pressed = true;
      return true;
    } else if (evt.key.keysym.sym == SDLK_RIGHT) {
      right.downs += 1;
      right.pressed = true;
      return true;
    } else if (evt.key.keysym.sym == SDLK_UP) {
      up.downs += 1;
      up.pressed = true;
      return true;
    } else if (evt.key.keysym.sym == SDLK_DOWN) {
      down.downs += 1;
      down.pressed = true;
      return true;
    }
  } else if (evt.type == SDL_KEYUP) {
    if (evt.key.keysym.sym == SDLK_LEFT) {
      left.pressed = false;
      return true;
    } else if (evt.key.keysym.sym == SDLK_RIGHT) {
      right.pressed = false;
      return true;
    } else if (evt.key.keysym.sym == SDLK_UP) {
      up.pressed = false;
      return true;
    } else if (evt.key.keysym.sym == SDLK_DOWN) {
      down.pressed = false;
      return true;
    }
  }

  return false;
}

void PlayMode::update(float elapsed) {
  constexpr static glm::vec2 velocity(30.0f, 30.0f);
  constexpr float gravity = 100.0f;

  player_velocity.x = 0.0f;
  if (left.pressed) player_velocity.x = -velocity.x;
  if (right.pressed) player_velocity.x = velocity.x;

  if (up.pressed) player_velocity.y = velocity.y;
  player_velocity.y -= gravity * elapsed;
  player_velocity.y = std::max(-velocity.y, player_velocity.y);

  player_location += player_velocity * elapsed;

  //reset button press counters:
  left.downs = 0;
  right.downs = 0;
  up.downs = 0;
  down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
  const auto &tiles = levels[level_id].tiles();

  ppu.background_color = glm::u8vec4(0x87, 0xce, 0xeb, 0xff);

  for (uint32_t y = 0; y < Level::height; ++y) {
    for (uint32_t x = 0; x < Level::width; ++x) {
      uint8_t tile_type = tiles[x + y * Level::width];
      uint16_t bits = tile_type | (tile_type << 8);
      ppu.background[x + PPU466::BackgroundWidth * y] = bits;
    }
  }

  //player sprite:
  ppu.sprites[0].x = int8_t(player_location.x);
  ppu.sprites[0].y = int8_t(player_location.y);
  ppu.sprites[0].index = 32;
  ppu.sprites[0].attributes = 7;

  //--- actually draw ---
  ppu.draw(drawable_size);
}
