// /////////////////////////////////////////////////////////////////
// @file ParticleSystem.h
// @author PJ O Halloran
// @date 29/09/2010
//
// File contains the header for the Particle and ParticleSystem 
// classes.
//
// /////////////////////////////////////////////////////////////////

#ifndef __GF_PARTICLE_SYSTEM_H
#define __GF_PARTICLE_SYSTEM_H

// External Headers
#include <boost/shared_ptr.hpp>

#include <list>

// Project Headers
#include "GameBase.h"
#include "Matrix.h"
#include "Vector.h"
#include "BoundingCube.h"
#include "BoundingSphere.h"
#include "IScreenElement.h"
#include "GLBatch.h"
#include "GLSLShader.h"
#include "GameColors.h"
#include "ImageResource.h"
#include "CRandom.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class Particle
	// @author PJ O Halloran
	//
	// Base class that represents attributes common to a single particle.
	//
	// /////////////////////////////////////////////////////////////////
	class Particle
	{
		private:

			Point3 m_position;						///< Current position of the particle.
			Vector3 m_velocity;						///< Current velocity.
			Vector3 m_acceleration;					///< Current acceleration.
			F32 m_lifetime;						///< Total lifetime of the particle in seconds.
			F32 m_age;							///< The current age of the particle in seconds.
			bool m_alive;							///< Is the particle currently active/alive.
			GameColor m_color;						///< Current color of the particle.
			bool m_rotate;							///< Should the particle rotate?
			F32 m_rotateAngle;					///< The angle of rotation in degrees per second that the particle rotates at.
			Vector3 m_rotationAxis;					///< The axis that the particle should rotate about.
			F32 m_size;							///< The size of the particle.

		protected:


		public:
		
			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// /////////////////////////////////////////////////////////////////
			inline explicit Particle() : m_position(), m_velocity(), m_acceleration(), m_lifetime(0.0f),\
				m_age(0.0f), m_alive(true), m_color(), m_rotate(false), m_rotateAngle(0.0f), m_rotationAxis(), m_size(0.0f) { };

			// /////////////////////////////////////////////////////////////////
			// Constructor.
			//
			// @param pos Starting position.
			// @param col Starting color.
			// @param v Starting velocity.
			// @param a Starting acceleration.
			// @param lifetime Total lifetime of the particle in seconds.
			// @param rotate Should the particle rotate.
			// @param rotateAngle The angle the particle should rotate per second
			//						(in degrees).
			// @param rotationAxis The axis of rotation.
			// @param size The size of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline explicit Particle(const Point3 &pos, const GameColor &col, const Vector3 &v, const Vector3 &a, const F32 lifetime, const bool rotate, const F32 rotateAngle, const Vector3 &rotationAxis, const F32 size)\
				: m_position(pos), m_velocity(v), m_acceleration(a), m_lifetime(lifetime), m_age(0.0f), m_alive(true), m_color(col),\
					m_rotate(rotate), m_rotateAngle(rotateAngle), m_rotationAxis(rotationAxis), m_size(size)  { };

			// /////////////////////////////////////////////////////////////////
			// Destructor.
			//
			// /////////////////////////////////////////////////////////////////
			virtual ~Particle() { };

			// /////////////////////////////////////////////////////////////////
			// Get the current position of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline Point3 GetPosition() const { return (m_position); };

			// /////////////////////////////////////////////////////////////////
			// Set the current position of the particle.
			//
			// @param pos The current position of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetPosition(const Point3 &pos) { m_position = pos; };

			// /////////////////////////////////////////////////////////////////
			// Get the color of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline GameColor GetColor() const { return (m_color); };

			// /////////////////////////////////////////////////////////////////
			// Set the color of the particle.
			//
			// @param col The current color of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetColor(const GameColor &col) { m_color = col; };

			// /////////////////////////////////////////////////////////////////
			// Get the velocity of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 GetVelocity() const { return (m_velocity); };

			// /////////////////////////////////////////////////////////////////
			// Set the velocity of the particle.
			//
			// @param vel The current velocity of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetVelocity(const Vector3 &vel) { m_velocity = vel; };

			// /////////////////////////////////////////////////////////////////
			// Get the acceleration of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 GetAcceleration() const { return (m_acceleration); };

			// /////////////////////////////////////////////////////////////////
			// Set the acceleration of the particle.
			//
			// @param accel The current acceleration of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetAcceleration(const Vector3 &accel) { m_acceleration = accel; };

			// /////////////////////////////////////////////////////////////////
			// Get the total lifetime of the particle in seconds.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetLifetime() const { return (m_lifetime); };

			// /////////////////////////////////////////////////////////////////
			// Set the total lifetime of the particle in seconds.
			//
			// @param lifetime The lifetime of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetLifetime(const F32 lifetime) { m_lifetime = lifetime; };

			// /////////////////////////////////////////////////////////////////
			// Get the age of the particle in seconds.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetAge() const { return (m_age); };

			// /////////////////////////////////////////////////////////////////
			// Set the age of the particle in seconds.
			//
			// @param age The current age of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetAge(const F32 age) { m_age = age; };

			// /////////////////////////////////////////////////////////////////
			// Check if the particle is currently alive.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool IsAlive() const { return (m_alive); };

			// /////////////////////////////////////////////////////////////////
			// Set if the particle is alive or not.
			//
			// @param alive The new alive flag of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetAlive(const bool alive) { m_alive = alive; };

			// /////////////////////////////////////////////////////////////////
			// Check if the particle is currently rotating.
			//
			// /////////////////////////////////////////////////////////////////
			inline bool IsRotating() const { return (m_rotate); };

			// /////////////////////////////////////////////////////////////////
			// Set if the particle is rotating or not.
			//
			// @param alive The new rotate flag of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetRotating(const bool rotate) { m_rotate = rotate; };

			// /////////////////////////////////////////////////////////////////
			// Check the angle of rotation per second (in degrees).
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetRotationAngle() const { return (m_rotateAngle); };

			// /////////////////////////////////////////////////////////////////
			// Set if the angle of rotation per second (in degrees).
			//
			// @param rotateAngle The new angle of rotation.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetRotationAngle(const F32 rotateAngle) { m_rotateAngle = rotateAngle; };

			// /////////////////////////////////////////////////////////////////
			// Check the axis of rotation.
			//
			// /////////////////////////////////////////////////////////////////
			inline Vector3 GetRotationAxis() const { return (m_rotationAxis); };

			// /////////////////////////////////////////////////////////////////
			// Set the rotation axis.
			//
			// @param alive The new alive flag of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetRotationAxis(const Vector3 &rotationAxis) { m_rotationAxis = rotationAxis; };

			// /////////////////////////////////////////////////////////////////
			// Get the size of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline F32 GetSize() const { return (m_size); };

			// /////////////////////////////////////////////////////////////////
			// Set the size of the particle.
			//
			// /////////////////////////////////////////////////////////////////
			inline void SetSize(const F32 size) { m_size = size; };

			// /////////////////////////////////////////////////////////////////
			// Get a matrix representing the particles current translation
			// information.
			//
			// /////////////////////////////////////////////////////////////////
			void GetMatrix(Matrix4 &mat) const;

	};

	// Define a list of particles
	typedef std::list<boost::shared_ptr<Particle> > ParticleList;

	// /////////////////////////////////////////////////////////////////
	// @class ParticleSystem
	// @author PJ O Halloran
	//
	// Base particle system class.  Performs common tasks for managing
	// a list of particles.
	//
	// /////////////////////////////////////////////////////////////////
	class ParticleSystem
	{
	private:

		Point3 m_origin;									///< The emit position of the particles.
		BoundingCube m_boundBox;							///< The area in the scene where the particles are allowed.
		F32 m_emitRate;									///< The rate at which particles are emitted per second.
		U32 m_maxParticles;						///< The maximum number of particles in the system.
		Vector3 m_windDir;									///< Direction and magnitude of wind in the system.
		F32 m_gravity;									///< Strength of gravity on the system.
		ImageResource m_textureResource;					///< The texture resource.

	protected:

		GLuint m_textureId;									///< The ID of the texture to apply to all the particles.
		ParticleList m_list;								///< List of particles.
		GLBatch m_pointSpritesBatch;						///< Batch of point sprites to render.
		GLSLShader m_shaderProg;							///< The GLSL shader program we will use to render the particles.
		CRandom m_rng;										///< Random number generator.

		// Maximum number of particles to send to the GPU at a time.
		static const U32 PARTICLE_BATCH_SIZE = 500;

		// /////////////////////////////////////////////////////////////////
		// Test if a valid texture has been assigned to the particle system.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsTextureAssigned() const
		{
			return (glIsTexture(m_textureId) == GL_TRUE);
		};

		// /////////////////////////////////////////////////////////////////
		// Remove all particles marked as dead from the list.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VRemoveDeadParticles();

	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// /////////////////////////////////////////////////////////////////
		explicit ParticleSystem();

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param pt Emission point of the particles.
		// @param bb Bounding box of the particle system.
		// @param emitRate The number of particles emitted per second.
		// @param maxParticles The maximum number of particles allowed.
		// @param textureResource The texture resource to apply to all particles.
		// @param windVec The wind vector.
		// @param gravity The strength of gravity in the system.
		//
		// /////////////////////////////////////////////////////////////////
		explicit ParticleSystem(const Point3 &pt, const BoundingCube &bb, const F32 emitRate, const U32 maxParticles, const ImageResource &textureResource, const Vector3 &windVec, const F32 gravity);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ~ParticleSystem();

		// /////////////////////////////////////////////////////////////////
		// Called when the application is restored after a minimize or other
		// such event.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRestore();

		// /////////////////////////////////////////////////////////////////
		// Called when the application display or device is lost after a
		// display resolution or other such system event.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnLostDevice();

		// /////////////////////////////////////////////////////////////////
		// Called before a render to set 3D scene state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnPreRender();

		// /////////////////////////////////////////////////////////////////
		// Render the particle system.
		//
		// @param time The total time in seconds so far.
		// @param elapsedTime The time elapsed in seconds since the last 
		//						render call.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRender(const F64 time, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Called after a render to restore 3D scene state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnPostRender();

		// /////////////////////////////////////////////////////////////////
		// Update the particle system.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the origin point of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline Point3 GetEmitOrigin() const { return (m_origin); };

		// /////////////////////////////////////////////////////////////////
		// Set the origin point of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetEmitOrigin(const Point3 &ptOrigin) { m_origin = ptOrigin; };

		// /////////////////////////////////////////////////////////////////
		// Get the bounding cube of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline BoundingCube GetBoundingCube() const { return (m_boundBox); };

		// /////////////////////////////////////////////////////////////////
		// Set the bounding cube of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetBoundingCube(const BoundingCube &bb) { m_boundBox = bb; };

		// /////////////////////////////////////////////////////////////////
		// Get the emit rate per second of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline F32 GetEmitRate() const { return (m_emitRate); };

		// /////////////////////////////////////////////////////////////////
		// Set the emit rate per second of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetEmitRate(const F32 emitRate) { m_emitRate = emitRate; };

		// /////////////////////////////////////////////////////////////////
		// Get the max number of particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline U32 GetMaxParticles() const { return (m_maxParticles); };

		// /////////////////////////////////////////////////////////////////
		// Set the max number of particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetMaxParticles(const U32 maxParticles) { m_maxParticles = maxParticles; };

		// /////////////////////////////////////////////////////////////////
		// Get the direction and magnitude of wind on the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline Vector3 GetWindDirection() const { return (m_windDir); };

		// /////////////////////////////////////////////////////////////////
		// Set the direction and magnitude of wind on the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetWindDirection(const Vector3 &windDir) { m_windDir = windDir; };

		// /////////////////////////////////////////////////////////////////
		// Get the strength of gravity on the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline F32 GetGravity() const { return (m_gravity); };

		// /////////////////////////////////////////////////////////////////
		// Set the strength of gravity on the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetGravity(const F32 gravity) { m_gravity = gravity; };

		// /////////////////////////////////////////////////////////////////
		// Get the texture resource of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline ImageResource GetTextureResource() const { return (m_textureResource); };

		// /////////////////////////////////////////////////////////////////
		// Set the texture resource of the particles.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetTextureId(const ImageResource &texRes)
		{
			m_textureResource = texRes;
			// Reload the texture.
			VOnRestore();
		};

		// /////////////////////////////////////////////////////////////////
		// Reset all particles.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VReset();
		
		// /////////////////////////////////////////////////////////////////
		// Reset a particle back to an initial state.
		// In some particle system types we will want to reuse particles
		// instead of removing them.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VResetParticle(boost::shared_ptr<Particle> particlePtr) = 0;

		// /////////////////////////////////////////////////////////////////
		// Add a new particle to the list so long as we have not yet reached
		// the maximum number of particles.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VAddParticle();

		// /////////////////////////////////////////////////////////////////
		// Is there any particles in the list?
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsEmpty() {	return (m_list.empty()); };

		// /////////////////////////////////////////////////////////////////
		// Are all particles in the list dead?
		//
		// /////////////////////////////////////////////////////////////////
		bool IsDead();

	};

}

#endif
