#include "LayerStack.h"

namespace Arklumos
{

	LayerStack::LayerStack()
	{
		// Initializes the m_LayerInsert iterator to the beginning of the m_Layers vector.
		// This iterator is used to insert new layers into the vector at the correct position.
		this->m_LayerInsert = this->m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		// Delete all the layers (removing every layer pointer)
		for (Layer *layer : this->m_Layers)
		{
			delete layer;
		}
	}

	/*
		The PushLayer function adds a new layer to the m_Layers vector at the current position of the m_LayerInsert iterator.
		This is done using the emplace function, which constructs a new Layer object in-place at the iterator position.
	*/
	void LayerStack::PushLayer(Layer *layer)
	{
		this->m_LayerInsert = this->m_Layers.emplace(this->m_LayerInsert, layer);
	}

	/*
		The PushOverlay function adds a new layer to the end of the m_Layers vector using the emplace_back function.
	*/
	void LayerStack::PushOverlay(Layer *overlay)
	{
		this->m_Layers.emplace_back(overlay);
	}

	/*
		The PopLayer function removes a layer from the m_Layers vector. It does this by searching for the layer pointer using the std::find algorithm and erasing it from the vector if it is found.
		It also updates the m_LayerInsert iterator to point to the correct position in the vector after the layer is removed.
	*/
	void LayerStack::PopLayer(Layer *layer)
	{
		auto it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
		if (it != this->m_Layers.end())
		{
			this->m_Layers.erase(it);
			this->m_LayerInsert--;
		}
	}

	/*
		The PopOverlay function removes an overlay layer from the m_Layers vector. It does this by searching for the overlay pointer using the std::find algorithm and erasing it from the vector if it is found.
	*/
	void LayerStack::PopOverlay(Layer *overlay)
	{
		auto it = std::find(this->m_Layers.begin(), this->m_Layers.end(), overlay);
		if (it != this->m_Layers.end())
		{
			this->m_Layers.erase(it);
		}
	}

}