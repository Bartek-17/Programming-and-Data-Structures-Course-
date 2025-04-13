#include "primlib.h"
#include <stdlib.h>
#include <stdio.h>


#define PEG_HEIGHT (gfx_screenHeight() / 4)
#if discCount <= 10
	#define DISC_HEIGHT (PEG_HEIGHT /  discCount / 2)
#else
	#define DISC_HEIGHT (PEG_HEIGHT / discCount)
	#endif



#define MIN_DISC_WIDTH (gfx_screenWidth() / pegCount / 10)
#define BASE_HEIGHT (gfx_screenHeight() / 50)
#define PEG_WIDTH (gfx_screenWidth() / 150)
#define PEG_HEIGHT (gfx_screenHeight() / 4)
#define ANIMATION_SPEED 10

// Global variables

#define discCount 5
#define pegCount 3
int pegs[pegCount][discCount];
float discWidths[discCount];

// game states
enum State {
    SELECT_SOURCE,
    SELECT_DESTINATION,
    VICTORY
};

enum State gameState = SELECT_SOURCE;
int sourcePeg = -1;



void initializeGameState() {
    float maxWidth = gfx_screenWidth() / (2 * pegCount);  // Disc width control
    for (int discNr = 0; discNr < discCount; discNr++) {
        discWidths[discNr] = MIN_DISC_WIDTH + (maxWidth - MIN_DISC_WIDTH) * discNr / (discCount);
    }

    // Place all discs on the first peg
    for (int discNr = 0; discNr < discCount; discNr++) {
        pegs[0][discNr] = discCount - discNr;
    }
}


int isLegalMove(int fromPeg, int toPeg) {
    // index of the top disc on the "fromPeg"
    int fromTopDiscIndex = -1;
    for (int discIndex = discCount - 1; discIndex >= 0; discIndex--) {
        if (pegs[fromPeg][discIndex] > 0) {
            fromTopDiscIndex = discIndex;
            break;
        }
    }

    // Find the index of the top disc on the "toPeg"
    int toTopDiscIndex = -1;
    for (int discIndex = discCount - 1; discIndex >= 0; discIndex--) {
        if (pegs[toPeg][discIndex] > 0) {
            toTopDiscIndex = discIndex;
            break;
        }
    }

    // Check if the destination peg is empty or the top disc on the destination peg is larger than the one being moved
    if (toTopDiscIndex == -1 || pegs[toPeg][toTopDiscIndex] > pegs[fromPeg][fromTopDiscIndex]) {
        return 1; // Move is legal
    }
    return 0; // Move is not legal
}



void drawPegsAndDiscs() {
    int pegSpacing = gfx_screenWidth() / (pegCount + 1);
    gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);

    for (int pegIndex = 0; pegIndex < pegCount; pegIndex++) {
        int pegX = (pegIndex + 1) * pegSpacing;

        // Draw peg
        gfx_filledRect(pegX - PEG_WIDTH / 2, gfx_screenHeight() - PEG_HEIGHT - BASE_HEIGHT, pegX + PEG_WIDTH / 2, gfx_screenHeight() - BASE_HEIGHT, YELLOW);

        // Draw base
        gfx_filledRect(pegX - pegSpacing / 3, gfx_screenHeight() - BASE_HEIGHT, pegX + pegSpacing / 3, gfx_screenHeight(), CYAN);

        for (int discIndex = 0; discIndex < discCount; discIndex++) {
            if (pegs[pegIndex][discIndex] > 0) {
                int discID = pegs[pegIndex][discIndex] - 1;
                int discWidth = discWidths[discID];
                int yTop = gfx_screenHeight() - BASE_HEIGHT - (discIndex + 1) * DISC_HEIGHT;
                gfx_filledRect(pegX - discWidth / 2, yTop, pegX + discWidth / 2, yTop + DISC_HEIGHT, RED);
            }
        }
    }
}



int countDiscs(int peg) {
    int count = 0;
    for (int discIndex = 0; discIndex < discCount; discIndex++) {
        if (pegs[peg][discIndex] > 0) {
            count++;
        } else {
            break;  // No more discs on peg
        }
    }
    return count;
}


