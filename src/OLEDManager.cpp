// OLEDManager.cpp
#include "OLEDManager.h"
#include <math.h>

OLEDManager::OLEDManager(uint8_t w, uint8_t h, int8_t rst_pin)
    : display(w, h, &Wire, rst_pin), width(w), height(h), eventDisplayStartTime(0), isDisplayingEvent(false) {}

bool OLEDManager::begin(uint8_t i2c_address) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2c_address)) {
        return false;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    return true;
}

void OLEDManager::displayTime(const DateTime& now) {
    const unsigned long EVENT_DISPLAY_DURATION = 5000; // 5 seconds in milliseconds
    
    // Check if we're still within the event display duration
    if (isDisplayingEvent && (millis() - eventDisplayStartTime < EVENT_DISPLAY_DURATION)) {
        return; // Continue displaying the event
    }

    // Event display time has elapsed, show the current time
    isDisplayingEvent = false;
    
    display.clearDisplay();

    // Display date (YYYY-MM-DD)
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.printf("%04d-%02d-%02d", now.year(), now.month(), now.day());
    
    // Display time (HH:MM:SS)
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    
    // Update the display
    display.display();
}

void OLEDManager::displayEvent(const EventManager::Event& event) {
    display.clearDisplay();
    display.setTextSize(1);
    
    display.setCursor(0, 0);
    display.println("Event Triggered!");
    
    display.setCursor(0, 16);
    display.printf("Scenario: %d", event.scenario);
    
    display.setCursor(0, 26);
    display.printf("Cycle: %d", event.cycle);
    
    display.setCursor(0, 36);
    display.println(event.description);
    
    display.display();
    
    isDisplayingEvent = true;
    eventDisplayStartTime = millis();
}

void OLEDManager::clear() {
    display.clearDisplay();
    display.display();
}

void OLEDManager::showTVTurnOnEffect() {
    flickerEffect();
    // Simulate TV turn on effect
    for (int i = 0; i < 3; i++) {
        drawTVNoise();
        delay(100);
        clear();
        delay(50);
    }

    scanningLines();

    clear();
    
    // Simulate horizontal line sweep
    for (int y = 0; y < height; y += 2) {
        display.drawFastHLine(0, y, width, SSD1306_WHITE);
        display.display();
        delay(10);
    }
    
    delay(200);
    //clear();

    textEmergence();

    //matrixEffect();

    //spiralEffect();
    //bounceEffect();
    //waveEffect();
    //starfieldEffect();
    starfieldEffectV();

    //mandelbrotEffect();
    //juliaEffect();
    //lSystemTreeEffect();

    //plasmaEffect();
    particleSystemEffect();
    //gameOfLifeEffect();

    vortexParticleEffect();

    //particleExplosionEffect();


    //starfieldEffect2();

    //marbleDropEffect();

    fallingLettersEffect();

    //rainEffect();

    //dynamicWaveEffect();

    //cyberGridEffect();
    
    // Show dynamic noise for 3 seconds
    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
        drawTVNoise();
        delay(50);  // Adjust this value to control the speed of noise changes
    }

    // Apply fade-out effect
    fadeOutEffect();
}

void OLEDManager::drawTVNoise() {
    display.clearDisplay();  // Clear before drawing new noise
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (random(2) == 1) {
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        }
    }
    display.display();
}

void OLEDManager::fadeOutEffect() {
    uint8_t* buffer = display.getBuffer();
    int bufferSize = width * height / 8;
    
    for (int step = 0; step < 16; step++) {
        for (int i = 0; i < bufferSize; i++) {
            buffer[i] = buffer[i] & random(0xFF);  // Randomly clear bits
        }
        display.display();
        delay(50);  // Adjust for faster/slower fade
    }
    
    // Ensure the display is completely clear at the end
    display.clearDisplay();
    display.display();
}

void OLEDManager::flickerEffect() {
    clear();
    for (int i = 0; i < 10; i++) {
        display.invertDisplay(true);
        delay(50);
        display.invertDisplay(false);
        delay(30);
    }
}

