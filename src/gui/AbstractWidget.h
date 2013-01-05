#pragma once
#ifndef __GF_ABSTRACT_WIDGET_H
#define __GF_ABSTRACT_WIDGET_H

// /////////////////////////////////////////////////////////////////
// @file AbstractWidget.h
// @author PJ O Halloran
// @date 20/10/2010
//
// Base level class for my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////


#ifdef WIN32
// Disable exception throw specification warning VS complains about.
#	pragma warning( push )
#	pragma warning( disable:4290 )
#endif

#include <string>

#include <boost/shared_ptr.hpp>

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

#include "GameBase.h"
#include "IScreenElement.h"
#include "Vector.h"
#include "TextureManager.h"
#include "ModelViewProjStackManager.h"
#include "GLSLShader.h"
#include "GLBatch.h"
#include "GameException.h"
#include "Matrix.h"
#include "BoundingCube.h"
#include "TextureAtlas.h"

namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	// @class AbstractWidget
	// @author PJ O Halloran
	//
	// Base class for all UI objects.
	//
	// /////////////////////////////////////////////////////////////////
	class AbstractWidget : public IScreenElement
	{
	private:

		static const I32 NUM_VERTICES = 6;										///< Number of vertices required for a quad.

        AtlasImage m_quadDim;                                                   ///< Texture quad dimensions.
        AtlasImage m_currQuadDim;                                               ///< Texture quad dimensions.
		Point3 m_position;														///< 2D Position and z depth.
		bool m_visible;															///< Visible flag.
		Vector4 m_color;														///< Color and Alpha of quad background.
		ScreenElementId m_id;													///< ID of the element on screen.
		F32 m_width;															///< Width of the widget.
		F32 m_height;															///< Height of the widget.
		bool m_applyTexture;													///< Is the widget rendered with a texture?
		GLuint m_vaoId;															///< This widgets GL VAO state array ID.
		GLuint m_vboId;															///< VBO buffer ID.

		// /////////////////////////////////////////////////////////////////
		// Common constructor helper function.
		//
		// @param textureNameRef The name of the texture.
		//
		// @throw GameException& If the shader objects are missing.
		//
		// /////////////////////////////////////////////////////////////////
		void Init(const std::string &textureNameRef) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// LUA constructor helper function.  Sets the widgets position from
		// the LUA table.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaPosition(const LuaPlus::LuaObject &table);

		// /////////////////////////////////////////////////////////////////
		// LUA constructor helper function.  Sets the widgets dimensions from
		// the LUA table.
		//
		// /////////////////////////////////////////////////////////////////
		void SetLuaDimensions(const LuaPlus::LuaObject &table);

		// /////////////////////////////////////////////////////////////////
		// Create the quad batch of geometry with position, color and texture
		// coordinate data according to the widgets current parameters.
		//
		// /////////////////////////////////////////////////////////////////
		void SetupQuad();
        
	protected:
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        AtlasImage GetQuadDim() const { return (m_quadDim); };
        
        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        void SetQuadDim(const AtlasImage &dim) { m_currQuadDim = dim; SetupQuad(); };
        
		// /////////////////////////////////////////////////////////////////
		// PreRender the widget with a texture.
		//
		// /////////////////////////////////////////////////////////////////
		bool PreRenderTexturedWidget();

		// /////////////////////////////////////////////////////////////////
		// PreRender the widget with a flat color.
		//
		// /////////////////////////////////////////////////////////////////
		bool PreRenderFlatWidget(boost::optional<Vector4> opColor = boost::optional<Vector4>());

        std::string m_atlasName;                                                ///< The name of the image atlas.
        std::string m_imageName;                                                ///< The name of the image file/atlas image.
		TexHandle m_tHandle;													///< Handle to the texture to render on the quad (0 if no texture).
		boost::shared_ptr<ModelViewProjStackManager> m_mvpStackManagerPtr;		///< Pointer to the global MVP stack manager.
		const boost::shared_ptr<GLSLShader> m_texShaderProg;					///< The GLSL shader program used to render textured widgets.
		const boost::shared_ptr<GLSLShader> m_flatShaderProg;					///< The GLSL shader program used to render non textured widgets.
		Matrix4 m_projMatrix;													///< Proj matrix set just as the widget is to be rendered.
		BoundingCube m_bb;														///< Bounding box.
		TexHandle m_currentTextureHandle;										///< ID of the current texture handle to render.

        ShaderUniformSPtr m_colorMapUniform;                                    ///< 
        ShaderUniformSPtr m_alphaUniform;                                       ///< 
        ShaderUniformSPtr m_projUniform;                                        ///< 
        ShaderUniformSPtr m_colorUniform;                                       ///< 
        
		// /////////////////////////////////////////////////////////////////
		// Set the current texture to render.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetCurrentTexture(const TexHandle h) { m_currentTextureHandle = h; };

		// /////////////////////////////////////////////////////////////////
		// Set the position of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		void SetPositionHelper(const Point3 &posRef);

		// /////////////////////////////////////////////////////////////////
		// Set the width of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		void SetWidthHelper(const F32 width);

		// /////////////////////////////////////////////////////////////////
		// Set the height of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		void SetHeightHelper(const F32 height);

        // /////////////////////////////////////////////////////////////////
        //
        //
        // /////////////////////////////////////////////////////////////////
        ScreenElementId GetId() const { return (m_id); };
        
	public:

		// /////////////////////////////////////////////////////////////////
		// Constructor.
		//
		// @param posRef Initial position and z depth.
		// @param colorRef Color of quad.
		// @param mvpStackManPtr Pointer to the MVP stack manager.
		// @param width Width of widget.
		// @param height Height of widget.
		// @param shaderFlatObj The GLSL shader object used to render the un-textured widget.
		// @param shaderTexObj The GLSL shader object used to render the textured widget.
		// @param textureNameRef Name of texture to apply. An empty string disables it.
        // @param atlasNameRef Name of the image atlas (if any).
		// @param visible Is the widget visible.
		// @param id Unique ID of widget.
		//
		// @throw GameException& If the shader object used is NULL.
		//
		// /////////////////////////////////////////////////////////////////
		explicit AbstractWidget(const Point3 &posRef,\
								const Vector4 &colorRef,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const F32 width,\
								const F32 height,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								const std::string &textureNameRef = std::string(),\
                                const std::string &atlasNameRef = std::string(),\
								const bool visible = true,\
								const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Contructor for widgets defined in an external LUA script.
		//
		// If LUA data is missing then the Widget will just use default
		// values.  i.e. If position data is missing then the widget will 
		// just be located at 0, 0, 0!
		//
		// @param widgetScriptData Lua object containing the widget data.
		// @param mvpStackManPtr Pointer to the MVP stack manager.
		// @param shaderFlatObj Flat shader to use.  Will be overridden
		//						if the scripter defined a flat shader.
		// @param shaderTexObj Texture shader to use.  Will be overridden
		//						if the scripter defined a texture shader.
		// @param id The screen element ID of the widget.
		//
		// @throw GameException& If the shader object used is NULL.
		// @throw GameException& If Lua data is invalid or incomplete.
		//
		// /////////////////////////////////////////////////////////////////
		explicit AbstractWidget(const LuaPlus::LuaObject &widgetScriptData,\
								boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
								const boost::shared_ptr<GLSLShader> shaderFlatObj,\
								const boost::shared_ptr<GLSLShader> shaderTexObj,\
								const ScreenElementId id = SCREEN_ELEMENT_ID_NOT_ASSIGNED) throw (GameException &);

		// /////////////////////////////////////////////////////////////////
		// Destructor.
		// 
		// /////////////////////////////////////////////////////////////////
		virtual ~AbstractWidget();

		// /////////////////////////////////////////////////////////////////
		// Get the ID of the screen element.
		//
		// /////////////////////////////////////////////////////////////////
		virtual ScreenElementId VGetId() const { return (m_id); };

		// /////////////////////////////////////////////////////////////////
		// Set the ID of the screen element.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetId(const ScreenElementId id) { m_id = id; };

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
		// Called when the applications screen resolution is changed.
		//
		// @param oldWidth The old screen width.
		// @param oldHeight The old screen height.
		// @param newWidth The new screen width.
		// @param newHeight The new screen height.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnResolutionChange(const I32 oldWidth, const I32 oldHeight, const I32 newWidth, const I32 newHeight);

		// /////////////////////////////////////////////////////////////////
		// Render the screen element.
		//
		// @param time The total time in seconds so far.
		// @param elapsedTime The time elapsed since the last render call.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnRender(const F64 time, const F32 elapsedTime);

		// /////////////////////////////////////////////////////////////////
		// Update the screen element.
		//
		// @param elapsedTime The number of seconds since the last update.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VOnUpdate(const F32 elapsedTime) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the Z order of the element.
		//
		// /////////////////////////////////////////////////////////////////
		virtual I32 VGetZOrder() const { return (static_cast<I32>(m_position.GetZ())); };

		// /////////////////////////////////////////////////////////////////
		// Set the Z order.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetZOrder(const I32 zOrder) { m_position.SetZ(static_cast<F32>(zOrder)); };

		// /////////////////////////////////////////////////////////////////
		// Is the element currently visible?
		//
		// @return bool True if it is visible and false if it is not.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VIsVisible() const { return (m_visible); };

		// /////////////////////////////////////////////////////////////////
		// Set if the element is currently visible.
		//
		// @param visible True if visible, false if not.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetVisible(const bool visible) { m_visible = visible; };

		// /////////////////////////////////////////////////////////////////
		// Called once per game loop so the element can check if there was
		// any input from the operating system or from the user.
		//
		// @param eventObj Event structure.
		// @param elapsedTime Number of seconds since the last event poll.
		//
		// @return bool True on success or false on failure.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VOnEvent(GF_Event &eventObj, const F32 elapsedTime) = 0;

		// /////////////////////////////////////////////////////////////////
		// Get the position of the widget.  This position refers to the top
		// left of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual Point3 VGetPosition() const { return (m_position); };

		// /////////////////////////////////////////////////////////////////
		// Set the position of the top left of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetPosition(const Point3 &posRef);

		// /////////////////////////////////////////////////////////////////
		// Set the position of the top left of the widget.
		//
		// @param posRef The new position.
		// @param normalMode Will we set the position relative to the screen
		//						dimensions (false) or set it absolutely (true).
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetPositionType(const Point3 &posRef, const bool normalMode = true);

		// /////////////////////////////////////////////////////////////////
		// Get the color, including the alpha component of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual Vector4 VGetColor() const { return (m_color); };

		// /////////////////////////////////////////////////////////////////
		// Set the color of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetColor(const Vector4 &colorRef) { m_color = colorRef; };

		// /////////////////////////////////////////////////////////////////
		// Set the width of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetWidth(const F32 width);

		// /////////////////////////////////////////////////////////////////
		// Get the current width.
		//
		// /////////////////////////////////////////////////////////////////
		virtual F32 VGetWidth() { return (m_width); };

		// /////////////////////////////////////////////////////////////////
		// Set the height of the widget.
		//
		// /////////////////////////////////////////////////////////////////
		virtual void VSetHeight(const F32 height);

		// /////////////////////////////////////////////////////////////////
		// Get the current height.
		//
		// /////////////////////////////////////////////////////////////////
		virtual F32 VGetHeight() { return (m_height); };

		// /////////////////////////////////////////////////////////////////
		// Set the MVP stack manager pointer used to render the widget.
		//
		// /////////////////////////////////////////////////////////////////
		inline void SetMvpStackManagerPtr(boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr) { m_mvpStackManagerPtr = mvpStackManPtr; };

		// /////////////////////////////////////////////////////////////////
		// Check if a texture is being applied for the widget.
		//
		// /////////////////////////////////////////////////////////////////
		inline bool IsTextured() const { return (m_applyTexture); };
        
        inline bool IsAtlased() const { return (!m_atlasName.empty()); };

		// /////////////////////////////////////////////////////////////////
		// Method that returns the value/setting data of the widget.  This
		// data will be in the form of a lua table.
		//
		// @return LuaPlus::LuaObject The lua script data encapsulating the
		//								controls state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual LuaPlus::LuaObject VGetLuaData() = 0;

		// /////////////////////////////////////////////////////////////////
		// Method that updates the current state(s) of the widget from a
		// LuaObject from application code or from script code.
		//
		// @param widgetData The lua script data encapsulating the
		//								controls new state.
		//
		// /////////////////////////////////////////////////////////////////
		virtual bool VSetLuaData(const LuaPlus::LuaObject &widgetData) = 0;

	};

	// Vector was chosen as we will be iterating over the widgets often while
	//  it is thought insertions and removal of widgets from anywhere in the 
	//   container will be quite rare.
	typedef std::vector<boost::shared_ptr<AbstractWidget> > WidgetList;

}

#ifdef WIN32
#	pragma warning( pop )
#endif

#endif
