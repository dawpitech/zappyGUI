#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

// ====== Classe pour un cube 3D animé (version simplifiée) ======

class AnimatedCube {
private:
    Vector3 position;
    Vector3 velocity;
    Vector3 rotation;
    Vector3 rotationSpeed;
    Vector3 size;
    Color color;
    float bounceHeight;
    float bounceSpeed;
    float bounceTimer;
    
public:
    AnimatedCube(Vector3 pos, Vector3 cubeSize, Color cubeColor) 
        : position(pos), size(cubeSize), color(cubeColor) {
        
        // Vitesse aléatoire
        velocity = {
            ((float)rand() / RAND_MAX - 0.5f) * 4.0f,
            ((float)rand() / RAND_MAX - 0.5f) * 2.0f,
            ((float)rand() / RAND_MAX - 0.5f) * 4.0f
        };
        
        // Rotation aléatoire
        rotation = {0, 0, 0};
        rotationSpeed = {
            ((float)rand() / RAND_MAX) * 2.0f,
            ((float)rand() / RAND_MAX) * 2.0f,
            ((float)rand() / RAND_MAX) * 2.0f
        };
        
        // Bounce effect
        bounceHeight = 1.0f + (float)rand() / RAND_MAX * 2.0f;
        bounceSpeed = 2.0f + (float)rand() / RAND_MAX * 2.0f;
        bounceTimer = (float)rand() / RAND_MAX * 6.28f; // Random phase
    }
    
    void update(float deltaTime) {
        // Mouvement de base
        position = Vector3Add(position, Vector3Scale(velocity, deltaTime));
        
        // Rotation
        rotation.x += rotationSpeed.x * deltaTime * 60.0f;
        rotation.y += rotationSpeed.y * deltaTime * 60.0f;
        rotation.z += rotationSpeed.z * deltaTime * 60.0f;
        
        // Effet de rebond vertical
        bounceTimer += bounceSpeed * deltaTime;
        float bounceOffset = sinf(bounceTimer) * bounceHeight;
        
        // Rebonds sur les "murs" invisibles (zone de -10 à 10)
        if (position.x < -10.0f || position.x > 10.0f) {
            velocity.x = -velocity.x;
            position.x = Clamp(position.x, -10.0f, 10.0f);
        }
        if (position.z < -10.0f || position.z > 10.0f) {
            velocity.z = -velocity.z;
            position.z = Clamp(position.z, -10.0f, 10.0f);
        }
        if (position.y < -3.0f || position.y > 8.0f) {
            velocity.y = -velocity.y;
            position.y = Clamp(position.y, -3.0f, 8.0f);
        }
        
        // Appliquer l'effet de rebond
        position.y += bounceOffset * 0.5f;
    }
    
    void draw() {
        // Version simplifiée : dessiner directement à la position avec rotation simple
        Vector3 drawPos = position;
        
        // Effet de rotation simple en utilisant DrawCubeV
        DrawCubeV(drawPos, size, color);
        DrawCubeWiresV(drawPos, size, BLACK);
        
        // Effet de rotation visuel avec plusieurs cubes légèrement décalés
        if (fmodf(rotation.y, 360.0f) > 180.0f) {
            Vector3 offsetPos = {drawPos.x + 0.1f, drawPos.y + 0.1f, drawPos.z + 0.1f};
            DrawCubeV(offsetPos, Vector3Scale(size, 0.8f), Fade(color, 0.5f));
        }
    }
    
    Vector3 getPosition() const { return position; }
    void setVelocity(Vector3 newVel) { velocity = newVel; }
};

// ====== Classe pour un cube orbital ======

class OrbitalCube {
private:
    Vector3 center;
    Vector3 offset;
    float orbitRadius;
    float orbitSpeed;
    float angle;
    Vector3 size;
    Color color;
    
public:
    OrbitalCube(Vector3 centerPos, float radius, float speed, Vector3 cubeSize, Color cubeColor)
        : center(centerPos), orbitRadius(radius), orbitSpeed(speed), size(cubeSize), color(cubeColor) {
        angle = (float)rand() / RAND_MAX * 2.0f * PI;
        
        // Offset aléatoire pour des orbites en 3D
        offset = {
            ((float)rand() / RAND_MAX - 0.5f) * 2.0f,
            ((float)rand() / RAND_MAX - 0.5f) * 2.0f,
            ((float)rand() / RAND_MAX - 0.5f) * 2.0f
        };
        offset = Vector3Normalize(offset);
    }
    
    void update(float deltaTime) {
        angle += orbitSpeed * deltaTime;
    }
    
    void draw() {
        // Calculer la position orbitale
        Vector3 orbitPos = {
            center.x + cosf(angle) * orbitRadius + offset.x * orbitRadius * 0.3f,
            center.y + sinf(angle * 0.7f) * orbitRadius * 0.5f + offset.y * orbitRadius * 0.3f,
            center.z + sinf(angle) * orbitRadius + offset.z * orbitRadius * 0.3f
        };
        
        // Dessiner le cube
        DrawCubeV(orbitPos, size, color);
        DrawCubeWiresV(orbitPos, size, DARKGRAY);
        
        // Dessiner la trajectoire orbitale
        DrawLine3D(center, orbitPos, Fade(color, 0.3f));
    }
};