void OLEDManager::scanningLines() {
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < height; y += 4) {
            display.clearDisplay();
            display.drawFastHLine(0, y, width, SSD1306_WHITE);
            display.display();
            delay(10);
        }
    }
}

void OLEDManager::geometricShapes() {
    for (int i = 0; i < 30; i++) {
        display.clearDisplay();
        int x = random(width);
        int y = random(height);
        int size = random(10, 30);
        
        switch (random(3)) {
            case 0:
                display.drawCircle(x, y, size/2, SSD1306_WHITE);
                break;
            case 1:
                display.drawRect(x, y, size, size, SSD1306_WHITE);
                break;
            case 2:
                display.drawTriangle(x, y, x + size, y, x + size/2, y - size, SSD1306_WHITE);
                break;
        }
        display.display();
        delay(100);
    }
}

void OLEDManager::textEmergence() {
    const char* bootText = "BOOTING...";
    int16_t x1, y1;
    uint16_t w, h;

    // Set text properties
    display.setTextSize(2);
    display.setTextColor(SSD1306_BLACK);  // Set text color to black

    // Calculate text bounds
    display.getTextBounds(bootText, 0, 0, &x1, &y1, &w, &h);
    int x = (width - w) / 2;
    int y = (height - h) / 2;

    for (int i = 0; bootText[i] != '\0'; i++) {
        // Fill the entire screen with white
        display.fillScreen(SSD1306_WHITE);
        
        // Set cursor and print the text
        display.setCursor(x, y);
        for (int j = 0; j <= i; j++) {
            display.print(bootText[j]);
        }
        
        display.display();
        delay(200);
    }
    
    // Keep the final text on screen for a moment
    delay(1000);
    display.setTextColor(SSD1306_WHITE);  // Set text color to black
}

void OLEDManager::matrixEffect() {
    const int numDrops = 10;
    int drops[numDrops];
    for (int i = 0; i < numDrops; i++) {
        drops[i] = random(-20, 0);
    }

    for (int frame = 0; frame < 100; frame++) {
        display.clearDisplay();
        for (int i = 0; i < numDrops; i++) {
            display.drawLine(i * (width / numDrops), drops[i], i * (width / numDrops), drops[i] + 20, SSD1306_WHITE);
            drops[i]++;
            if (drops[i] > height) {
                drops[i] = random(-20, 0);
            }
        }
        display.display();
        delay(50);
    }
}

void OLEDManager::spiralEffect() {
    int centerX = width / 2;
    int centerY = height / 2;
    float angle = 0;
    float radius = 0;

    for (int i = 0; i < 100; i++) {
        int x = centerX + cos(angle) * radius;
        int y = centerY + sin(angle) * radius;
        display.drawPixel(x, y, SSD1306_WHITE);
        display.display();
        angle += 0.5;
        radius += 0.2;
        if (radius > width / 2) {
            radius = 0;
            display.clearDisplay();
        }
        delay(20);
    }
}

void OLEDManager::bounceEffect() {
    int x = random(width);
    int y = random(height);
    int dx = 2;
    int dy = 2;
    int size = 5;

    for (int i = 0; i < 200; i++) {
        display.clearDisplay();
        display.fillRect(x, y, size, size, SSD1306_WHITE);
        display.display();

        x += dx;
        y += dy;

        if (x <= 0 || x >= width - size) dx = -dx;
        if (y <= 0 || y >= height - size) dy = -dy;

        delay(20);
    }
}

void OLEDManager::waveEffect() {
    for (int frame = 0; frame < 100; frame++) {
        display.clearDisplay();
        for (int x = 0; x < width; x++) {
            int y = height / 2 + sin((x + frame) * 0.2) * 20;
            display.drawPixel(x, y, SSD1306_WHITE);
        }
        display.display();
        delay(50);
    }
}

void OLEDManager::starfieldEffect() {
    const int numStars = 50;
    int starX[numStars], starY[numStars], starSpeed[numStars];

    for (int i = 0; i < numStars; i++) {
        starX[i] = random(width);
        starY[i] = random(height);
        starSpeed[i] = random(1, 5);
    }

    for (int frame = 0; frame < 200; frame++) {
        display.clearDisplay();
        for (int i = 0; i < numStars; i++) {
            display.drawPixel(starX[i], starY[i], SSD1306_WHITE);
            starX[i] -= starSpeed[i];
            if (starX[i] < 0) {
                starX[i] = width;
                starY[i] = random(height);
                starSpeed[i] = random(1, 5);
            }
        }
        display.display();
        delay(20);
    }
}

