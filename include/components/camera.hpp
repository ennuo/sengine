#ifndef SENGINE_JUMP_KING_CAMERA_HPP
#define SENGINE_JUMP_KING_CAMERA_HPP

#include "core/types.hpp"
#include "core/engine.hpp"
#include "components/component.hpp"

namespace components {
    class Camera : public Component {
    public:
        inline explicit Camera(EntityId entityId) : Component(), projection(), view()
        {
            FetchEntity(entityId);
        }

        inline void SetFOV(float newFov) { fov = newFov; }
        inline void SetZFar(float newZFar) { far = newZFar; }
        inline void SetZNear(float newZNear) { near = newZNear; }

        inline mat4 GetProjectionMatrix() { return projection; }
        inline mat4 GetViewMatrix() { return view; }

        inline void ComputeProjectionMatrix()
        {
            projection = glm::perspective(glm::radians(fov), static_cast<float>(g_Engine->GetWindowWidth()) / static_cast<float>(g_Engine->GetWindowHeight()), near, far);
        }

        inline void ComputeViewMatrix()
        {
            auto translation = entity->GetTranslation();
            view = glm::lookAt(translation, translation + entity->GetForward(), entity->GetUp());
        }
    private:
        float aspect = 16.0f / 9.0f;
        float near = 0.01f, far = 1000.0f;
        float fov = 90.0f;
        mat4 projection, view;
    };
}

#endif //SENGINE_JUMP_KING_CAMERA_HPP