// ====== Cube qui tourne sur lui-même ======

class SpinningCube {
private:
    Vector3 position;
    Vector3 size;
    Color color;
    float spinSpeed;
    float currentRotation;
    
public:
    SpinningCube(Vector3 pos, Vector3 cubeSize, Color cubeColor, float speed)
        : position(pos), size(cubeSize), color(cubeColor), spinSpeed(speed), currentRotation(0) {}
    
    void update(float deltaTime) {
        currentRotation += spinSpeed * deltaTime;
        if (currentRotation > 360.0f) currentRotation -= 360.0f;
    }
    
    void draw() {
    // Simuler la rotation en dessinant le cube avec un léger effet
    DrawCubeV(position, size, color);
    
    // Ajouter des faces légèrement décalées pour l'effet de rotation
    float rotFactor = sinf(currentRotation * DEG2RAD);
    Vector3 rotatedSize = {
        size.x + rotFactor * 0.1f,
        size.y,
        size.z + rotFactor * 0.1f
    };
    
    // Correction ici :
    DrawCubeWiresV(position, rotatedSize, DARKGRAY);
    
    // Effet de pulsation
    if (fmodf(currentRotation, 90.0f) < 45.0f) {
        DrawCubeV(position, Vector3Scale(size, 1.1f), Fade(color, 0.3f));
    }
}

};

// ====== Classe pour générer un terrain simple ======

class SimpleTerrain {
public:
    void draw() {
        // Sol en damier
        for (int x = -10; x <= 10; x++) {
            for (int z = -10; z <= 10; z++) {
                Color tileColor = ((x + z) % 2 == 0) ? GREEN : DARKGREEN;
                DrawCube({(float)x, -4.0f, (float)z}, 1.0f, 0.1f, 1.0f, tileColor);
            }
        }
        
        // Quelques piliers décoratifs
        DrawCube({-8, -2, -8}, 0.5f, 4.0f, 0.5f, BROWN);
        DrawCube({8, -2, -8}, 0.5f, 4.0f, 0.5f, BROWN);
        DrawCube({-8, -2, 8}, 0.5f, 4.0f, 0.5f, BROWN);
        DrawCube({8, -2, 8}, 0.5f, 4.0f, 0.5f, BROWN);
        
        // Cubes décoratifs sur les piliers
        DrawCube({-8, 0.5f, -8}, 1.0f, 1.0f, 1.0f, ORANGE);
        DrawCube({8, 0.5f, -8}, 1.0f, 1.0f, 1.0f, PURPLE);
        DrawCube({-8, 0.5f, 8}, 1.0f, 1.0f, 1.0f, PINK);
        DrawCube({8, 0.5f, 8}, 1.0f, 1.0f, 1.0f, LIME);
    }
};


void DrawColoredGrid(int slices, float spacing, Color color) {
    for (int i = -slices; i <= slices; i++) {
        DrawLine3D((Vector3){i * spacing, 0.0f, -slices * spacing},
                   (Vector3){i * spacing, 0.0f, slices * spacing}, color);

        DrawLine3D((Vector3){-slices * spacing, 0.0f, i * spacing},
                   (Vector3){slices * spacing, 0.0f, i * spacing}, color);
    }
}