void OLEDManager::starfieldEffectV() {
    const int numStars = 50;
    int starX[numStars], starY[numStars], starSpeed[numStars];

    // Initialize stars
    for (int i = 0; i < numStars; i++) {
        starX[i] = random(width);
        starY[i] = random(-height, 0);  // Start stars above the screen
        starSpeed[i] = random(1, 5);
    }

    for (int frame = 0; frame < 200; frame++) {
        display.clearDisplay();
        
        for (int i = 0; i < numStars; i++) {
            // Draw star
            display.drawPixel(starX[i], starY[i], SSD1306_WHITE);
            
            // Move star downwards
            starY[i] += starSpeed[i];
            
            // If star has moved off screen, reset it to the top
            if (starY[i] >= height) {
                starY[i] = 0;
                starX[i] = random(width);
                starSpeed[i] = random(1, 5);
            }
        }
        
        display.display();
        delay(20);
    }
}

void OLEDManager::mandelbrotEffect() {
    const float xMin = -2.0, xMax = 1.0, yMin = -1.0, yMax = 1.0;
    const int maxIterations = 30;

    for (int px = 0; px < width; px++) {
        for (int py = 0; py < height; py++) {
            float x = xMin + (xMax - xMin) * px / width;
            float y = yMin + (yMax - yMin) * py / height;
            
            if (mandelbrotCalculation(x, y, maxIterations)) {
                display.drawPixel(px, py, SSD1306_WHITE);
            }
        }
        
        if (px % 4 == 0) {  // Update display every 4 columns for smoother drawing
            display.display();
        }
    }
    display.display();
    delay(3000);  // Display the final image for 3 seconds
}

bool OLEDManager::mandelbrotCalculation(float x0, float y0, int maxIterations) {
    float x = 0, y = 0;
    int iteration = 0;
    while (x*x + y*y <= 4 && iteration < maxIterations) {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
    }
    return iteration < maxIterations;
}

void OLEDManager::juliaEffect() {
    const float xMin = -1.5, xMax = 1.5, yMin = -1.5, yMax = 1.5;
    const int maxIterations = 30;
    const float cx = -0.7, cy = 0.27;  // Julia set parameters

    for (int px = 0; px < width; px++) {
        for (int py = 0; py < height; py++) {
            float x = xMin + (xMax - xMin) * px / width;
            float y = yMin + (yMax - yMin) * py / height;
            
            if (juliaCalculation(x, y, cx, cy, maxIterations)) {
                display.drawPixel(px, py, SSD1306_WHITE);
            }
        }
        
        if (px % 4 == 0) {
            display.display();
        }
    }
    display.display();
    delay(3000);
}

bool OLEDManager::juliaCalculation(float x, float y, float cx, float cy, int maxIterations) {
    int iteration = 0;
    while (x*x + y*y <= 4 && iteration < maxIterations) {
        float xtemp = x*x - y*y + cx;
        y = 2*x*y + cy;
        x = xtemp;
        iteration++;
    }
    return iteration < maxIterations;
}

void OLEDManager::lSystemTreeEffect() {
    display.clearDisplay();
    drawLSystemTree(width / 2, height - 1, -M_PI / 2, 7);
    display.display();
    delay(3000);
}

void OLEDManager::drawLSystemTree(int x, int y, float angle, int depth) {
    if (depth == 0) return;

    int x2 = x + (int)(cos(angle) * depth * 3.5);
    int y2 = y + (int)(sin(angle) * depth * 3.5);

    display.drawLine(x, y, x2, y2, SSD1306_WHITE);

    drawLSystemTree(x2, y2, angle - M_PI / 5, depth - 1);
    drawLSystemTree(x2, y2, angle + M_PI / 5, depth - 1);
}

