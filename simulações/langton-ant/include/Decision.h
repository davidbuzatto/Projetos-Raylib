/**
 * @file Decision.h
 * @author Prof. Dr. David Buzatto
 * @brief Decision class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <TurnType.h>

class Decision {

    unsigned int color;
    TurnType turnType;

public:

    /**
     * @brief Construct a new Decision object.
     */
    Decision( unsigned int color, TurnType turnType );

    /**
     * @brief Destroy the Decision object.
     */
    ~Decision();

    TurnType getTurnType() const;
    unsigned int getColor() const;

};