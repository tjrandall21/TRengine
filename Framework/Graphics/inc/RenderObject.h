#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"

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
}