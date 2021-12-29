#include <cstdlib>

void advance_feuerwerks(unsigned long delta);
void render();

class Feuerwerk
{
    private:
        bool start_phase = true;
        float position = 0;
        int hue_base;
        int hue_scatter;
        int speed;

    public:
        float age = 0;

        static const int START_PIXELS = 31;
        static const int HUE_SCATTER_MAX = 20;

        Feuerwerk()
        {
            hue_base = std::rand() % 360;
            hue_scatter = (2 * std::rand() % (2 * HUE_SCATTER_MAX)) - HUE_SCATTER_MAX;
            speed = std::rand() % 3 + 1;
            start_phase = true;
        }

        int get_hue()
        {
            return hue_base + age * hue_scatter;
        }

        void advance(float delta)
        {
            age += delta;
            position += speed * delta;

            if (start_phase) {

                if (position >= START_PIXELS) {
                    start_phase = false;
                    position = 0;
                }

            }
        }

        bool is_in_start_phase()
        {
            return start_phase;
        }

        bool get_position()
        {
            return position;
        }
};