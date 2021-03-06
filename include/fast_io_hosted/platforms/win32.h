#pragma once

namespace fast_io
{
namespace details
{
#ifndef _MSC_VER
inline void* create_win32_temp_file()
{
	std::array<wchar_t,512> arr;
	std::uint32_t temp_path_size{win32::GetTempPathW(300,arr.data())};
	if(temp_path_size==0)
		throw_win32_error();
	for(std::size_t i{};i!=128;++i)
	{
		std::array<char,512> buffer1;
		secure_clear_guard<char> guard(buffer1.data(),buffer1.size());
		if(!win32::SystemFunction036(buffer1.data(),buffer1.size()))[[unlikely]]
			continue;
		fast_io::sha256 sha;
		fast_io::hash_processor processor(sha);
		write(processor,buffer1.begin(),buffer1.end());
		processor.do_final();
		ospan osp(arr);
		obuffer_set_curr(osp,arr.data()+temp_path_size);
		print_freestanding(osp,sha,L".tmp");
		put(osp,0);
		void* handle = win32::CreateFileW(arr.data(),
		0x40000000|0x80000000,	//GENERIC_READ|GENERIC_WRITE
		0,		//Prevents other processes from opening a file or device if they request delete, read, or write access.
		nullptr,
		1,		//CREATE_NEW
		0x100|0x04000000|0x01000000|0x08000000,	//FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE|FILE_FLAG_POSIX_SEMANTICS|FILE_FLAG_SEQUENTIAL_SCAN
		nullptr);
		if(handle==(void*) (std::intptr_t)-1)[[unlikely]]
		{
			auto last_error{win32::GetLastError()};
			if(last_error==80)	//ERROR_FILE_EXISTS
				continue;
			throw_win32_error();
		}
		return handle;
	}
	throw_win32_error();
	return nullptr;
}
#endif
template<typename... Args>
requires (sizeof...(Args)==4)
inline auto create_io_completion_port(Args&&... args)
{
	auto ptr{fast_io::win32::CreateIoCompletionPort(std::forward<Args>(args)...)};
	if(ptr==nullptr)[[unlikely]]
		throw_win32_error();
	return ptr;
}

struct win32_open_mode
{
std::uint32_t dwDesiredAccess{};
std::uint32_t dwShareMode{1|2};//FILE_SHARE_READ|FILE_SHARE_WRITE
bool inherit{};
std::uint32_t dwCreationDisposition{};	//depends on EXCL
std::uint32_t dwFlagsAndAttributes{};//=128|0x10000000;//FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS
};

inline void* win32_create_file_a_impld(wchar_t const* lpFileName,win32_open_mode const& mode)
{
	win32::security_attributes sec_attr{sizeof(win32::security_attributes),nullptr,true};
	auto handle(win32::CreateFileW(lpFileName,
	mode.dwDesiredAccess,
	mode.dwShareMode,
	mode.inherit?std::addressof(sec_attr):nullptr,
	mode.dwCreationDisposition,
	mode.dwFlagsAndAttributes,
	nullptr));
	if(handle==((void*) (std::intptr_t)-1))
		throw_win32_error();
	return handle;
}

template<std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
inline void* win32_create_file_impl(basic_cstring_view<char_type> path,win32_open_mode const& mode)
{
	if constexpr(std::same_as<char_type,char>)
	{
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[path.size()+1]);
		*utf_code_convert(path.data(),path.data()+path.size(),buffer.get())=0;
		return win32_create_file_a_impld(buffer.get(),mode);
	}
	else
	{
		return win32_create_file_a_impld(path.data(),mode);
	}
}


inline constexpr win32_open_mode calculate_win32_open_mode(open_mode value,perms pm)
{
	win32_open_mode mode;
	if((value&open_mode::app)!=open_mode::none)
		mode.dwDesiredAccess|=4;//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
		mode.dwDesiredAccess|=0x40000000;//GENERIC_WRITE
	if(((value&open_mode::in)!=open_mode::none)|((value&open_mode::app)!=open_mode::none))
	{
		mode.dwDesiredAccess|=0x80000000;//GENERIC_READ
		if((value&open_mode::out)!=open_mode::none&&((value&open_mode::app)!=open_mode::none&&(value&open_mode::trunc)!=open_mode::none))
			mode.dwDesiredAccess|=0x40000000;//GENERIC_WRITE
	}
/*
Referenced partially from ReactOS
https://github.com/changloong/msvcrt/blob/master/io/wopen.c



https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew

CREATE_ALWAYS
2
Creates a new file, always.
If the specified file exists and is writable, the function overwrites the file, the function succeeds, and last-error code is set to ERROR_ALREADY_EXISTS (183).

If the specified file does not exist and is a valid path, a new file is created, the function succeeds, and the last-error code is set to zero.

For more information, see the Remarks section of this topic.

CREATE_NEW
1
Creates a new file, only if it does not already exist.
If the specified file exists, the function fails and the last-error code is set to ERROR_FILE_EXISTS (80).

If the specified file does not exist and is a valid path to a writable location, a new file is created.

OPEN_ALWAYS
4
Opens a file, always.
If the specified file exists, the function succeeds and the last-error code is set to ERROR_ALREADY_EXISTS (183).

If the specified file does not exist and is a valid path to a writable location, the function creates a file and the last-error code is set to zero.

OPEN_EXISTING
3
Opens a file or device, only if it exists.
If the specified file or device does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND (2).

For more information about devices, see the Remarks section.

TRUNCATE_EXISTING
5
Opens a file and truncates it so that its size is zero bytes, only if it exists.
If the specified file does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND (2).

The calling process must open the file with the GENERIC_WRITE bit set as part of the dwDesiredAccess parameter.



File access
mode string	Meaning	Explanation	Action if file
already exists	Action if file
does not exist
"r"	read	Open a file for reading	read from start	failure to open
"w"	write	Create a file for writing	destroy contents	create new
"a"	append	Append to a file	write to end	create new
"r+"	read extended	Open a file for read/write	read from start	error
"w+"	write extended	Create a file for read/write	destroy contents	create new
"a+"	append extended	Open a file for read/write	write to end	create new
*/
	if ((value&open_mode::trunc)!=open_mode::none)
	{
		if((value&open_mode::excl)!=open_mode::none)
			mode.dwCreationDisposition=1;// CREATE_NEW
		else
			mode.dwCreationDisposition=2;// CREATE_ALWAYS
	}
	else if((value&open_mode::in)==open_mode::none)
	{
		if((value&open_mode::app)!=open_mode::none)
			mode.dwCreationDisposition=4;//OPEN_ALWAYS
		else if((value&open_mode::out)!=open_mode::none)
		{
			if((value&open_mode::excl)!=open_mode::none)
				mode.dwCreationDisposition=1;// CREATE_NEW
			else
				mode.dwCreationDisposition=2;// CREATE_ALWAYS
		}
	}
	else if((value&open_mode::app)!=open_mode::none)
		mode.dwCreationDisposition=4;//OPEN_ALWAYS
	else
		mode.dwCreationDisposition=3;//OPEN_EXISTING
	if((value&open_mode::creat)!=open_mode::none)
	{
		if((value&open_mode::excl)!=open_mode::none)
			mode.dwCreationDisposition=1;//CREATE_NEW
		else
			mode.dwCreationDisposition=4;//OPEN_ALWAYS
	}
	if((value&open_mode::direct)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x20000000;//FILE_FLAG_NO_BUFFERING
	if((value&open_mode::sync)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x80000000;//FILE_FLAG_WRITE_THROUGH
	if((value&open_mode::no_block)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x40000000;//FILE_FLAG_OVERLAPPED
	if((value&open_mode::follow)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00200000;	//FILE_FLAG_OPEN_REPARSE_POINT
	if((value&open_mode::directory)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x02000000;	//FILE_FLAG_BACKUP_SEMANTICS
		if(mode.dwCreationDisposition==0)
		{
			mode.dwDesiredAccess|=0x80000000;	//GENERIC_READ
			mode.dwCreationDisposition=3;		//OPEN_EXISTING
		}
	}
	bool set_normal{true};
	if((value&open_mode::archive)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x20;		//FILE_ATTRIBUTE_ARCHIVE
		set_normal={};
	}
	if((value&open_mode::encrypted)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x4000;		//FILE_ATTRIBUTE_ENCRYPTED
		set_normal={};
	}
	if((value&open_mode::hidden)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x2;			//FILE_ATTRIBUTE_HIDDEN
		set_normal={};
	}
	if((value&open_mode::compressed)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x800;		//FILE_ATTRIBUTE_COMPRESSED
		set_normal={};
	}
	if((value&open_mode::system)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x4;							//FILE_ATTRIBUTE_SYSTEM
		set_normal={};
	}
	if((value&open_mode::offline)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x1000;						//FILE_ATTRIBUTE_OFFLINE
		set_normal={};
	}
	if(set_normal)[[likely]]
		mode.dwFlagsAndAttributes|=0x80;						//FILE_ATTRIBUTE_NORMAL
	if((value&open_mode::random_access)==open_mode::none)
		mode.dwFlagsAndAttributes|=0x10000000;		//FILE_FLAG_SEQUENTIAL_SCAN
	else
		mode.dwFlagsAndAttributes|=0x08000000;		//FILE_FLAG_RANDOM_ACCESS
	if((value&open_mode::no_recall)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00100000;					//FILE_FLAG_OPEN_NO_RECALL
	if((value&open_mode::case_insensitive)==open_mode::none)
		mode.dwFlagsAndAttributes|=0x01000000;					//FILE_FLAG_POSIX_SEMANTICS
	if((value&open_mode::session_aware)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00800000;					//FILE_FLAG_SESSION_AWARE
	if((value&open_mode::temporary)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x04000000;					//FILE_FLAG_DELETE_ON_CLOSE
		mode.dwFlagsAndAttributes|=0x100;					//FILE_ATTRIBUTE_TEMPORARY
	}
	if((pm&perms::owner_write)==perms::none)
		mode.dwFlagsAndAttributes|=1;						//FILE_ATTRIBUTE_READONLY
	return mode;
}

}
template<std::integral ch_type>
class basic_win32_io_observer
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	using async_scheduler_type = basic_win32_io_observer<char>;
	native_handle_type handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	explicit operator bool() const noexcept
	{
		return handle!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
	}
	explicit constexpr operator basic_nt_io_observer<char_type>() const noexcept
	{
		return basic_nt_io_observer<char_type>{handle};
	}
	inline native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		return temp;
	}
};

template<std::integral ch_type>
inline constexpr nt_at_entry at(basic_win32_io_observer<ch_type> wiob) noexcept
{
	return nt_at_entry{wiob.handle};
}

template<std::integral ch_type>
inline constexpr basic_win32_io_observer<ch_type> io_value_handle(basic_win32_io_observer<ch_type> other)
{
	return other;
}

namespace win32::details
{

inline void* win32_dup_impl(void* handle)
{
	void* current_process{reinterpret_cast<void*>(static_cast<intptr_t>(-1))};
	void* new_handle{};
	if(!DuplicateHandle(current_process,handle,current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
		throw_win32_error();
	return handle;
}

inline void* win32_dup2_impl(void* handle,void* newhandle)
{
	auto temp{win32_dup_impl(handle)};
	if(newhandle!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
		CloseHandle(newhandle);
	return temp;
}

}

template<std::integral ch_type>
class basic_win32_io_handle:public basic_win32_io_observer<ch_type>
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	using async_scheduler_type = basic_win32_io_observer<char>;
	constexpr basic_win32_io_handle() noexcept =default;

	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_win32_io_handle(native_hd handle) noexcept:
		basic_win32_io_observer<ch_type>{handle}{}
	basic_win32_io_handle(basic_win32_io_handle const& other):basic_win32_io_observer<ch_type>{win32::details::win32_dup_impl(other.native_handle())}{}
	basic_win32_io_handle& operator=(basic_win32_io_handle const& other)
	{
		this->handle=win32::details::win32_dup2_impl(other.handle,this->handle);
		return *this;
	}
	basic_win32_io_handle(basic_win32_io_handle&& b) noexcept:
		basic_win32_io_observer<ch_type>{b.release()}
	{}
	basic_win32_io_handle& operator=(basic_win32_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			if(*this)[[likely]]
				fast_io::win32::CloseHandle(this->native_handle());
			this->native_handle() = b.native_handle();
			b.native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		}
		return *this;
	}
	void reset(native_handle_type newhandle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))) noexcept
	{
		if(*this)[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
		this->native_handle()=newhandle;
	}
	void close()
	{
		if(*this)[[likely]]
		{
			if(!fast_io::win32::CloseHandle(this->native_handle()))[[unlikely]]
				throw_win32_error();
			this->native_handle()=nullptr;
		}
	}
};

template<std::integral ch_type>
inline constexpr auto redirect_handle(basic_win32_io_observer<ch_type> hd)
{
	return hd.native_handle();
}

namespace win32::details
{

inline std::size_t read_impl(void* __restrict handle,void* __restrict begin,std::size_t to_read)
{
	std::uint32_t number_of_bytes_read{};
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	if(!win32::ReadFile(handle,begin,static_cast<std::uint32_t>(to_read),std::addressof(number_of_bytes_read),nullptr))
	{
		auto err(win32::GetLastError());
		if(err==109)
			return 0;
		throw_win32_error(err);
	}
	return number_of_bytes_read;
}

struct file_lock_guard
{
	void* handle;
	constexpr file_lock_guard(void* h):handle(h){}
	file_lock_guard(file_lock_guard const&)=delete;
	file_lock_guard& operator=(file_lock_guard const&)=delete;
	~file_lock_guard()
	{
		win32::overlapped overlap{};
		win32::UnlockFileEx(handle,0,UINT32_MAX,UINT32_MAX,std::addressof(overlap));
	}
};

inline std::size_t readv_impl(void* __restrict handle,std::span<io_scatter_t const> sp)
{
	std::size_t has_read{};
	for(auto const& e : sp)
	{
		std::size_t read_this_round{read_impl(handle,const_cast<void*>(e.base),e.len)};
		has_read+=read_this_round;
		if(read_this_round<e.len)[[unlikely]]
			break;
	}
	return has_read;
}

inline std::uint32_t write_simple_impl(void* __restrict handle,void const* __restrict cbegin,std::size_t to_write)
{
	std::uint32_t number_of_bytes_written{};
	if(!win32::WriteFile(handle,cbegin,static_cast<std::uint32_t>(to_write),std::addressof(number_of_bytes_written),nullptr))
		throw_win32_error();
	return number_of_bytes_written;
}

inline std::size_t write_nolock_impl(void* __restrict handle,void const* __restrict cbegin,std::size_t to_write)
{
	if constexpr(4<sizeof(std::size_t))		//above the size of std::uint32_t, unfortunately, we cannot guarantee the atomicity of syscall
	{
		std::size_t written{};
		for(;to_write;)
		{
			std::uint32_t to_write_this_round{UINT32_MAX};
			if(to_write<static_cast<std::size_t>(UINT32_MAX))
				to_write_this_round=static_cast<std::uint32_t>(to_write);
			std::uint32_t number_of_bytes_written{write_simple_impl(handle,cbegin,to_write_this_round)};
			written+=number_of_bytes_written;
			if(number_of_bytes_written<to_write_this_round)
				break;
			to_write-=to_write_this_round;
		}
		return written;
	}
	else
	{
		return write_simple_impl(handle,cbegin,to_write);
	}
}

inline std::size_t write_impl(void* __restrict handle,void const* __restrict cbegin,std::size_t to_write)
{
	if constexpr(4<sizeof(std::size_t))		//above the size of std::uint32_t, unfortunately, we cannot guarantee the atomicity of syscall
	{
		if(static_cast<std::size_t>(UINT32_MAX)<to_write)
		{
			win32::overlapped overlap{};
			if(win32::LockFileEx(handle,0x00000002,0,UINT32_MAX,UINT32_MAX,std::addressof(overlap)))
			{
				file_lock_guard lg{handle};
				return write_nolock_impl(handle,cbegin,to_write);
			}
			else
				return write_nolock_impl(handle,cbegin,to_write);
		}
		else
			return write_simple_impl(handle,cbegin,to_write);
	}
	else
		return write_nolock_impl(handle,cbegin,to_write);
}

inline std::uintmax_t seek_impl(void* handle,std::intmax_t offset,seekdir s)
{
	std::int64_t distance_to_move_high{};
	if(!win32::SetFilePointerEx(handle,offset,std::addressof(distance_to_move_high),static_cast<std::uint32_t>(s)))
		throw_win32_error();
	return distance_to_move_high;
}

inline std::size_t write_v_unhappy_path_impl(void* __restrict handle,std::span<io_scatter_t const> sp)
{
	std::size_t total_bytes{};
	constexpr std::size_t pipe_size_limits{33525760U};
	bool ok{true};
	for(auto const& e : sp)
	{
		total_bytes+=e.len;
		if(e.len<pipe_size_limits)
			ok=false;
	}
	if(!total_bytes)
		return 0;
	if(ok)
	{
		std::size_t written{};
		for(auto const& e : sp)
			if(e.len)
			{
				std::size_t written_this_round{write_nolock_impl(handle,e.base,e.len)};
				written+=written_this_round;
				if(written_this_round<e.len)
					break;
			}
		return written;
	}
	std::size_t buffer_size{pipe_size_limits<total_bytes?pipe_size_limits:total_bytes};
	std::unique_ptr<std::byte[]> buffer{new std::byte[buffer_size]};
	auto position{buffer.get()};
	auto const ed{buffer.get()+buffer_size};
	std::size_t written{};
	for(auto const& e : sp)
	{
		std::size_t remain_space(ed-position);
		if(e.len<remain_space)
		{
			if(e.len)[[likely]]
				std::memcpy(position,e.base,e.len);
			position+=e.len;
		}
		else if(buffer_size==remain_space)
		{
			std::size_t actual_written{write_nolock_impl(handle,e.base,e.len)};
			written+=actual_written;
			if(actual_written<e.len)
				return written;
		}
		else
		{
			if(remain_space)
				memcpy(position,e.base,remain_space);
			{
				std::size_t actual_written{write_simple_impl(handle,buffer.get(),buffer_size)};
				written+=actual_written;
				if(actual_written<remain_space)
					return written;
			}
			std::byte const* ebaseoffset{reinterpret_cast<std::byte const*>(e.base)+remain_space};
			std::size_t to_write{e.len-remain_space};
			if(buffer_size<=to_write)
			{
				std::size_t actual_written{write_nolock_impl(handle,ebaseoffset,to_write)};
				written+=actual_written;
				if(actual_written<remain_space)
					return written;
			}
			else if(to_write)
			{
				memcpy(buffer.get(),ebaseoffset,to_write);
				position=buffer.get()+to_write;
			}
			else
				position=buffer.get();
		}
	}
	if(position==buffer.get())
		return written;
	else
		return written+write_simple_impl(handle,buffer.get(),position-buffer.get());
}

inline std::size_t writev_impl(void* __restrict handle,std::span<io_scatter_t const> sp)
{
	if(sp.empty())
		return 0;
	else
	{
		win32::overlapped overlap{};
		auto succ{win32::LockFileEx(handle,0x00000002,0,UINT32_MAX,UINT32_MAX,std::addressof(overlap))};
		if(!succ)
			return write_v_unhappy_path_impl(handle,sp);
		file_lock_guard gd{handle};
		std::size_t total_write{};
		for(auto const & e : sp)
		{
			std::size_t written{write_nolock_impl(handle,e.base,e.len)};
			total_write+=written;
			if(e.len<written)
				break;
		}
		return total_write;
	}
}


}

template<std::integral ch_type>
inline std::uintmax_t seek(basic_win32_io_observer<ch_type> handle,std::intmax_t offset=0,seekdir s=seekdir::cur)
{
	return win32::details::seek_impl(handle.handle,offset,s);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
[[nodiscard]] inline Iter read(basic_win32_io_observer<ch_type> handle,Iter begin,Iter end)
{
	return begin+win32::details::read_impl(handle.handle,std::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_io_observer<ch_type> handle,Iter cbegin,Iter cend)
{
	return cbegin+win32::details::write_impl(handle.handle,std::to_address(cbegin),(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<std::integral ch_type>
inline std::size_t scatter_read(basic_win32_io_observer<ch_type> handle,std::span<io_scatter_t const> sp)
{
	return win32::details::readv_impl(handle.handle,sp);
}

template<std::integral ch_type>
inline std::size_t scatter_write(basic_win32_io_observer<ch_type> handle,std::span<io_scatter_t const> sp)
{
	return win32::details::writev_impl(handle.handle,sp);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline void async_read_callback(basic_win32_io_observer<char>,basic_win32_io_observer<ch_type> h,Iter cbegin,Iter cend,
	iocp_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	std::size_t to_read((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	if constexpr(4<sizeof(std::size_t))
	{
		callback.native_handle()->Offset=static_cast<std::size_t>(offset)&std::numeric_limits<std::uint32_t>::max();
		callback.native_handle()->OffsetHigh=static_cast<std::size_t>(offset)>>static_cast<std::size_t>(32);
	}
	else
	{
		callback.native_handle()->Offset=static_cast<std::uint32_t>(offset);
		callback.native_handle()->OffsetHigh=0;
	}
	if(!win32::ReadFile(h.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_read),nullptr,callback.native_handle()))[[likely]]
	{
		auto err(win32::GetLastError());
		if(err==997)[[likely]]
			return;
		throw_win32_error(err);
	}
}

template<std::integral char_type>
inline constexpr io_type_t<basic_win32_io_observer<char>> async_scheduler_type(basic_win32_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
inline constexpr io_type_t<iocp_overlapped> async_overlapped_type(basic_win32_io_observer<char_type>)
{
	return {};
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline void async_write_callback(basic_win32_io_observer<char> over,basic_win32_io_observer<ch_type> h,Iter cbegin,Iter cend,
	iocp_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	std::size_t to_write((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_write)
			to_write=static_cast<std::size_t>(UINT32_MAX);
	if constexpr(4<sizeof(std::size_t))
	{
		callback.native_handle()->Offset=static_cast<std::size_t>(offset)&std::numeric_limits<std::uint32_t>::max();
		callback.native_handle()->OffsetHigh=static_cast<std::size_t>(offset)>>static_cast<std::size_t>(32);
	}
	else
	{
		callback.native_handle()->Offset=static_cast<std::uint32_t>(offset);
		callback.native_handle()->OffsetHigh=0;
	}
	if(!win32::WriteFile(h.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_write),nullptr,callback.native_handle()))[[likely]]
	{
		auto err(win32::GetLastError());
		if(err==997)[[likely]]
			return;
		throw_win32_error(err);
	}
}

template<std::integral ch_type>
inline void cancel(basic_win32_io_observer<ch_type> h)
{
	if(!fast_io::win32::CancelIo(h.native_handle()))
		throw_win32_error();
}

template<std::integral ch_type,typename... Args>
requires requires(basic_win32_io_observer<ch_type> h,Args&& ...args)
{
	fast_io::win32::DeviceIoControl(h.native_handle(),std::forward<Args>(args)...);
}
inline void io_control(basic_win32_io_observer<ch_type> h,Args&& ...args)
{
	if(!fast_io::win32::DeviceIoControl(h.native_handle(),std::forward<Args>(args)...))
		throw_win32_error();
}

template<std::integral ch_type>
class basic_win32_file:public basic_win32_io_handle<ch_type>
{
public:
	using char_type=ch_type;
	using native_handle_type = basic_win32_io_handle<ch_type>::native_handle_type;
	using basic_win32_io_handle<ch_type>::native_handle;
	using async_scheduler_type = basic_win32_io_observer<char>;
	explicit constexpr basic_win32_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_win32_file(native_hd handle) noexcept:basic_win32_io_handle<ch_type>(handle){}

	basic_win32_file(io_dup_t,basic_win32_io_observer<ch_type> wiob):basic_win32_io_handle<ch_type>(win32::details::win32_dup_impl(wiob.native_handle()))
	{}


#ifndef _MSC_VER
	explicit basic_win32_file(io_temp_t):basic_win32_io_handle<char_type>(details::create_win32_temp_file()){}
#endif
	explicit basic_win32_file(nt_at_entry nate,cstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
				basic_win32_io_handle<char_type>(win32::nt::details::nt_create_file_directory_impl(nate.handle,filename,win32::nt::details::calculate_nt_open_mode(om,pm)))
	{}

	explicit basic_win32_file(cstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
				basic_win32_io_handle<char_type>(details::win32_create_file_impl(filename,details::calculate_win32_open_mode(om,pm)))
	{}
	explicit basic_win32_file(nt_at_entry nate,wcstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
				basic_win32_io_handle<char_type>(win32::nt::details::nt_create_file_directory_impl(nate.handle,filename,win32::nt::details::calculate_nt_open_mode(om,pm)))
	{}

	explicit basic_win32_file(wcstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
				basic_win32_io_handle<char_type>(details::win32_create_file_impl(filename,details::calculate_win32_open_mode(om,pm)))
	{}

#if 0
	explicit basic_win32_file(io_async_t) requires(std::same_as<char_type,char>):
		basic_win32_io_handle<char_type>(details::create_io_completion_port(bit_cast<void*>(static_cast<std::uintptr_t>(-1)),nullptr,0,0)){}

	template<typename... Args>
	basic_win32_file(io_async_t,basic_win32_io_observer<char> iob,nt_at_entry nate,basic_cstring_view<filename_char_type auto> filename,Args&& ...args):basic_win32_file(nate,filename,std::forward<Args>(args)...)
	{
		basic_win32_file<ch_type> guard(this->native_handle());
		details::create_io_completion_port(this->native_handle(),iob.native_handle(),bit_cast<std::uintptr_t>(this->native_handle()),0);
		guard.release();
	}
#endif
	~basic_win32_file()
	{
		if(*this)[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
	}
	constexpr basic_win32_file(basic_win32_file const&)=default;
	constexpr basic_win32_file& operator=(basic_win32_file const&)=default;
	constexpr basic_win32_file(basic_win32_file&&) noexcept=default;
	constexpr basic_win32_file& operator=(basic_win32_file&&) noexcept=default;
};

template<std::integral ch_type>
inline constexpr auto zero_copy_in_handle(basic_win32_io_observer<ch_type> iob)
{
	return iob.handle;
}

template<std::integral ch_type>
inline void truncate(basic_win32_io_observer<ch_type> handle,std::uintmax_t size)
{
	seek(handle,size,seekdir::beg);
	if(!win32::SetEndOfFile(handle.handle))
		throw_win32_error();
}

namespace win32::details
{

inline file_type file_type_impl(void* handle)
{
/*
https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletype
*/
	std::uint32_t ftvalue{GetFileType(handle)};
	switch(ftvalue)
	{
	case 0x0002:
		return file_type::character;
	case 0x0001:
		return file_type::regular;
	case 0x0003:
		return file_type::fifo;
	case 0x8000:
		return file_type::remote;
	case 0x0000:
	{
		auto errcd=GetLastError();
		if(errcd==0)
			return file_type::unknown;
		throw_win32_error(errcd);
		[[fallthrough]];
	}
	default:
		return file_type::unknown;
	};
}

inline posix_file_status win32_status_impl(void* __restrict handle)
{
	file_type ft{file_type_impl(handle)};
	if(ft==file_type::fifo||ft==file_type::character)
	{
		return posix_file_status{.type=ft,.nlink=1,
		.rdev=static_cast<std::uintmax_t>(reinterpret_cast<std::uintptr_t>(handle)),.blksize=65536};
	}
	by_handle_file_information bhdi;
	if(!GetFileInformationByHandle(handle,std::addressof(bhdi)))
		throw_win32_error();
	std::uintmax_t file_size{static_cast<std::uintmax_t>((static_cast<std::uint64_t>(bhdi.nFileSizeHigh)<<32)|bhdi.nFileSizeLow)};
	std::underlying_type_t<perms> pm{0444};
	if((bhdi.dwFileAttributes&0x1)==0x0)
		pm|=0222;
	if((bhdi.dwFileAttributes&0x400)==0x400)
		ft=file_type::symlink;
	else if((bhdi.dwFileAttributes&0x10)==0x10)
		ft=file_type::directory;
	return posix_file_status{static_cast<std::uintmax_t>(bhdi.dwVolumeSerialNumber),
	static_cast<std::uintmax_t>((static_cast<std::uint64_t>(bhdi.nFileIndexHigh)<<32)|bhdi.nFileIndexLow),
	static_cast<perms>(pm),
	ft,
	static_cast<std::uintmax_t>(bhdi.nNumberOfLinks),
	0,0,0,
	file_size,
	65536,file_size>>9,
	to_struct_timespec(bhdi.ftLastAccessTime),
	to_struct_timespec(bhdi.ftLastWriteTime),
	to_struct_timespec(bhdi.ftCreationTime),
	0,0};
}


}

template<std::integral ch_type>
inline posix_file_status status(basic_win32_io_observer<ch_type> wiob)
{
	return win32::details::win32_status_impl(wiob.handle);
}

template<std::integral ch_type>
class basic_win32_pipe
{
public:
	using char_type = ch_type;
	using native_handle_type = std::array<basic_win32_file<ch_type>,2>;
	native_handle_type pipes;
	basic_win32_pipe()
	{
		win32::security_attributes sec_attr{sizeof(win32::security_attributes),nullptr,true};
		if(!win32::CreatePipe(
			std::addressof(pipes.front().native_handle()),
			std::addressof(pipes.back().native_handle()),
			std::addressof(sec_attr),0))
			throw_win32_error();
	}
	auto& native_handle()
	{
		return pipes;
	}
	auto& in()
	{
		return pipes.front();
	}
	auto& out()
	{
		return pipes.back();
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return read(h.in(),begin,end);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return write(h.out(),begin,end);
}

template<std::integral ch_type>
inline std::size_t scatter_read(basic_win32_pipe<ch_type>& h,std::span<io_scatter_t const> sp)
{
	return scatter_read(h.in(),sp);
}

template<std::integral ch_type>
inline std::size_t scatter_write(basic_win32_pipe<ch_type>& h,std::span<io_scatter_t const> sp)
{
	return scatter_write(h.out(),sp);
}

template<std::integral ch_type>
inline std::array<void*,2> redirect_handle(basic_win32_pipe<ch_type>& hd)
{
	return {hd.in().native_handle(),hd.out().native_handle()};
}

template<std::integral ch_type>
inline constexpr void flush(basic_win32_pipe<ch_type>&){}
using win32_io_observer=basic_win32_io_observer<char>;
using win32_io_handle=basic_win32_io_handle<char>;
using win32_file=basic_win32_file<char>;
using win32_pipe=basic_win32_pipe<char>;

using u8win32_io_observer=basic_win32_io_observer<char8_t>;
using u8win32_io_handle=basic_win32_io_handle<char8_t>;
using u8win32_file=basic_win32_file<char8_t>;
using u8win32_pipe=basic_win32_pipe<char8_t>;

using wwin32_io_observer=basic_win32_io_observer<wchar_t>;
using wwin32_io_handle=basic_win32_io_handle<wchar_t>;
using wwin32_file=basic_win32_file<wchar_t>;
using wwin32_pipe=basic_win32_pipe<wchar_t>;

using io_async_observer=win32_io_observer;
using io_async_scheduler=win32_file;

inline constexpr std::uint32_t win32_stdin_number(-10);
inline constexpr std::uint32_t win32_stdout_number(-11);
inline constexpr std::uint32_t win32_stderr_number(-12);

template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> win32_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(-10)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> win32_stdout() noexcept
{
	return basic_win32_io_observer<char_type>{fast_io::win32::GetStdHandle(-11)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> win32_stderr() noexcept
{
	return basic_win32_io_observer<char_type>{fast_io::win32::GetStdHandle(-12)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> native_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(-10)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> native_stdout() noexcept
{
	return {fast_io::win32::GetStdHandle(-11)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> native_stderr() noexcept
{
	return {fast_io::win32::GetStdHandle(-12)};
}

}
