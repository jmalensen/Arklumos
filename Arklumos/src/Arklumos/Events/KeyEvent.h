#pragma once

#include "Arklumos/Events/Event.h"
#include "Arklumos/Core/Input.h"

namespace Arklumos
{
	/// All the events for the keys
	// KeyEvent
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(KeyCode keycode)
				: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	// KeyPressedEvent
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
				: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		int GetRepeatCount() const
		{
			return m_RepeatCount;
		}

		// For debugging purpose
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	// KeyReleasedEvent
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
				: KeyEvent(keycode) {}

		// For debugging purpose
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	// KeyTypedEvent
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
				: KeyEvent(keycode) {}

		// For debugging purpose
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}