#ifndef SENGINE_ENTITY_H
#define SENGINE_ENTITY_H

#include <typeindex>
#include <typeinfo>

#include "core/types.hpp"
#include "components/component.hpp"

typedef unsigned long EntityId;

namespace core {
    class Entity {
    public:
        inline Entity() :
        translation(), scale(1.0f), rotation(),
        id(Id++),
        forward(0.0f, 0.0f, -1.0f),
        up(0.0f, 1.0f, 0.0f),
        right(1.0f, 0.0f, 0.0f),
        transform(1.0f)
        {
        }

        inline virtual ~Entity() {}

        inline vec3 GetTranslation() { return translation; }
        inline quat GetRotation() { return rotation; }
        inline vec3 GetScale() { return scale; }

        inline vec3 GetForward() { return forward; }
        inline vec3 GetUp() { return up; }
        inline vec3 GetRight() { return right; }

        inline mat4 GetTransform() { return transform; }

        // NOTE: I don't really like Getters/Setters for the translation
        // but the matrix and direction vectors being updated is important.
        // #WheresStoozey
        
        inline void SetTranslation(vec3 t)
        {
            translation = t;
            UpdateGlobalSpace();
        }

        inline void SetRotation(quat r)
        {
            rotation = r;
            UpdateGlobalSpace();
        }

        inline void SetScale(vec3 s)
        {
            scale = s;
            UpdateGlobalSpace();
        }

        void Translate(const vec3 &trans);
        void Rotate(const vec3 &rot);

        inline EntityId GetEntityId() const { return id; }

        template<typename T>
        bool HasComponent();

        template<typename T>
        void AddComponent();

        template<typename T>
        Ref<T> GetComponent();

        virtual void PreUpdate(double deltaTime);
        virtual void Update(double deltaTime);
        virtual void PostUpdate(double deltaTime);

        virtual void PreRender();
        virtual void Render();
        virtual void PostRender();
    protected:
        static EntityId Id;

        vec3 translation;
        quat rotation;
        vec3 scale;

        vec3 forward;
        vec3 up;
        vec3 right;

        mat4 transform;

        void UpdateGlobalSpace();

        void UpdateComponents(void(components::Component::*func)(double), double deltaTime);
        void RenderComponents(void(components::Component::*func)());
    private:
        EntityId id;
        std::map<std::type_index, std::shared_ptr<components::Component>> components;
    };
}

#include "core/entity.tpp"
#endif //SENGINE_ENTITY_H
