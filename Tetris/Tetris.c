#include "primlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "pieces.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define DELAY 10
#define SPAWN_SPACE 3
#define ARRAY_DIMENSION 4
#define POSSIBLE_ROTATIONS 4
#define POSSIBLE_TYPES 7

#if FIELD_WIDTH > FIELD_HEIGHT
    #define BLOCK_SIZE gfx_screenWidth() / (FIELD_WIDTH * 1.3)
#elif FIELD_WIDTH <= FIELD_HEIGHT
    #define BLOCK_SIZE gfx_screenHeight() / (FIELD_HEIGHT * 1.3)
#endif

// automatic variables
#define SCREEN_WIDTH gfx_screenWidth()
#define SCREEN_HEIGHT gfx_screenHeight()
#define FIELD_OFFSET_X ((SCREEN_WIDTH - FIELD_WIDTH * BLOCK_SIZE) / 2)
#define FIELD_OFFSET_Y ((SCREEN_HEIGHT - FIELD_HEIGHT * BLOCK_SIZE) / 2)

int gameField[FIELD_HEIGHT+SPAWN_SPACE][FIELD_WIDTH] = {0};
int layersCompleted = 0;

int current_piece;
int rotation;
int nextPiece;
int nextRotation;
int pieceX;
int pieceY;

// Movement flags
int moveLeft = 0;
int moveRight = 0;
int rotateFlag = 0;
int gameOver = 0;

void initGame();
void spawnNewPiece();
void drawGameBorder();
void drawGameField();
void drawPiece();
void handleInput();
void updateGame();
int canMovePiece(int dx, int dy);
void rotatePiece();
int canRotate(int newRotation, int anchorDiffX, int anchorDiffY);
void lockPiece();
void checkLayers();
void clearLayer(int row);
void drawScore();
void drawGameOverScreen();

void initGame() {
    if (gfx_init()) {
        exit(3);
    }
    srand(time(NULL));
    spawnNewPiece();
}

void spawnNewPiece() {
    // Exchange current piece with a new one
    current_piece = nextPiece;
    rotation = nextRotation;

    // Generate new piece and rotation
    nextPiece = rand() % POSSIBLE_TYPES;
    nextRotation = rand() % POSSIBLE_ROTATIONS;

    pieceX = FIELD_WIDTH / 2 - 1;
    pieceY = 0;
}

void drawGameBorder() {
    int border_thickness = 5;
    int left_x = FIELD_OFFSET_X - border_thickness;
    int top_y = FIELD_OFFSET_Y - border_thickness;
    int right_x = left_x + FIELD_WIDTH * BLOCK_SIZE + 2 * border_thickness;
    int bottom_y = top_y + FIELD_HEIGHT * BLOCK_SIZE + 2 * border_thickness;

    gfx_filledRect(left_x, top_y, right_x, top_y + border_thickness, YELLOW); // Top border
    gfx_filledRect(left_x, bottom_y - border_thickness, right_x, bottom_y, YELLOW); // Bottom border
    gfx_filledRect(left_x, top_y, left_x + border_thickness, bottom_y, YELLOW); // Left border
    gfx_filledRect(right_x - border_thickness, top_y, right_x, bottom_y, YELLOW); // Right border
}

void drawGameField() {
    for (int row = SPAWN_SPACE; row < FIELD_HEIGHT; row++) {
        for (int column = 0; column < FIELD_WIDTH; column++) {
            int draw_x = FIELD_OFFSET_X + column * BLOCK_SIZE;
            int draw_y = FIELD_OFFSET_Y + row * BLOCK_SIZE;
            if (gameField[row][column] != 0) {
                gfx_filledRect(draw_x, draw_y, draw_x + BLOCK_SIZE, draw_y + BLOCK_SIZE, MAGENTA);
            }
            gfx_line(draw_x, draw_y, draw_x + BLOCK_SIZE, draw_y, BLUE);
            gfx_line(draw_x, draw_y, draw_x, draw_y + BLOCK_SIZE, BLUE);
        }
    }
}

void drawPiece() {
    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            int value = pieces[current_piece][rotation][row][column];
            if (value != 0) {
                int draw_x = FIELD_OFFSET_X + (pieceX + column) * BLOCK_SIZE;
                int draw_y = FIELD_OFFSET_Y + (pieceY + row) * BLOCK_SIZE;
                gfx_filledRect(draw_x, draw_y, draw_x + BLOCK_SIZE, draw_y + BLOCK_SIZE, value == 1 ? WHITE : CYAN);
            }
        }
    }
}

void handleInput() {
    // Checks for input and sets flags
    if (gfx_isKeyDown(SDLK_LEFT) && !moveLeft && canMovePiece(-1, 0)) {
        moveLeft = 1;
        pieceX--;
    } else if (!gfx_isKeyDown(SDLK_LEFT)) {
        moveLeft = 0;
    }

    if (gfx_isKeyDown(SDLK_RIGHT) && !moveRight && canMovePiece(1, 0)) {
        moveRight = 1;
        pieceX++;
    } else if (!gfx_isKeyDown(SDLK_RIGHT)) {
        moveRight = 0;
    }

    if (gfx_isKeyDown(SDLK_SPACE) && !rotateFlag) {
        rotateFlag = 1;
        rotatePiece();
    } else if (!gfx_isKeyDown(SDLK_SPACE)) {
        rotateFlag = 0;
    }

    if (gfx_isKeyDown(SDLK_DOWN) && canMovePiece(0, 1)) {
        pieceY ++;
    }
}


