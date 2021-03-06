#pragma once
#ifndef __GAME_FRAMEWORK_COLOURS_H
#define __GAME_FRAMEWORK_COLOURS_H

// /////////////////////////////////////////////////////////////////
// @file GameColors.h
// @author PJ O Halloran
// @date 27/07/2010
//
// File defines various colors used for graphics and screen elements
// throughout the application.
//
// It also holds classes which help to define an objects color in a 3D
// scene, namely Material and Light.
//
// /////////////////////////////////////////////////////////////////

#ifdef WIN32
#   pragma warning( push )
#   pragma warning( disable:4290 )
#endif

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "CommonMath.h"
#include "Vector.h"
#include "GameException.h"
#include "LuaStateManager.h"

namespace GameHalloran {
    // An OpenGL color needs 4 componenets (rgba) so we will just use our Vector4 class.
    typedef Vector4 GameColor;

    // Some often used colors.
    extern const GameColor g_gcBlack;
    extern const GameColor g_gcRed;
    extern const GameColor g_gcGreen;
    extern const GameColor g_gcBlue;
    extern const GameColor g_gcYellow;
    extern const GameColor g_gcMagenta;
    extern const GameColor g_gcCyan;
    extern const GameColor g_gcDarkGray;
    extern const GameColor g_gcLightGray;
    extern const GameColor g_gcBrown;
    extern const GameColor g_gcOrange;
    extern const GameColor g_gcPink;
    extern const GameColor g_gcPurple;
    extern const GameColor g_gcWhite;

    // Transparency values.
    extern const F32 g_OPAQUE;
    extern const F32 g_TRANSPARENT;

    // /////////////////////////////////////////////////////////////////
    // @class Material
    // @author PJ O Halloran
    //
    // The color and light properties of a 3D object.
    //
    // /////////////////////////////////////////////////////////////////
    class Material {
    private:
        GameColor m_ambient;                ///< Ambient color of a material.
        GameColor m_diffuse;                ///< Diffuse color of a material (light shading, dependant on object position with respect to light position).
        GameColor m_specular;               ///< Specular color of a material (shininess).
        F32 m_specPower;                    ///< Power of specular light (traditionally 128 from OpenGL Red book).
        GameColor m_emissive;               ///< Emissive color of a material (how much light it reflects or emits).

    public:
        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        Material();

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param ambient Ambient color of a material.
        // @param diffuse Diffuse color of a material.
        // @param specular Specular color of a material.
        // @param specPower Power of specular light.
        // @param emissiveEmissive color of a material.
        //
        // /////////////////////////////////////////////////////////////////
        Material(const GameColor &ambient,
                 const GameColor &diffuse,
                 const GameColor &specular,
                 const F32 specPower,
                 const GameColor &emissive);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Material();

        // /////////////////////////////////////////////////////////////////
        // Get the ambient color.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetAmbient() const;

        // /////////////////////////////////////////////////////////////////
        // Set the ambient color.
        //
        // /////////////////////////////////////////////////////////////////
        void SetAmbient(const GameColor &ambient);

        // /////////////////////////////////////////////////////////////////
        // Get the diffuse color.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetDiffuse() const;

        // /////////////////////////////////////////////////////////////////
        // Set the diffuse color.
        //
        // /////////////////////////////////////////////////////////////////
        void SetDiffuse(const GameColor &diffuse);

        // /////////////////////////////////////////////////////////////////
        // Get the specular color.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetSpecular() const;

        // /////////////////////////////////////////////////////////////////
        // Set the specular color.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSpecular(const GameColor &specular);

        // /////////////////////////////////////////////////////////////////
        // Get the specular power.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetSpecularPower() const;

        // /////////////////////////////////////////////////////////////////
        // Set the specular power.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSpecularPower(const F32 power);

        // /////////////////////////////////////////////////////////////////
        // Get the emissive color.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetEmissive() const;

        // /////////////////////////////////////////////////////////////////
        // Set the emissive color.
        //
        // /////////////////////////////////////////////////////////////////
        void SetEmissive(const GameColor &emissive);

        // /////////////////////////////////////////////////////////////////
        // Set the alpha value of the material.
        //
        // /////////////////////////////////////////////////////////////////
        void SetAlpha(const F32 alpha);