void OLEDManager::plasmaEffect() {
    float time = 0;
    for (int frame = 0; frame < 200; frame++) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                uint8_t color = plasma(x, y, time);
                display.drawPixel(x, y, color > 127 ? SSD1306_WHITE : SSD1306_BLACK);
            }
        }
        display.display();
        time += 0.1;
        delay(20);
    }
}

uint8_t OLEDManager::plasma(int x, int y, float time) {
    float v1 = sin(x * 10.0 / width + time);
    float v2 = sin(10.0 * (x * sin(time / 2) + y * cos(time / 3)) / width);
    float cx = x + 0.5 * sin(time / 5.0) * width;
    float cy = y + 0.5 * cos(time / 3.0) * height;
    float v3 = sin(sqrt((cx - width / 2) * (cx - width / 2) + 
                        (cy - height / 2) * (cy - height / 2)) / 8.0);
    return (uint8_t)((v1 + v2 + v3 + 3) * 64);
}

void OLEDManager::particleSystemEffect() {
    const int numParticles = 50;
    float particles[numParticles][4];  // x, y, dx, dy

    // Initialize particles
    for (int i = 0; i < numParticles; i++) {
        particles[i][0] = random(width);   // x
        particles[i][1] = random(height);  // y
        particles[i][2] = random(-10, 10) / 10.0;  // dx
        particles[i][3] = random(-10, 10) / 10.0;  // dy
    }

    for (int frame = 0; frame < 300; frame++) {
        display.clearDisplay();

        // Update and draw particles
        for (int i = 0; i < numParticles; i++) {
            // Update position
            particles[i][0] += particles[i][2];
            particles[i][1] += particles[i][3];

            // Bounce off edges
            if (particles[i][0] < 0 || particles[i][0] >= width) particles[i][2] *= -1;
            if (particles[i][1] < 0 || particles[i][1] >= height) particles[i][3] *= -1;

            // Draw particle
            display.drawPixel(particles[i][0], particles[i][1], SSD1306_WHITE);
        }

        display.display();
        delay(20);
    }
}

void OLEDManager::gameOfLifeEffect() {
    bool grid[width][height] = {0};
    bool newGrid[width][height] = {0};

    // Initialize random cells
    for (int i = 0; i < width * height / 4; i++) {
        grid[random(width)][random(height)] = true;
    }

    for (int generation = 0; generation < 100; generation++) {
        display.clearDisplay();

        // Draw current generation
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (grid[x][y]) {
                    display.drawPixel(x, y, SSD1306_WHITE);
                }
            }
        }

        display.display();
        delay(100);

        // Compute next generation
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int neighbors = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = (x + dx + width) % width;
                        int ny = (y + dy + height) % height;
                        if (grid[nx][ny]) neighbors++;
                    }
                }

                if (grid[x][y]) {
                    newGrid[x][y] = (neighbors == 2 || neighbors == 3);
                } else {
                    newGrid[x][y] = (neighbors == 3);
                }
            }
        }

        // Copy new grid to current grid
        memcpy(grid, newGrid, sizeof(grid));
    }
}

void OLEDManager::vortexParticleEffect() {
    const int NUM_PARTICLES = 100;
    Particle particles[NUM_PARTICLES];
    
    // Initialize particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].x = random(width);
        particles[i].y = random(height);
        particles[i].vx = 0;
        particles[i].vy = 0;
        particles[i].life = random(50, 200) / 100.0;
    }
    
    float centerX = width / 2.0;
    float centerY = height / 2.0;
    float time = 0;
    
    for (int frame = 0; frame < 300; frame++) {
        display.clearDisplay();
        
        for (int i = 0; i < NUM_PARTICLES; i++) {
            Particle& p = particles[i];
            
            // Calculate direction to center
            float dx = centerX - p.x;
            float dy = centerY - p.y;
            float distance = sqrt(dx*dx + dy*dy);
            
            // Normalize and apply force towards center
            float forceX = dx / distance * 0.1;
            float forceY = dy / distance * 0.1;
            
            // Apply swirl effect
            float swirl = sin(time * 0.1) * 0.05;
            float swirlX = -dy * swirl;
            float swirlY = dx * swirl;
            
            // Update velocity
            p.vx += forceX + swirlX;
            p.vy += forceY + swirlY;
            
            // Apply drag
            p.vx *= 0.95;
            p.vy *= 0.95;
            
            // Update position
            p.x += p.vx;
            p.y += p.vy;
            
            // Decrease life and reset if necessary
            p.life -= 0.01;
            if (p.life <= 0 || p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) {
                p.x = random(width);
                p.y = random(height);
                p.vx = 0;
                p.vy = 0;
                p.life = random(50, 200) / 100.0;
            }
            
            // Draw particle
            uint8_t brightness = (uint8_t)(p.life * 255);
            display.drawPixel(p.x, p.y, brightness > 127 ? SSD1306_WHITE : SSD1306_BLACK);
        }
        
        display.display();
        time += 0.1;
        delay(20);
    }
}

