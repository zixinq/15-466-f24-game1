 #include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"
#include "asset_save.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>
#include "read_write_chunk.hpp"
#include "data_path.hpp"
#include "asset_save.hpp"
#include <iostream>
#include <fstream>

#include <random>

PlayMode::PlayMode() {
    std::vector< std::string > assets = {
            "hamster",
            "mushroom",
        };

        for (uint32_t asset_index = 0; asset_index < assets.size(); ++asset_index ) {
            std::vector< glm::u8vec4 > data;
            glm::uvec2 size;
            load_png("sprites/" + assets[asset_index] + ".png", &size, &data, UpperLeftOrigin);
            
            //create the palette
            for (uint32_t i = 0; i < 8; ++i) {
                for (uint32_t j = 0; j < 8; ++j) {
                    uint32_t index = 8 * (7-j) + (7-i);
    
                    
                    int palette_size = 0;
                    uint32_t palette_index = 0;
                    
                    
                    //find the color in palette
                    for(uint32_t k = 0; k < 4; ++k){
                        if(ppu.palette_table[asset_index][k] == data[index]){
                            palette_index = k;
                            break;
                        }
                    }
                    //color is not in the palette and palette size < 4
                    if(palette_size < 4 ){
                        ppu.palette_table[asset_index][palette_size] = data[index];
                        palette_size++;
                        palette_index = palette_size;
                    }
                    
                    //store the palette index in the tile
                    uint32_t bit0 = palette_index % 2;
                    uint32_t bit1 = palette_index / 2;
                    
                    ppu.tile_table[asset_index].bit0[i] |= bit0 << j;
                    ppu.tile_table[asset_index].bit1[i] |= bit1 << j;
                    
                }
            }
        }
            
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == SDLK_SPACE && onground == true) {
            isjumping = true;
        }
	} else if (evt.type == SDL_KEYUP) {
        if (evt.key.keysym.sym == SDLK_SPACE && onground == false) {
            isjumping = false;
        }
	}

	return false;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// (will be used to set background color)
	background_fade += elapsed / 10.0f;
	background_fade -= std::floor(background_fade);

	constexpr float PlayerSpeed = 1.0f;
    if (isjumping) {
        player_at.x += PlayerSpeed;
        player_at.y += 2 * PlayerSpeed;
        if(player_at.y >= 30.0f){
            isjumping = false;
            isfalling = true;
        }
    }else if(isfalling){
        player_at.x += PlayerSpeed;
        player_at.y -= 2 * PlayerSpeed;
        if(player_at.y <= 0.0f){
            isfalling = false;
        }
    }

}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

    /*
	//background color will be some hsv-like fade:
	ppu.background_color = glm::u8vec4(
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 0.0f / 3.0f) ) ) ))),
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 1.0f / 3.0f) ) ) ))),
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 2.0f / 3.0f) ) ) ))),
		0xff
	);
     
	//tilemap gets recomputed every frame as some weird plasma thing:
	//NOTE: don't do this in your game! actually make a map or something :-)
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			//TODO: make weird plasma thing
			ppu.background[x+PPU466::BackgroundWidth*y] = ((x+y)%16);
		}
	}
 */
    ppu.background_color = glm::u8vec4(0xff, 0xff, 0xff, 0xff);
    

	//background scroll:
	ppu.background_position.x = int32_t(-0.5f * player_at.x);
	ppu.background_position.y = int32_t(-0.5f * player_at.y);
    
    
    
	//player sprite:
    ppu.sprites[0].x = int32_t(player_at.x);
    ppu.sprites[0].y = int32_t(player_at.y);
    ppu.sprites[0].index = 0;
    ppu.sprites[0].attributes = 0;
    
   



/*
	//some other misc sprites:
	for (uint32_t i = 1; i < 63; ++i) {
		float amt = (i + 2.0f * background_fade) / 62.0f;
		ppu.sprites[i].x = int8_t(0.5f * PPU466::ScreenWidth + std::cos( 2.0f * M_PI * amt * 5.0f + 0.01f * player_at.x) * 0.4f * PPU466::ScreenWidth);
		ppu.sprites[i].y = int8_t(0.5f * PPU466::ScreenHeight + std::sin( 2.0f * M_PI * amt * 3.0f + 0.01f * player_at.y) * 0.4f * PPU466::ScreenWidth);
		ppu.sprites[i].index = 32;
		ppu.sprites[i].attributes = 6;
		if (i % 2) ppu.sprites[i].attributes |= 0x80; //'behind' bit
	}
 */

	//--- actually draw ---
	ppu.draw(drawable_size);
}
