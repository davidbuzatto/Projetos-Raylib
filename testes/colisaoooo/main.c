/*******************************************************************************
 * Collision detection for 2D games.
 * Based on: https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_Rectangles.cpp
 * Work in progress.
 * 
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"
#include "include/raymath.h"

typedef enum {
    COLLISION_NONE = 0,
    COLLISION_LEFT,
    COLLISION_RIGHT,
    COLLISION_TOP,
    COLLISION_BOTTOM
} CollisionType;

typedef struct {
    Vector2 point;
    CollisionType type;
} CollisionData;

typedef struct SpriteData {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    struct SpriteData *contact[4];
} SpriteData;

typedef struct {
    SpriteData data;
    Color color;
} Player;

typedef struct {
    SpriteData data;
    Color color;
} Obstacle;

typedef struct {
    Player *player;
    Obstacle *obstacle;
} GameWorld;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void drawPlayer( Player *player );
void inputAndUpdatePlayer( GameWorld *gw );
void drawObstacle( Obstacle *obstacle );

bool pointVsRectangle( const Vector2 point, const Rectangle *rectangle );
bool rectangleVsRectangle( const Rectangle *rectangle1, const Rectangle *rectangle2 );
bool rayVsRectangle( 
    const Vector2 *rayOrigin,
    const Vector2 *rayDirection,
    const SpriteData *target,
    Vector2 *contactPoint,
    Vector2 *contactNormal,
    float *targetHitNear );
bool dynamicRectangleVsRectangle(
    const SpriteData *dynamicRectangle,
    const float frameTimeStep, 
    const SpriteData *staticRectangle,
	Vector2 *contactPoint, 
    Vector2 *contactNormal, 
    float *contactTime );
bool resolveDynamicRectangleVsRectangle(
    SpriteData *dynamicRectangle, 
    const float frameTimeStep, 
    SpriteData *staticRectangle );

void drawCollisionData( const CollisionData *cd, int x, int y );
void drawVector2( const char *label, const Vector2 *vector, int x, int y );

int main( void ) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    Player player = {0};
    player.data.size.x = 50;
    player.data.size.y = 50;
    player.data.position.x = screenWidth / 2 - player.data.size.x / 2 - 100;
    player.data.position.y = screenHeight / 2 - player.data.size.y / 2;
    player.color = BLUE;

    Obstacle obstacle = {0};
    obstacle.data.size.x = 50;
    obstacle.data.size.y = 50;
    obstacle.data.position.x = screenWidth / 2 - obstacle.data.size.x / 2;
    obstacle.data.position.y = screenHeight / 2 - obstacle.data.size.y / 2;
    obstacle.color = BLACK;

    GameWorld gw = {
        .player = &player,
        .obstacle = &obstacle
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Collision Detection" );
    SetTargetFPS( 60 );    

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {
    inputAndUpdatePlayer( gw );
}

void draw( GameWorld *gw ) {
    BeginDrawing();
    ClearBackground( WHITE );
    drawPlayer( gw->player );
    drawObstacle( gw->obstacle );
    EndDrawing();
}

void drawPlayer( Player *player ) {
    DrawRectangle( 
        player->data.position.x, 
        player->data.position.y, 
        player->data.size.x, 
        player->data.size.y, 
        player->color );
}

void inputAndUpdatePlayer( GameWorld *gw ) {

    Player *player = gw->player;
    Obstacle *obstacle = gw->obstacle;

    int baseSpeed = 5;

    if ( IsKeyDown( KEY_LEFT ) ) {
        player->data.velocity.x = -baseSpeed;
    } else if ( IsKeyDown( KEY_RIGHT ) ) {
        player->data.velocity.x = baseSpeed;
    } else {
        player->data.velocity.x = 0;
    }

    if ( IsKeyDown( KEY_UP ) ) {
        player->data.velocity.y = -baseSpeed;
    } else if ( IsKeyDown( KEY_DOWN ) ) {
        player->data.velocity.y = baseSpeed;
    } else {
        player->data.velocity.y = 0;
    }

    player->data.position.x += player->data.velocity.x;
    player->data.position.y += player->data.velocity.y;

    resolveDynamicRectangleVsRectangle( &player->data, GetFrameTime(), &obstacle->data );
    
}

void drawObstacle( Obstacle *obstacle ) {
    DrawRectangle( 
        obstacle->data.position.x, 
        obstacle->data.position.y, 
        obstacle->data.size.x, 
        obstacle->data.size.y, 
        obstacle->color );
}

void drawCollisionData( const CollisionData *cd, int x, int y ) {

    char data[40];

    switch ( cd->type ) {
        case   COLLISION_NONE: sprintf( data, "%s", "  NONE" ); break;
        case   COLLISION_LEFT: sprintf( data, "%s", "  LEFT" ); break;
        case  COLLISION_RIGHT: sprintf( data, "%s", " RIGHT" ); break;
        case    COLLISION_TOP: sprintf( data, "%s", "   TOP" ); break;
        case COLLISION_BOTTOM: sprintf( data, "%s", "BOTTOM" ); break;
    }

    // 6 => buffer offset
    sprintf( data + 6, " at[%.2f, %.2f]", cd->point.x, cd->point.y );
    DrawText( data, x, y, 20, BLACK );

}

void drawVector2( const char *label, const Vector2 *vector, int x, int y ) {
    char data[40];
    sprintf( data, "%s [%.2f, %.2f]", label, vector->x, vector->y );
    DrawText( data, x, y, 20, BLACK );
}

bool pointVsRectangle( const Vector2 point, const Rectangle *rectangle ) {
    return point.x >= rectangle->x && 
           point.y >= rectangle->y && 
           point.x < rectangle->x + rectangle->width && 
           point.y < rectangle->height + rectangle->height;
}

bool rectangleVsRectangle( const Rectangle *rectangle1, const Rectangle *rectangle2 ) {
    return rectangle1->x < rectangle2->x + rectangle2->width && 
           rectangle1->x + rectangle1->width > rectangle2->x && 
           rectangle1->y < rectangle2->y + rectangle2->height && 
           rectangle1->y + rectangle1->height > rectangle2->y;
}

bool rayVsRectangle( 
    const Vector2 *rayOrigin,
    const Vector2 *rayDirection,
    const SpriteData *targetRectangle,
    Vector2 *contactPoint,
    Vector2 *contactNormal,
    float *targetHitNear ) {

    CollisionData cd = {0};

    // cache division
    Vector2 inverseDirection = Vector2Invert( *rayDirection );

    Vector2 nearIntersection = Vector2Multiply( 
        Vector2Subtract( 
            targetRectangle->position, 
            *rayOrigin ), 
        inverseDirection
    );
    Vector2 farIntersection = Vector2Multiply( 
        Vector2Subtract( 
            Vector2Add( 
                targetRectangle->position, 
                targetRectangle->size
            ), 
            *rayOrigin ), 
        inverseDirection
    );

    if ( isnan( farIntersection.x ) || isnan( farIntersection.y ) ) {
        return false;
    }
    if ( isnan( nearIntersection.x ) || isnan( nearIntersection.y ) ) {
        return false;
    }

    // sort
    if ( nearIntersection.x > farIntersection.x ) {
        float t = nearIntersection.x;
        nearIntersection.x = farIntersection.x;
        farIntersection.x = t;
    }
    if ( nearIntersection.y > farIntersection.y ) {
        float t = nearIntersection.y;
        nearIntersection.y = farIntersection.y;
        farIntersection.y = t;
    }

    // early rejection
    if ( nearIntersection.x > farIntersection.y || nearIntersection.y > farIntersection.x ) {
        return false;
    }

    // time of contact
    // closest time will be first contact
    *targetHitNear = fmax( nearIntersection.x, nearIntersection.y );
    // furthest time is contact of opposite side
    float targetHitFar = fmin( farIntersection.x, farIntersection.y );

    // reject if ray direction is pointing away from object
    if ( targetHitFar < 0 ) {
        return false;
    }

    // contact point of collision from parametric line equation
    *contactPoint = Vector2Multiply( 
        Vector2AddValue( *rayOrigin, *targetHitNear ), *rayDirection );

    if ( nearIntersection.x > nearIntersection.y ) {
        if ( inverseDirection.x < 0 ) {
            *contactNormal = (Vector2){ 1, 0 };
        } else {
            *contactNormal = (Vector2){ -1, 0 };
        }
    } else if ( nearIntersection.x < nearIntersection.y ) {
        if ( inverseDirection.y < 0 ) {
            *contactNormal = (Vector2){ 0, 1 };
        } else {
            *contactNormal = (Vector2){ 0, -1 };
        }
    }

    return true;

}

bool dynamicRectangleVsRectangle(
    const SpriteData *dynamicRectangle,
    const float frameTimeStep, 
    const SpriteData *staticRectangle,
	Vector2 *contactPoint, 
    Vector2 *contactNormal, 
    float *contactTime ) {

    // check if dynamic rectangle is actually moving - we assume rectangles
    // are NOT in collision to start
    if ( dynamicRectangle->velocity.x == 0 && dynamicRectangle->velocity.y == 0 ) {
        return false;
    }

    // expand target rectangle by source dimensions
    SpriteData expandedTarget = {0};
    Vector2 halfDynamicRectangleSize = {
        .x = dynamicRectangle->size.x / 2,
        .y = dynamicRectangle->size.y / 2,
    };

    
    expandedTarget.position = Vector2Subtract( 
        staticRectangle->position, halfDynamicRectangleSize );

    expandedTarget.size = Vector2Add( 
        staticRectangle->size, dynamicRectangle->size );

    Vector2 rayOrigin = Vector2Add( 
        dynamicRectangle->position, halfDynamicRectangleSize );
    Vector2 rayDirection = {
        .x = dynamicRectangle->velocity.x * frameTimeStep,
        .y = dynamicRectangle->velocity.y * frameTimeStep,
    };

    if ( rayVsRectangle( 
        &rayOrigin,
        &rayDirection, 
        &expandedTarget, 
        contactPoint, 
        contactNormal, 
        contactTime ) ) {
        return *contactTime >= 0.0f && *contactTime < 1.0f;
    } else {
        return false;
    }

}

bool resolveDynamicRectangleVsRectangle(
    SpriteData *dynamicRectangle, 
    const float frameTimeStep, 
    SpriteData *staticRectangle ) {

    Vector2 contact_point;
    Vector2 contact_normal;

    float contact_time = 0.0f;

    if ( dynamicRectangleVsRectangle( 
        dynamicRectangle, 
        frameTimeStep, 
        staticRectangle, 
        &contact_point, 
        &contact_normal, 
        &contact_time ) ) {

        if ( contact_normal.y > 0 ) {
            dynamicRectangle->contact[0] = staticRectangle;
        } else {
            dynamicRectangle->contact[0] = NULL;
        }

        if ( contact_normal.x < 0 ) {
            dynamicRectangle->contact[1] = staticRectangle;
        } else {
            dynamicRectangle->contact[1] = NULL;
        }

        if ( contact_normal.y < 0 ) {
            dynamicRectangle->contact[2] = staticRectangle;
        } else {
            dynamicRectangle->contact[2] = NULL;
        }

        if ( contact_normal.x > 0 ) {
            dynamicRectangle->contact[3] = staticRectangle;
        } else {
            dynamicRectangle->contact[3] = NULL;
        }

        Vector2 v = {
            .x = fabs( dynamicRectangle->velocity.x ),
            .y = fabs( dynamicRectangle->velocity.y )
        };
        
        Vector2 x = {
            .x = contact_normal.x * v.x * ( 1 - contact_time ),
            .y = contact_normal.y * v.y * ( 1 - contact_time )
        };

        dynamicRectangle->velocity = Vector2Add( 
            dynamicRectangle->velocity,
            x
        );

        return true;

    }

    return false;

}