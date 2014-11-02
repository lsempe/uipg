#pragma once

#include "engine.core/sys.h"
#include "engine.math/vector.h"
#include "engine.math/matrix.h"
#include "engine.math/rectangle.h"

namespace render
{

	class viewport
	{
	public:
	
		viewport();
		viewport(float left, float top, float right, float bottom, float minDepth, float maxDepth, float titleSafeRatio = 0.8f);
		viewport(const viewport& rhs);
		viewport& operator = (const viewport& rhs);

		const float Left() const;
		const float Right() const;
		const float Top() const;
		const float Bottom() const;

		const float Width() const;
		const float Height() const;

		const float Aspect() const;

		const float MinDepth() const;
		const float MaxDepth() const;

		const math::vector2 Center() const { return math::vector2(Left() + (Width()/2), Top() + (Height()/2)); }

		const math::rectangle& TitleSafeArea() const;

		///
		/// Project
		///
		/// Projects a world space position unto the viewport 2D space.
		///
		/// worldPosition - A 3D space position
		/// projection  - The projection matrix
		/// view - The camera's view matrix
		/// world - The world matrix
		///
		/// returns a vector3 in which x,y will be screen coordinates.
		///
		math::vector3 Project(const math::vector3& worldPosition, const math::matrix& projection, const math::matrix& view, const math::matrix& world) const;
	
		///
		/// Unproject
		///
		/// Converts a screen position into a world space position. Typically unproject is used to generate a ray that spans the view frustum. 
		/// Unproject the screen coordinate the first time using 0.0 as the Z value, and a second time using 1.0 as the Z value, then calculate
		/// a direction vector between these two world space points and normalize it. The resulting ray may be used to perform interesection testing.
		///
		/// screenPosition - A screen space position, the z component represents a ratio of depth.
		/// projection  - The projection matrix
		/// view - The camera's view matrix
		/// world - The world matrix
		///
		/// Returns a world space position.
		///
		math::vector3 Unproject(const math::vector3& screenPosition, const math::matrix& projection, const math::matrix& view, const math::matrix& world) const;

	
	protected:

		float m_left;
		float m_top;
		float m_right;
		float m_bottom;

		float m_minDepth;
		float m_maxDepth;

		float m_titleSafeRatio;
		math::rectangle m_titleSafeArea;

		void CalculateTitleSafeArea();

	};

} // render