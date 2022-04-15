//
// Created by imper on 3/29/22.
//

#ifndef CALL_MESSENGER_BACKEND_HPP
#define CALL_MESSENGER_BACKEND_HPP

#include <cstring>
#include <execute-process-linux>
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
	
	inline bool begin_session()
	{
		if (!began_session)
		{
			::pipe(op);
			
			char* args[]{
					backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
					"-l", PLACEHOLDER, "-p", PLACEHOLDER,
					"-O", PLACEHOLDER, "-I", PLACEHOLDER,
					"-M", PLACEHOLDER,
					nullptr
			};
			args[4] = address.data();
			args[6] = "s_begin_session";
			args[8] = login.data();
			args[10] = password.data();
			args[12] = std::to_string(op[exec::pipe::write]).data();
			args[13] = nullptr;
			
			pid_t pid = exec::execute_program(args, nullptr);
			std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
			::close(op[exec::pipe::write]);
			
			auto res = false;
			rd_pipe(res);
			if ((began_session = res))
				rd_pipe(prikey);
			::close(op[exec::pipe::read]);
			return res;
		}
	}
	
	inline bool end_session()
	{
		if (began_session)
		{
			::pipe(op);
			char* args[]{
					backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
					"-l", PLACEHOLDER, "-p", PLACEHOLDER,
					"-O", PLACEHOLDER, "-I", PLACEHOLDER,
					"-M", PLACEHOLDER,
					nullptr
			};
			args[4] = address.data();
			args[6] = "s_end_session";
			args[8] = login.data();
			args[10] = password.data();
			args[12] = std::to_string(op[exec::pipe::write]).data();
			args[13] = nullptr;
			
			pid_t pid = exec::execute_program(args, nullptr);
			std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
			::close(op[exec::pipe::write]);
			
			auto res = false;
			rd_pipe(res);
			::close(op[exec::pipe::read]);
			return !(began_session = !res);
		}
	}
	
	inline bool register_user(std::string display_name)
	{
		::pipe(op);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_register_user";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = display_name.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[15] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool set_password(std::string new_password)
	{
		::pipe(op);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_set_password";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = new_password.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[15] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool set_display_name(std::string display_name)
	{
		::pipe(op);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_set_display_name";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = display_name.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[13] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool get_display_name(std::string& display_name)
	{
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				"-M", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_get_display_name";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = std::to_string(op[exec::pipe::write]).data();
		args[13] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		if (res)
			rd_pipe(display_name);
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool check_online_status(bool& online, std::string user)
	{
		::pipe(op);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_check_online_status";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = user.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[15] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		if (res)
			rd_pipe(online);
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool find_users_by_display_name(std::list<std::string>& list, std::string key)
	{
		::pipe(op);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_find_users_by_display_name";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = key.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[15] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		size_t size;
		rd_pipe(size);
		std::string tmp;
		for (size_t i = 0; i < size; ++i)
		{
			rd_pipe(tmp);
			list.push_back(tmp);
		}
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool find_users_by_login(std::list<std::string>& list, std::string key)
	{
		::pipe(op);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_find_users_by_login";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = key.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[15] = nullptr;
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		size_t size;
		rd_pipe(size);
		std::string tmp;
		for (size_t i = 0; i < size; ++i)
		{
			rd_pipe(tmp);
			list.push_back(tmp);
		}
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool send_message(std::string user, const std::vector<uint8_t>& message)
	{
		::pipe(op);
		::pipe(ip);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_send_message";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = user.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[16] = std::to_string(ip[exec::pipe::read]).data();
		
		wr_pipe(message);
		wr_pipe(prikey);
		::close(ip[exec::pipe::write]);
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(ip[exec::pipe::read]);
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		::close(op[exec::pipe::read]);
		return res;
	}
	
	inline bool query_incoming(std::string& user, std::vector<uint8_t>& message)
	{
		::pipe(op);
		::pipe(ip);
		char* args[]{
				backend, "-m", "client", "-a", PLACEHOLDER, "-o", PLACEHOLDER,
				"-l", PLACEHOLDER, "-p", PLACEHOLDER,
				"-M", PLACEHOLDER,
				"-O", PLACEHOLDER, "-I", PLACEHOLDER,
				nullptr
		};
		args[4] = address.data();
		args[6] = "s_query_incoming";
		args[8] = login.data();
		args[10] = password.data();
		args[12] = user.data();
		args[14] = std::to_string(op[exec::pipe::write]).data();
		args[16] = std::to_string(ip[exec::pipe::read]).data();
		
		wr_pipe(prikey);
		::close(ip[exec::pipe::write]);
		
		pid_t pid = exec::execute_program(args, nullptr);
		std::clog << "Backend executed with exit code " << exec::wait_for_program(pid) << ".\n";
		::close(ip[exec::pipe::read]);
		::close(op[exec::pipe::write]);
		
		auto res = false;
		rd_pipe(res);
		if (res)
		{
			rd_pipe(user);
			rd_pipe(message);
		}
		::close(op[exec::pipe::read]);
		return res;
	}

private:
	std::string address;
	std::string login;
	std::string password;
	bool began_session = false;
	std::vector<uint8_t> prikey;
	int ip[2], op[2];
	
	template <typename T>
	void rd_pipe(T& val)
	{
		::read(op[exec::pipe::read], &val, sizeof val);
	}
	
	template <template <typename> typename Container, typename T>
	void rd_pipe(Container<T>& cont)
	{
		size_t size = 0;
		::read(op[exec::pipe::read], &size, sizeof size);
		if (size > 0)
		{
			cont.resize(size, 0);
			::read(op[exec::pipe::read], cont.data(), size);
		}
	}
	
	template <typename T>
	void wr_pipe(const T& val)
	{
		::write(ip[exec::pipe::write], &val, sizeof val);
	}
	
	template <template <typename> typename Container, typename T>
	void wr_pipe(const Container<T>& cont)
	{
		size_t size = cont.size();
		::write(ip[exec::pipe::write], &size, sizeof size);
		if (size > 0) ::write(ip[exec::pipe::write], cont.data(), size);
	}
};

#endif //CALL_MESSENGER_BACKEND_HPP
