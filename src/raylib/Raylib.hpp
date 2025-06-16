#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace GUI {
    class GameObject {
        public:
            std::string texture;
            Vector2 pos;
            Vector2 vel;
            float rot;
            float scale;
            Color color;
            bool visible;

            GameObject(const std::string& tex, Vector2 position = {0, 0});
            void update(float dt);
            void setVelocity(float x, float y);
            void setPosition(float x, float y);
            void setRotation(float r);
            void setScale(float s);
            void setColor(Color t);
            void show();
            void hide();
    };
    class RaylibEngine {
        private:
            int width, height;
            std::string title;
            bool init;
            std::unordered_map<std::string, Texture2D> textures;
            std::unordered_map<std::string, Sound> sounds;
            std::vector<GameObject> objects;
        public:
            RaylibEngine(int w = 800, int h = 600, const std::string& t = "Game");
            ~RaylibEngine();

            void start();
            void run();

            void loadTexture(const std::string& key, const std::string& file);
            void loadSound(const std::string& key, const std::string& file);
            void playSound(const std::string& key);

            int addObject(const std::string& textureKey, Vector2 pos = {0, 0});
            GameObject* getObject(int id);

            void drawTexture(const std::string& key, Vector2 pos, Color color = WHITE);
            void drawText(const std::string& text, Vector2 pos, int size = 20, Color color = BLACK);
            void drawRect(Vector2 pos, Vector2 size, Color color);
            void drawCircle(Vector2 pos, float radius, Color color);

            bool keyPressed(int key);
            bool keyDown(int key);
            bool mousePressed(int btn);
            Vector2 mousePos();

            bool collision(Vector2 p1, Vector2 s1, Vector2 p2, Vector2 s2);
    };
} // namespace GUI
