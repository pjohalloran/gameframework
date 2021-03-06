// /////////////////////////////////////////////////////////////////
// @file GameColors.cpp
// @author PJ O Halloran
// @date 27/07/2010
//
// File contains some global used colors and implementations of the
// Light and Material classes.
//
// /////////////////////////////////////////////////////////////////

#include <string>

#include <boost/algorithm/string/case_conv.hpp>

#include "GameColors.h"
#include "GameMain.h"

// Namespace Declarations
using std::string;

namespace GameHalloran {

    const GameColor g_gcBlack(0.0f, 0.0f, 0.0f, 1.0f);
    const GameColor g_gcRed(1.0f, 0.0f, 0.0f, 1.0f);
    const GameColor g_gcGreen(0.0f, 1.0f, 0.0f, 1.0f);
    const GameColor g_gcBlue(0.0f, 0.0f, 1.0f, 1.0f);
    const GameColor g_gcYellow(1.0f, 1.0f, 0.0f, 1.0f);
    const GameColor g_gcMagenta(1.0f, 0.0f, 1.0f, 1.0f);
    const GameColor g_gcCyan(0.0f, 1.0f, 1.0f, 1.0f);
    const GameColor g_gcDarkGray(0.25f, 0.25f, 0.25f, 1.0f);
    const GameColor g_gcLightGray(0.75f, 0.75f, 0.75f, 1.0f);
    const GameColor g_gcBrown(0.60f, 0.40f, 0.12f, 1.0f);
    const GameColor g_gcOrange(0.98f, 0.625f, 0.12f, 1.0f);
    const GameColor g_gcPink(0.98f, 0.04f, 0.7f, 1.0f);
    const GameColor g_gcPurple(0.60f, 0.40f, 0.70f, 1.0f);
    const GameColor g_gcWhite(1.0f, 1.0f, 1.0f, 1.0f);

    const F32 g_OPAQUE = 1.0f;
    const F32 g_TRANSPARENT = 0.0f;

