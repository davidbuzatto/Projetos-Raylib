#include "base.h"
#include "tile.h"

void drawTile( Tile *tile ) {

    SpriteData *data = &tile->data;

    if ( tile->visible ) {
        DrawRectangle( 
            (int) data->x, (int) data->y,
            (int) data->width, (int) data->height, 
            data->baseColor );
    }

}

/*CollisionResult interceptsTyle( Tile *tile, Player *player ) {
    return COLLISION_NONE;
}*/