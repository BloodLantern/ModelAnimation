#include "gltf/gltf_chunk_bin.h"

#include <filesystem>
#include <iostream>
#include <glad/glad.h>

#include "stb_image.h"

ChunkBin::Buffer::Buffer(const char* const ptr, const int length)
    : ptr(ptr)
    , length(length)
{
}

ChunkBin::BufferView::BufferView(const char* bufferPtr, int bufferLength, const ChunkBufferView* bufferView)
    : Buffer(bufferPtr, bufferLength)
    , bufferView(bufferView)
{
}

ChunkBin::AccessorBuffer::AccessorBuffer(const char* bufferPtr, const int bufferLength, const unsigned int typeSize, const unsigned int componentCount, const unsigned int componentType, const bool normalized, const ChunkAccessor* const accessor)
    : Buffer(bufferPtr, bufferLength)
    , typeSize(typeSize)
    , componentCount(componentCount)
    , componentType(componentType)
    , normalized(normalized)
    , accessor(accessor)
{
}

ChunkBin::ChunkBin(const unsigned int length, char type[4], std::ifstream& file, const ChunkJson* const jsonHeader, const std::filesystem::path& filepath)
    : Chunk(length, type)
    , m_JsonHeader(jsonHeader)
{
    // ChunkBuffer
    m_RawBuffers.resize(jsonHeader->buffers.size());
    for (unsigned int i = 0; i < jsonHeader->buffers.size(); i++)
    {
        const ChunkBuffer& buffer = jsonHeader->buffers[i];
        
        if (buffer.uri.empty())
        {
            if (static_cast<unsigned int>(buffer.byteLength) != length)
                std::cout << "Buffer byteLength (" << buffer.byteLength << ") is different from binary buffer length (" << length << "). Using byteLength\n";
            
            m_RawBuffers[i] = new char[buffer.byteLength];
            file.read(m_RawBuffers[i], buffer.byteLength);
        }
        else
        {
            std::ifstream rawBufferFile(relative(buffer.uri, filepath));
            m_RawBuffers[i] = new char[buffer.byteLength];
            rawBufferFile.read(m_RawBuffers[i], buffer.byteLength);
        }
    }
    
    // Padding
    while (file.peek() == '\0')
        file.get();

    // ChunkBufferView
    m_BufferViews.resize(jsonHeader->bufferViews.size());
    for (unsigned int i = 0; i < jsonHeader->bufferViews.size(); i++)
    {
        const ChunkBufferView& bufferView = jsonHeader->bufferViews[i];
        
        if (bufferView.target != 34962 && bufferView.target != 34963)
            std::cout << "Invalid buffer view target: " << bufferView.target << ", expected 34962 or 34963\n";
        else
            m_BufferViews[i] = BufferView(m_RawBuffers[bufferView.buffer] + bufferView.byteOffset, bufferView.byteLength, &bufferView);
    }

    // ChunkAccessor
    m_AccessorBuffers.resize(jsonHeader->accessors.size());
    for (unsigned int i = 0; i < jsonHeader->accessors.size(); i++)
    {
        const ChunkAccessor& accessor = jsonHeader->accessors[i];
        const BufferView& view = m_BufferViews[accessor.bufferView];

        int typeSize;
        switch (accessor.componentType)
        {
            case GL_BYTE:
                typeSize = sizeof(char);
                break;
            
            case GL_UNSIGNED_BYTE:
                typeSize = sizeof(unsigned char);
                break;
            
            case GL_SHORT:
                typeSize = sizeof(short);
                break;
            
            case GL_UNSIGNED_SHORT:
                typeSize = sizeof(unsigned short);
                break;
            
            case GL_UNSIGNED_INT:
                typeSize = sizeof(unsigned int);
                break;
            
            case GL_FLOAT:
                typeSize = sizeof(float);
                break;

            default:
                std::cout << "Invalid component type in accessor: " << accessor.componentType << " -> 0x" << std::hex << accessor.componentType << '\n';
                continue;
        }

        unsigned int componentCount;
        if (accessor.type == "SCALAR")
            componentCount = 1;
        else if (accessor.type == "VEC2")
            componentCount = 2;
        else if (accessor.type == "VEC3")
            componentCount = 3;
        else if (accessor.type == "VEC4")
            componentCount = 4;
        else if (accessor.type == "MAT2")
            componentCount = 2 * 2;
        else if (accessor.type == "MAT3")
            componentCount = 3 * 3;
        else if (accessor.type == "MAT4")
            componentCount = 4 * 4;
        else
        {
            std::cout << "Invalid type in accessor: " << accessor.type << '\n';
            continue;
        }

        // TODO: Sparse accessors
        
        m_AccessorBuffers[i] = AccessorBuffer(view.ptr + accessor.byteOffset, static_cast<int>(accessor.count * typeSize * componentCount), typeSize, componentCount, accessor.componentType, accessor.normalized, &accessor);
    }
    
    m_Images.resize(jsonHeader->images.size());
    for (unsigned int i = 0; i < jsonHeader->images.size(); i++)
    {
        const ChunkImage& image = jsonHeader->images[i];
        const BufferView& view = m_BufferViews[image.bufferView];

        ImageData data;
        data.data = stbi_load_from_memory(reinterpret_cast<const unsigned char* const>(view.ptr), view.length, &data.width, &data.height, &data.channels, 4);
        m_Images[i] = data;
    }
    
    m_Meshes.resize(jsonHeader->meshes.size());
    for (unsigned int i = 0; i < jsonHeader->meshes.size(); i++)
    {
        const ChunkMesh& mesh = jsonHeader->meshes[i];

        MeshData data;
        data.primitives.resize(mesh.primitives.size());
        for (unsigned int j = 0; j < mesh.primitives.size(); j++)
        {
            const MeshPrimitive& primitive = mesh.primitives[j];
            MeshPrimitiveData& primitiveData = data.primitives[j];

            primitiveData.attributes.reserve(primitive.attributes.MemberCount());
            for (auto it = primitive.attributes.MemberBegin(); it != primitive.attributes.MemberEnd(); it++)
                primitiveData.attributes.emplace(it->name.GetString(), it->value.GetInt());
            
            primitiveData.indices = primitive.indices;
            primitiveData.material = primitive.material;
            primitiveData.mode = primitive.mode;
        }
        m_Meshes[i] = data;
    }
}

ChunkBin::~ChunkBin()
{
    for (const char* const rawBuffer : m_RawBuffers)
        delete rawBuffer;
    for (const ImageData& imageData : m_Images)
        stbi_image_free(imageData.data);
}

const std::vector<ChunkBin::BufferView>& ChunkBin::GetBufferViews() const
{
    return m_BufferViews;
}

const std::vector<ChunkBin::AccessorBuffer>& ChunkBin::GetAccessorBuffers() const
{
    return m_AccessorBuffers;
}

const std::vector<ChunkBin::ImageData>& ChunkBin::GetImages() const
{
    return m_Images;
}

const std::vector<ChunkBin::MeshData>& ChunkBin::GetMeshes() const
{
    return m_Meshes;
}
