#include "akpch.h"
#include "LayerStack.h"

namespace Arklumos
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		// Delete all the layers (removing every layer pointer)
		for (Layer *layer : m_Layers)
		{
			delete layer;
		}
	}

	/*
		The PushLayer function takes a pointer to a Layer object and adds it to the m_Layers vector member of the LayerStack class at a specific index. The m_LayerInsertIndex variable is used to keep track of where to insert new layers.

		The emplace function is used to insert the layer pointer at a specific position in the m_Layers vector.
		It takes two arguments: the first is an iterator pointing to the position in the vector where the element should be inserted, and the second is the arguments to construct the object to be inserted.
		After inserting the new layer, m_LayerInsertIndex is incremented so that the next layer added will be inserted at the next position in the vector.
	*/
	void LayerStack::PushLayer(Layer *layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	/*
		The PushOverlay function adds a new layer to the end of the m_Layers vector using the emplace_back function.
	*/
	void LayerStack::PushOverlay(Layer *overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	/*
		The PopLayer function removes a layer from the m_Layers vector. It does this by searching for the layer pointer using the std::find algorithm and erasing it from the vector if it is found.
		It also updates the m_LayerInsertIndex iterator to point to the correct position in the vector after the layer is removed.
	*/
	void LayerStack::PopLayer(Layer *layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	/*
		The PopOverlay function removes an overlay layer from the m_Layers vector. It does this by searching for the overlay pointer using the std::find algorithm and erasing it from the vector if it is found.
	*/
	void LayerStack::PopOverlay(Layer *overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}