#include "FontRenderer.h"

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "ZipFile.h"
#include "ResCache2.h"
#include "GameMain.h"
#include "GLSLShader.h"
#include "FontBufferCache.h"

namespace GameHalloran {
    FontRenderer::FontRenderer()
        : m_charset()
        , m_atlas(0)
        , m_font(0)
        , m_modelMat(g_identityMat)
        , m_text()
    {
    }

    FontRenderer::~FontRenderer()
    {
        if(m_atlas) {
            texture_atlas_delete(m_atlas);
            m_atlas = 0;
        }
        if(m_font) {
            texture_font_delete(m_font);
            m_font = 0;
        }
    }

    void FontRenderer::LoadFont(const std::string &fontName, const F32 fontsize)
    {
        Resource fontResource(std::string("fonts") + ZipFile::ZIP_PATH_SEPERATOR + fontName);
        boost::shared_ptr<ResHandle> fontHandle(g_appPtr->GetResourceCache()->GetHandle(&fontResource));

        if(!fontHandle) {
            GF_LOG_TRACE_ERR("FontRenderer::LoadFont", "Failed to load " + fontName);
            return;
        }

        m_atlas = texture_atlas_new(512, 512, 1);
        texture_atlas_clear(m_atlas);

        glBindTexture(GL_TEXTURE_2D, m_atlas->id);

        m_font = texture_font_new_memory_buffer(m_atlas, fontHandle->Buffer(), fontHandle->Size(), fontsize);
        std::wstring charset(m_charset.begin(), m_charset.end());
        texture_font_load_glyphs(m_font, charset.c_str());
    }

    void FontRenderer::PreRender()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_atlas->id);
        m_stackManager->GetModelViewMatrixStack()->PushMatrix(m_modelMat);
    }

    void FontRenderer::Render()
    {
        m_shader->SetUniform("texture", 0);
        m_shader->SetUniform("mvMat", m_stackManager->GetModelViewMatrixStack()->GetMatrix());
        m_shader->SetUniform("projection", m_stackManager->GetProjectionMatrixStack()->GetMatrix());
        m_shader->Activate();
        g_appPtr->GetFontBufferCachePtr()->Draw(m_text);
    }

    void FontRenderer::PostRender()
    {
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        m_stackManager->GetModelViewMatrixStack()->PopMatrix();
    }

    void FontRenderer::SetText(std::string text, const GameColor &color, const Point3 &position)
    {
        m_text.assign(text);
        m_modelMat.SetPosition(position);
        g_appPtr->GetFontBufferCachePtr()->Add(text, position, color, m_font);
    }
}