void OLEDManager::rainEffect() {
    const int NUM_RAINDROPS = 15;
    const int MAX_RIPPLES = 10;
    Raindrop raindrops[NUM_RAINDROPS];
    Ripple ripples[MAX_RIPPLES];
    int rippleCount = 0;

    // Initialize raindrops
    for (int i = 0; i < NUM_RAINDROPS; i++) {
        raindrops[i].x = random(width);
        raindrops[i].y = random(-50, -10);
        raindrops[i].speed = random(1, 3);
        raindrops[i].size = random(1, 3);
    }

    for (int frame = 0; frame < 300; frame++) {
        display.clearDisplay();

        // Update and draw raindrops
        for (int i = 0; i < NUM_RAINDROPS; i++) {
            raindrops[i].y += raindrops[i].speed;
            
            // Draw raindrop
            display.drawFastVLine(raindrops[i].x, raindrops[i].y, raindrops[i].size, SSD1306_WHITE);

            // Create ripple when raindrop hits bottom
            if (raindrops[i].y >= height) {
                if (rippleCount < MAX_RIPPLES) {
                    ripples[rippleCount].x = raindrops[i].x;
                    ripples[rippleCount].y = height - 1;
                    ripples[rippleCount].size = 1;
                    ripples[rippleCount].life = 15;
                    rippleCount++;
                }
                // Reset raindrop
                raindrops[i].x = random(width);
                raindrops[i].y = random(-50, -10);
            }
        }

        // Update and draw ripples
        for (int i = 0; i < rippleCount; i++) {
            if (ripples[i].life > 0) {
                // Draw ripple
                display.drawCircle(ripples[i].x, ripples[i].y, ripples[i].size, SSD1306_WHITE);
                // Update ripple
                ripples[i].size++;
                ripples[i].life--;
            }
        }

        // Remove dead ripples
        for (int i = 0; i < rippleCount; i++) {
            if (ripples[i].life <= 0) {
                ripples[i] = ripples[rippleCount - 1];
                rippleCount--;
                i--;
            }
        }

        display.display();
        delay(30);
    }
}

void OLEDManager::dynamicWaveEffect() {
    const int NUM_WAVES = 3;
    float time = 0;
    float frequencies[NUM_WAVES] = {0.02, 0.03, 0.05};
    float amplitudes[NUM_WAVES] = {10, 7, 5};
    float phaseShifts[NUM_WAVES] = {0, M_PI / 3, 2 * M_PI / 3};

    for (int frame = 0; frame < 300; frame++) {
        display.clearDisplay();

        // Calculate and draw the composite wave
        for (int x = 0; x < width; x++) {
            float y = height / 2; // Center line
            for (int i = 0; i < NUM_WAVES; i++) {
                y += calculateWave(x, time, frequencies[i], amplitudes[i] + sin(time * 0.1) * 2) * sin(time * 0.1 + phaseShifts[i]);
            }
            display.drawPixel(x, int(y), SSD1306_WHITE);
        }

        // Connect points with lines for a smoother appearance
        for (int x = 1; x < width; x++) {
            float y1 = height / 2;
            float y2 = height / 2;
            for (int i = 0; i < NUM_WAVES; i++) {
                y1 += calculateWave(x - 1, time, frequencies[i], amplitudes[i] + sin(time * 0.1) * 2) * sin(time * 0.1 + phaseShifts[i]);
                y2 += calculateWave(x, time, frequencies[i], amplitudes[i] + sin(time * 0.1) * 2) * sin(time * 0.1 + phaseShifts[i]);
            }
            display.drawLine(x - 1, int(y1), x, int(y2), SSD1306_WHITE);
        }

        display.display();
        time += 0.1;
        delay(30);
    }
}

