#ifndef SENGINE_TEST1_MODEL_H
#define SENGINE_TEST1_MODEL_H

#include "core/types.hpp"

#include "classes/mesh.hpp"
#include "assets/shader.hpp"

namespace assets {
    class Model : public Asset {
    public:
        Model();

        void Save(const string &filePath) override;
        void Load(const string &filePath) override;
    private:
        classes::Mesh mesh;
    };
}

#endif //SENGINE_TEST1_MODEL_H
