#pragma once

enum MessageType : BYTE {
	// A packet that does not signify a request and does not need a response.
	MSG_INFO,

	// A packet signifying a request.
	MSG_REQUEST,

	// A packet signifying a response to a request.
	MSG_RESPONSE
};

// TODO: Rename this.
enum MessageSubType : BYTE {
	SUBTYPE_STRING,

	SUBTYPE_LOGIN,

	SUBTYPE_FILE
};

//class SuperPacket : public KryNet::Packet {
//public:
//	SuperPacket(MessageType type, MessageSubType subType) : Packet() {
//		PutBack(type);
//		PutBack(subType);
//	}
//
//	SuperPacket(Packet const& packet) : Packet(packet) {
//	}
//
//	MessageType Type(void) {
//		return Peek<MessageType>(0);
//	}
//
//	MessageSubType SubType(void) {
//		return Peek<MessageSubType>(sizeof(MessageType));
//	}
//};

class MyPacket : public KryNet::Packet {
public:
	enum class PacketType : BYTE {
		INFO,
		REQUEST,
		RESPONSE
	};

	MyPacket(PacketType packetType) {
		PutBack(packetType);
	}

	MyPacket(const Packet& other) : Packet(other) {
	}

	PacketType Type(void) {
		return Peek<PacketType>(0);
	}
};

class InfoPacket : public MyPacket {
public:
	enum class InfoType : BYTE {
		STRING
	};

	InfoPacket(InfoType infoType) : MyPacket(PacketType::INFO) {
		PutBack(infoType);
	}

	InfoPacket(MyPacket const& myPacket) : MyPacket(myPacket) {
	}

	InfoType SubType(void) {
		return Peek<InfoType>(sizeof(PacketType));
	}
};

//class RequestPacket : public MyPacket {
//public:
//	enum class RequestType : BYTE {
//		LOGIN,
//		FILE
//	};
//
//	RequestPacket(RequestType requestType) : MyPacket(PacketType::REQUEST) {
//		PutBack(requestType);
//	}
//};
//
//class ResponsePacket : public MyPacket {
//public:
//	enum class ResponseType : BYTE {
//		LOGIN,
//		FILE
//	};
//
//	ResponsePacket(ResponseType responseType) : MyPacket(PacketType::RESPONSE) {
//		PutBack(responseType);
//	}
//};

