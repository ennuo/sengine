#include "core/entity.hpp"
#include "utils/entity_util.hpp"

namespace core {
    EntityId Entity::Id = 0;

    void Entity::Translate(const vec3 &trans) {
        translation += (trans.x * forward);
        translation += (trans.y * up);
        translation += (trans.z * right);

        UpdateGlobalSpace();
    }

    void Entity::Rotate(const vec3 &rot)
    {
        glm::quat q { glm::radians(rot) };
        rotation = q * rotation;

        UpdateGlobalSpace();
    }

    void Entity::UpdateGlobalSpace()
    {
        transform = glm::translate(glm::mat4(1.0f), translation) *
                    glm::toMat4(rotation) *
                    glm::scale(glm::mat4(1.0f), scale);
        forward = rotation * vec3(0.0, 0.0, -1.0);
        up = rotation * vec3(0.0, 1.0, 0.0);
        right = rotation * vec3(1.0, 0.0, 0.0);
    }

    void Entity::PreUpdate(double deltaTime) {
        UpdateComponents(&components::Component::PreUpdate, deltaTime);
    }

    void Entity::Update(double deltaTime) {
        UpdateComponents(&components::Component::Update, deltaTime);
    }

    void Entity::PostUpdate(double deltaTime) {
        UpdateComponents(&components::Component::PostUpdate, deltaTime);
    }

    void Entity::PreRender() {
        RenderComponents(&components::Component::PreRender);
    }

    void Entity::Render() {
        RenderComponents(&components::Component::Render);
    }

    void Entity::PostRender() {
        RenderComponents(&components::Component::PostRender);
    }

    void Entity::UpdateComponents(void(components::Component::*func)(double), double deltaTime) {
        if (components.empty()) return;

        auto runner = utils::GetUpdateRunner<components::Component>(func);
        std::map<std::type_index, std::shared_ptr<components::Component>>::iterator iterator;
        for (iterator = components.begin(); iterator != components.end(); ++iterator) {
            std::shared_ptr<components::Component> component = iterator->second;
            runner(component.get(), deltaTime);
        }
    }

    void Entity::RenderComponents(void(components::Component::*func)()) {
        if (components.empty()) return;

        auto runner = utils::GetRenderRunner<components::Component>(func);
        std::map<std::type_index, std::shared_ptr<components::Component>>::iterator iterator;
        for (iterator = components.begin(); iterator != components.end(); ++iterator) {
            std::shared_ptr<components::Component> component = iterator->second;
            runner(component.get());
        }
    }
}