#pragma once

#define stringify(enum_name) # enum_name

class MyPacket : public KryNet::Packet {
public:
	enum class PacketType : BYTE {
		INFO,
		REQUEST,
		RESPONSE
	};

	// Constructors
	MyPacket(PacketType packetType) {
		PutBack(packetType);
	}

	MyPacket(const Packet& other) : Packet(other) {
	}

	// Getters
	PacketType Type(void) {
		return Peek<PacketType>(0);
	}
};

class InfoPacket : public MyPacket {
public:
	enum class InfoType : BYTE {
		STRING
	};

	// Constructors
	InfoPacket(InfoType infoType) : MyPacket(PacketType::INFO) {
		PutBack(infoType);
	}

	InfoPacket(MyPacket const& myPacket) : MyPacket(myPacket) {
	}

	// Getters
	InfoType SubType(void) {
		return Peek<InfoType>(sizeof(PacketType));
	}

};

class RequestPacket : public MyPacket {
public:
	enum class RequestType : BYTE {
		LOGIN,
		FILE
	};

	struct LoginData {
		char username[32];
		char password[32];
	};

	// Constructors
	RequestPacket(RequestType requestType) : MyPacket(PacketType::REQUEST) {
		PutBack(requestType);
	}

	RequestPacket(MyPacket const& myPacket) : MyPacket(myPacket) {
	}

	// Getters
	RequestType SubType(void) {
		return Peek<RequestType>(sizeof(RequestType));
	}

	LoginData Data(void) {
		return Peek<LoginData>(sizeof(PacketType) + sizeof(RequestType));
	}
};

class ResponsePacket : public MyPacket {
public:
	enum class ResponseType : BYTE {
		LOGIN,
		FILE
	};

	struct LoginData {
		bool success;

		enum class Error {
			INVALID_CREDENTIALS,
			ACCOUNT_BANNED
		} error;
	};

	// Constructors
	ResponsePacket(ResponseType responseType) : MyPacket(PacketType::RESPONSE) {
		PutBack(responseType);
	}

	ResponsePacket(MyPacket const& myPacket) : MyPacket(myPacket) {
	}

	// Getters
	ResponseType SubType(void) {
		return Peek<ResponseType>(sizeof(ResponseType));
	}

	template <class T>
	T Data(void) {
		return Peek<T>(sizeof(PacketType) + sizeof(ResponseType));
	}
};