void updateGame() {
    if (canMovePiece(0, 1)) {
        pieceY++;
    } else {
        lockPiece();
        spawnNewPiece();
        checkLayers();

        // Check if there's space to spawn a new piece
        int spawn = 3;
        int canPlaceNewPiece = 1;
        for (int column = 0; column < FIELD_WIDTH; column++) {
            if (gameField[spawn][column] != 0) {
                canPlaceNewPiece = 0;
                break;
            }
        }
        
        if (!canPlaceNewPiece) {
            gameOver = 1;
        }
    }
}


int canMovePiece(int dx, int dy) {
    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            if (pieces[current_piece][rotation][row][column] != 0) {
                int new_x = pieceX + column + dx;
                int new_y = pieceY + row + dy;
                // Check boundaries and collisions with other pieces
                if (new_x < 0 || new_x >= FIELD_WIDTH || new_y >= FIELD_HEIGHT || (new_y >= 0 && gameField[new_y][new_x] != 0)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void rotatePiece() {

    int anchorX = 99;
    int anchorY = 99;

    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            if (pieces[current_piece][rotation][row][column] == 2) {
                anchorX = column;
                anchorY = row;
                break;
            }
        }
    }

    int newRotation = (rotation + 1) % POSSIBLE_ROTATIONS;

    int anchorXnew = 99;
    int anchorYnew = 99;

    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            if (pieces[current_piece][newRotation][row][column] == 2) {
                anchorXnew = column;
                anchorYnew = row;
                break;
            }
        }
    }


    int anchorDiffX = anchorXnew - anchorX;
    int anchorDiffY = anchorYnew - anchorY;

    
    if (canRotate(newRotation, anchorDiffX, anchorDiffY) == 1){
    pieceX -= anchorDiffX;
    pieceY -= anchorDiffY;
    rotation = newRotation;
    }
}


int canRotate(int newRotation, int anchorDiffX, int anchorDiffY) {

    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            if (pieces[current_piece][newRotation][row][column] != 0) {
                int new_x = pieceX + column - anchorDiffX;
                int new_y = pieceY + row - anchorDiffY;
                // Check boundaries and collisions with other pieces
                if (new_x < 0 || new_x >= FIELD_WIDTH || new_y >= FIELD_HEIGHT || (new_y >= 0 && gameField[new_y][new_x] != 0)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}


void lockPiece() {
    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            if (pieces[current_piece][rotation][row][column] != 0) {
                int field_x = pieceX + column;
                int field_y = pieceY + row;
                if (field_y >= 0) {
                    gameField[field_y][field_x] = pieces[current_piece][rotation][row][column];
                }
            }
        }
    }
}

void checkLayers() {
    // Check for completed layers and clear them
    int completeCount = 0;

    for (int row = FIELD_HEIGHT - 1; row >= 0; row--) {
        int rowComplete = 1;
        for (int column = 0; column < FIELD_WIDTH; column++) {
            if (gameField[row][column] == 0) {
                rowComplete = 0;
                break;
            }
        }
        if (rowComplete) {
            clearLayer(row);
            completeCount++;
            row++; // Re-check the same row after clearing
        }
    }

    if (completeCount > 0) {
        layersCompleted += completeCount;
    }
}

void clearLayer(int row) {
    // Clear the full row by shifting all rows above it down
    for (int Y = row; Y > 0; Y--) {
        for (int X = 0; X < FIELD_WIDTH; X++) {
            gameField[Y][X] = gameField[Y - 1][X];
        }
    }
    // Clear the top row
    for (int X = 0; X < FIELD_WIDTH; X++) {
        gameField[0][X] = 0;
    }
}


void drawScore() {
    char scoreText[20];
    snprintf(scoreText, sizeof(scoreText), "SCORE: %d", layersCompleted);

    gfx_textout(30, 30, scoreText, GREEN);
}

void drawGameOverScreen() {
    gfx_filledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    gfx_textout(SCREEN_WIDTH / 2.3, SCREEN_HEIGHT / 2 - 20, "GAME OVER", RED);

    char scoreText[20];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", layersCompleted);
    gfx_textout(SCREEN_WIDTH / 2.3, SCREEN_HEIGHT / 2, scoreText, GREEN);
}


void drawNextPiece() {
    // position of the next piece preview
    int previewOffsetX = FIELD_OFFSET_X + (FIELD_WIDTH + 2) * BLOCK_SIZE;
    int previewOffsetY = FIELD_OFFSET_Y + 2 * BLOCK_SIZE;

    for (int row = 0; row < ARRAY_DIMENSION; row++) {
        for (int column = 0; column < ARRAY_DIMENSION; column++) {
            int value = pieces[nextPiece][nextRotation][row][column];
            if (value != 0) {
                int draw_x = previewOffsetX + column * BLOCK_SIZE;
                int draw_y = previewOffsetY + row * BLOCK_SIZE;
                gfx_filledRect(draw_x, draw_y, draw_x + BLOCK_SIZE, draw_y + BLOCK_SIZE, value == 1 ? WHITE : CYAN);
            }
        }
    }
}


int main(int argc, char* argv[]) {
    initGame();
    int fallTimer = 0;

    while (!gfx_isKeyDown(SDLK_ESCAPE)) {
        if (!gameOver) {
            gfx_filledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            drawGameField();
            drawGameBorder();
            drawPiece();
            drawScore();
            drawNextPiece();
            handleInput();

            if (fallTimer == 10) {
                updateGame();
            }

            fallTimer++;
            fallTimer = fallTimer % 20;
        }
        else {
            drawGameOverScreen(); 
            gfx_updateScreen();
            while (!gfx_isKeyDown(SDLK_ESCAPE)){
                SDL_Delay(DELAY);
            }
            break;
        }

        gfx_updateScreen();
        SDL_Delay(DELAY);
    }

    return 0;
}
