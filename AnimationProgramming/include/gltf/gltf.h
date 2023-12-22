#pragma once

#include <fstream>
#include <vector>

#include "gltf_chunk.h"
#include "gltf_chunk_bin.h"

class Gltf
{
public:
    explicit Gltf(std::ifstream& file, const std::filesystem::path& filepath);

    ~Gltf();

    _NODISCARD const ChunkJson* GetJsonChunk(int index = 0) const;

    _NODISCARD const ChunkBin* GetBinaryChunk(int index = 0) const;

private:
    unsigned int m_Version;
    unsigned int m_FileSize = 0;
    
    std::vector<Chunk*> m_Chunks;

    bool LoadHeader(std::ifstream& file);
};
