#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include <Arduino.h>

void logf(const char *f, ...) {
    char buffer[256];
    va_list args;
    va_start(args, f);
    vsnprintf(buffer, sizeof(buffer), f, args);
    Serial.println(buffer);
    va_end(args);
}

extern "C" char *sbrk(int32_t i);

constexpr uint32_t RamBaseAddr = 0x20000000;

constexpr byte Pattern[] = { 0xc5, 0xaa, 0x55, 0x0f };

size_t stackpainter_fill() {
    size_t painted = 0;
    byte *bottom;
    byte *top = nullptr;
    byte *dummy = nullptr;

    top = (byte *)&dummy;
    bottom = (byte *)sbrk(0);

    while (bottom + 4 < top) {
        bottom[0] = Pattern[0];
        bottom[1] = Pattern[1];
        bottom[2] = Pattern[2];
        bottom[3] = Pattern[3];
        bottom += 4;
        painted += 4;
    }

    return painted;
}

size_t stackpainter_find_unused() {
    size_t upwards = 0;
    size_t downwards = 0;
    byte *p, *bottom, *top;
    byte *dummy = nullptr;

    top = (byte *)&dummy;
    bottom = (byte *)sbrk(0);

    p = bottom;

    while (p + 4 < top) {
        if (p[0] == Pattern[0] &&
            p[1] == Pattern[1] &&
            p[2] == Pattern[2] &&
            p[3] == Pattern[3]) {
            break;
        }
        ++p;
    }

    while (p + 4 < top) {
        if (p[0] == Pattern[0] &&
            p[1] == Pattern[1] &&
            p[2] == Pattern[2] &&
            p[3] == Pattern[3]) {
            p += 4;
            upwards += 4;
        }
        else {
            break;
        }
    }

    p = top;

    while (p >= bottom) {
        if (p[0] == Pattern[0] &&
            p[1] == Pattern[1] &&
            p[2] == Pattern[2] &&
            p[3] == Pattern[3]) {
            break;
        }
        --p;
    }

    while (p >= bottom) {
        if (p[0] == Pattern[0] &&
            p[1] == Pattern[1] &&
            p[2] == Pattern[2] &&
            p[3] == Pattern[3]) {
            p -= 4;
            downwards += 4;
        }
        else {
            break;
        }
    }

    if (upwards != downwards) {
        logf("Up/Down %d %d", upwards, downwards);
    }

    return max(upwards, downwards);
}

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(100);
    }

    size_t filled = stackpainter_fill();

    size_t unused = stackpainter_find_unused();

    logf("Filled: %d Unused: %d", filled, unused);

    while (1) {
        delay(1000);
        logf("Hi");
    }

}

void loop() {
}

