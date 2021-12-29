#include "FastLED.h"
#include "LED.h"
#include "feuerwerk.h"

#define NEOPIXEL_PIN1 12
#define NEOPIXEL_PIN2 13
#define NUM_LEDS1 200
#define NUM_LEDS2 200
#define NUM_LEDS (NUM_LEDS1+NUM_LEDS2)
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
LED values[NUM_LEDS];

unsigned long runtime = 0;

std::vector<Feuerwerk> feuerwerks {};
const long MAX_AGE = 30;
const float DIM_FACTOR = 0.9;
const float GLOBAL_BRIGHTNESS = 1;

void setup()
{
    FastLED.addLeds<NEOPIXEL, NEOPIXEL_PIN1>(leds1, NUM_LEDS1);
    FastLED.addLeds<NEOPIXEL, NEOPIXEL_PIN2>(leds2, NUM_LEDS2);

    runtime = millis();
}

void loop()
{
    const unsigned long now = millis();

    advance_feuerwerks(now - runtime);
    render();

    delay(100); // TODO: remove
    FastLED.show();
    runtime = now;
}

void advance_feuerwerks(unsigned long delta)
{
    for (auto it = std::begin(feuerwerks); it != std::end(feuerwerks); ++it)
    {
        it->advance(delta);

        if (it->age > MAX_AGE)
        {
            it = feuerwerks.erase(it);
            --it; // is this right?
        }
    }
}

const int STRIPE1_START = 0;
const int STRIPE2_START = 50; // TO BE FOUND OUT; this stripe is at the end
const int START_STRIPE_LENGTH = 31;

std::pair<int, int> get_indices_for_start(float position)
{

    int pos1 = STRIPE1_START + (int)position;
    int pos2 = STRIPE2_START + START_STRIPE_LENGTH - (int)position;

    return std::pair<int, int>{pos1, pos2};
}

void render_start_phase(Feuerwerk feuerwerk)
{
    auto indices = get_indices_for_start(feuerwerk.get_position());
    values[indices.first] = LED(1, 1);
    values[indices.second] = LED(1, 1);
}

std::pair<std::vector<int>, std::vector<int>> flash_pixels {
    {0, 15, 45, 58, 89, 104, 137, 148},
    {0, 15, 45, 58, 89, 104, 137}
};

void render_flash_phase(Feuerwerk feuerwerk)
{
    // for now: use every existing stretch;
    for(int route = 0; route < 2; route++)
    {
        const std::vector<int> pixels = route == 0 ? flash_pixels.first : flash_pixels.second;
        const int start_offset = route == 0 ? STRIPE1_START : STRIPE2_START;

        for (int stretch = 0; stretch < pixels.size(); stretch++)
        {
            // here: could limit the amount of stretch per feuerwerk
            // could also distinguish between thin (3cm distance) and dense (1.67cm distance) strips

            const int index = start_offset + pixels[stretch] + (int)feuerwerk.get_position();
            if (stretch < pixels.size() - 1 && index > pixels[stretch + 1])
            {
                continue;
            }

            values[index] = LED(feuerwerk.get_hue(), 0, 1);
        }
    }
}

void render()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        values[i].dim(DIM_FACTOR);
    }

    for (auto it = std::begin(feuerwerks); it != std::end(feuerwerks); ++it)
    {
        auto render_phase = it->is_in_start_phase() ? render_start_phase : render_flash_phase;
        render_phase(*it);
    }

    for (int i=0; i < NUM_LEDS; i++)
    {
        auto result = CRGB(
            GLOBAL_BRIGHTNESS * values[i].getR(),
            GLOBAL_BRIGHTNESS * values[i].getG(),
            GLOBAL_BRIGHTNESS * values[i].getB()
        );

        auto is_first_stripe = i < NUM_LEDS1;
        auto leds = is_first_stripe ? leds1 : leds2;
        auto index = is_first_stripe ? i : i - NUM_LEDS1;
        leds[index] = result;
    }

}