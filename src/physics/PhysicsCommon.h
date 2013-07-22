#pragma once
#ifndef __GF_PHYSICS_COMMON_H
#define __GF_PHYSICS_COMMON_H

//========================================================================
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here:
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
//
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file PhysicsCommon.h
// @author PJ O Halloran & Mike McShaffry.
// @date 14/10/2010
//
// Common physics system enums, constants, functions, data, etc.
//
// /////////////////////////////////////////////////////////////////

#include "GameTypes.h"

namespace GameHalloran {

    // /////////////////////////////////////////////////////////////////
    // @struct MaterialData
    // @author Mike McShaffry.
    //
    // Holds the retitution and friction values for a physics material.
    //
    // /////////////////////////////////////////////////////////////////
    struct MaterialData {
        F32 m_restitution;                  ///< A materials/objects bounce.
        F32 m_friction;                     ///< A materials friction.
    };

    // /////////////////////////////////////////////////////////////////
    // @enum PhysicsMaterial
    // @author Mike McShaffry
    //
    // Used to easily find materials in the global g_PhysicsMaterials
    // array of MaterialData structs.
    //
    // /////////////////////////////////////////////////////////////////
    enum PhysicsMaterial {
        PhysMat_Playdough,
        PhysMat_Normal,
        PhysMat_Bouncy,
        PhysMat_Slippery,
        PhysMat_MaxMaterials
    };

    // /////////////////////////////////////////////////////////////////
    // g_PhysicsMaterials Description.
    //
    // The enum PhysicsMaterial, is used to find the material you want
    // easily.
    //
    // Predefines some useful physics materials. Define new ones here, and
    // have similar objects use it, so if you ever need to change it you'll
    // only have to change it here.
    // - Chapter 15, page 535
    //
    // /////////////////////////////////////////////////////////////////
    static const MaterialData g_PhysicsMaterials[] = {
        { 0.05f, 0.9f },    // playdough
        { 0.25f, 0.5f },    // a 'normal' material
        { 0.95f, 0.5f },    // a 'bouncy' material
        { 0.25f, 0.0f },    // a 'slippery' material
    };

    // /////////////////////////////////////////////////////////////////
    // Get friction/bounce information for a material.
    //
    // @param index The index of the material in the global static
    //                  array.
    // @param matInfo The material information retrieved on success.
    //
    // @return bool True on success and false if the index is out of
    //              bounds.
    //
    // /////////////////////////////////////////////////////////////////
    inline bool GetMaterialInfo(const enum PhysicsMaterial index, struct MaterialData &matInfo)
    {
        if(index > PhysMat_MaxMaterials) {
            return (false);
        }

        matInfo.m_restitution = g_PhysicsMaterials[index].m_restitution;
        matInfo.m_friction = g_PhysicsMaterials[index].m_friction;
        return (true);
    };

    // /////////////////////////////////////////////////////////////////
    // @enum PhysicsDensity
    // @author Mike McShaffry
    //
    // Used to easily find materials in the global g_DensityTable
    // array of F32s.
    //
    // /////////////////////////////////////////////////////////////////
    enum PhysicsDensity {
        // Elemental
        PhysDens_Air,
        PhysDens_Water,

        // Synthetics
        PhysDens_Styrofoam,

        // Woods
        PhysDens_Balsa,
        PhysDens_Bamboo,
        PhysDens_Pine,
        PhysDens_Oak,
        PhysDens_Ebony,

        // Biologic
        PhysDens_Blood,
        PhysDens_Bone,

