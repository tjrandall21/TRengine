#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "ModelManager.h"

namespace TRengine::Graphics
{
    class RenderObject
    {
    public:
        void Terminate();

        MeshBuffer meshBuffer;
        Transform transform;
        Material material;
        TextureId diffuseMapId;
        TextureId specMapId;
        TextureId normMapId;
        TextureId bumpMapId;
    };

    class RenderGroup
    {
    public:
        void Initialize(const std::filesystem::path& modelFilePath);
        void Initialize(const Model& model);
        void Terminate();

        ModelId modelId;
        Transform transform;
        std::vector<RenderObject> renderObjects;
    };
}