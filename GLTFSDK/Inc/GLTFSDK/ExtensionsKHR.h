// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <GLTFSDK/ExtensionHandlers.h>
#include <GLTFSDK/IndexedContainer.h>
#include <GLTFSDK/Optional.h>

#include <memory>
#include <string>

namespace Microsoft
{
    namespace glTF
    {
        namespace KHR
        {
            ExtensionSerializer   GetKHRExtensionSerializer();
            ExtensionDeserializer GetKHRExtensionDeserializer();

            namespace Materials
            {
                constexpr const char* PBRSPECULARGLOSSINESS_NAME = "KHR_materials_pbrSpecularGlossiness";

                // KHR_materials_pbrSpecularGlossiness
                struct PBRSpecularGlossiness : Extension, glTFProperty
                {
                    PBRSpecularGlossiness();

                    Color4 diffuseFactor;
                    TextureInfo diffuseTexture;
                    Color3 specularFactor;
                    float glossinessFactor;
                    TextureInfo specularGlossinessTexture;

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;
                };

                std::string SerializePBRSpecGloss(const PBRSpecularGlossiness& specGloss, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                std::unique_ptr<Extension> DeserializePBRSpecGloss(const std::string& json, const ExtensionDeserializer& extensionDeserializer);

                constexpr const char* UNLIT_NAME = "KHR_materials_unlit";

                // KHR_materials_unlit
                struct Unlit : Extension, glTFProperty
                {
                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;
                };

                std::string SerializeUnlit(const Unlit& unlit, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                std::unique_ptr<Extension> DeserializeUnlit(const std::string& json, const ExtensionDeserializer& extensionDeserializer);
            }

            namespace MeshPrimitives
            {
                constexpr const char* DRACOMESHCOMPRESSION_NAME = "KHR_draco_mesh_compression";

                // KHR_draco_mesh_compression
                struct DracoMeshCompression : Extension, glTFProperty
                {
                    std::string bufferViewId;

                    std::unordered_map<std::string, uint32_t> attributes;

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;
                };

                std::string SerializeDracoMeshCompression(const DracoMeshCompression& dracoMeshCompression, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                std::unique_ptr<Extension> DeserializeDracoMeshCompression(const std::string& json, const ExtensionDeserializer& extensionDeserializer);
            }

            namespace TextureInfos
            {
                constexpr const char* TEXTURETRANSFORM_NAME = "KHR_texture_transform";

                // KHR_texture_transform
                struct TextureTransform : Extension, glTFProperty
                {
                    TextureTransform();
                    TextureTransform(const TextureTransform&);

                    Vector2 offset;
                    float rotation;
                    Vector2 scale;
                    Optional<size_t> texCoord; // TexCoord is an optional property

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;
                };

                std::string SerializeTextureTransform(const TextureTransform& textureTransform, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                std::unique_ptr<Extension> DeserializeTextureTransform(const std::string& json, const ExtensionDeserializer& extensionDeserializer);
            }

            // TODO (matt): go look at some implementations
            // NOTE (matt): there are some shared properties and some light specific
            // REFERENCE: https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_lights_punctual/README.md
            namespace Lights
            {
                constexpr const char* LIGHTSPUNCTUAL_NAME = "KHR_lights_punctual";

                struct Punctual : Extension, glTFChildOfRootProperty
                {
                    Punctual();

                    enum LightType
                    {
                        SPOT,
                        POINT,
                        DIRECTIONAL
                    };

                    struct Node : Extension, glTFProperty
                    {
                        Node() : Extension(), glTFProperty(), light("") {}
                        std::string light;
                        std::unique_ptr<Extension> Clone() const override;
                        bool IsEqual(const Extension &rhs) const override;
                    };

                    std::string type; // required 
                    std::string name;
                    Color3 color;
                    float intensity;
                    float range;
                    // required for spot light
                    float innerConeAngle;
                    float outerConeAngle;

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;

                    LightType GetLightType() const
                    {
                        if(type == "spot") return SPOT;
                        if(type == "directional") return DIRECTIONAL;
                        return POINT;
                    }
                };

                // Note (matt): this is to be the thing to serialize at the document level
                // need to get this working...
                struct LightExtension : Extension, glTFProperty
                {
                    std::string hax;
                    IndexedContainer<const Punctual> lights;

                    std::unique_ptr<Extension> Clone() const override;
                    bool IsEqual(const Extension& rhs) const override;
                };

                std::string SerializeLights(const LightExtension& lightExt, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                std::string SerializeForNode(const Punctual::Node& lightNode, const Document& gltfDocument, const ExtensionSerializer& extensionSerializer);
                // this one is for the document level data
                std::unique_ptr<Extension> DeserializeLight(const std::string& json, const ExtensionDeserializer& extensionDeserializer);
                // this one is for the node level data (matrix, index into light array)
                std::unique_ptr<Extension> DeserializeLightNode(const std::string& json, const ExtensionDeserializer& extensionDeserializer);
            }
        }
    }
}
