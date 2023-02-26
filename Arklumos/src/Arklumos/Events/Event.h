#pragma once

#include "akpch.h"
#include "Arklumos/Core.h"

namespace Arklumos
{

	// The events in Arklumos are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.

	// Enum to store all event type we will need (1 -> 14)
	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0), // All events for the application
		EventCategoryInput = BIT(1),			 // All events for the input
		EventCategoryKeyboard = BIT(2),		 // All events for the keyboard
		EventCategoryMouse = BIT(3),			 // All events for the mouse
		EventCategoryMouseButton = BIT(4)	 // All events for the mouse buttons
	};

// EVENT_CLASS_TYPE
#define EVENT_CLASS_TYPE(type)                    \
	static EventType GetStaticType()                \
	{                                               \
		return EventType::type;                       \
	}                                               \
	virtual EventType GetEventType() const override \
	{                                               \
		return GetStaticType();                       \
	}                                               \
	virtual const char *GetName() const override    \
	{                                               \
		return #type;                                 \
	}

// EVENT_CLASS_CATEGORY
#define EVENT_CLASS_CATEGORY(category)          \
	virtual int GetCategoryFlags() const override \
	{                                             \
		return category;                            \
	}

	class ARKLUMOS_API Event
	{
		friend class EventDispatcher;

	public:
		// When it's virtual, require implementation later
		virtual EventType GetEventType() const = 0;
		virtual const char *GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		// For debugging purpose
		virtual std::string ToString() const
		{
			return GetName();
		}

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template <typename T>
		using EventFn = std::function<bool(T &)>;

	public:
		EventDispatcher(Event &event)
				: m_Event(event)
		{
		}

		template <typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T *)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event &m_Event;
	};

	inline std::ostream &operator<<(std::ostream &os, const Event &e)
	{
		return os << e.ToString();
	}
}
