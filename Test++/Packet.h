#pragma once

class Packet {
public:
	static const size_t header_length = 4;
	static const size_t max_body_length = 1024;
private:
	std::vector<BYTE> data_;
public:
	// Constructors
	Packet(void) {}

	// Methods

	// Getters
	size_t Length(void) const { return header_length + data_.size(); }
	size_t BodyLength(void) const { return data_.size(); }

	std::vector<BYTE> Data(void) const { return data_; }

	~Packet(void) {}
};