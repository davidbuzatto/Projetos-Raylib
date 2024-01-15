#pragma once

enum TurnType {
    TURN_0 = 0,      // N
    TURN_60 = 60,    // R1
    TURN_120 = 120,  // R2
    TURN_180 = 180,  // U
    TURN_240 = 240,  // L2   (120 counter-clockwize)
    TURN_300 = 300   // L1   ( 60 counter-clockwize)
};