        // Metals and Stone
        PhysDens_Silicon,
        PhysDens_Aluminum,
        PhysDens_Asbestos,
        PhysDens_Barite,
        PhysDens_Basalt,
        PhysDens_Bismuth,
        PhysDens_Borax,
        PhysDens_Boron,
        PhysDens_Brass,
        PhysDens_Brick,
        PhysDens_Bronze,
        PhysDens_Calcium,
        PhysDens_Carbon,
        PhysDens_Chromium,
        PhysDens_Clay,
        PhysDens_Coal,
        PhysDens_Cobalt,
        PhysDens_Copper,
        PhysDens_Diamond,
        PhysDens_Dolomite,
        PhysDens_Epoxy,
        PhysDens_Glass,
        PhysDens_Grystal,
        PhysDens_Granite,
        PhysDens_Gold,
        PhysDens_Hematite,
        PhysDens_Iridium,
        PhysDens_Iron_Cast,
        PhysDens_Iron_Wrought,
        PhysDens_Limestone,
        PhysDens_Lead,
        PhysDens_Magnetite,
        PhysDens_Manganese,
        PhysDens_Magnesium,
        PhysDens_Marble,
        PhysDens_Mercury,
        PhysDens_Molybdenum,
        PhysDens_Nickel,
        PhysDens_Platinum,
        PhysDens_Potassium,
        PhysDens_Quartz,
        PhysDens_Sandstone,
        PhysDens_Serpentine,
        PhysDens_Silver,
        PhysDens_Sodium,
        PhysDens_Steel,
        PhysDens_Talc,
        PhysDens_Tar,
        PhysDens_Tellurium,
        PhysDens_Tin,
        PhysDens_Titanium,
        PhysDens_Tungsten,
        PhysDens_Uranium,
        PhysDens_Vanadium,
        PhysDens_Vinyl,
        PhysDens_Wool,
        PhysDens_Zinc,

        PhysDens_Infinite,

        PhysDens_MaxDensities
    };

    // /////////////////////////////////////////////////////////////////
    // Table of materials densities.  The enum PhysicsDensity is used to
    // easily find the density you want.
    //
    // Physics Densities were grabbed from various resources, including:
    // http://www.engineeringtoolbox.com/24_293.html
    // http://www.simetric.co.uk/si_wood.htm
    // http://www.phy.cmich.edu/people/andy/Physics110/Book/Chapters/Chapter8.htm
    //
    // /////////////////////////////////////////////////////////////////
    static const F32 g_DensityTable[] = {
        // specific gravity (these numbers are easier to find
        .0013f,     // air
        1.000f,     // water

        // Synthetics
        .0100f,     // styrofoam

        // Woods
        .0170f,     // balsa
        .3500f,     // bamboo
        .5000f,     // pine
        .8300f,     // oak
        1.100f,     // ebony

        // Biologic
        1.060f,     // blood
        1.800f,     // bone

        // Metals and Stone
        2.400f,     // silicon
        2.650f,     // aluminum
        2.450f,     // asbestos
        4.500f,     // barite
        3.350f,     // basalt
        9.800f,     // bismuth
        1.750f,     // borax
        2.320f,     // boron
        8.550f,     // brass
        8.640f,     // brick
        8.400f,     // bronze
        4.580f,     // calcium
        1.950f,     // carbon
        7.100f,     // chromium
        2.200f,     // clay - average
        0.800f,     // coal - average
        8.900f,     // cobalt
        8.750f,     // copper - average
        3.510f,     // diamond
        2.900f,     // dolomite
        1.800f,     // epoxy
        2.600f,     // glass
        2.950f,     // crystal
        2.550f,     // granite
        19.30f,     // gold
        5.200f,     // hematite
        21.60f,     // iridium
        7.200f,     // cast iron
        7.750f,     // wrought iron
        2.400f,     // limestone
        11.34f,     // lead
        3.200f,     // Magnetite
        7.420f,     // Manganese
        1.740f,     // Magnesium
        2.720f,     // Marble
        13.54f,     // Mercury
        10.20f,     // Molybdenum
        8.900f,     // Nickel
        21.45f,     // Platinum
        0.860f,     // Potassium
        2.650f,     // Quartz
        2.300f,     // Sandstone
        2.750f,     // Serpentine
        10.50f,     // Silver
        0.970f,     // Sodium
        7.800f,     // Steel
        2.700f,     // Talc
        1.200f,     // Tar
        6.120f,     // Tellurium
        7.350f,     // Tin
        4.500f,     // Titanium
        19.22f,     // Tungsten
        18.70f,     // Uranium
        5.960f,     // Vanadium
        1.800f,     // Vinyl
        1.320f,     // Wool
        7.050f,     // Zinc

        0.000f      // Infinite Density - objects will never move
    };

    // /////////////////////////////////////////////////////////////////
    // Get density/gravity for a material.
    //
    // @param index The index of the material in the global static
    //                  array.
    // @param gravity The gravity retrieved on success.
    //
    // @return bool True on success and false if the index is out of
    //              bounds.
    //
    // /////////////////////////////////////////////////////////////////
    inline bool GetMaterialGravity(const enum PhysicsDensity index, F32 &gravity)
    {
        if(index > PhysDens_MaxDensities) {
            return (false);
        }

        gravity = g_DensityTable[index];
        return (true);
    };

}

#endif
