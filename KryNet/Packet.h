#pragma once

using HEADER_TYPE = uint16_t;

namespace KryNet {
	class Packet {
	public:
		static const size_t max_size = 1024;
		static const size_t header_size = sizeof(HEADER_TYPE);
		static const size_t max_body_size = max_size - header_size;
	protected:
		// Buffer.
		BYTE data_[header_size + max_body_size];

		// Size of body data.
		HEADER_TYPE body_size_;
	public:
		// Constructors
		Packet(void) : body_size_(0) {
		}

		// Methods
		template <class T>
		void PutBack(const T& t);

		//template <class T>
		//void PutBack(const T& t, size_t uSize);

		template <class T>
		T Peek(size_t uOffset);

		template <class T>
		T Peek(size_t uOffset, size_t uSize);

		/*	template <class T>
			T GetFront(void);
	
			template <class T>
			T GetFront(size_t uSize);*/

		void Clear(void) {
			ZeroMemory(data_, max_size);
		}

		// Getters
		size_t Size(void) const {
			// Full size of packet (header and body).
			return header_size + body_size_;
		}

		size_t BodySize(void) const {
			return body_size_;
		}

		HEADER_TYPE DecodeHeader(void) {
			return body_size_ = *reinterpret_cast<const HEADER_TYPE*>(data_);
		}

		void EncodeHeader(void) {
			auto val = static_cast<HEADER_TYPE>(body_size_);
			memcpy_s(data_, header_size, &val, header_size);
		}

		BYTE* Data(void) {
			return data_;
		}

		BYTE* Body(void) const {
			return const_cast<BYTE*>(data_ + header_size);
		}

		// Destructor
		~Packet(void) {
		}

	private:
		// Methods
		template <class T>
		void PutData(HEADER_TYPE uOffset, const T& t);

		//template <class T>
		//void PutData(HEADER_TYPE uOffset, const T& t, size_t uSize);
	};

	template <class T>
	void Packet::PutBack(const T& t) {
		PutData(body_size_, t);
	}

	//template <class T>
	//void Packet::PutBack(const T& t, size_t uSize) {
	//	PutData(body_size_, t, uSize);
	//}

	template <class T>
	T Packet::Peek(size_t uOffset) {
		return Peek<T>(uOffset, sizeof(T));
	}

	template <class T>
	T Packet::Peek(size_t uOffset, size_t uSize) {
		if (uSize > BodySize()) {
			throw std::range_error("Not enough packet data.");
		}

		T t;

		memcpy_s(&t, uSize, &data_[uOffset], uSize);

		return t;
	}

	template <class T>
	void Packet::PutData(HEADER_TYPE uOffset, const T& t) {
		//PutData(uOffset, t, sizeof(T));

		if (uOffset + sizeof(T) > max_body_size) {
			throw std::range_error("Packet size exceeded.");
		}

		auto pSource = reinterpret_cast<const BYTE*>(std::addressof(t));
		memcpy_s(LPVOID(Body() + uOffset), sizeof(T), pSource, sizeof(T));

		body_size_ += sizeof(T);

		// Update the header.
		EncodeHeader();
	}

	//template <class T>
	//void Packet::PutData(HEADER_TYPE uOffset, const T& t, size_t uSize) {
	//	if (uOffset + uSize > max_body_size) {
	//		throw std::range_error("Packet size exceeded.");
	//	}

	//	auto pSource = reinterpret_cast<const BYTE*>(std::addressof(t));
	//	memcpy_s(LPVOID(Body() + uOffset), uSize, pSource, uSize);

	//	body_size_ += uSize;

	//	// Update the header.
	//	EncodeHeader();
	//}
}

