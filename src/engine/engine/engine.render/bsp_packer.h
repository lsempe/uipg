#pragma once

#include <memory>

#include "engine.math/rectangle.h"

namespace render
{

class bsp_packer
{
public:

	class node
	{
	public:

		node() : m_isEmpty(true) { }

		std::shared_ptr<node>& Child(int i) { return m_children[i]; }

		const bool IsLeaf() const { return m_children[0] == nullptr && m_children[0] == m_children[1]; }
		bool& IsEmpty() { return m_isEmpty; }

		math::rectangle& Rectangle() { return m_rectangle; }
				
	private:

		bool m_isEmpty;
		std::shared_ptr<node> m_children[2];
		math::rectangle m_rectangle;

	};
	
	std::shared_ptr<node> Insert(const math::rectangle& rectangle)
	{
		if ( m_root == nullptr )
		{
			m_root = std::shared_ptr<node>(new node() );
			m_root->Rectangle() = rectangle;
			return m_root;
		}

		return Insert(m_root, rectangle);
	}

	const std::shared_ptr<node>& Root() const { return m_root; }

	const math::vector4 TextureCoordinates(const std::shared_ptr<node>& node) const 
	{ 
		auto width = m_root->Rectangle().Width();
		auto height = m_root->Rectangle().Height();
		const auto& rectangle = node->Rectangle();
		
		return math::vector4(rectangle.Left() / width, rectangle.Top() / height, rectangle.Right() / width, rectangle.Bottom() / height );
	}

private:


	std::shared_ptr<node> Insert(std::shared_ptr<node>& n, const math::rectangle& rectangle)
	{
		if ( !n->IsLeaf() )
		{
			auto newNode = Insert( n->Child(0), rectangle );
			if ( newNode != nullptr )
				return newNode;

			return Insert(n->Child(1), rectangle);
		}
		else
		{
			if ( !n->IsEmpty() )
				return nullptr;

			auto& parentRect = n->Rectangle();

			if ( rectangle.Width() > parentRect.Width() || rectangle.Height() > parentRect.Height() )
				return nullptr;

			// If it fits perfectly
			if ( math::IsEqual(parentRect.Width(), rectangle.Width() ) && math::IsEqual(parentRect.Height(), rectangle.Height() ) )
			{
				n->IsEmpty() = false;
				return n;
			}
			
			n->Child(0) = std::shared_ptr<node>(new node());
			n->Child(1) = std::shared_ptr<node>(new node());

			auto w = parentRect.Width() - rectangle.Width();
			auto h = parentRect.Height() - rectangle.Height();

			if ( w > h )
			{				
				n->Child(0)->Rectangle() = math::rectangle::MakeRectangle(parentRect.Left(), parentRect.Top(), parentRect.Left() + w, parentRect.Bottom());				
				n->Child(1)->Rectangle() = math::rectangle::MakeRectangle(parentRect.Left() + w, parentRect.Top(), parentRect.Right(), parentRect.Bottom());
			}
			else
			{
				n->Child(0)->Rectangle() = math::rectangle::MakeRectangle(parentRect.Left(), parentRect.Top(), parentRect.Right(), parentRect.Top() + h);				
				n->Child(1)->Rectangle() = math::rectangle::MakeRectangle(parentRect.Left(), parentRect.Top() + h, parentRect.Right(), parentRect.Bottom());
			}

			return Insert(n->Child(0), rectangle);

		}
	}

private:

	std::shared_ptr<node> m_root;

};

} // render