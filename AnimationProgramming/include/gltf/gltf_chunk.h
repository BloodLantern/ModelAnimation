#pragma once

#include <vector>

#include "chunk_accessor.h"
#include "chunk_animation.h"
#include "chunk_asset.h"
#include "chunk_buffer.h"
#include "chunk_buffer_view.h"
#include "chunk_camera.h"
#include "chunk_image.h"
#include "chunk_material.h"
#include "chunk_mesh.h"
#include "chunk_node.h"
#include "chunk_sampler.h"
#include "chunk_scene.h"
#include "chunk_skin.h"
#include "chunk_texture.h"

class Chunk
{
protected:
    unsigned int m_Length;

    char m_Type[4];
    
public:
    Chunk() = default;
    
    explicit Chunk(unsigned int length, char type[4]);

    _NODISCARD unsigned int GetLength() const;
};
