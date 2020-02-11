//
// Copyright (C) 2019 Google, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include <cstdint>
#include <cstdio>
#include <memory>
#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../../../SPIRV/GlslangToSpv.h"
#include "../../../glslang/Include/intermediate.h"
#include "../../../glslang/Public/ShaderLang.h"

#ifndef __EMSCRIPTEN__
#define EMSCRIPTEN_KEEPALIVE
#endif

const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

static bool initialized = false;

extern "C" {

/*
 * Takes in a GLSL shader as a string and converts it to SPIR-V in binary form.
 *
 * |glsl|          Null-terminated string containing the shader to be converted.
 * |stage_int|     Magic number indicating the type of shader being processed.
*                  Legal values are as follows:
 *                   Vertex = 0
 *                   Fragment = 4
 *                   Compute = 5
 * |gen_debug|     Flag to indicate if debug information should be generated.
 * |spirv|         Output parameter for a pointer to the resulting SPIR-V data.
 * |spirv_len|     Output parameter for the length of the output binary buffer.
 *
 * Returns a void* pointer which, if not null, must be destroyed by
 * destroy_output_buffer.o. (This is not the same pointer returned in |spirv|.)
 * If null, the compilation failed.
 */
EMSCRIPTEN_KEEPALIVE
void* convert_glsl_to_spirv(const char* glsl,
                            int stage_int,
                            bool gen_debug,
                            glslang::EShTargetLanguageVersion spirv_version,
                            uint32_t** spirv,
                            size_t* spirv_len)
{
    if (glsl == nullptr) {
        fprintf(stderr, "Input pointer null\n");
        return nullptr;
    }
    if (spirv == nullptr || spirv_len == nullptr) {
        fprintf(stderr, "Output pointer null\n");
        return nullptr;
    }
    *spirv = nullptr;
    *spirv_len = 0;

    if (stage_int != 0 && stage_int != 4 && stage_int != 5) {
        fprintf(stderr, "Invalid shader stage\n");
        return nullptr;
    }
    EShLanguage stage = static_cast<EShLanguage>(stage_int);
    switch (spirv_version) {
    case glslang::EShTargetSpv_1_0:
    case glslang::EShTargetSpv_1_1:
    case glslang::EShTargetSpv_1_2:
    case glslang::EShTargetSpv_1_3:
    case glslang::EShTargetSpv_1_4:
    case glslang::EShTargetSpv_1_5:
        break;
    default:
        fprintf(stderr, "Invalid SPIR-V version number\n");
        return nullptr;
    }

    if (!initialized) {
        glslang::InitializeProcess();
        initialized = true;
    }

    glslang::TShader shader(stage);
    shader.setStrings(&glsl, 1);
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, spirv_version);
    if (!shader.parse(&DefaultTBuiltInResource, 100, true, EShMsgDefault)) {
        fprintf(stderr, "Parse failed\n");
        fprintf(stderr, "%s\n", shader.getInfoLog());
        return nullptr;
    }

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(EShMsgDefault)) {
        fprintf(stderr, "Link failed\n");
        fprintf(stderr, "%s\n", program.getInfoLog());
        return nullptr;
    }

    glslang::SpvOptions spvOptions;
    spvOptions.generateDebugInfo = gen_debug;
    spvOptions.optimizeSize = false;
    spvOptions.disassemble = false;
    spvOptions.validate = false;

    std::vector<uint32_t>* output = new std::vector<uint32_t>;
    glslang::GlslangToSpv(*program.getIntermediate(stage), *output, nullptr, &spvOptions);

    *spirv_len = output->size();
    *spirv = output->data();
    return output;
}

static char const *get_type_str(glslang::TIntermSymbol* symbol) {
    using namespace glslang;
    static const char* TypeString[] = {
    "bool",  "bvec2", "bvec3", "bvec4",
    "float",  "vec2",  "vec3",  "vec4",
    "int",   "ivec2", "ivec3", "ivec4",
    "uint",  "uvec2", "uvec3", "uvec4",
    "float",  "mat2", "mat3", "mat4",
    };
    int isf = symbol->getBasicType() == EbtFloat ? 1 : 0;
    int isu = symbol->getBasicType() == EbtUint ? 1 : 0;
    int isi = symbol->getBasicType() == EbtInt ? 1 : 0;
    int rows = symbol->getType().getMatrixRows();
    int comps = symbol->getType().getVectorSize();
    int isv = symbol->getType().isVector() ? 1 : 0;
    int iss = symbol->getType().isScalar() ? 1 : 0;
    int ism = symbol->getType().isMatrix() ? 1 : 0;
    int rowid = ism ? 4 : isf * 1 + isu * 3 + isi * 2;
    int colid = isv * (comps - 1) + ism * (rows - 1);
    return TypeString[colid + rowid * 4];
}

