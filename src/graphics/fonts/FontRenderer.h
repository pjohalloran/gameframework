#pragma once
#ifndef __FONT_RENDERER_H
#define __FONT_RENDERER_H

#include <string>

#include <boost/shared_ptr.hpp>

#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#include "GameTypes.h"
#include "GameColors.h"
#include "Matrix.h"
#include "ModelViewProjStackManager.h"

namespace GameHalloran {
    class Point3;
    class GLSLShader;

    class FontRenderer {
    public:
        FontRenderer();

        virtual ~FontRenderer();

        inline void SetFontCharset(const std::string &charset) {
            m_charset = charset;
        };

        inline void SetShader(boost::shared_ptr<GLSLShader> shader) {
            m_shader = shader;
        };

        inline void SetMatrictStack(boost::shared_ptr<ModelViewProjStackManager> stackManager) {
            m_stackManager = stackManager;
        };

        void LoadFont(const std::string &fontName, const F32 fontsize);

        void SetText(std::string text, const GameColor &color, const Point3 &position);

        void PreRender();

        void Render();

        void PostRender();

    protected:

    private:

        std::string m_charset;
        std::string m_text;
        texture_atlas_t *m_atlas;
        texture_font_t *m_font;
        boost::shared_ptr<GLSLShader> m_shader;
        Matrix4 m_modelMat;
        boost::shared_ptr<ModelViewProjStackManager> m_stackManager;
    };
}

#endif // __FONT_RENDERER_H
