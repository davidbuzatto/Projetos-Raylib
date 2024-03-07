#include <math.h>
#include "Tile.h"

void drawTile( Tile *tile ) {

    DrawRectangle( 
        tile->pos.x, 
        tile->pos.y, 
        tile->dim.x, 
        tile->dim.y, 
        Fade( tile->color, 0.5f ) );

    if ( tile->collisionType == COLLISION_TYPE_UP ) {
        if ( tile->type == TILE_TYPE_SLOPE_UP ) {
            DrawTriangle( 
                (Vector2){ tile->pos.x + tile->dim.x, tile->pos.y }, 
                (Vector2){ tile->pos.x, tile->pos.y }, 
                (Vector2){ tile->pos.x, tile->pos.y + tile->dim.y }, 
                Fade( tile->color, 0.5 )
            );
        } else {
            DrawTriangle( 
                (Vector2){ tile->pos.x, tile->pos.y }, 
                (Vector2){ tile->pos.x + tile->dim.x, tile->pos.y + tile->dim.y }, 
                (Vector2){ tile->pos.x + tile->dim.x, tile->pos.y }, 
                Fade( tile->color, 0.5 )
            );
        }
    } else if ( tile->collisionType == COLLISION_TYPE_DOWN ) {
        if ( tile->type == TILE_TYPE_SLOPE_UP ) {
            DrawTriangle( 
                (Vector2){ tile->pos.x + tile->dim.x, tile->pos.y }, 
                (Vector2){ tile->pos.x, tile->pos.y + tile->dim.y }, 
                (Vector2){ tile->pos.x + tile->dim.x, tile->pos.y + tile->dim.y }, 
                Fade( tile->color, 0.5 )
            );
        } else {
            DrawTriangle( 
                (Vector2){ tile->pos.x, tile->pos.y }, 
                (Vector2){ tile->pos.x, tile->pos.y + tile->dim.y }, 
                (Vector2){ tile->pos.x + tile->dim.x, tile->pos.y + tile->dim.y }, 
                Fade( tile->color, 0.5 )
            );
        }
    }

    if ( tile->type == TILE_TYPE_SLOPE_UP ) {
        DrawLine( 
            tile->pos.x, 
            tile->pos.y + tile->dim.y,
            tile->pos.x + tile->dim.x,
            tile->pos.y, 
            tile->color );
    } else {
        DrawLine( 
            tile->pos.x, 
            tile->pos.y,
            tile->pos.x + tile->dim.x,
            tile->pos.y + tile->dim.y, 
            tile->color );
    }

}

// angular coefficient -> 1 (45 degrees)
float linearFunc1( float x, float offset ) {
    return offset - x;
}

float linearFunc2( float x, float offset ) {
    return offset - x;
}

CollisionType checkCollisionTileVector2( Tile *tile, Vector2 pos ) {

    const float xStart = tile->pos.x;
    const float xEnd = xStart + tile->dim.x;
    const float yStart = tile->pos.y;
    const float yEnd = yStart + tile->dim.y;
    const float width = tile->dim.x;
    const float height = tile->dim.y;

    if ( pos.x >= xStart && 
         pos.x <= xEnd &&
         pos.y >= yStart &&
         pos.y <= yEnd ) {
        
        float adjPosX = pos.x - tile->pos.x;
        float adjPosY = pos.y - tile->pos.y;
        const float ratio = height / width;
        float projectionY = 0;

        if ( tile->type == TILE_TYPE_SLOPE_UP ) {
            projectionY = height - adjPosX * ratio;
        } else if ( tile->type == TILE_TYPE_SLOPE_DOWN ) {
            projectionY = adjPosX * ratio;
        }

        DrawCircle( adjPosX, adjPosY, 5, BLACK );
        DrawCircle( adjPosX, projectionY, 5, ORANGE );

        if ( adjPosY <= projectionY ) {
            return COLLISION_TYPE_UP;
        } else {
            return COLLISION_TYPE_DOWN;
        }

    }

    return COLLISION_TYPE_NONE;

}
