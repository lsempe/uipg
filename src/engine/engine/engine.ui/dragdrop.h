#pragma once

#include "engine.math/vector.h"
#include "engine.core/event_handler.h"
#include "engine.core/input.h"

namespace ui
{

class idrag_item
{
};

class draggable;

class drag_manager 
{
public:

	static drag_manager& Get() 
	{
		static drag_manager instance;
		return instance;
	}

	drag_manager()
		: m_draggedObject(nullptr)
	{
	}

	void Set(const draggable* draggedObject)
	{
		m_draggedObject = draggedObject;
	}

	void Clear()
	{
		m_draggedObject = nullptr;
	}

	const draggable* DraggedObject() { return m_draggedObject; }

private:

	const draggable* m_draggedObject;

	

};

class draggable
{
public:

	enum class State
	{
		NoInput,
		Pressed,
		Dragging,
		Released
	};

	State m_state;

	draggable()
		: m_state(State::NoInput)
		, m_bounds()
	{}

	draggable(const math::rectangle& bounds)
		: m_state(State::NoInput)
	{}

	struct drag_event
	{
		math::vector2 m_position;

		drag_event(const math::vector2& position)
			: m_position(position)
		{
		}
	};

	event_handler<drag_event> OnDragBegin() { return m_onDragBegin; }
	event_handler<drag_event> OnDrop() { return m_onDrop; }

	virtual bool HandleInput(const input::input_state& inputState)
	{
		switch ( m_state )
		{
		case State::NoInput:
			return HandleInputState(inputState);
		case State::Pressed:
			return HandleInputPressed(inputState);
		case State::Dragging:
			return HandleDragging(inputState);
		case State::Released:
			return HandleRelease(inputState);
		}

		return false;
	}
	
	bool HandleInputState(const input::input_state& inputState)
	{
		if (drag_manager::Get().DraggedObject() != nullptr)
			return false;

            if (IsPressed(inputState))
            {
                m_startDragPosition = GetCurrentPosition(inputState);
                m_state = State::Pressed;
                return true;
            }

            return false;
		return false;
	}

	bool HandleInputPressed(const input::input_state& inputState)
	{
		auto mouse = inputState.GetMouse();
		if ( mouse != nullptr )
		{
			if (!IsPressed(inputState))
			{
				m_state = State::Released;
				return false;
			}

			//if ( m_bounds.Contains( mouse->GetPosition() ) )
			{
				if ( m_state == State::Pressed )
				{
					m_state = State::Dragging;
					return true;
				}

				m_state = State::Pressed;
				return true;
			}
		}
		return false;
	}
	
	bool HandleDragging(const input::input_state& inputState)
	{
		if (IsReleased(inputState))
        {
            m_state = State::Released;
            return true;
        }

        m_startDragPosition = GetCurrentPosition(inputState);

        if (m_isDragging)
        {
            m_position = m_startDragPosition - m_delta;
            m_bounds.Left() = (int)(m_position.x());
            m_bounds.Top() = (int)(m_position.y());
        }
        else
        {
           // if (m_bounds.Contains(m_startDragPosition))
            {
                    // Start dragging
				math::vector2 itemPosition(m_bounds.Left(), m_bounds.Top());
                m_delta = m_startDragPosition - itemPosition;

                m_isDragging = true;
                    
				m_onDragBegin.Invoke(this, drag_event(m_startDragPosition));

				drag_manager::Get().Set(this);

                return true;
            }
        }


        return true;
	}
	
	bool HandleRelease(const input::input_state& inputState)
	{
		  m_state = State::NoInput;

        if (m_isDragging)
        {
            m_isDragging = false;

			auto position = GetCurrentPosition(inputState);
			
            m_onDrop.Invoke(this, drag_event(position));
            
            //DragManager.m_object = null;

            return true;
        }
        return false;
	}

	bool IsPressed(const input::input_state& input)
    {
		auto mouse = input.GetMouse();
		if ( mouse != nullptr )
		{
			return ( mouse->ButtonPressed(input::mouse::Left) );
		}
		return false;
    }

	bool IsReleased(const input::input_state& input)
    {
		auto mouse = input.GetMouse();
		if ( mouse != nullptr )
		{
			return ( mouse->ButtonReleased(input::mouse::Left) );
		}
		return true;
    }

	math::vector2 GetCurrentPosition(const input::input_state& input)
    {
		auto mouse = input.GetMouse();
		if ( mouse != nullptr )
		{
			return ( mouse->GetPosition() );
		}

        return math::vector2::Zero;
    }

	math::rectangle& Bounds() { return m_bounds; }
	bool IsDragging() const { return m_isDragging; }

protected:

	bool m_isDragging;
    math::rectangle m_bounds;

	math::vector2 m_position;
	math::vector2 m_startDragPosition;
	math::vector2 m_delta;

	event_handler<drag_event> m_onDrop;
	event_handler<drag_event> m_onDragBegin;
};




} // ui