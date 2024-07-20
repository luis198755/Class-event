// OLEDManager.h
#ifndef OLED_MANAGER_H
#define OLED_MANAGER_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "RTClib.h"
#include "EventManager.h" // Include this to use Event struct

class OLEDManager {
public:
    OLEDManager(uint8_t w = 128, uint8_t h = 64, int8_t rst_pin = -1);
    bool begin(uint8_t i2c_address = 0x3C);
    void displayTime(const DateTime& now);
    void displayEvent(const EventManager::Event& event);
    void clear();
    void showTVTurnOnEffect();

private:
    Adafruit_SSD1306 display;
    uint8_t width;
    uint8_t height;
    unsigned long eventDisplayStartTime;
    bool isDisplayingEvent;
    void drawTVNoise();
    void fadeOutEffect();
    void flickerEffect();
    void scanningLines();
    void geometricShapes();
    void textEmergence();
    void matrixEffect();
    void spiralEffect();
    void bounceEffect();
    void waveEffect();
    void starfieldEffect();
    void starfieldEffectV();
     // New fractal effect methods
    void mandelbrotEffect();
    void juliaEffect();
    void lSystemTreeEffect();

    void plasmaEffect();
    void particleSystemEffect();
    void gameOfLifeEffect();

    void vortexParticleEffect();
    // New rain effect method
    void rainEffect();

    void dynamicWaveEffect();
    void cyberGridEffect();

    void starfieldEffect2();

    void particleExplosionEffect();

    void marbleDropEffect();

    // New falling letters effect method
    void fallingLettersEffect();

    // New neural cell brain effect method
    void neuralCellBrainEffect();

    // Helper methods for fractal calculations
    bool mandelbrotCalculation(float x, float y, int maxIterations);
    bool juliaCalculation(float x, float y, float cx, float cy, int maxIterations);
    void drawLSystemTree(int x, int y, float angle, int depth); 

    // Helper methods for new effects
    uint8_t plasma(int x, int y, float time);
    void updateParticles();
    void updateGameOfLife();

    // Helper struct for particle properties
    struct Particle {
        float x, y;
        float vx, vy;
        float life;
    };

     // Helper struct for raindrop properties
    struct Raindrop {
        int x, y;
        int speed;
        int size;
    };

    // Helper struct for ripple properties
    struct Ripple {
        int x, y;
        int size;
        int life;
    };

     // Helper method for wave calculation
    float calculateWave(float x, float time, float frequency, float amplitude);

    // Helper struct for energy line properties
    struct EnergyLine {
        int x, y;
        int length;
        int speed;
        bool vertical;
    };

    void drawStar(int x, int y, int brightness);
    int stars[100][3]; // Array to hold star positions and brightness

    void drawParticle(int x, int y);
    struct Particle2 {
        int x, y;
        float vx, vy;
        int lifetime;
    };
    Particle2 particles2[100]; // Array to hold particle data

    // Helper struct for marble properties
    struct Marble {
        float x, y;    // Position
        float vy;      // Vertical velocity
        float radius;  // Marble radius
        bool active;   // Whether the marble is still bouncing
    };

    // Helper method for updating marble physics
    void updateMarble(Marble& marble, float dt);

    // Helper struct for neuron properties
    struct Neuron {
        float x, y;
        float activityLevel;
        std::vector<int> connections;
    };

    // Helper methods for neural network simulation
    void updateNeurons(std::vector<Neuron>& neurons);
    void drawNeurons(const std::vector<Neuron>& neurons);

    // Updated Helper struct for letter properties
    struct FallingLetter {
        char letter;
        float x, y;
        float vy;
        bool settled;
        float targetY;  // Added targetY member
    };

    // Helper methods for falling letters effect
    void updateLetter(FallingLetter& letter);
    void drawLetters(const std::vector<FallingLetter>& letters);
    std::string getRandomMessage();
};

#endif