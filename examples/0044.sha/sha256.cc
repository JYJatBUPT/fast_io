#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"

int main(int argc,char** argv)
try
{
	if(argc!=2)
	{
		print_err("Usage: ",std::string_view(*argv)," <file>\n");
		return 1;
	}
	auto t0{std::chrono::high_resolution_clock::now()};
	std::size_t transmitted{};
	fast_io::sha256 sha;
	{
	fast_io::block_processor processor(sha);
	fast_io::ibuf_file ibf(argv[1]);
	transmitted=transmit(processor,ibf);
	}
	println(sha," *",argv[1],"\nTransmitted:",transmitted,u8" bytes\tElapsed Time:",std::chrono::high_resolution_clock::now()-t0);
}
catch(std::exception const& e)
{
	println_err(e);
	return 2;
}