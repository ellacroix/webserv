#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "webserv.hpp"
# include "Port.hpp"

# define N_DIR	12

enum	contexts
{
	MAIN_CONTEXT = -1,
	SERVER_CONTEXT,
	LOCATION_CONTEXT
};

enum	directives
{
	DIR_ERROR = -1,
	SERVER,
	LISTEN,
	SERVER_NAME,
	CLIENT_MAX_BODY_SIZE,
	LOCATION,
	ROOT,
	ERROR_PAGE,
	AUTOINDEX,
	INDEX,
	RETURN,
	OPENING_BRACKET,
	CLOSING_BRACKET
};

enum	arguments
{
	ARG_ERROR = -1,
	IP,
	PORT,
	DOMAIN,
	BODY_SIZE,
	PATH,
	HTTP_CODE,
	URI,
	ON_OFF,
	FILE_NAME,
	FILE_EXTENSION
};

class	ConfigParser {
	private:
		int											_context;
		std::vector<std:: vector<std::string > >	_lines;
		std::vector<std::string>					_line;
		std::string									_curLine;
		static const char *							_directives[N_DIR];
		int											_dir;

		ConfigParser	&operator=(ConfigParser const & rhs);

		ConfigParser(ConfigParser const &src);

	public:
		ConfigParser(void);
		~ConfigParser(void);

		int										getContext(void) const;
		std::vector<std::vector<std::string> >	getLines(void) const;	
		std::string								getCurLine(void) const;

		std::list<Port *>			parse(char *arg);
		void						displayLine(std::vector<std::string> v) const;
		void						displayLines(void) const;
		void						splitLineIntoTokens(void);
		int							validateDirective(void);

};

#endif
