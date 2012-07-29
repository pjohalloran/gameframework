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
// @file IActors.h
// @author Michael L. McShaffry (edited by PJ O Halloran)
// @date 22/07/2010
//
// File contains the interface for game Actors.
//
// This class was extracted from the Game Coding Complete 3 code.
// Originally written by Mike McShaffry, author of Game
// Coding Complete and games programmer.
// 
// I have cleaned it up a little with better comments etc. to fit
// in with the rest of my code.
//
// I have added/modified the following functionality:
//
// IActor:
// - I moved the interface class in here IActor as it makes sense
//		to me that they be located in the same file.
// - I put everythin under my game namespace.
// - I removed a reference to IGame in IActor,(friend class IGame).
//		I dont like the friend hack, it breaks encapsulation.
// - I have commented out all references to Mat4x4 until i define it.
// - I moved SetId and SetMat to be protected members.
//
// - Created an interface that all ActorParam classes will have to
//		implement.
//
// /////////////////////////////////////////////////////////////////

#ifndef __I_ACTORS_H
#define __I_ACTORS_H

// External Headers
#include <string>
#include <sstream>
#include <deque>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>


#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

// Project Headers
#include "GameTypes.h"
#include "Matrix.h"
#include "Vector.h"


namespace GameHalloran
{

	// Define the ActorId type used to uniquely identify all game actors.
	typedef U32 ActorId;

	// Definition so that Actors can be unattached to any view when they are created.
	static const ActorId VIEWID_NO_VIEW_ATTACHED = 0;

	//
	// TODO: To remove, The actor type should be a hashed string like the event framework uses.
	//
	enum ActorType
	{
		AT_Unknown,
		AT_Ball,
		AT_Table,
		AT_Cue,
		AT_Static
	};

	// Forward declaration of IActorParams for IActor to compile.
	class IActorParams;

	// /////////////////////////////////////////////////////////////////
	// @class IActor
	// @author Michael L. McShaffry (edited by PJ O Halloran).
	//
	// This is a class is the base interface that all actor
	// implementations must implement.
	//
	// /////////////////////////////////////////////////////////////////
	class IActor
	{
	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IActor() { };
		
		// /////////////////////////////////////////////////////////////////
		// Set the matrix encapsulating the actors position and orientation.
		//
		// (Comment here from GCC3: This method should not be public...)
		//
		// @param newMat
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetMat(const Matrix4 &newMat) = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Get the matrix encapsulating the actors position and orientation.
		//
		// @return Matrix4
		//
		// /////////////////////////////////////////////////////////////////
		virtual Matrix4 VGetMat() = 0;

		// /////////////////////////////////////////////////////////////////
		// Set the ID of the actor.
		//
		// (Comment here from GCC3: This method should not be public...)
		//
		// @param id The new ID of the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetID(const ActorId id) = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Get the ID of the actor.
		//
		// @return ActorId The actor ID.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const ActorId VGetID() = 0; 

		// /////////////////////////////////////////////////////////////////
		// Get the type of actor.
		//
		// @return const I32 The actor type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const I32 VGetType() = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Get the parameters associated with the actor.
		//
		// @return boost::shared_ptr<ActorParams> The parameters belonging to
		//											the actor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IActorParams> VGetParams() = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Check if the actor is physical.  Physical means is this pointer
		// representing the actor in the logic layer.
		//
		// @return bool Is the actor physical.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsPhysical() = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Check if the actor is geometrical. Physical means is this pointer
		// representing the actor in the view layer (for rendering).
		//
		// @return bool Is the actor geometrical?
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsGeometrical() = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Called once per main loop to update the state of the actor.
		//
		// @param deltaMilliseconds The number of milliseconds since the last
		//								update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const I32 deltaMilliseconds) = 0;
		
		// /////////////////////////////////////////////////////////////////
		// Rotate the actor along the Y axis.
		//
		// @param angleRadians The angle to rotate the actor by.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRotateY(const F32 angleRadians) = 0;
	};

	// Container of error messages.
	typedef std::deque<std::string> TErrorMessageList;

	// Forward declaration.
	class ISceneNode;

	// /////////////////////////////////////////////////////////////////
	// @class IActorParams
	// @author PJ O Halloran
	//
	// This interface class for all actor parameters.
	//
	// /////////////////////////////////////////////////////////////////
	class IActorParams
	{

	public:

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~IActorParams() { };

		// /////////////////////////////////////////////////////////////////
		// Get/Set the ID of the actor associated with these parameters.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::optional<ActorId> VGetId() = 0;
		virtual void VSetId(boost::optional<ActorId> id) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get/Set the size of the Actor parameters.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 VGetSize() const = 0;
		virtual void VSetSize(const I32 size) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get/Set the actors position.
		//
		// /////////////////////////////////////////////////////////////////
		virtual Point3 VGetPos() const = 0;
		virtual void VSetPos(const Point3 &position) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get/Set the D3D/GL/SDL color.
		//
		// /////////////////////////////////////////////////////////////////
		virtual Vector4 VGetColor() const = 0;
		virtual void VSetColor(const Vector4 &color) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get/Set the actor type.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ActorType VGetType() const = 0;
		virtual void VSetType(const ActorType type) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get/Set the Create Function name.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const char * const VGetCreateFuncName() const = 0;
		virtual bool VSetCreateFuncName(const char * const createFuncName) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get/Set the Destroy Function name.
		//
		// /////////////////////////////////////////////////////////////////
		virtual const char * const VGetDestroyFuncName() const = 0;
		virtual bool VSetDestroyFuncName(const char * const destroyFuncName) = 0;

		// /////////////////////////////////////////////////////////////////
		// Initialize the parameters from a stream.
		//
		// @param in The input stream to initialize the parameters from.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VInit(std::istringstream &in) = 0;

		// /////////////////////////////////////////////////////////////////
		// Serialize the parameters out to a stream.
		//
		// @param out The output stream 
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSerialize(std::ostringstream &out) const = 0;

		// /////////////////////////////////////////////////////////////////
		// Initialize the parameters from a Lua script.
		// 
		// @param srcData The Lua object containing the parameter data.
		// @param errorMessages The error messages (maybe passed back from the script?)
		// 
		// /////////////////////////////////////////////////////////////////
		virtual bool VInit(LuaPlus::LuaObject srcData, TErrorMessageList &errorMessages) = 0;

		// /////////////////////////////////////////////////////////////////
		// The logic layers create function.  Creates an Actor in respect to
		// the logic layer from the parameters.
		//
		// The actor created should be added manually to the logic layer.
		//
		// @return boost::shared_ptr<IActor> A pointer to an actor or NULL
		//										on error.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<IActor> VCreate() = 0;

		// /////////////////////////////////////////////////////////////////
		// The view layers create function.  Creates an SceneNode representing
		// the Actor in respect to the view layer from the parameters.
		//
		// @return boost::shared_ptr<ISceneNode> A pointer to a scene node.
		//
		// /////////////////////////////////////////////////////////////////
		virtual boost::shared_ptr<ISceneNode> VCreateSceneNode() = 0;
	};

}

#endif
