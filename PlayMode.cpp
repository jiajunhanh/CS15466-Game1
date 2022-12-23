#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <iostream>

namespace {
constexpr auto empty_tile = 0;
constexpr auto wall_tile = 1;
constexpr auto spike_tile = 2;
constexpr auto artillery_tile = 3;
constexpr auto target_point_tile = 4;
constexpr auto player_tile = 5;
constexpr auto shell_tile = 6;

constexpr auto wall_palette = 0;
constexpr auto spike_palette = 1;
constexpr auto artillery_palette = 2;
constexpr auto target_point_palette = 3;
constexpr auto player_palette = 4;
constexpr auto shell_palette = 5;
}

PlayMode::PlayMode() {
  load_level(0);

  ppu.tile_table[empty_tile].bit0 = {};
  ppu.tile_table[empty_tile].bit1 = {};

  ppu.tile_table[wall_tile].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[wall_tile].bit1 = {};

  ppu.tile_table[spike_tile].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[spike_tile].bit1 = {};

  ppu.tile_table[artillery_tile].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[artillery_tile].bit1 = {};

  ppu.tile_table[target_point_tile].bit0 = {
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
  };
  ppu.tile_table[target_point_tile].bit1 = {};

  ppu.tile_table[player_tile].bit0 = {
      0b01111110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
  };
  ppu.tile_table[player_tile].bit1 = {
      0b00000000,
      0b00000000,
      0b00011000,
      0b00100100,
      0b00000000,
      0b00100100,
      0b00000000,
      0b00000000,
  };

  ppu.tile_table[shell_tile].bit0 = {
      0b00000000,
      0b00000000,
      0b00000000,
      0b00011000,
      0b00011000,
      0b00000000,
      0b00000000,
      0b00000000,
  };
  ppu.tile_table[shell_tile].bit1 = {};

  ppu.palette_table[wall_palette] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0xa5, 0x2a, 0x2a, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[spike_palette] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x80, 0x80, 0x80, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[artillery_palette] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x4b, 0x53, 0x20, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[target_point_palette] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x00, 0x00, 0xff, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[player_palette] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0xff, 0xff, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
  };

  ppu.palette_table[shell_palette] = {
      glm::u8vec4(0x00, 0x00, 0x00, 0x00),
      glm::u8vec4(0x00, 0x00, 0x00, 0xff),
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
  constexpr auto max_velocity = glm::vec2(30.0f, 30.0f);
  constexpr auto gravity = 100.0f;
  constexpr auto launch_interval = 2.0f;
  static auto time_since_last_launch = 0.0f;
  static int next_object = 1;

  time_since_last_launch += elapsed;
  if (time_since_last_launch > launch_interval) {
    time_since_last_launch = 0.0f;
    for (auto artillery_idx : levels[level_id].artillery()) {
      auto x = artillery_idx % Level::width;
      auto y = artillery_idx / Level::width;
      auto pos_x = float(x) * 8.0f;
      auto pos_y = float(y) * 8.0f;
      auto &object = objects[next_object];
      next_object = (next_object + 1) % 63 + 1;
      object.pos = glm::vec2(pos_x, pos_y);
      object.v = glm::normalize(objects[0].pos - object.pos) * glm::length(max_velocity);
      object.type = ObjectType::shell;
    }
  }

  // process player object
  {
    auto &object = objects[0];
    // update player behavior
    object.v.x = 0.0f;
    if (left.pressed) object.v.x = -max_velocity.x;
    if (right.pressed) object.v.x = max_velocity.x;
    if (up.pressed) object.v.y = max_velocity.y;

    // add gravity force
    object.v.y -= gravity * elapsed;
    object.v.x = std::min(max_velocity.x, std::max(-max_velocity.x, object.v.x));
    object.v.y = std::min(max_velocity.y, std::max(-max_velocity.y, object.v.y));

    // ------------ update position ------------

    // horizontal movement
    const auto &tiles = levels[level_id].tiles();
    auto ghost_pos = object.pos + object.v * elapsed;
    auto tile_left = int(std::floor(object.pos.x)) / 8;
    auto tile_right = (int(std::ceil(object.pos.x + 8.0f)) - 1) / 8;
    auto tile_bottom = int(std::floor(object.pos.y)) / 8;
    auto tile_up = (int(std::ceil(object.pos.y + 8.0f)) - 1) / 8;
    tile_right = std::min(Level::width - 1, tile_right);
    tile_up = std::min(Level::height - 1, tile_up);

    if (object.v.x < 0.0f &&
        (tile_left == 0
            || tiles[tile_left - 1 + tile_bottom * Level::width] == std::uint8_t(Level::TileType::wall)
            || tiles[tile_left - 1 + tile_up * Level::width] == std::uint8_t(Level::TileType::wall))
        && ghost_pos.x < float(tile_left) * 8.0f) {
      object.pos.x = float(tile_left) * 8.0f;
      object.v.x = 0.0f;
    } else if (object.v.x > 0.0f &&
        (tile_right == Level::width - 1
            || tiles[tile_right + 1 + tile_bottom * Level::width] == std::uint8_t(Level::TileType::wall)
            || tiles[tile_right + 1 + tile_up * Level::width] == std::uint8_t(Level::TileType::wall))
        && ghost_pos.x > float(tile_right) * 8.0f) {
      object.pos.x = float(tile_right) * 8.0f;
      object.v.x = 0.0f;
    }
    object.pos.x += object.v.x * elapsed;

    // vertical movement
    ghost_pos = object.pos + object.v * elapsed;
    tile_left = int(std::floor(object.pos.x)) / 8;
    tile_right = (int(std::ceil(object.pos.x + 8.0f)) - 1) / 8;
    tile_bottom = int(std::floor(object.pos.y)) / 8;
    tile_up = (int(std::ceil(object.pos.y + 8.0f)) - 1) / 8;
    tile_right = std::min(Level::width - 1, tile_right);
    tile_up = std::min(Level::height - 1, tile_up);

    if (object.v.y < 0.0f &&
        (tile_bottom == 0
            || tiles[tile_left + (tile_bottom - 1) * Level::width] == std::uint8_t(Level::TileType::wall)
            || tiles[tile_right + (tile_bottom - 1) * Level::width] == std::uint8_t(Level::TileType::wall))
        && ghost_pos.y < float(tile_bottom) * 8.0f) {
      object.pos.y = float(tile_bottom) * 8.0f;
      object.v.y = 0.0f;
    } else if (object.v.y > 0.0f && (tile_up == Level::height - 1
        || tiles[tile_left + (tile_up + 1) * Level::width] == std::uint8_t(Level::TileType::wall)
        || tiles[tile_right + (tile_up + 1) * Level::width] == std::uint8_t(Level::TileType::wall))
        && ghost_pos.y > float(tile_up) * 8.0f) {
      object.pos.y = float(tile_up) * 8.0f;
      object.v.y = 0.0f;
    }
    object.pos.y += object.v.y * elapsed;


    // check if player passed the level
    tile_left = int(std::floor(object.pos.x)) / 8;
    tile_right = (int(std::ceil(object.pos.x + 8.0f)) - 1) / 8;
    tile_bottom = int(std::floor(object.pos.y)) / 8;
    tile_up = (int(std::ceil(object.pos.y + 8.0f)) - 1) / 8;
    if (tiles[tile_left + tile_bottom * Level::width] == std::uint8_t(Level::TileType::target_point)
        || tiles[tile_right + tile_bottom * Level::width] == std::uint8_t(Level::TileType::target_point)
        || tiles[tile_left + tile_up * Level::width] == std::uint8_t(Level::TileType::target_point)
        || tiles[tile_right + tile_up * Level::width] == std::uint8_t(Level::TileType::target_point)) {
      level_id = (level_id + 1) % levels.size();
      load_level(level_id);
      time_since_last_launch = 0.0f;
    }

    // check if player touched spikes
    tile_left = int(std::floor(object.pos.x)) / 8;
    tile_right = (int(std::ceil(object.pos.x + 8.0f)) - 1) / 8;
    tile_bottom = int(std::floor(object.pos.y)) / 8;
    tile_up = (int(std::ceil(object.pos.y + 8.0f)) - 1) / 8;
    if (tiles[tile_left + tile_bottom * Level::width] == std::uint8_t(Level::TileType::spike)
        || tiles[tile_right + tile_bottom * Level::width] == std::uint8_t(Level::TileType::spike)
        || tiles[tile_left + tile_up * Level::width] == std::uint8_t(Level::TileType::spike)
        || tiles[tile_right + tile_up * Level::width] == std::uint8_t(Level::TileType::spike)) {
      load_level(level_id);
      time_since_last_launch = 0.0f;
    }
  }

  // process for other objects
  for (int i = 1; i < 64; ++i) {
    objects[i].pos += objects[i].v * elapsed;
    // check if shell hits player
    if (objects[i].pos.x >= 0.0f && objects[i].pos.x < 256 && objects[i].pos.y >= 0.0f && objects[i].pos.y < 240 &&
        std::abs(objects[i].pos.x - objects[0].pos.x) < 5.0f && std::abs(objects[i].pos.y - objects[0].pos.y) < 5.0f) {
      load_level(level_id);
      time_since_last_launch = 0.0f;
    }
  }

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
      auto tile_type = Level::TileType(tiles[x + y * Level::width]);
      switch (tile_type) {
        case Level::TileType::wall :ppu.background[x + PPU466::BackgroundWidth * y] = wall_tile | (wall_palette << 8);
          break;
        case Level::TileType::spike :
          ppu.background[x + PPU466::BackgroundWidth * y] = spike_tile | (spike_palette << 8);
          break;
        case Level::TileType::artillery:
          ppu.background[x + PPU466::BackgroundWidth * y] = artillery_tile | (artillery_palette << 8);
          break;
        case Level::TileType::target_point :
          ppu.background[x + PPU466::BackgroundWidth * y] = target_point_tile | (target_point_palette << 8);
          break;
        default:ppu.background[x + PPU466::BackgroundWidth * y] = empty_tile;
      }
    }
  }

  //sprite:
  ppu.sprites[0].x = int8_t(objects[0].pos.x);
  ppu.sprites[0].y = int8_t(objects[0].pos.y);
  ppu.sprites[0].index = player_tile;
  ppu.sprites[0].attributes = player_palette;

  for (int i = 1; i < 64; ++i) {
    if (objects[i].pos.x >= 0.0f && objects[i].pos.x < 256 && objects[i].pos.y >= 0.0f && objects[i].pos.y < 240) {
      ppu.sprites[i].x = int8_t(objects[i].pos.x);
      ppu.sprites[i].y = int8_t(objects[i].pos.y);
      ppu.sprites[i].index = shell_tile;
      ppu.sprites[i].attributes = shell_palette;
    } else {
      ppu.sprites[i].y = 240;
    }
  }

  //--- actually draw ---
  ppu.draw(drawable_size);
}

void PlayMode::load_level(size_t id) {
  level_id = id;
  objects[0] = Object{levels[id].spawn_pos(), glm::vec2(0.0f), ObjectType::player};
  for (int i = 1; i < 64; ++i) {
    objects[i] = Object{glm::vec2(0.0f, 250.0f), glm::vec2(0.0f), ObjectType::shell};
  }
}
