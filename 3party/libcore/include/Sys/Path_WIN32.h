#ifndef Foundation_Path_WIN32_INCLUDED
#define Foundation_Path_WIN32_INCLUDED

#include <vector>

class  PathImpl
{
public:
	static std::string currentImpl();
	static std::string homeImpl();
	static std::string tempImpl();
	static std::string nullImpl();
	static std::string expandImpl(const std::string& path);
	static void listRootsImpl(std::vector<std::string>& roots);
};

#endif // Foundation_Path_WIN32_INCLUDED
