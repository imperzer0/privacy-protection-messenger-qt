//
// Created by imper on 3/29/22.
//

#ifndef PRIVACY_PROTECTION_MESSENGER_CLI_CALL_BACKEND_HPP
#define PRIVACY_PROTECTION_MESSENGER_CLI_CALL_BACKEND_HPP

#include <cstring>
#include <execute-process-linux-defs>
#include <iostream>
#include <vector>

#define PLACEHOLDER nullptr

static constexpr char* backend = "privacy-protection-messenger";


class call_backend
{
public:
	inline call_backend(const std::string& address, const std::string& login, const std::string& password)
			: address(address), login(login), password(password)
	{ }
	
	inline ~call_backend()
	{ end_session(); }
	
	inline void begin_session()
	{
		if (!began_session)
		{
			char* args[]{
					backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
					"-l", PLACEHOLDER, "-p", PLACEHOLDER,
					"-M", PLACEHOLDER, "-P", PLACEHOLDER,
					nullptr
			};
			args[4] = address.data();
			args[6] = "s_begin_session";
			args[8] = login.data();
			args[10] = password.data();
			args[11] = nullptr;
			
			pid_t pid = exec::execute_program(args, nullptr);
			std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
			began_session = true;
		}
	}
	
	inline void end_session()
	{
		if (began_session)
		{
			char* args[]{
					backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
					"-l", PLACEHOLDER, "-p", PLACEHOLDER,
					"-M", PLACEHOLDER, "-P", PLACEHOLDER,
					nullptr
			};
			args[4] = address.data();
			args[6] = "s_end_session";
			args[8] = login.data();
			args[10] = password.data();
			args[11] = nullptr;
			
			pid_t pid = exec::execute_program(args, nullptr);
			std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
			began_session = false;
		}
	}
	
	inline void register_user(std::string display_name)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_register_user";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = display_name.data();
		args[13] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
	}
	
	inline std::string set_password(std::string new_password)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_set_password";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = new_password.data();
		args[13] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
	}
	
	inline void set_display_name(std::string display_name)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_set_display_name";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = display_name.data();
		args[13] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
	}
	
	inline std::string get_display_name()
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_get_display_name";
		args[8] = login.data();
		args[10] = password.data();
		args[11] = nullptr;
		
		int p[2];
		::pipe(p);
		
		pid_t pid = exec::execute_program(args, nullptr, exec::std_out.redirect_to(p[exec::pipe::write]));
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(p[exec::pipe::write]);
		
		FILE* fd = ::fdopen(p[exec::pipe::read], "rb");
		std::array<char, 64> tmp{ };
		std::string res;
		while (::fgets(tmp.data(), 64, fd))
			res += tmp.data();
		::fclose(fd);
		
		::close(p[exec::pipe::read]);
		
		return res;
	}
	
	inline std::string check_online_status(std::string user)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_check_online_status";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = user.data();
		args[13] = nullptr;
		
		int p[2];
		::pipe(p);
		
		pid_t pid = exec::execute_program(args, nullptr, exec::std_out.redirect_to(p[exec::pipe::write]));
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(p[exec::pipe::write]);
		
		FILE* fd = ::fdopen(p[exec::pipe::read], "rb");
		std::array<char, 64> tmp{ };
		std::string res;
		while (::fgets(tmp.data(), 64, fd))
			res += tmp.data();
		::fclose(fd);
		
		::close(p[exec::pipe::read]);
		
		return res;
	}
	
	inline std::string find_users_by_display_name(std::string key)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_find_users_by_display_name";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = key.data();
		args[13] = nullptr;
		
		int p[2];
		::pipe(p);
		
		pid_t pid = exec::execute_program(args, nullptr, exec::std_out.redirect_to(p[exec::pipe::write]));
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(p[exec::pipe::write]);
		
		FILE* fd = ::fdopen(p[exec::pipe::read], "rb");
		std::array<char, 64> tmp{ };
		std::string res;
		while (::fgets(tmp.data(), 64, fd))
			res += tmp.data();
		::fclose(fd);
		
		::close(p[exec::pipe::read]);
		
		return res;
	}
	
	inline std::string find_users_by_login(std::string key)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_find_users_by_login";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = key.data();
		args[13] = nullptr;
		
		int p[2];
		::pipe(p);
		
		pid_t pid = exec::execute_program(args, nullptr, exec::std_out.redirect_to(p[exec::pipe::write]));
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(p[exec::pipe::write]);
		
		FILE* fd = ::fdopen(p[exec::pipe::read], "rb");
		std::array<char, 64> tmp{ };
		std::string res;
		while (::fgets(tmp.data(), 64, fd))
			res += tmp.data();
		::fclose(fd);
		
		::close(p[exec::pipe::read]);
		
		return res;
	}
	
	inline void send_message(std::string user, const std::vector<char>& message)
	{
		int datapipe[2];
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_send_message";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = user.data();
		args[14] = std::to_string(datapipe[exec::pipe::read]).data();
		
		size_t size = message.size();
		::write(datapipe[exec::pipe::write], &size, sizeof size);
		if (!message.empty()) ::write(datapipe[exec::pipe::write], message.data(), size);
		::close(datapipe[exec::pipe::write]);
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(datapipe[exec::pipe::read]);
	}
	
	inline std::vector<char> query_incoming(std::string& user)
	{
		int datapipe[2];
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER, "-P", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_query_incoming";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = "placeholder";
		args[14] = std::to_string(datapipe[exec::pipe::write]).data();
		
		int p[2];
		::pipe(p);
		
		pid_t pid = exec::execute_program(args, nullptr, exec::std_out.redirect_to(p[exec::pipe::write]));
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(datapipe[exec::pipe::write]);
		::close(p[exec::pipe::write]);
		
		size_t size;
		::read(datapipe[exec::pipe::read], &size, sizeof size);
		std::vector<char> message(size, 0);
		if (size > 0) ::read(datapipe[exec::pipe::write], message.data(), size);
		::close(datapipe[exec::pipe::read]);
		
		FILE* fd = ::fdopen(p[exec::pipe::read], "rb");
		std::array<char, 64> tmp{ };
		std::string res;
		while (::fgets(tmp.data(), 64, fd))
			res += tmp.data();
		::fclose(fd);
		
		::close(p[exec::pipe::read]);
	}

private:
	std::string address;
	std::string login;
	std::string password;
	bool began_session = false;
};

#endif //PRIVACY_PROTECTION_MESSENGER_CLI_CALL_BACKEND_HPP
