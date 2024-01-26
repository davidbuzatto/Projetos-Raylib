/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Base template for game
 * development in C using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/*---------------------------------------------
 * Library headers.
 --------------------------------------------*/
#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#undef RAYGUI_IMPLEMENTATION

#include <styles/style_cyber.h>             // raygui style: cyber
#include <styles/style_jungle.h>            // raygui style: jungle
#include <styles/style_lavanda.h>           // raygui style: lavanda
#include <styles/style_dark.h>              // raygui style: dark
#include <styles/style_bluish.h>            // raygui style: bluish
#include <styles/style_terminal.h>          // raygui style: terminal

/*---------------------------------------------
 * Project headers.
 --------------------------------------------*/
#include <utils.h>

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/


/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct GameWorld {
    int dummy;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;

int dropdownBox000Active = 0;
bool dropDown000EditMode = false;

int dropdownBox001Active = 0;
bool dropDown001EditMode = false;

int spinner001Value = 0;
bool spinnerEditMode = false;

int valueBox002Value = 0;
bool valueBoxEditMode = false;

char textBoxText[64] = "Text box";
bool textBoxEditMode = false;

char textBoxMultiText[1024] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\nThisisastringlongerthanexpectedwithoutspacestotestcharbreaksforthosecases,checkingifworkingasexpected.\n\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
bool textBoxMultiEditMode = false;

int listViewScrollIndex = 0;
int listViewActive = -1;

int listViewExScrollIndex = 0;
int listViewExActive = 2;
int listViewExFocus = -1;
const char *listViewExList[8] = { "This", "is", "a", "list view", "with", "disable", "elements", "amazing!" };

char multiTextBoxText[256] = "Multi text box";
bool multiTextBoxEditMode = false;
Color colorPickerValue;

float sliderValue = 50.0f;
float sliderBarValue = 60;
float progressValue = 0.1f;

bool forceSquaredChecked = false;

float alphaValue = 0.5f;

//int comboBoxActive = 1;
int visualStyleActive = 0;
int prevVisualStyleActive = 0;

int toggleGroupActive = 0;
int toggleSliderActive = 0;

Vector2 viewScroll = { 0, 0 };

bool exitWindow = false;
bool showMessageBox = false;

char textInput[256] = { 0 };
char textInputFileName[256] = { 0 };
bool showTextInputBox = false;

float alpha = 1.0f;


/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( GameWorld *gw );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( const GameWorld *gw );

/**
 * @brief Create the global Game World object and all of its dependecies.
 */
void createGameWorld( void );

/**
 * @brief Destroy the global Game World object and all of its dependencies.
 */
void destroyGameWorld( void );

/**
 * @brief Load game resources like images, textures, sounds,
 * fonts, shaders etc.
 */
void loadResources( void );

/**
 * @brief Unload the once loaded game resources.
 */
void unloadResources( void );

int main( void ) {

    const int screenWidth = 960;
    const int screenHeight = 560;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Testes Raygui!" );
    SetExitKey(0);
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld();
    while ( !exitWindow ) {
        exitWindow = WindowShouldClose();
        inputAndUpdate( &gw );
        draw( &gw );
    }
    unloadResources();
    destroyGameWorld();

    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    if ( IsKeyPressed( KEY_ESCAPE ) ) {
        showMessageBox = !showMessageBox;
    }

    if ( IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S) ) {
        showTextInputBox = true;
    }

    if ( IsFileDropped() ) {

        FilePathList droppedFiles = LoadDroppedFiles();

        if ( ( droppedFiles.count > 0 ) && IsFileExtension( droppedFiles.paths[0], ".rgs" ) ) {
            GuiLoadStyle(droppedFiles.paths[0]);
        }

        UnloadDroppedFiles( droppedFiles );    // Clear internal buffers

    }

    //alpha -= 0.002f;
    if ( alpha < 0.0f ) {
        alpha = 0.0f;
    }
    if ( IsKeyPressed( KEY_SPACE ) ) {
        alpha = 1.0f;
    }

    GuiSetAlpha( alpha );

    //progressValue += 0.002f;
    if ( IsKeyPressed( KEY_LEFT ) ) {
        progressValue -= 0.1f;
    } else if ( IsKeyPressed( KEY_RIGHT ) ) {
        progressValue += 0.1f;
    }

    if ( progressValue > 1.0f ) { 
        progressValue = 1.0f;
    } else if ( progressValue < 0.0f ) {
        progressValue = 0.0f;
    }

    if ( visualStyleActive != prevVisualStyleActive ) {

        GuiLoadStyleDefault();

        switch (visualStyleActive) {
            case 0: break;      // Default style
            case 1: GuiLoadStyleJungle(); break;
            case 2: GuiLoadStyleLavanda(); break;
            case 3: GuiLoadStyleDark(); break;
            case 4: GuiLoadStyleBluish(); break;
            case 5: GuiLoadStyleCyber(); break;
            case 6: GuiLoadStyleTerminal(); break;
            default: break;
        }

        GuiSetStyle( LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT );

        prevVisualStyleActive = visualStyleActive;

    }

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );


    ClearBackground( GetColor( GuiGetStyle( DEFAULT, BACKGROUND_COLOR ) ) );

    // raygui: controls drawing
    //----------------------------------------------------------------------------------
    // Check all possible events that require GuiLock
    if ( dropDown000EditMode || dropDown001EditMode ) {
        GuiLock();
    }

    // First GUI column
    //GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiCheckBox((Rectangle){ 25, 108, 15, 15 }, "FORCE CHECK!", &forceSquaredChecked);

    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    //GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiSpinner((Rectangle){ 25, 135, 125, 30 }, NULL, &spinner001Value, 0, 100, spinnerEditMode)) spinnerEditMode = !spinnerEditMode;
    if (GuiValueBox((Rectangle){ 25, 175, 125, 30 }, NULL, &valueBox002Value, 0, 100, valueBoxEditMode)) valueBoxEditMode = !valueBoxEditMode;
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiTextBox((Rectangle){ 25, 215, 125, 30 }, textBoxText, 64, textBoxEditMode)) textBoxEditMode = !textBoxEditMode;

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    if (GuiButton((Rectangle){ 25, 255, 125, 30 }, GuiIconText(ICON_FILE_SAVE, "Save File"))) showTextInputBox = true;

    GuiGroupBox((Rectangle){ 25, 310, 125, 150 }, "STATES");
    //GuiLock();
    GuiSetState(STATE_NORMAL); if (GuiButton((Rectangle){ 30, 320, 115, 30 }, "NORMAL")) { }
    GuiSetState(STATE_FOCUSED); if (GuiButton((Rectangle){ 30, 355, 115, 30 }, "FOCUSED")) { }
    GuiSetState(STATE_PRESSED); if (GuiButton((Rectangle){ 30, 390, 115, 30 }, "#15#PRESSED")) { }
    GuiSetState(STATE_DISABLED); if (GuiButton((Rectangle){ 30, 425, 115, 30 }, "DISABLED")) { }
    GuiSetState(STATE_NORMAL);
    //GuiUnlock();

    GuiComboBox((Rectangle){ 25, 480, 125, 30 }, "default;Jungle;Lavanda;Dark;Bluish;Cyber;Terminal", &visualStyleActive);

    // NOTE: GuiDropdownBox must draw after any other control that can be covered on unfolding
    GuiUnlock();
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 4);
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiDropdownBox((Rectangle){ 25, 65, 125, 30 }, "#01#ONE;#02#TWO;#03#THREE;#04#FOUR", &dropdownBox001Active, dropDown001EditMode)) dropDown001EditMode = !dropDown001EditMode;
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DROPDOWNBOX, TEXT_PADDING, 0);

    if (GuiDropdownBox((Rectangle){ 25, 25, 125, 30 }, "ONE;TWO;THREE", &dropdownBox000Active, dropDown000EditMode)) dropDown000EditMode = !dropDown000EditMode;

    // Second GUI column
    GuiListView((Rectangle){ 165, 25, 140, 124 }, "Charmander;Bulbasaur;#18#Squirtel;Pikachu;Eevee;Pidgey", &listViewScrollIndex, &listViewActive);
    GuiListViewEx((Rectangle){ 165, 162, 140, 184 }, listViewExList, 8, &listViewExScrollIndex, &listViewExActive, &listViewExFocus);

    //GuiToggle((Rectangle){ 165, 400, 140, 25 }, "#1#ONE", &toggleGroupActive);
    GuiToggleGroup((Rectangle){ 165, 360, 140, 24 }, "#1#ONE\n#3#TWO\n#8#THREE\n#23#", &toggleGroupActive);
    //GuiDisable();
    GuiSetStyle(SLIDER, SLIDER_PADDING, 2);
    GuiToggleSlider((Rectangle){ 165, 480, 140, 30 }, "ON;OFF", &toggleSliderActive);
    GuiSetStyle(SLIDER, SLIDER_PADDING, 0);

    // Third GUI column
    GuiPanel((Rectangle){ 320, 25, 225, 140 }, "Panel Info");
    GuiColorPicker((Rectangle){ 320, 185, 196, 192 }, NULL, &colorPickerValue);

    //GuiDisable();
    GuiSlider((Rectangle){ 355, 400, 165, 20 }, "TEST", TextFormat("%2.2f", sliderValue), &sliderValue, -50, 100);
    GuiSliderBar((Rectangle){ 320, 430, 200, 20 }, NULL, TextFormat("%i", (int)sliderBarValue), &sliderBarValue, 0, 100);
    
    GuiProgressBar((Rectangle){ 320, 460, 200, 20 }, NULL, TextFormat("%i%%", (int)(progressValue*100)), &progressValue, 0.0f, 1.0f);
    GuiEnable();

    // NOTE: View rectangle could be used to perform some scissor test
    Rectangle view = { 0 };
    GuiScrollPanel((Rectangle){ 560, 25, 102, 354 }, NULL, (Rectangle){ 560, 25, 300, 1200 }, &viewScroll, &view);

    Vector2 mouseCell = { 0 };
    GuiGrid((Rectangle) { 560, 25 + 180 + 195, 100, 120 }, NULL, 20, 3, &mouseCell);

    GuiColorBarAlpha((Rectangle){ 320, 490, 200, 30 }, NULL, &alphaValue);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);   // WARNING: Word-wrap does not work as expected in case of no-top alignment
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);            // WARNING: If wrap mode enabled, text editing is not supported
    if (GuiTextBox((Rectangle){ 678, 25, 258, 492 }, textBoxMultiText, 1024, textBoxMultiEditMode)) textBoxMultiEditMode = !textBoxMultiEditMode;
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_NONE);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiStatusBar((Rectangle){ 0, (float)GetScreenHeight() - 20, (float)GetScreenWidth(), 20 }, "This is a status bar");
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    //GuiSetStyle(STATUSBAR, TEXT_INDENTATION, 20);

    if (showMessageBox)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
        int result = GuiMessageBox((Rectangle){ (float)GetScreenWidth()/2 - 125, (float)GetScreenHeight()/2 - 50, 250, 100 }, GuiIconText(ICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No");

        if ((result == 0) || (result == 2)) showMessageBox = false;
        else if (result == 1) exitWindow = true;
    }

    if (showTextInputBox)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
        int result = GuiTextInputBox((Rectangle){ (float)GetScreenWidth()/2 - 120, (float)GetScreenHeight()/2 - 60, 240, 140 }, GuiIconText(ICON_FILE_SAVE, "Save file as..."), "Introduce output file name:", "Ok;Cancel", textInput, 255, NULL);

        if (result == 1)
        {
            // TODO: Validate textInput value and save

            TextCopy(textInputFileName, textInput);
        }

        if ((result == 0) || (result == 1) || (result == 2))
        {
            showTextInputBox = false;
            TextCopy(textInput, "\0");
        }
    }

    EndDrawing();

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );
    colorPickerValue = RED;
    gw = (GameWorld) {
        .dummy = 0
    };

}

void destroyGameWorld( void ) {
    printf( "destroying game world...\n" );
}

void loadResources( void ) {
    printf( "loading resources...\n" );
}

void unloadResources( void ) {
    printf( "unloading resources...\n" );
}