float OLEDManager::calculateWave(float x, float time, float frequency, float amplitude) {
    return sin(x * frequency + time) * amplitude;
}

void OLEDManager::cyberGridEffect() {
    const int GRID_SIZE = 16;  // Increased grid size for more space
    const int NUM_ENERGY_LINES = 5;
    EnergyLine energyLines[NUM_ENERGY_LINES];

    // Initialize energy lines
    for (int i = 0; i < NUM_ENERGY_LINES; i++) {
        energyLines[i].vertical = random(2) == 0;
        if (energyLines[i].vertical) {
            energyLines[i].x = random(width / GRID_SIZE) * GRID_SIZE;
            energyLines[i].y = -GRID_SIZE;
            energyLines[i].length = random(2, 4) * GRID_SIZE;
            energyLines[i].speed = random(1, 3);
        } else {
            energyLines[i].x = -GRID_SIZE;
            energyLines[i].y = random(height / GRID_SIZE) * GRID_SIZE;
            energyLines[i].length = random(2, 4) * GRID_SIZE;
            energyLines[i].speed = random(1, 3);
        }
    }

    for (int frame = 0; frame < 300; frame++) {
        display.clearDisplay();

        // Draw faint grid
        for (int x = 0; x < width; x += GRID_SIZE) {
            display.drawFastVLine(x, 0, height, SSD1306_WHITE);
        }
        for (int y = 0; y < height; y += GRID_SIZE) {
            display.drawFastHLine(0, y, width, SSD1306_WHITE);
        }

        // Update and draw energy lines
        for (int i = 0; i < NUM_ENERGY_LINES; i++) {
            if (energyLines[i].vertical) {
                for (int j = 0; j < energyLines[i].length; j++) {
                    int brightness = 255 - (j * 255 / energyLines[i].length);
                    display.drawPixel(energyLines[i].x, energyLines[i].y + j, brightness > 127 ? SSD1306_WHITE : SSD1306_BLACK);
                }
                energyLines[i].y += energyLines[i].speed;
                if (energyLines[i].y > height) {
                    energyLines[i].y = -energyLines[i].length;
                    energyLines[i].x = random(width / GRID_SIZE) * GRID_SIZE;
                }
            } else {
                for (int j = 0; j < energyLines[i].length; j++) {
                    int brightness = 255 - (j * 255 / energyLines[i].length);
                    display.drawPixel(energyLines[i].x + j, energyLines[i].y, brightness > 127 ? SSD1306_WHITE : SSD1306_BLACK);
                }
                energyLines[i].x += energyLines[i].speed;
                if (energyLines[i].x > width) {
                    energyLines[i].x = -energyLines[i].length;
                    energyLines[i].y = random(height / GRID_SIZE) * GRID_SIZE;
                }
            }
        }

        // Add subtle pulsating effect to grid intersections
        for (int x = 0; x < width; x += GRID_SIZE) {
            for (int y = 0; y < height; y += GRID_SIZE) {
                int brightness = (sin(frame * 0.1 + x * 0.2 + y * 0.2) + 1) * 63;  // Reduced brightness
                display.drawPixel(x, y, brightness > 31 ? SSD1306_WHITE : SSD1306_BLACK);
            }
        }

        display.display();
        delay(30);
    }
}

void OLEDManager::drawStar(int x, int y, int brightness) {
    if (brightness > 0 && x >= 0 && x < width && y >= 0 && y < height) {
        display.drawPixel(x, y, SSD1306_WHITE);
    }
}