void moveDisc(int fromPeg, int toPeg) {
    int discToMove = -1;
    
    if (isLegalMove(fromPeg, toPeg)) {
        // Find the top disc on the "fromPeg"
        for (int discNr = discCount - 1; discNr >= 0; discNr--) {
            if (pegs[fromPeg][discNr] > 0) {
                discToMove = pegs[fromPeg][discNr];
                pegs[fromPeg][discNr] = 0; // Remove the disc from the "fromPeg"
                break;
            }
        }
    } else {
        gfx_textout(gfx_screenWidth() / 3, gfx_screenHeight() / 2, " -------- ILLEGAL MOVE! PRESS RETURN TO CONTINUE --------", WHITE);
        gfx_updateScreen();
        
        while (!gfx_isKeyDown(SDLK_RETURN)) {
            SDL_Delay(30);
        }
        return;
    }
    
    if (discToMove == -1) {
        gfx_textout(gfx_screenWidth() / 3, gfx_screenHeight() / 2, " -------- NO DISCS TO MOVE! PRESS RETURN TO CONTINUE --------", WHITE);
        gfx_updateScreen();
        
        while (!gfx_isKeyDown(SDLK_RETURN)) {
            SDL_Delay(30);
        }
        return;
    }
    

    //  ------------------ Animation -----------------------------
    
    // Determine the initial and final positions of the disc
    int fromY = gfx_screenHeight() - BASE_HEIGHT - (countDiscs(fromPeg) + 1) * DISC_HEIGHT; // Initial y position
    int toY = gfx_screenHeight() - BASE_HEIGHT - (countDiscs(toPeg) + 1) * DISC_HEIGHT; // Final y position
    int fromX = (fromPeg + 1) * (gfx_screenWidth() / (pegCount + 1)); // Initial x position
    int toX = (toPeg + 1) * (gfx_screenWidth() / (pegCount + 1)); // Final x position
    
	int DiscMaxPos = gfx_screenHeight() - 3.5 * PEG_HEIGHT;

		
	// Upward movement
	for (int position = fromY; position > DiscMaxPos; position -= ANIMATION_SPEED) {
		drawPegsAndDiscs();
		SDL_Delay(10);
		gfx_filledRect(fromX - discWidths[discToMove - 1] / 2, position, fromX + discWidths[discToMove - 1] / 2, position + DISC_HEIGHT, RED);		
		gfx_updateScreen();
	}


	// Horizontal movement
	if (fromX < toX) {
		for (int posX = fromX; posX < toX; posX += ANIMATION_SPEED) {
			drawPegsAndDiscs();
			SDL_Delay(10);
			gfx_filledRect(posX - discWidths[discToMove - 1] / 2, DiscMaxPos, posX + discWidths[discToMove - 1] / 2, DiscMaxPos + DISC_HEIGHT, RED);
			gfx_updateScreen();

		}
	} else {
		for (int posX = fromX; posX > toX; posX -= ANIMATION_SPEED) {
			drawPegsAndDiscs();
			SDL_Delay(10);
			gfx_filledRect(posX - discWidths[discToMove - 1] / 2, DiscMaxPos, posX + discWidths[discToMove - 1] / 2, DiscMaxPos + DISC_HEIGHT, RED);
			gfx_updateScreen();
		}
	}


	// Downward movement
	for (int position = DiscMaxPos; position < toY + DISC_HEIGHT; position += ANIMATION_SPEED) {
		drawPegsAndDiscs();
		SDL_Delay(10);
		gfx_filledRect(toX - discWidths[discToMove - 1] / 2, position, toX + discWidths[discToMove - 1] / 2, position + DISC_HEIGHT, RED);
		gfx_updateScreen();
	}


    // Place disc on first free slot on the "toPeg"

	pegs[toPeg][countDiscs(toPeg)] = discToMove;
}



int checkVictory() {
    for (int disc_index = 0; disc_index < discCount; disc_index++) {
        if (pegs[pegCount - 1][disc_index] == 0) {
            return 0; // Not all discs are on the last peg
        }
    }
    return 1; // All discs are on the last peg
}

int main() {
    if (gfx_init()) {
        exit(3);
    }

    initializeGameState();

    int running = 1;

    while (running) {
        gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);

        // Event handling
        if (gfx_isKeyDown(SDLK_ESCAPE) == 1) {
            running = 0;
        }

        // Check for victory
        if (checkVictory()) {
            gameState = VICTORY;
            gfx_textout(gfx_screenWidth() / 3, gfx_screenHeight() / 2, " -------- VICTORY! PRESS ESCAPE TO EXIT --------", WHITE);
            gfx_updateScreen();
            while (!gfx_isKeyDown(SDLK_ESCAPE)) {
                SDL_Delay(30);
            }
            running = 0;
        }

       /*
      for (int peg = 0; peg < pegCount; peg++) {
            printf("peg nr %d:", peg);
            for (int disc = 0; disc < discCount; disc++) {
                printf("  %d ", pegs[peg][disc]);
            }
            printf("\n");
        }
        */

        // Handle game states
        if (gameState == SELECT_SOURCE) {
            // Check if any key (1-peg count) is pressed
            for (int peg = 0; peg < pegCount; peg++) {
				if (countDiscs(peg) == 0){
					continue;
				}
                if (gfx_isKeyDown(SDLK_1 + peg)) {

                    sourcePeg = peg;
                    gameState = SELECT_DESTINATION;
                    while (!gfx_isKeyDown(SDLK_RETURN)) {
                        SDL_Delay(30);
                    }
                }
            }
        } else if (gameState == SELECT_DESTINATION) {
            // Check if any number key (1-10) is pressed
            for (int peg = 0; peg < pegCount; peg++) {
                if (gfx_isKeyDown(SDLK_1 + peg)) {
                    if (peg != sourcePeg) { // destination peg != source peg
                        moveDisc(sourcePeg, peg);
                    }
                    gameState = SELECT_SOURCE;
			continue;
                }
            }
        }

        drawPegsAndDiscs();
        gfx_updateScreen();
        SDL_Delay(30);
    }
    return 0;
}
