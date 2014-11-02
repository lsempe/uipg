#pragma once

#include "engine.core/sys.h"
#include "engine.math/math.h"
#include "engine.core/input.h"

namespace ui
{

class slot
{
public:

	slot()
		: m_count(0)
		, m_maximum(0)
		, m_stackSize(0)
	{}

	slot(slot&&) {}
	slot& operator = (slot&) { return *this; }

	int Stacks() const { if ( m_stackSize == 0 ) return 0; return m_count % m_stackSize; }
	int& Count() { return m_count; }
	int& Maximum() { return m_maximum; }

	int Count() const { return m_count; }
	int Maximum() const { return m_maximum; }

protected:

	int m_count;
	int m_maximum;
	int m_stackSize;

};

template <int m, int n, typename T>
class grid 
{
public:

	class slot_item : public slot
	{
	public:

		slot_item()
			: slot()
		{}

		slot_item(T&& item)
		{
			m_item = std::move(item.Item());
		}

		slot_item& operator = (slot_item& rhs)
		{
			if (&rhs != this)
			{
				m_item = std::move(rhs.Item());
			}
			return *this;
		}

		T& Item() { return m_item; }
		const T& Item() const { return m_item; }

		bool operator == (const slot_item& rhs) const
		{
			return m_item == rhs.Item();
		}

		bool operator != (const slot_item& rhs) const
		{
			return m_item != rhs.Item();
		}

		static slot_item Empty;

	protected:

		T m_item;
	};

	const slot_item& operator ()(int i, int j) { return m_items[i][j]; }

	int Rows() const { return m; }
	int Columns() const { return n; }

	void Remove(int m, int n, int count)
	{
		slot_item& slot = m_items[m][n];
		if ( slot.Count() != 0 )
		{
			slot.Count() = math::Max(0, slot.Count() - count);			
		}
		
		if ( slot.Count() == 0 )
		{
			m_items[m][n] = slot_item::Empty;
		}
	}

	bool HandleInput(float deltaTime, const input::input_state& inputState)
	{
		for (int j = 0; j < m; ++j )
		{
			for ( int i = 0; i < n; ++i )
			{
				slot_item& slot = m_items[i][j];
				if ( slot.Item().HandleInput(inputState) )
				{
					return true;
				}
			}
		}

		return false;
	}

	void Swap(int m0, int n0, int m1, int n1)
	{
		auto tmp = std::move(m_items[m0][n0]);
		m_items[m0][n0] = m_items[m1][n1];
		m_items[m1][n1] = tmp;		
	}
	
	const slot_item GetSlot(int m, int n)
	{
		slot_item& slot = m_items[m][n];
		return slot;
	}

	bool Add(int m, int n, T&& item, int count, int maximum, int stackSize, int& remainder)
	{
		UNREFERENCED(stackSize);

		slot_item& slot = m_items[m][n];
		if (slot.Count() > 0 && slot.Item() != item )
		{
			return false;
		}

		if ( slot.Count() == 0 )
		{
			slot.Maximum() = maximum;
			Add(slot, item, count, remainder);
		}
		else
		{
			Add(slot, item, count, remainder);
		}		
		
		return true;

	}

	bool IsEmpty(int m, int n) const { return m_items[m][n] == slot_item::Empty; }

protected:

	void Add(slot_item& slot, T& item, int count, int& remainder)
	{
		int difference = slot.Maximum() - count;
		if ( difference < 0 )
		{
			remainder = math::Max(count - slot.Maximum(), 0);
			count -= remainder;
		}			

		slot.Item() = std::move(item);
		slot.Count() = count;
	}

	slot_item m_items[m][n];
};

template<int m, int n, typename T>
typename grid<m,n,T>::slot_item grid<m,n,T>::slot_item::Empty;

} // ui