void OLEDManager::starfieldEffect2() {
    // Initialize stars with random positions and brightness
    for (int i = 0; i < 100; i++) {
        stars[i][0] = rand() % width;        // x position
        stars[i][1] = rand() % height;       // y position
        stars[i][2] = rand() % 4 + 1;        // brightness (1-4)
    }

    float z = 0;

    for (int frame = 0; frame < 300; frame++) { // Infinite loop to continuously show the effect
        display.clearDisplay();

        for (int i = 0; i < 100; i++) {
            int x = stars[i][0];
            int y = stars[i][1];
            int brightness = stars[i][2];
            
            // Update star position based on z depth
            int nx = (x - width / 2) * (1.0 / (z + 0.1)) + width / 2;
            int ny = (y - height / 2) * (1.0 / (z + 0.1)) + height / 2;

            drawStar(nx, ny, brightness);

            // Randomly reset star position and brightness to simulate new stars
            if (rand() % 100 < 2) {
                stars[i][0] = rand() % width;
                stars[i][1] = rand() % height;
                stars[i][2] = rand() % 4 + 1;
            }
        }

        display.display();
        delay(20);

        z += 0.1; // Move stars forward
        if (z > 5.0) { // Reset z if it goes too far
            z = 0;
        }
    }
}

void OLEDManager::drawParticle(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        display.drawPixel(x, y, SSD1306_WHITE);
    }
}

void OLEDManager::particleExplosionEffect() {
    // Initialize particles at the center with random velocities
    for (int i = 0; i < 100; i++) {
        particles2[i].x = width / 2;
        particles2[i].y = height / 2;
        particles2[i].vx = (rand() % 100 - 50) / 10.0; // Random velocity x
        particles2[i].vy = (rand() % 100 - 50) / 10.0; // Random velocity y
        particles2[i].lifetime = rand() % 50 + 20;     // Random lifetime (20 to 70 frames)
    }

    for (int frame = 0; frame < 300; frame++)  { // Infinite loop to continuously show the effect
        display.clearDisplay();

        for (int i = 0; i < 100; i++) {
            Particle2 &p = particles2[i];
            if (p.lifetime > 0) {
                p.x += p.vx;
                p.y += p.vy;
                p.lifetime--;

                drawParticle(p.x, p.y);
            }
        }

        display.display();
        delay(20);

        // Reset particles when all have expired
        bool allExpired = true;
        for (int i = 0; i < 100; i++) {
            if (particles2[i].lifetime > 0) {
                allExpired = false;
                break;
            }
        }
        if (allExpired) {
            // Reinitialize particles
            for (int i = 0; i < 100; i++) {
                particles2[i].x = width / 2;
                particles2[i].y = height / 2;
                particles2[i].vx = (rand() % 100 - 50) / 10.0; // Random velocity x
                particles2[i].vy = (rand() % 100 - 50) / 10.0; // Random velocity y
                particles2[i].lifetime = rand() % 50 + 20;     // Random lifetime (20 to 70 frames)
            }
        }
    }
}

void OLEDManager::marbleDropEffect() {
    const int NUM_MARBLES = 5;
    Marble marbles[NUM_MARBLES];
    const float GRAVITY = 0.2;
    const float BOUNCE_DAMPING = 0.8;
    const float FLOOR_Y = height - 1;
    const float DT = 0.5;  // Time step for physics simulation

    // Initialize marbles
    for (int i = 0; i < NUM_MARBLES; i++) {
        marbles[i].x = random(width);
        marbles[i].y = random(-50, -10);
        marbles[i].vy = 0;
        marbles[i].radius = random(2, 4);
        marbles[i].active = true;
    }

    for (int frame = 0; frame < 500; frame++) {
        display.clearDisplay();

        // Draw floor
        display.drawFastHLine(0, FLOOR_Y, width, SSD1306_WHITE);

        // Update and draw marbles
        for (int i = 0; i < NUM_MARBLES; i++) {
            if (marbles[i].active) {
                updateMarble(marbles[i], DT);
                
                // Draw marble
                display.fillCircle(marbles[i].x, marbles[i].y, marbles[i].radius, SSD1306_WHITE);
            }
        }

        display.display();
        delay(20);

        // Check if all marbles have stopped
        bool allStopped = true;
        for (int i = 0; i < NUM_MARBLES; i++) {
            if (marbles[i].active) {
                allStopped = false;
                break;
            }
        }
        if (allStopped) {
            delay(1000);  // Wait a moment before resetting
            // Reset marbles
            for (int i = 0; i < NUM_MARBLES; i++) {
                marbles[i].x = random(width);
                marbles[i].y = random(-50, -10);
                marbles[i].vy = 0;
                marbles[i].active = true;
            }
        }
    }
}