EMSCRIPTEN_KEEPALIVE
char *parse_attributes(const char* glsl, int stage_int, char **out_str, size_t *out_size)
{
    using namespace glslang;
    if (glsl == nullptr) {
        fprintf(stderr, "Input pointer null\n");
        return nullptr;
    }

    if (stage_int != 0 && stage_int != 4 && stage_int != 5) {
        fprintf(stderr, "Invalid shader stage\n");
        return nullptr;
    }
    if (out_size == nullptr || out_str == nullptr) {
        fprintf(stderr, "Output pointer null\n");
        return nullptr;
    }
    *out_size = 0;
    *out_str = nullptr;

    EShLanguage stage = static_cast<EShLanguage>(stage_int);

    if (!initialized) {
        glslang::InitializeProcess();
        initialized = true;
    }

    glslang::TShader shader(stage);
    shader.setStrings(&glsl, 1);
    shader.setAutoMapBindings(true);
    shader.setAutoMapLocations(true);
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientOpenGL, 100);
    shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
    if (!shader.parse(&DefaultTBuiltInResource, 100, true, EShMsgDefault)) {
        fprintf(stderr, "Parse failed\n");
        fprintf(stderr, "%s\n", shader.getInfoLog());
        return nullptr;
    }

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(EShMsgDefault)) {
        fprintf(stderr, "Link failed\n");
        fprintf(stderr, "%s\n", program.getInfoLog());
        return nullptr;
    }

    struct MyVisitor : public TIntermTraverser {
        bool is_vertex_shader;
        std::stringstream attributes;
        std::stringstream uniforms;
        std::unordered_set<TString> visited_nodes;
        bool error = false;
        void visitSymbol(TIntermSymbol* symbol) override
        {
            if (visited_nodes.find(symbol->getName()) != visited_nodes.end())
                    return;
            visited_nodes.insert(symbol->getName());
            if (is_vertex_shader &&
                  (symbol->getType().getQualifier().storage == EvqVaryingIn ||
                   symbol->getType().getQualifier().storage == EvqIn)) {
                attributes << "{\"name\":\"" << symbol->getName() << "\",\"type\":\"" << get_type_str(symbol) << "\"},";
            } else if (symbol->getType().getQualifier().storage == EvqUniform) {
                uniforms << "{\"name\":\"" << symbol->getName() << "\",\"type\":\"" << get_type_str(symbol) << "\"},";
            }

        }
    };
    MyVisitor visitor;
    visitor.attributes << "\"attributes\":[";
    visitor.uniforms << "\"uniforms\":[";
    visitor.is_vertex_shader = stage_int == 0;
    program.getIntermediate(stage)->getTreeRoot()->traverse(&visitor);
    if (visitor.error)
        return nullptr;
    visitor.attributes << "{}]";
    visitor.uniforms << "{}]";
    std::stringstream json;
    json << "{" << visitor.attributes.str() << "," << visitor.uniforms.str() << "}";
    auto str = json.str();
    char *out = (char *)malloc(str.size() + 1);
    if (!out)
        return nullptr;
    memcpy(out, str.c_str(), str.size() + 1);
    out[str.size()] = '\0';
    *out_str = out;
    *out_size = str.size() + 1;
    return out;
}

/*
 * Destroys a buffer created by parse_attributes
 */
EMSCRIPTEN_KEEPALIVE
void destroy_output_string(void* p)
{
    free(p);
}

/*
 * Destroys a buffer created by convert_glsl_to_spirv
 */
EMSCRIPTEN_KEEPALIVE
void destroy_output_buffer(void* p)
{
    delete static_cast<std::vector<uint32_t>*>(p);
}

}  // extern "C"

/*
 * For non-Emscripten builds we supply a generic main, so that the glslang.js
 * build target can generate an executable with a trivial use case instead of
 * generating a WASM binary. This is done so that there is a target that can be
 * built and output analyzed using desktop tools, since WASM binaries are
 * specific to the Emscripten toolchain.
 */
#ifndef __EMSCRIPTEN__
int main() {
    const char* input = R"(#version 310 es

void main() { })";

    uint32_t* output;
    size_t output_len;

    void* id = convert_glsl_to_spirv(input, 4, false, glslang::EShTargetSpv_1_0, &output, &output_len);
    assert(output != nullptr);
    assert(output_len != 0);
    destroy_output_buffer(id);
    return 0;
}
#endif  // ifndef __EMSCRIPTEN__
