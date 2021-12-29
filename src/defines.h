
#define LIMIT_BRIGHTNESS 70                   // maximum brightness in %
#define WHITE_SUPPRESSION 5                   // power to favor high saturations

// STORING ADDRESSES
#define MAX_STORE_SIZE 20
#define STORE_PATTERN 0
#define STORE_BRIGHTNESS 4
#define STORE_HUE 8
#define STORE_WHITE 12
#define STORE_ON_NEXT_BOOT 16

// for all dynamic pattern-specific variables (cf. somewhere in main program)
#define SLOTS 3

void initVars();
void handleFrames(float time, float delta);
void showStrip(bool visible);
float shiftWhite(float white);
void initCounters();
void handleInput();
void controlGlobalBrightness(char);
void controlGlobalHue(char);
void controlGlobalWhite(char);