void OLEDManager::updateMarble(Marble& marble, float dt) {
    const float FLOOR_Y = height - 1 - marble.radius;
    const float GRAVITY = 0.2;
    const float BOUNCE_DAMPING = 0.8;
    const float STOP_VELOCITY = 0.5;

    // Apply gravity
    marble.vy += GRAVITY * dt;

    // Update position
    marble.y += marble.vy * dt;

    // Check for floor collision
    if (marble.y > FLOOR_Y) {
        marble.y = FLOOR_Y;
        marble.vy = -marble.vy * BOUNCE_DAMPING;  // Bounce with damping

        // Check if marble should stop bouncing
        if (abs(marble.vy) < STOP_VELOCITY) {
            marble.vy = 0;
            marble.active = false;
        }
    }
}

void OLEDManager::fallingLettersEffect() {
    const float GRAVITY = 0.2;
    const float BOUNCE_DAMPING = 0.7;
    const float FLOOR_Y = height - 30;  // Leave some space at the bottom
    const float DT = 0.5;  // Time step for physics simulation
    const int LETTER_WIDTH = 8;  // Width of each letter (including spacing)
    const int LETTER_HEIGHT = 10;  // Height of each letter

    std::string message = getRandomMessage();
    std::vector<FallingLetter> letters;

    // Calculate starting X position to center the message
    int totalWidth = message.length() * LETTER_WIDTH;
    int startX = (width - totalWidth) / 2;

    // Initialize letters
    for (size_t i = 0; i < message.length(); i++) {
        FallingLetter letter;
        letter.letter = message[i];
        letter.x = startX + i * LETTER_WIDTH;
        letter.y = random(-50, -10);
        letter.vy = 0;
        letter.settled = false;
        letter.targetY = FLOOR_Y - LETTER_HEIGHT;  // Set target Y position
        letters.push_back(letter);
    }

    unsigned long startTime = millis();
    bool allSettled = false;

    while (millis() - startTime < 20000) {  // Run for 20 seconds max
        display.clearDisplay();

        // Update and draw letters
        allSettled = true;
        for (auto& letter : letters) {
            if (!letter.settled) {
                updateLetter(letter);
                allSettled = false;
            }
        }
        drawLetters(letters);

        display.display();
        delay(20);

        // If all letters have settled, wait for 3 seconds and then exit
        if (allSettled) {
            delay(3000);
            break;
        }
    }
}

void OLEDManager::updateLetter(FallingLetter& letter) {
    const float GRAVITY = 0.2;
    const float BOUNCE_DAMPING = 0.7;
    const float STOP_VELOCITY = 0.5;
    const float DT = 0.5;

    // Apply gravity
    letter.vy += GRAVITY * DT;

    // Update position
    letter.y += letter.vy * DT;

    // Check for floor collision
    if (letter.y > letter.targetY) {
        letter.y = letter.targetY;
        letter.vy = -letter.vy * BOUNCE_DAMPING;  // Bounce with damping

        // Check if letter should stop bouncing
        if (abs(letter.vy) < STOP_VELOCITY) {
            letter.vy = 0;
            letter.settled = true;
            letter.y = letter.targetY;  // Ensure letter is exactly at the target position
        }
    }
}

void OLEDManager::drawLetters(const std::vector<FallingLetter>& letters) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    for (const auto& letter : letters) {
        display.setCursor(letter.x, letter.y);
        display.print(letter.letter);
    }
}

std::string OLEDManager::getRandomMessage() {
    const char* messages[] = {
        "TrafficLight",
        "Designing Emotions",
        "Centurion",
        "CENTURION",
        "Mater Fatima"
    };
    return messages[random(0, 5)];
}