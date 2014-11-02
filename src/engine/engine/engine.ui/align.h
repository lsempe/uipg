#pragma once

#include "engine.math/rectangle.h"

namespace ui
{

	class align
	{
	public:

		enum eAlignmentFlags
		{
			Top			= 0x01,
			Bottom		= 0x02,
			Left		= 0x04,
			Right		= 0x08,
			Center		= 0x10,
			Middle		= 0x20		
		};

		align(math::rectangle& from, math::rectangle& to, unsigned int flags = 0)
			: m_from(from)
			, m_to(to)
			, m_fromList(EmptyList)
			, m_flags(flags)
		{		
			if ( flags != 0 )
			{
				Apply(flags, from, to);
			}
		}

		align(std::list<math::rectangle>& fromList, math::rectangle& to, unsigned int flags = 0)
			: m_from(math::rectangle::Zero)
			, m_to(to)
			, m_fromList(fromList)
			, m_flags(flags)
		{
			if ( flags != 0 )
			{
				Apply(flags, fromList, to);
			}
		}

		void Apply()
		{
			Apply(m_flags);
		}

		void Apply(unsigned int flags)
		{
			if ( m_fromList.size() == 0 )
			{
				Apply(flags, m_from, m_to);
			}
			else
			{
				for ( auto& rectangle : m_fromList )
				{
					Apply(flags, rectangle, m_to);
				}
			}
		}

		void Apply(unsigned int flags, std::list<math::rectangle>& from, const math::rectangle& to)
		{
			for ( auto& rectangle : from )
			{
				Apply(flags, rectangle, to);
			}
		}

		void Apply(unsigned int flags, math::rectangle& from, const math::rectangle& to)
		{
			if ( IsSet(flags, Top) )
			{
				from.Top() = to.Top();
			}
			else
			if ( IsSet(flags, Bottom) )
			{
				from.Top() = to.Bottom() - from.Height();
			}

			if ( IsSet(flags, Left) )
			{
				from.Left() = to.Left();
			}
			else
			if ( IsSet(flags, Right) )
			{
				auto offset = to.Right() - from.Left();
				from.Left() += (offset - from.Width());			
			}

			if ( IsSet(flags, Center) )
			{
				from.Left() = to.Center().x() - (from.Width()/2);
			}

			if ( IsSet(flags, Middle) )
			{
				from.Top() = to.Center().y() - (from.Height()/2);
			}
		}
	
	protected:

		unsigned int m_flags;
		math::rectangle& m_to;
		math::rectangle& m_from;
		std::list<math::rectangle>& m_fromList;
		std::list<math::rectangle> EmptyList;

		bool IsSet(int flags, eAlignmentFlags flag)
		{
			return ( flag & flags ) == flag;
		}

		align(const align& a) : m_from(a.m_from), m_to(a.m_to), m_fromList(a.m_fromList) {}
		align operator = (const align&) { return *this; }

	};


} // ui