    // /////////////////////////////////////////////////////////////////
    // *********************** Material *******************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Material::Material()
        : m_ambient(g_gcWhite)
        , m_diffuse(g_gcWhite)
        , m_specular(g_gcWhite)
        , m_specPower(128.0f)
        , m_emissive(g_gcBlack)
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Material::Material(const GameColor &ambient,
                       const GameColor &diffuse,
                       const GameColor &specular,
                       const F32 specPower,
                       const GameColor &emissive)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_specPower(specPower)
        , m_emissive(emissive)
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Material::~Material()
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Material::GetAmbient() const
    {
        return (m_ambient);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::SetAmbient(const GameColor &ambient)
    {
        m_ambient = ambient;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Material::GetDiffuse() const
    {
        return (m_diffuse);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::SetDiffuse(const GameColor &diffuse)
    {
        m_diffuse = diffuse;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Material::GetSpecular() const
    {
        return (m_specular);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::SetSpecular(const GameColor &specular)
    {
        m_specular = specular;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Material::GetSpecularPower() const
    {
        return (m_specPower);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::SetSpecularPower(const F32 power)
    {
        m_specPower = power;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Material::GetEmissive() const
    {
        return (m_emissive);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::SetEmissive(const GameColor &emissive)
    {
        m_emissive = emissive;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::SetAlpha(const F32 alpha)
    {
        F32 clampedAlpha = alpha;
        Clamp<F32>(clampedAlpha, 0.0f, 1.0f);
        m_ambient.SetW(clampedAlpha);
        m_diffuse.SetW(clampedAlpha);
        m_specular.SetW(clampedAlpha);
        m_emissive.SetW(clampedAlpha);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Material::GetAlpha() const
    {
        return (m_diffuse.GetW());
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Material::Reset(const GameColor &color)
    {
        m_diffuse = color;
        m_ambient = g_gcWhite;
        m_specular = g_gcWhite;
        m_emissive = g_gcBlack;
    }

    // /////////////////////////////////////////////////////////////////
    // ************************* Light *********************************
    // /////////////////////////////////////////////////////////////////

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    bool Light::SetLightTypeFromLua(const LuaPlus::LuaObject &lightTypeData)
    {
        bool result = true;

        if(lightTypeData.IsString()) {
            std::string typeStr(lightTypeData.GetString());
            boost::algorithm::to_lower(typeStr);
            if(typeStr.compare("positional") == 0) {
                m_type = ePositional;
            } else if(typeStr.compare("directional") == 0) {
                m_type = eDirectional;
            } else if(typeStr.compare("spotlight") == 0) {
                m_type = eSpotlight;
            } else {
                result = false;
            }
        }

        return (result);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Light::Light()
        : m_type(eDirectional)
        , m_position(g_originPt)
        , m_direction(g_forward)
        , m_ambient(g_gcBlack)
        , m_diffuse(g_gcWhite)
        , m_specular(g_gcWhite)
        , m_cAtt(1.0f)
        , m_lAtt(0.0f)
        , m_qAtt(0.0f)
        , m_slCutoff(180.0f)
        , m_slExponent(0.0f)
        , m_on(true)
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Light::Light(LuaPlus::LuaObject &srcData) throw(GameException &)
        : m_type(eDirectional)
        , m_position(g_originPt)
        , m_direction(g_forward)
        , m_ambient(g_gcBlack)
        , m_diffuse(g_gcWhite)
        , m_specular(g_gcWhite)
        , m_cAtt(1.0f)
        , m_lAtt(0.0f)
        , m_qAtt(0.0f)
        , m_slCutoff(180.0f)
        , m_slExponent(0.0f)
        , m_on(true)
    {
        if(!srcData.IsTable()) {
            throw GameException(std::string("Invalid script data"));
        }

        // All fields may be optionally entered by the scripter, if they dont supply a parameter, the default is used.

        // LightType
        if(!SetLightTypeFromLua(srcData["LightType"])) {
            throw GameException(std::string("Invalid light type supplied"));
        }

        // Other light parameters.
        SetPoint3FromLua(srcData["Position"], m_position);
        SetVector3FromLua(srcData["Direction"], m_direction);
        SetColorFromLua(srcData["Ambient"], m_ambient);
        SetColorFromLua(srcData["Diffuse"], m_diffuse);
        SetColorFromLua(srcData["Specular"], m_specular);
        SetFloatFromLua(srcData["ConstantAttenuation"], m_cAtt);
        SetFloatFromLua(srcData["LinearAttenuation"], m_lAtt);
        SetFloatFromLua(srcData["QuadraticAttenuation"], m_qAtt);
        SetFloatFromLua(srcData["SpotlightCutoff"], m_slCutoff);
        SetFloatFromLua(srcData["SpotlightExponent"], m_slExponent);
        SetBoolFromLua(srcData["On"], m_on);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Light::Light(const LightType type,
                 const Point3 &position,
                 const Vector3 &direction,
                 const GameColor &ambient,
                 const GameColor &diffuse,
                 const GameColor &specular,
                 const F32 constantAttenuation,
                 const F32 linearAttenuation,
                 const F32 quadraticAttenuation,
                 const F32 spotlightCutoff,
                 const F32 spotlightExponent)
        : m_type(type)
        , m_position(position)
        , m_direction(direction)
        , m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_cAtt(constantAttenuation)
        , m_lAtt(linearAttenuation)
        , m_qAtt(quadraticAttenuation)
        , m_slCutoff(spotlightCutoff)
        , m_slExponent(spotlightExponent)
        , m_on(true)
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Light::~Light()
    {
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::Reset()
    {
        m_type = eDirectional;
        m_position = g_originPt;
        m_direction = g_forward;
        m_ambient = g_gcBlack;
        m_diffuse = g_gcWhite;
        m_specular = g_gcWhite;
        m_cAtt = 1.0f;
        m_lAtt = 0.0f;
        m_qAtt = 0.0f;
        m_slCutoff = 180.0f;
        m_slExponent = 0.0f;
        m_on = true;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Light::LightType Light::GetLightType() const
    {
        return (m_type);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetLightType(const LightType type)
    {
        m_type = type;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Point3 Light::GetPosition() const
    {
        return (m_position);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetPosition(const Point3 &position)
    {
        m_position = position;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    Vector3 Light::GetDirection() const
    {
        return (m_direction);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetDirection(const Vector3 &direction)
    {
        m_direction = direction;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Light::GetAmbient() const
    {
        return (m_ambient);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetAmbient(const GameColor &ambient)
    {
        m_ambient = ambient;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Light::GetDiffuse() const
    {
        return (m_diffuse);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetDiffuse(const GameColor &diffuse)
    {
        m_diffuse = diffuse;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    GameColor Light::GetSpecular() const
    {
        return (m_specular);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetSpecular(const GameColor &specular)
    {
        m_specular = specular;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Light::GetConstantAttenuation() const
    {
        if(m_type == eDirectional) {
            return (1.0f);
        }
        return (m_cAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetConstantAttenuation(const F32 cAtt)
    {
        m_cAtt = cAtt;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Light::GetLinearAttenuation() const
    {
        if(m_type == eDirectional) {
            return (0.0f);
        }
        return (m_lAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetLinearAttenuation(const F32 lAtt)
    {
        m_lAtt = lAtt;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Light::GetQuadraticAttenuation() const
    {
        if(m_type == eDirectional) {
            return (0.0f);
        }
        return (m_qAtt);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetQuadraticAttenuation(const F32 qAtt)
    {
        m_qAtt = qAtt;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Light::GetSpotlightCutoff() const
    {
        if(m_type == eDirectional) {
            return (180.0f);
        }
        return (m_slCutoff);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetSpotlightCutoff(const F32 spotCutoff)
    {
        m_slCutoff = spotCutoff;
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    F32 Light::GetSpotlightExponent() const
    {
        if(m_type == eDirectional) {
            return (0.0f);
        }
        return (m_slExponent);
    }

    // /////////////////////////////////////////////////////////////////
    //
    // /////////////////////////////////////////////////////////////////
    void Light::SetSpotlightExponent(const F32 spotExponent)
    {
        m_slExponent = spotExponent;
    }

}
