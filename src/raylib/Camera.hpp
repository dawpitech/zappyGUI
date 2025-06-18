namespace GUI {
    class Camera3DController {
    private:
        Camera3D camera;
        float distance;
        float angleX;
        float angleY;
        Vector3 target;

        void updateCameraPosition() {
            float radX = angleX * DEG2RAD;
            float radY = angleY * DEG2RAD;

            camera.position.x = target.x + distance * cos(radX) * cos(radY);
            camera.position.y = target.y + distance * sin(radX);
            camera.position.z = target.z + distance * cos(radX) * sin(radY);

            camera.target = target;
        }

    public:
        Camera3DController(Vector3 mapCenter, float dist = 20.0f) {
            distance = dist;
            angleX = 45.0f;
            angleY = 45.0f;
            target = mapCenter;

            camera.up = { 0.0f, 1.0f, 0.0f };
            camera.fovy = 45.0f;
            camera.projection = CAMERA_PERSPECTIVE;

            updateCameraPosition();
        }

        void update(RaylibEngine& engine) {
            // Rotation avec les flèches ou WASD
            if (engine.keyDown(KEY_LEFT) || engine.keyDown(KEY_A)) {
                angleY -= 2.0f;
            }
            if (engine.keyDown(KEY_RIGHT) || engine.keyDown(KEY_D)) {
                angleY += 2.0f;
            }
            if (engine.keyDown(KEY_UP) || engine.keyDown(KEY_W)) {
                angleX += 2.0f;
                if (angleX > 89.0f) angleX = 89.0f;
            }
            if (engine.keyDown(KEY_DOWN) || engine.keyDown(KEY_S)) {
                angleX -= 2.0f;
                if (angleX < -89.0f) angleX = -89.0f;
            }

            // Zoom avec Q/E
            if (engine.keyDown(KEY_Q)) {
                distance += 0.5f;
                if (distance > 50.0f) distance = 50.0f;
            }
            if (engine.keyDown(KEY_E)) {
                distance -= 0.5f;
                if (distance < 5.0f) distance = 5.0f;
            }

            updateCameraPosition();
        }

        // Getters
        Camera3D getCamera() const { return camera; }
        float getDistance() const { return distance; }
        float getAngleX() const { return angleX; }
        float getAngleY() const { return angleY; }
        Vector3 getTarget() const { return target; }

        // Setters
        void setDistance(float dist) {
            distance = dist;
            if (distance < 5.0f) distance = 5.0f;
            if (distance > 50.0f) distance = 50.0f;
            updateCameraPosition();
        }

        void setAngles(float x, float y) {
            angleX = x;
            angleY = y;
            if (angleX > 89.0f) angleX = 89.0f;
            if (angleX < -89.0f) angleX = -89.0f;
            updateCameraPosition();
        }

        void setTarget(Vector3 newTarget) {
            target = newTarget;
            updateCameraPosition();
        }

        void reset() {
            angleX = 45.0f;
            angleY = 45.0f;
            distance = 20.0f;
            updateCameraPosition();
        }
    };
}
