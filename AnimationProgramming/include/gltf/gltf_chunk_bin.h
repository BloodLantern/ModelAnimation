#pragma once

#include "gltf_chunk.h"

#include <fstream>
#include <unordered_map>

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

    struct BufferView : Buffer
    {
        const ChunkBufferView* bufferView;

        BufferView() = default;

        BufferView(const char* bufferPtr, int bufferLength, const ChunkBufferView* bufferView);
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

    struct MeshPrimitiveData
    {
        std::unordered_map<std::string, unsigned int> attributes;
        int indices;
        int material;
        int mode;
    };

    struct MeshData
    {
        std::vector<MeshPrimitiveData> primitives;
    };
    
private:
    const ChunkJson* m_JsonHeader;
    
    std::vector<char*> m_RawBuffers;

    std::vector<BufferView> m_BufferViews;
    
    std::vector<AccessorBuffer> m_AccessorBuffers;

    std::vector<ImageData> m_Images;

    std::vector<MeshData> m_Meshes;

public:
    ChunkBin() = default;
    
    explicit ChunkBin(unsigned int length, char type[4], std::ifstream& file, const ChunkJson* jsonHeader, const std::filesystem::path& filepath);

    ~ChunkBin();

    _NODISCARD const std::vector<BufferView>& GetBufferViews() const;
    
    _NODISCARD const std::vector<AccessorBuffer>& GetAccessorBuffers() const;
    
    _NODISCARD const std::vector<ImageData>& GetImages() const;
    
    _NODISCARD const std::vector<MeshData>& GetMeshes() const;
};