// ====== Programme principal ======

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Cubes 3D Animés - Raylib (Version Compatible)");
    SetTargetFPS(60);
    
    srand(time(NULL));
    
    // Configuration de la caméra
    Camera3D camera = { 0 };
    camera.position = {15.0f, 10.0f, 15.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Créer les cubes animés
    std::vector<AnimatedCube> cubes;
    std::vector<OrbitalCube> orbitalCubes;
    std::vector<SpinningCube> spinningCubes;
    
    // Cubes avec mouvement libre
    for (int i = 0; i < 6; i++) {
        Vector3 pos = {
            ((float)rand() / RAND_MAX - 0.5f) * 16.0f,
            ((float)rand() / RAND_MAX) * 6.0f,
            ((float)rand() / RAND_MAX - 0.5f) * 16.0f
        };
        
        Vector3 size = {
            0.8f + (float)rand() / RAND_MAX * 1.2f,
            0.8f + (float)rand() / RAND_MAX * 1.2f,
            0.8f + (float)rand() / RAND_MAX * 1.2f
        };
        
        Color color = {
            (unsigned char)(128 + rand() % 128),
            (unsigned char)(128 + rand() % 128),
            (unsigned char)(128 + rand() % 128),
            255
        };
        
        cubes.emplace_back(pos, size, color);
    }
    
    // Cubes orbitaux
    for (int i = 0; i < 4; i++) {
        Vector3 center = {0, 2.0f, 0};
        float radius = 3.0f + i * 1.5f;
        float speed = 1.0f - i * 0.15f;
        
        Vector3 size = {0.8f, 0.8f, 0.8f};
        Color color = {
            (unsigned char)(100 + rand() % 156),
            (unsigned char)(100 + rand() % 156),
            (unsigned char)(100 + rand() % 156),
            255
        };
        
        orbitalCubes.emplace_back(center, radius, speed, size, color);
    }
    
    // Cubes qui tournent sur place
    spinningCubes.emplace_back(Vector3{-5, 1, -5}, Vector3{1.5f, 1.5f, 1.5f}, RED, 90.0f);
    spinningCubes.emplace_back(Vector3{5, 1, -5}, Vector3{1.2f, 1.2f, 1.2f}, BLUE, 120.0f);
    spinningCubes.emplace_back(Vector3{-5, 1, 5}, Vector3{1.3f, 1.3f, 1.3f}, GREEN, 75.0f);
    spinningCubes.emplace_back(Vector3{5, 1, 5}, Vector3{1.4f, 1.4f, 1.4f}, YELLOW, 105.0f);
    
    // Terrain
    SimpleTerrain terrain;
    
    // Variables pour l'interaction
    bool freeCameraMode = true;
    float cameraAngle = 0.0f;
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Contrôles de la caméra
        if (IsKeyPressed(KEY_SPACE)) {
            freeCameraMode = !freeCameraMode;
        }
        
        if (freeCameraMode) {
            UpdateCamera(&camera, CAMERA_FREE);
        } else {
            // Caméra orbitale automatique
             cameraAngle += deltaTime * 0.5f;
            camera.position.x = cosf(cameraAngle) * 20.0f;
            camera.position.z = sinf(cameraAngle) * 20.0f;
            camera.position.y = 8.0f;  // Hauteur fixe
        }
        
        // Mise à jour des cubes
        for (auto& cube : cubes) {
            cube.update(deltaTime);
        }
        
        for (auto& orbitalCube : orbitalCubes) {
            orbitalCube.update(deltaTime);
        }
        
        for (auto& spinningCube : spinningCubes) {
            spinningCube.update(deltaTime);
        }
        
        // Rendu
        BeginDrawing();
        ClearBackground(DARKBLUE);
        
        BeginMode3D(camera);
        
        // Dessiner le terrain
        terrain.draw();
        
        // Dessiner tous les cubes
        for (auto& cube : cubes) {
            cube.draw();
        }
        
        // for (auto& orbitalCube : orbitalCubes) {
        //     orbitalCube.draw();
        // }
        
        for (auto& spinningCube : spinningCubes) {
            spinningCube.draw();
        }
        
        // Repères 3D
        DrawGrid(20, 1.0f);
        // Cube central de référence
        DrawCube({0, 0, 0}, 0.5f, 0.5f, 0.5f, WHITE);
        
        EndMode3D();
        
        // Interface utilisateur
        DrawText("CUBES 3D ANIMES - VERSION COMPATIBLE", 10, 10, 28, WHITE);
        DrawText("ESPACE: Basculer mode caméra", 10, 45, 18, LIGHTGRAY);
        DrawText("WASD + Souris: Caméra libre", 10, 65, 18, LIGHTGRAY);
        
        const char* cameraMode = freeCameraMode ? "LIBRE" : "ORBITALE";
        DrawText(TextFormat("Mode caméra: %s", cameraMode), 10, 90, 18, YELLOW);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 110, 18, GREEN);
        
        // Légende
        DrawText("Cubes colorés: Mouvement libre avec rebonds", 10, screenHeight - 100, 14, WHITE);
        DrawText("Cubes avec lignes: Mouvement orbital autour du centre", 10, screenHeight - 80, 14, WHITE);
        DrawText("Cubes aux coins: Rotation sur place", 10, screenHeight - 60, 14, WHITE);
        DrawText("Sol en damier avec piliers et cubes décoratifs", 10, screenHeight - 40, 14, WHITE);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

// ====== Version ultra simple pour tester ======

/*
#include <raylib.h>
#include <raymath.h>
#include <cmath>

int main() {
    InitWindow(800, 600, "Test Cube 3D Simple");
    SetTargetFPS(60);
    
    Camera3D camera = { 0 };
    camera.position = {10.0f, 10.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    float rotation = 0.0f;
    
    while (!WindowShouldClose()) {
        rotation += 50.0f * GetFrameTime();
        
        UpdateCamera(&camera, CAMERA_FREE);
        
        BeginDrawing();
        ClearBackground(DARKBLUE);
        
        BeginMode3D(camera);
        
        // Cubes simples
        DrawCube({0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires({0, 0, 0}, 2.0f, 2.0f, 2.0f, MAROON);
        
        DrawCube({4, 0, 0}, 1.5f, 1.5f, 1.5f, BLUE);
        DrawCube({-4, 0, 0}, 1.5f, 1.5f, 1.5f, GREEN);
        DrawCube({0, 0, 4}, 1.5f, 1.5f, 1.5f, YELLOW);
        DrawCube({0, 0, -4}, 1.5f, 1.5f, 1.5f, PURPLE);
        
        DrawGrid(10, 1.0f);
        
        EndMode3D();
        
        DrawText("Test Cubes 3D - WASD + Souris pour bouger", 10, 10, 20, WHITE);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 40, 20, GREEN);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
*/