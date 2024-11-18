#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererId;

public:
    VertexBuffer();
    ~VertexBuffer();

    void bind();
    void unbind();
};