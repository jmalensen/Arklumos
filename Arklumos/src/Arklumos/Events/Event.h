#pragma once

#include "akpch.h"
#include "Arklumos/Core/Base.h"

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
		KeyTyped,
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

	class Event
	{

	public:
		bool Handled = false;

		// When it's virtual, require implementation later ;)
		virtual EventType GetEventType() const = 0;
		virtual const char *GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		// For debugging purpose
		virtual std::string ToString() const
		{
			return GetName();
		}

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	// The purpose of this class is to dispatch events to event handlers
	// using template metaprogramming = allow dispatching of different kinds of events
	class EventDispatcher
	{

	public:
		EventDispatcher(Event &event)
				: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template <typename T, typename F>
		/* The Dispatch function is a template function that takes a function object of type EventFn<T>, where T is a type derived from the Event class
			If the type of the event matches the static type of T, the function object is called with a casted Event object of type T.
			The result of the function is stored in the Handled member of the Event object, and the function returns true.
			If the event types do not match, the function returns false.
		*/
		bool Dispatch(const F &func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T &>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event &m_Event;
	};

	/* The operator<< function is defined outside of the EventDispatcher class and provides a way to output an Event object to an std::ostream.
		It uses the ToString member function of the Event class to convert the event to a string and outputs it to the std::ostream.
		This operator overload is used for debugging purposes, and it makes it easy to print out the contents of an Event object to the console.
	*/
	inline std::ostream &operator<<(std::ostream &os, const Event &e)
	{
		return os << e.ToString();
	}
}
