#pragma once

#include <functional>
#include <vector>

#include <Shared/IFaces/ICopyable.h>
#include <Shared/Range.hpp>
#include <Shared/Types.hpp>

namespace uf {

struct GridTransformation {
	vec3i originDelta = {0,0,0};
	vec3i sizeDelta = {0,0,0};
};

template<typename T>
class Grid: public ICopyable {
public:
	typedef typename std::vector<T>::iterator        flat_iterator;
	typedef typename std::vector<T>::const_iterator  const_flat_iterator;
	typedef typename std::vector<T>::reference       reference;
	typedef typename std::vector<T>::const_reference const_reference;

	class iterator {
		Grid<T>& grid;
		vec3u dataPos;
	public:
		iterator(Grid<T>& _grid): grid(_grid) {};
		iterator(Grid<T>& _grid, vec3u _dataPos): grid(_grid), dataPos(_dataPos) {};
		iterator& operator++();
		std::pair<reference, vec3u> operator*();
		bool operator!=(const iterator& it) const;
	};

	void SetSize(vec3u size);
	void SetSize(uint x, uint y, uint z);
	vec3u GetSize() const;

	iterator begin();
	iterator end();

	const Range<flat_iterator> Items();
	const Range<const_flat_iterator> Items() const;

	reference       At(vec3u pos);
	reference       At(uint x, uint y, uint z);
	const_reference At(vec3u pos) const;
	const_reference At(uint x, uint y, uint z) const;

	void SetMovedCallback(std::function<void(vec3u, vec3u)> movedCallback);
	void SetRemovedCallback(std::function<void(vec3u)> removedCallback);

	void Transform(const GridTransformation transformation);
private:
	static uint flatIndex (const vec3u xyz, uint w, uint h);
	vec3u dataSize;
	std::vector<T> data;
	std::function<void(vec3u, vec3u)> movedCallback = [](vec3u, vec3u){};
	std::function<void(vec3u)> removedCallback = [](vec3u){};
};

} //namespace uf

#include "Grid.inl"
