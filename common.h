#pragma once


class xEvtIPComm : public wxEvent {
public:
	enum EVENT : int8_t {
		EVT_CONNECTED,
		EVT_NOT_CONNECTED,
		EVT_DISCONNECTED,
		EVT_MESSAGE,
		
		nTYPE
	};

	EVENT m_evt;
	std::string m_msg;
protected:
	xEvtIPComm(xEvtIPComm const&) = default;
public:
	xEvtIPComm(int winid, wxEventType eventType, EVENT evt, std::string const& msg) : wxEvent(winid, eventType), m_evt(evt), m_msg(std::move(msg)) { }

	virtual wxEvent *Clone() const override { return new xEvtIPComm(*this); }
};
//wxDEFINE_EVENT(wxEVT_IP_COMM, xEvtIPComm);
inline static const wxEventTypeTag< xEvtIPComm > wxEVT_IP_COMM( wxNewEventType() );


// Class to manage the memory to be used for handler-based custom allocation.
// It contains a single block of memory which may be returned for allocation
// requests. If the memory is in use when an allocation request is made, the
// allocator delegates allocation to the global heap.
class handler_memory {
public:
	handler_memory() = default;

	handler_memory(const handler_memory&) = delete;
	handler_memory& operator=(const handler_memory&) = delete;

	void* allocate(std::size_t size) {
		static_assert(sizeof(storage_) == 1024);
		if (!in_use_ && size < sizeof(storage_)) {
			in_use_ = true;
			return &storage_;
		}
		else {
			return ::operator new(size);
		}
	}

	void deallocate(void* pointer) {
		if (pointer == &storage_) {
			in_use_ = false;
		}
		else {
			::operator delete(pointer);
		}
	}

private:
	// Storage space used for handler-based custom memory allocation.
	using buffer_t = std::array<std::byte, 1024>;
	//typename std::aligned_storage<1024>::type storage_;
	alignas(buffer_t) std::byte storage_[sizeof(buffer_t)];

	// Whether the handler-based custom allocation storage has been used.
	bool in_use_{};
};



// The allocator to be associated with the handler objects. This allocator only
// needs to satisfy the C++11 minimal allocator requirements.
template <typename T>
class handler_allocator {
public:
	using value_type = T;

	explicit handler_allocator(handler_memory& mem) noexcept : memory_(mem) { }

	template <typename U>
	handler_allocator(const handler_allocator<U>& other) noexcept : memory_(other.memory_) { }

	bool operator==(const handler_allocator& other) const noexcept { return &memory_ == &other.memory_; }

	bool operator!=(const handler_allocator& other) const noexcept { return &memory_ != &other.memory_; }

	T* allocate(std::size_t n) const {
		return static_cast<T*>(memory_.allocate(sizeof(T) * n));
	}

	void deallocate(T* p, std::size_t /*n*/) const {
		return memory_.deallocate(p);
	}

private:
	template <typename> friend class handler_allocator;

	// The underlying memory.
	handler_memory& memory_;
};
