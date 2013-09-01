#ifndef gameframework_Header_h
#define gameframework_Header_h

//
//  Header.h
//  gameframework
//
//  Created by PJ O Halloran on 06/08/2013.
//
//

#include <string>
#include <map>

#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/vertex-buffer.h>

#include "GameTypes.h"
#include "Vector.h"
#include "GameColors.h"
#include "HashedString.h"

namespace GameHalloran {
    class FontBufferCache {
    public:
        FontBufferCache();

        virtual ~FontBufferCache();

        void Clear();

        bool Contains(const std::string &text) const;

        void Add(const std::string &text, const Point3 &position, const GameColor &color, texture_font_t *font);

        void Draw(const std::string &text) const;

    private:
        typedef std::map<U64, vertex_buffer_t *> VertexBufferMap;
        VertexBufferMap m_vertexBufferMap;
    };
}

#endif
