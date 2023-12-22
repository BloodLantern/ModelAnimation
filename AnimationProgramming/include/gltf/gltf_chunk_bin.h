#pragma once

#include "gltf_chunk.h"

#include <fstream>

#include "gltf_chunk_json.h"

class ChunkBin : public Chunk
{
public:
    struct Buffer
    {
        const char* ptr;
        int length;

        Buffer() = default;

        Buffer(const char* ptr, int length);
    };

    struct VertexBuffer : Buffer
    {
        int stride;
        int target;

        VertexBuffer() = default;

        VertexBuffer(const char* bufferPtr, int bufferLength, int stride, int target);
    };

    struct AccessorBuffer : Buffer
    {
        unsigned int typeSize;
        unsigned int componentCount;
        unsigned int componentType;
        bool normalized;

        const ChunkAccessor* accessor;

        AccessorBuffer() = default;

        AccessorBuffer(const char* bufferPtr, int bufferLength, unsigned int typeSize, unsigned int componentCount, unsigned int componentType, bool normalized, const ChunkAccessor* accessor);
    };

    struct ImageData
    {
        unsigned char* data;
        int width, height;
        int channels;
    };
    
private:
    const ChunkJson* m_JsonHeader;
    
    std::vector<char*> m_RawBuffers;

    std::vector<VertexBuffer> m_VertexBuffers;
    
    std::vector<AccessorBuffer> m_AccessorBuffers;

    std::vector<ImageData> m_Images;

public:
    ChunkBin() = default;
    
    explicit ChunkBin(unsigned int length, char type[4], std::ifstream& file, const ChunkJson* jsonHeader, const std::filesystem::path& filepath);

    ~ChunkBin();

    _NODISCARD const std::vector<VertexBuffer>& GetVertexBuffers() const;
    
    _NODISCARD const std::vector<AccessorBuffer>& GetAccessorBuffers() const;
    
    _NODISCARD const std::vector<ImageData>& GetImages() const;
};
