#include "components/mesh_renderer.hpp"
#include "core/log.hpp"
#include "core/engine.hpp"

namespace components {
    MeshRenderer::MeshRenderer(EntityId id) : Component(), model()
    {
        FetchEntity(id);
    }

    void MeshRenderer::Render()
    {
        if (model == nullptr) return;

        auto perspective = glm::perspective(glm::radians(90.0f), static_cast<float>(g_Engine->GetWindowWidth()) / static_cast<float>(g_Engine->GetWindowHeight()), 0.01f, 1000.0f);

        model->Bind();
        for (const auto& mesh : model->GetMeshes())
        {
            auto& material = mesh.material;
            if (material == nullptr) continue;

            material->Bind();

            auto shader = material->GetShader();
            shader->SetUniform("u_modelMatrix", entity->GetTransform());
            shader->SetUniform("u_projection", perspective);

            glDrawElements(mesh.primitiveType, mesh.indexCount, GL_UNSIGNED_SHORT, (void*)(mesh.indexStart * sizeof(GLuint)));
        }

        glBindVertexArray(0);
        glUseProgram(0);
    }

}