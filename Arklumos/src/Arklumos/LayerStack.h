#pragma once

#include "akpch.h"
#include "Arklumos/Core.h"
#include "Layer.h"

namespace Arklumos
{

	class ARKLUMOS_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);
		void PopLayer(Layer *layer);
		void PopOverlay(Layer *overlay);

		std::vector<Layer *>::iterator begin() { return this->m_Layers.begin(); }
		std::vector<Layer *>::iterator end() { return this->m_Layers.end(); }

	private:
		std::vector<Layer *> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

}