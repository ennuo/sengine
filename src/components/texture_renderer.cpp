#include "components/texture_renderer.hpp"
#include "core/log.hpp"
#include "core/engine.hpp"
#include "classes/primitive.hpp"
#include "structs/primitive_type.hpp"

namespace components {
    TextureRenderer::TextureRenderer(EntityId id) : Component()
    {
        model = classes::Primitive::GetModel(structs::PrimitiveType::Plane);
        FetchEntity(id);
    }

    void TextureRenderer::Render()
    {
        if (material == nullptr) return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        material->SetTexture(texture, 0);
        material->Bind();

        auto shader = material->GetShader();

        mat4 translation = glm::translate(glm::mat4(1.0f), entity->position);
        mat4 scale = glm::scale(glm::mat4(1.0f), entity->scale);
        auto perspective = glm::perspective(glm::radians(90.0f), static_cast<float>(g_Engine->GetWindowWidth()) / static_cast<float>(g_Engine->GetWindowHeight()), 0.1f, 3.0f);

        shader->SetUniform("u_modelMatrix", (translation * scale));
        shader->SetUniform("u_projection", perspective);

        model->Bind();
        for (const auto& mesh : model->GetMeshes())
        {
            glDrawElements(mesh.primitiveType, mesh.indexCount, GL_UNSIGNED_SHORT, (void*)(mesh.indexStart * sizeof(GLuint)));
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glDisable(GL_BLEND);
    }

}