        // /////////////////////////////////////////////////////////////////
        // Get the alpha value of the material.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetAlpha() const;

        // /////////////////////////////////////////////////////////////////
        // Check if the material is partly transparent or not.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool HasAlpha() {
            return (GetAlpha() != g_OPAQUE);
        };

        // /////////////////////////////////////////////////////////////////
        // Reset the material to have a diffuse color (supplied), an ambient
        // and specular color of white and an emissive color of black.
        //
        // /////////////////////////////////////////////////////////////////
        void Reset(const GameColor &color);
    };

    // /////////////////////////////////////////////////////////////////
    // @class Light
    // @author PJ O Halloran
    //
    // This class holds the properties of a light in a 3D scene.  It uses
    // the ADS lighting model common in 3D scenes.  This lighting model
    // emulates the fixed function pipeline in earlier versions of OpenGL
    // (pre 3.3 which I'm learning from).
    //
    // I am using the properties defined in chapter 5 of the OpenGL
    // Red Book, 7th Edition.
    //
    // /////////////////////////////////////////////////////////////////
    class Light {
    public:

        // /////////////////////////////////////////////////////////////////
        // @enum LightType
        //
        // The type of light:
        // - eDirectional:  Infinitely far away light source (e.g. sun).
        // - ePositional:   Light sources positioned somewhere in the scene.
        //                  e.g. Candle.
        // - eSpotlight:    Light source positioned in the scene and emits
        //                  light in a specific direction.
        //
        // /////////////////////////////////////////////////////////////////
        enum LightType {
            eDirectional = 0,
            ePositional,
            eSpotlight,
            eNumTypes
        };

    private:

        LightType m_type;                   ///< Type of light.
        Point3 m_position;                  ///< Position of light.
        Vector3 m_direction;                ///< Direction of light source.
        GameColor m_ambient;                ///< Ambient property.
        GameColor m_diffuse;                ///< Diffuse property.
        GameColor m_specular;               ///< Specular property.
        F32 m_cAtt;                         ///< Constant attenuation factor.
        F32 m_lAtt;                         ///< Linear attenuation factor.
        F32 m_qAtt;                         ///< Quadratic attenuation factor.
        F32 m_slCutoff;                     ///< Angle of the cone of light for a spotlight.
        F32 m_slExponent;                   ///< Concentation factor of a spotlight.
        bool m_on;                          ///< Flag telling us if the light is currently on or off.

        // /////////////////////////////////////////////////////////////////
        // Sets the member light type variable if the contents of the lua
        // object are valid and known.
        //
        // @param lightTypeData LUA data.
        //
        // @return bool True on success or false on failure.
        //
        // /////////////////////////////////////////////////////////////////
        bool SetLightTypeFromLua(const LuaPlus::LuaObject &lightTypeData);

    public:

        // /////////////////////////////////////////////////////////////////
        // Default constructor.
        //
        // /////////////////////////////////////////////////////////////////
        Light();

        // /////////////////////////////////////////////////////////////////
        // Constructor that creates a light from script data.
        //
        // @param srcData Table of data passed in from an external lua script.
        //
        // @throw GameException& If the script data was not valid.
        //
        // /////////////////////////////////////////////////////////////////
        Light(LuaPlus::LuaObject &srcData) throw(GameException &);

        // /////////////////////////////////////////////////////////////////
        // Constructor.
        //
        // @param type Type of light.
        // @param position Position of light.
        // @param direction Direction of light source.
        // @param ambient Ambient property.
        // @param diffuse Diffuse property.
        // @param specular Specular property.
        // @param constantAttenuation Constant attenuation factor.
        // @param linearAttenuation Linear attenuation factor.
        // @param quadraticAttenuation Quadratic attenuation factor.
        // @param spotlightCutoff Angle of the cone of light for a spotlight.
        //                          (0.0 <= Angle <= 90.0).
        // @param spotlightExponent Concentation factor of a spotlight.
        //
        // /////////////////////////////////////////////////////////////////
        Light(const LightType type, \
              const Point3 &position, \
              const Vector3 &direction, \
              const GameColor &ambient, \
              const GameColor &diffuse, \
              const GameColor &specular, \
              const F32 constantAttenuation, \
              const F32 linearAttenuation, \
              const F32 quadraticAttenuation, \
              const F32 spotlightCutoff, \
              const F32 spotlightExponent);

        // /////////////////////////////////////////////////////////////////
        // Destructor.
        //
        // /////////////////////////////////////////////////////////////////
        ~Light();

        // /////////////////////////////////////////////////////////////////
        // Reset light source to default values.
        //
        // Directional light, no ambient, white diffuse and white specualar
        // properties.
        //
        // /////////////////////////////////////////////////////////////////
        void Reset();

        // /////////////////////////////////////////////////////////////////
        // Get the type of the light.
        //
        // /////////////////////////////////////////////////////////////////
        LightType GetLightType() const;

        // /////////////////////////////////////////////////////////////////
        // Set the type of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLightType(const LightType type);

        // /////////////////////////////////////////////////////////////////
        // Get the position of the light.
        //
        // /////////////////////////////////////////////////////////////////
        Point3 GetPosition() const;

        // /////////////////////////////////////////////////////////////////
        // Set the position of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetPosition(const Point3 &position);

        // /////////////////////////////////////////////////////////////////
        // Get the direction of the light.
        //
        // /////////////////////////////////////////////////////////////////
        Vector3 GetDirection() const;

        // /////////////////////////////////////////////////////////////////
        // Set the type of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetDirection(const Vector3 &direction);

        // /////////////////////////////////////////////////////////////////
        // Get the ambient property of the light.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetAmbient() const;

        // /////////////////////////////////////////////////////////////////
        // Set the ambient property of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetAmbient(const GameColor &ambient);

        // /////////////////////////////////////////////////////////////////
        // Get the diffuse property of the light.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetDiffuse() const;

        // /////////////////////////////////////////////////////////////////
        // Set the diffuse property of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetDiffuse(const GameColor &diffuse);

        // /////////////////////////////////////////////////////////////////
        // Get the specular property of the light.
        //
        // /////////////////////////////////////////////////////////////////
        GameColor GetSpecular() const;

        // /////////////////////////////////////////////////////////////////
        // Set the specular property of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSpecular(const GameColor &specular);

        // /////////////////////////////////////////////////////////////////
        // Get the constant attenuation factor of the light.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetConstantAttenuation() const;

        // /////////////////////////////////////////////////////////////////
        // Set the constant attenuation factor of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetConstantAttenuation(const F32 cAtt);

        // /////////////////////////////////////////////////////////////////
        // Get the linear attenuation factor of the light.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetLinearAttenuation() const;

        // /////////////////////////////////////////////////////////////////
        // Set the linear attenuation factor of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetLinearAttenuation(const F32 lAtt);

        // /////////////////////////////////////////////////////////////////
        // Get the quadratic attenuation factor of the light.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetQuadraticAttenuation() const;

        // /////////////////////////////////////////////////////////////////
        // Set the quadratic attenuation factor of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetQuadraticAttenuation(const F32 qAtt);

        // /////////////////////////////////////////////////////////////////
        // Get the spotlight cutoff angle of the light (in degrees between
        // 0 and 90).
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetSpotlightCutoff() const;

        // /////////////////////////////////////////////////////////////////
        // Set the spotlight cutoff angle of the light (in degrees between
        // 0 and 90).
        //
        // /////////////////////////////////////////////////////////////////
        void SetSpotlightCutoff(const F32 spotCutoff);

        // /////////////////////////////////////////////////////////////////
        // Get the spotlight exponent (intensity) of the light.
        //
        // /////////////////////////////////////////////////////////////////
        F32 GetSpotlightExponent() const;

        // /////////////////////////////////////////////////////////////////
        // Set the spotlight exponent (intensity) of the light.
        //
        // /////////////////////////////////////////////////////////////////
        void SetSpotlightExponent(const F32 spotExponent);

        // /////////////////////////////////////////////////////////////////
        // Check if the light is currently turned on.
        //
        // /////////////////////////////////////////////////////////////////
        inline bool IsOn() const {
            return (m_on);
        };

        // /////////////////////////////////////////////////////////////////
        // Toggle the light on or off.
        //
        // /////////////////////////////////////////////////////////////////
        inline void ToggleOn() {
            m_on = !m_on;
        };

    };

    // Typedef for a container of lights.
    typedef std::vector<boost::shared_ptr<Light> > LightVector;

}

#ifdef WIN32
#   pragma warning( pop )
#endif

#endif
