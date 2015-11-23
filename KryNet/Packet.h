/*
* Copyright 2015 John Fedorchak

* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

* http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#pragma once

using HEADER_TYPE = uint16_t;

namespace KryNet {
	class Packet {
	public:
		// The maximum packet size, including the packet header and body.
		static const size_t max_size = 1024;

		// The size of the packet's header.
		static const size_t header_size = sizeof(HEADER_TYPE);

		// The maximum packet body size.
		static const size_t max_body_size = max_size - header_size;
	protected:
		// The packet's internal buffer.
		BYTE data_[header_size + max_body_size];

		// The packet's current body size.
		HEADER_TYPE body_size_;
	public:
		Packet(void) : body_size_(0) {
		}

		/// <summary>Adds an item to the back of the packet's body.</summary>
		/// <param name="t">The item to add to the packet's body.</param>
		template <class T>
		void PutBack(const T& t);

		/// <summary>Gets (without removing) data at the specified offset in the packet's body.</summary>
		/// <param name="offset">The offset within the packet's body.</param>
		/// <returns>Returns data, of the specified template type, retreived from the packet's body.</returns>
		template <class T>
		T Peek(size_t offset);

		/// <summary>Clears the packet's data.</summary>
		void Clear(void) {
			ZeroMemory(data_, max_size);
			body_size_ = 0;
		}

		/// <summary>Gets the packet's current size (header and body).</summary>
		/// <returns>Returns the packet's current size.</returns>
		size_t Size(void) const {
			return header_size + body_size_;
		}

		/// <summary>Gets the packet's current body size.</summary>
		/// <returns>Returns the packet's current body size.</returns>
		size_t BodySize(void) const {
			return body_size_;
		}

		/// <summary>Decodes the packet's body size from its header.</summary>
		/// <returns>Returns the packet's body size.</returns>
		HEADER_TYPE DecodeHeader(void) {
			return body_size_ = *reinterpret_cast<const HEADER_TYPE*>(data_);
		}

		/// <summary>Encodes the packet's current body size into its header.</summary>
		void EncodeHeader(void) {
			// Convert the current body size to the header type.
			auto bodySize = static_cast<HEADER_TYPE>(body_size_);

			// Copy the current body size into the packet buffer.
			memcpy_s(data_, header_size, &bodySize, header_size);
		}

		/// <summary>Gets a pointer to the packet's internal buffer.</summary>
		/// <returns>Returns a pointer to the packet's internal buffer.</returns>
		BYTE* Data(void) {
			return data_;
		}

		/// <summary>Gets a pointer to the packet's body.</summary>
		/// <returns>Returns a pointer to the packet's body.</returns>
		BYTE* Body(void) const {
			return const_cast<BYTE*>(data_ + header_size);
		}

		~Packet(void) {
		}

	private:
		template <class T>
		void PutData(HEADER_TYPE offset, const T& t);
	};

	template <class T>
	void Packet::PutBack(const T& t) {
		PutData(body_size_, t);
	}

	template <class T>
	T Packet::Peek(size_t offset) {
		// Make sure the body is large enough to be holding the requested type.
		if (sizeof(T) > BodySize()) {
			throw std::range_error("Not enough packet data.");
		}

		T t;

		// Copy the data out of the buffer.
		memcpy_s(&t, sizeof(T), &data_[header_size + offset], sizeof(T));

		return t;
	}

	template <class T>
	void Packet::PutData(HEADER_TYPE offset, const T& t) {
		// Make sure the packet is not too full to fit the requested type.
		if (offset + sizeof(T) > max_body_size) {
			throw std::range_error("Packet size exceeded.");
		}

		auto pSource = reinterpret_cast<const BYTE*>(std::addressof(t));

		// Copy the data into the buffer.
		memcpy_s(&data_[header_size + offset], sizeof(T), pSource, sizeof(T));

		// Increase the body size.
		body_size_ += sizeof(T);

		// Re-encode the header.
		EncodeHeader();
	}
}

