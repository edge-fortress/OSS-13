#pragma once

#include <chrono>

#include <Shared/Network/Archive.h>
#include <Shared/ErrorHandling.h>

uf::Archive &operator&(uf::Archive &ar, long int &li);

template<class T>
uf::Archive &operator&(uf::Archive &ar, std::vector<T> &vector) {
	if (ar.IsOutput()) {
		EXPECT_WITH_MSG(vector.size() == 0, "Try unpack Archive to non-empty vector");
		sf::Int32 size;
		ar >> size;
		vector.resize(size);
	} else {
		ar << sf::Int32(vector.size());
	}
	for (auto &item : vector) {
		ar & item;
	}
	return ar;
}

uf::Archive &operator&(uf::Archive &ar, uf::Direction &d);

template<class T>
uf::Archive &operator&(uf::Archive &ar, uf::vec2<T> &vec) {
	ar & vec.x;
	ar & vec.y;
	return ar;
}

template<class T>
uf::Archive &operator&(uf::Archive &ar, uf::vec3<T> &vec) {
	ar & vec.x;
	ar & vec.y;
	ar & vec.z;
	return ar;
}

template<class RepType, class Ratio>
uf::Archive &operator&(uf::Archive &ar, std::chrono::duration<RepType, Ratio> &duration) {
	if (ar.IsOutput()) {
		RepType buf;
		ar >> buf;
		duration = std::chrono::duration<RepType, Ratio>(buf);
	} else {
		ar << duration.count();
	}
	return ar;
}

