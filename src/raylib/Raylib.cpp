#include "Raylib.hpp"
#include <string>

GUI::GameObject::GameObject(const std::string& tex, Vector2 position)
    : texture(tex), pos(position), vel{0, 0}, rot(0), scale(1.0f), color(WHITE), visible(true) {
}

void GUI::GameObject::update(float dt) {
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
}

void GUI::GameObject::setVelocity(float x, float y) {
    vel = {x, y};
}

void GUI::GameObject::setPosition(float x, float y) {
    pos = {x, y};
}

void GUI::GameObject::setRotation(float r) {
    rot = r;
}

void GUI::GameObject::setScale(float s) {
    scale = s;
}

void GUI::GameObject::setColor(Color t) {
    color = t;
}

void GUI::GameObject::show() {
    visible = true;
}

void GUI::GameObject::hide() {
    visible = false;
}

GUI::RaylibEngine::RaylibEngine(int w, int h, const std::string& t)
    : width(w), height(h), title(t), init(false) {
}

GUI::RaylibEngine::~RaylibEngine() {
    if (init) {
        for (auto& p : textures) {
            UnloadTexture(p.second);
        }
        for (auto& p : sounds) {
            UnloadSound(p.second);
        }
        CloseAudioDevice();
        CloseWindow();
    }
}

void GUI::RaylibEngine::start() {
    InitWindow(width, height, title.c_str());
    InitAudioDevice();
    SetTargetFPS(60);
    init = true;
}

void GUI::RaylibEngine::run() {
    if (!init) start();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        for (auto& obj : objects) {
            obj.update(dt);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const auto& obj : objects) {
            if (obj.visible && textures.count(obj.texture)) {
                DrawTextureEx(textures.at(obj.texture), obj.pos, obj.rot, obj.scale, obj.color);
            }
        }

        EndDrawing();
    }
}

void GUI::RaylibEngine::loadTexture(const std::string& key, const std::string& file) {
    textures[key] = LoadTexture(file.c_str());
}

void GUI::RaylibEngine::loadSound(const std::string& key, const std::string& file) {
    sounds[key] = LoadSound(file.c_str());
}

void GUI::RaylibEngine::playSound(const std::string& key) {
    if (sounds.count(key)) {
        PlaySound(sounds.at(key));
    }
}

int GUI::RaylibEngine::addObject(const std::string& textureKey, Vector2 pos) {
    objects.emplace_back(textureKey, pos);
    return objects.size() - 1;
}

GUI::GameObject* GUI::RaylibEngine::getObject(int id) {
    return (id >= 0 && id < objects.size()) ? &objects[id] : nullptr;
}

void GUI::RaylibEngine::drawTexture(const std::string& key, Vector2 pos, Color color) {
    if (textures.count(key)) {
        DrawTextureV(textures.at(key), pos, color);
    }
}

void GUI::RaylibEngine::drawText(const std::string& text, Vector2 pos, int size, Color color) {
    DrawText(text.c_str(), pos.x, pos.y, size, color);
}

void GUI::RaylibEngine::drawRect(Vector2 pos, Vector2 size, Color color) {
    DrawRectangleV(pos, size, color);
}

void GUI::RaylibEngine::drawCircle(Vector2 pos, float radius, Color color) {
    DrawCircleV(pos, radius, color);
}

bool GUI::RaylibEngine::keyPressed(int key) {
    return IsKeyPressed(key);
}

bool GUI::RaylibEngine::keyDown(int key) {
    return IsKeyDown(key);
}

bool GUI::RaylibEngine::mousePressed(int btn) {
    return IsMouseButtonPressed(btn);
}

Vector2 GUI::RaylibEngine::mousePos() {
    return GetMousePosition();
}

bool GUI::RaylibEngine::collision(Vector2 p1, Vector2 s1, Vector2 p2, Vector2 s2) {
    return (p1.x < p2.x + s2.x && p1.x + s1.x > p2.x &&
            p1.y < p2.y + s2.y && p1.y + s1.y > p2.y);
}

void GUI::RaylibEngine::beginMode3D(Camera3D camera) {
    BeginMode3D(camera);
}

void GUI::RaylibEngine::endMode3D() {
    EndMode3D();
}

void GUI::RaylibEngine::drawCube(Vector3 position, float width, float height, float length, Color color) {
    DrawCube(position, width, height, length, color);
}

void GUI::RaylibEngine::drawCubeWires(Vector3 position, float width, float height, float length, Color color) {
    DrawCubeWires(position, width, height, length, color);
}

void GUI::RaylibEngine::drawLine3D(Vector3 startPos, Vector3 endPos, Color color) {
    DrawLine3D(startPos, endPos, color);
}

void GUI::RaylibEngine::clearBackground(Color color) {
    ClearBackground(color);
}

bool GUI::RaylibEngine::windowShouldClose() {
    return WindowShouldClose();
}
