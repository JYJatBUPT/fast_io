#pragma once

namespace fast_io
{

enum class scan_transmit_operation
{
single,until
};

namespace manip
{

template<std::invocable<char8_t> Func,scan_transmit_operation operation>
struct scan_transmitter
{
	using function_type = Func;
	[[no_unique_address]] function_type reference;
};

}

template<std::invocable<char8_t> Func>
inline constexpr manip::scan_transmitter<Func,scan_transmit_operation::single> st_single(Func func)
{
	return {func};
}

template<std::invocable<char8_t> Func>
inline constexpr manip::scan_transmitter<Func,scan_transmit_operation::until> st_until(Func func)
{
	return {func};
}

namespace details
{
template<buffer_output_stream output,buffer_input_stream input,std::invocable<char8_t> Func,scan_transmit_operation operation>
inline constexpr void impl_svt(output& out,input& in,manip::scan_transmitter<Func,operation> func)
{
	static_assert(operation==scan_transmit_operation::until||operation==scan_transmit_operation::single);
	if constexpr(operation==scan_transmit_operation::until)
	{
		do
		{
			auto curr{ibuffer_curr(in)};
			auto ed{ibuffer_end(in)};
			if(curr!=ed)[[likely]]
			{
				auto i{std::find_if(curr,ed,func.reference)};
				write(out,curr,i);
				ibuffer_set_curr(in,i);
				if(i!=ed)[[likely]]
					return;
			}
		}
		while(underflow(in));
	}
	else
	{
		auto curr{ibuffer_curr(in)};
		if(curr==ibuffer_end(in))[[unlikely]]
		{
			if(!underflow(in))
				return;
			curr=ibuffer_curr(in);
		}
		auto ch(*curr);
		if(func.reference(ch))
		{
			put(out,ch);
			ibuffer_set_curr(in,curr+1);
		}
		return;
	}
}

template<buffer_output_stream output,typename T,typename U,std::invocable<char8_t> Func,scan_transmit_operation operation>
inline constexpr void impl_svt_none_buffer(output& out,T& bg,U& ed,manip::scan_transmitter<Func,operation> func)
{
	static_assert(operation==scan_transmit_operation::until||operation==scan_transmit_operation::single);
	if constexpr(operation==scan_transmit_operation::until)
	{
		for(;bg!=ed&&!func.reference(*bg);++bg)
			put(out,*bg);
	}
	else
	{
		if(bg!=ed&&func.reference(*bg))
		{
			put(out,*bg);
			++bg;
		}
	}
}
}

template<buffer_output_stream output,input_stream input,typename... Args>
inline constexpr bool scan_transmit(output&& out,input&& in,Args&& ...args)
{
	using real_input_type = std::remove_cvref_t<input>;
	if constexpr(buffer_input_stream<real_input_type>)
	{
		auto curr{ibuffer_curr(in)};
		auto ed{ibuffer_end(in)};
		if(curr==ed)
		{
			if(!underflow(in))
				return false;
		}
		((details::impl_svt(out,in,std::forward<Args>(args))),...);
		return true;
	}
	else
	{
		auto igen{igenerator(in)};
		auto bg{begin(igen)};
		auto ed{end(igen)};
		if(bg==ed)
			return false;
		((details::impl_svt_none_buffer(out,bg,ed,std::forward<Args>(args))),...);
		return true;
	}
}

}