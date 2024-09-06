//
//  asset_save.cpp
//  
//
//  Created by Joyce Qiu on 9/5/24.
//

#include "convert_assets.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <array>

#include <glm/glm.hpp>
#include "load_save_png.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"
#include "PPU466.hpp"


int main(){
    //create the palette
    for (uint32_t asset_index = 0; asset_index < assets.size(); ++asset_index ){
        for (uint32_t i = 0; i < 8; ++i) {
            for (uint32_t j = 0; j < 8; ++j) {
                uint32_t index = 8 * j + i;
                int palette_size = 0;
                uint32_t palette_index = 0;
                
                color = asset[index];
                //find the color in palette
                for(uint32_t k = 0; k < 4; ++k){
                    if(ppu.palette_table[asset_index][k] == color ){
                        palette_index = k;
                        break;
                    }
                }
                //color is not in the palette and palette size < 4
                if(palette.size < 4 ){
                        ppu.palette_table[asset_index][palette_size] = color;
                        palette_size++;
                        palette_index = palette_size;
                    }
                
                //store the palette index in the tile
                uint32_t bit0 = palette_index % 2;
                uint32_t bit0 = palette_index / 2;
                
                ppu.tile_table[aseet_index].bit0[i] |= bit0 << (7-j);
                ppu.tile_table[asset_index].bit1[i] |= bit1 << (7-j);
                
            }
        }
    }
                
}
