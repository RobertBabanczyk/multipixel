#pragma once

#include "server.hpp"
#include "util/smartptr.hpp"
#include "util/types.hpp"
#include <atomic>
#include <mutex>
#include <vector>

struct ChunkSystem;
struct Session;

struct ChunkPixel {
	UInt2 pos;
	u8 r, g, b;
};

struct Chunk {
private:
	ChunkSystem *chunk_system;
	Int2 position;
	u32 chunk_size;

	std::atomic<bool> modified = false;

	std::mutex mtx_access;

	uniqdata<u8> image;

	std::atomic<bool> linked_sessions_empty = true;
	std::vector<Session *> linked_sessions;

	void allocateImage_nolock();
	void sendChunkDataToSession_nolock(Session *session);
	uniqdata<u8> encodeChunkData_nolock();
	void decodeChunkData_nolock(const void *data, size_t size);
	void getPixel_nolock(UInt2 chunk_pixel_pos, u8 *r, u8 *g, u8 *b);

public:
	Chunk(ChunkSystem *chunk_system, Int2 position, uniqdata<u8> *compressed_chunk_data);
	~Chunk();

	friend struct ChunkSystem;

	void linkSession(Session *session);
	void unlinkSession(Session *session);
	bool isLinkedSessionsEmpty();

	uniqdata<u8> encodeChunkData();
	bool isModified();
	void setModified(bool n);

	void setPixels(ChunkPixel *pixels, size_t count);
	Int2 getPosition() const;
};
