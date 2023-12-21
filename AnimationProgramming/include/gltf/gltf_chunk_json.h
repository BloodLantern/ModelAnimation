#pragma once

#include "gltf_chunk.h"

class ChunkJson : public Chunk
{
public:
    std::vector<ChunkAccessor> accessors;
    std::vector<ChunkAnimation> animations;
    ChunkAsset asset;
    std::vector<ChunkBuffer> buffers;
    std::vector<ChunkBufferView> bufferViews;
    std::vector<ChunkCamera> cameras;
    std::vector<ChunkImage> images;
    std::vector<ChunkMaterial> materials;
    std::vector<ChunkMesh> meshes;
    std::vector<ChunkNode> nodes;
    std::vector<ChunkSampler> samplers;
    int scene;
    std::vector<ChunkScene> scenes;
    std::vector<ChunkSkin> skins;
    std::vector<ChunkTexture> textures;

    ChunkJson() = default;
    
    explicit ChunkJson(unsigned int length, char type[4], std::ifstream& file);
};
