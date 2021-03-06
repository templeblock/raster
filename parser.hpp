/*

Copyright (c) 2017-2018, Elias Aebi
All rights reserved.

*/

#include "document.hpp"
#include <string>

class Character {
	char c;
public:
	constexpr Character(char c): c(c) {}
	constexpr operator char() const {
		return c;
	}
	constexpr bool between(Character min, Character max) const {
		return c >= min.c && c <= max.c;
	}
	constexpr int get_digit() const {
		return between('0', '9') ? c - '0' : (between('a', 'f') ? c - 'a' + 10 : (between('A', 'F') ? c - 'A' + 10 : -1));
	}
};

class StringView {
	const char* s;
	size_t size;
	static constexpr size_t strlen(const char* s, size_t i = 0) {
		return *s == '\0' ? i : strlen(s + 1, i + 1);
	}
	static constexpr char to_lower(char c) {
		return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c;
	}
	static constexpr int strncmp(const char* s0, const char* s1, size_t n) {
		return n == 0 ? 0 : (*s0 != *s1 ? *s0 - *s1 : strncmp(s0 + 1, s1 + 1, n - 1));
	}
public:
	constexpr StringView(const char* s, size_t length): s(s), size(length) {}
	constexpr StringView(): s(nullptr), size(0) {}
	constexpr StringView(const char* s): s(s), size(strlen(s)) {}
	StringView(const std::string& s): s(s.data()), size(s.size()) {}
	constexpr bool operator ==(const StringView& rhs) const {
		return size != rhs.size ? false : strncmp(s, rhs.s, size) == 0;
	}
	constexpr bool operator !=(const StringView& rhs) const {
		return !operator ==(rhs);
	}
	constexpr bool operator <(const StringView& rhs) const {
		return size != rhs.size ? size < rhs.size : strncmp(s, rhs.s, size) < 0;
	}
	constexpr operator bool() const {
		return size > 0;
	}
	constexpr bool has_next() const {
		return size > 0;
	}
	Character next() {
		Character c(*s);
		++s;
		--size;
		return c;
	}
	constexpr StringView operator -(const StringView& rhs) const {
		return StringView(rhs.s, s - rhs.s);
	}
	std::string to_string() const {
		return std::string(s, size);
	}
};

Document parse(const StringView& svg);
