#include "DrawUtils.h"
#include "includes/opengl-common.hpp"

namespace
{
GLenum MapPrimitiveType(PrimitiveType primitive)
{
    switch (primitive)
    {
    case PrimitiveType::Lines:
        return GL_LINES;
    case PrimitiveType::Points:
        return GL_POINTS;
    case PrimitiveType::TriangleFan:
        return GL_TRIANGLE_FAN;
    case PrimitiveType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case PrimitiveType::Triangles:
        return GL_TRIANGLES;
    default:
        throw std::runtime_error("Unhandled primitive type");
    }
}
}

glm::mat4 CDrawUtils::GetNormalMatrix(const glm::mat4 &modelView)
{
    return glm::transpose(glm::inverse(modelView));
}

void CDrawUtils::DrawRangeElements(const SGeometryLayout &layout)
{
    const GLenum primitive = MapPrimitiveType(layout.m_primitive);
    const GLuint start = 0u;
    const GLuint end = unsigned(layout.m_vertexCount);
    const GLsizei count = GLsizei(layout.m_indexCount);

    // Указатель используется вместо целочисленной переменной
    //  по историческим причинам: ранее семейство функций glDrawElements
    //  принимало указатель на область оперативной памяти,
    //  а сейчас принимает смещение от начала буфера в видеопамяти.
    const void *offset = reinterpret_cast<const void*>(layout.m_baseIndexOffset);

    // glDrawRangeElements эффективнее, чем glDrawElements,
    //  т.к. ему передаются максимальный и минимальный индекс вершины,
    //  что позволяет видеокарте определить и подготовить область памяти
    //  с вершинными данными.
    glDrawRangeElements(primitive, start, end, count, GL_UNSIGNED_INT,
                        reinterpret_cast<const void*>(offset));
}

void CDrawUtils::MaybeBind(const CTexture2DSharedPtr &pTexture)
{
    if (pTexture)
    {
        pTexture->Bind();
    }
    else
    {
        CTexture2D::Unbind();
    }
}
