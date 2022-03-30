#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "webserv.hpp"
# include "Location.hpp"
# include "VirtualServer.hpp"
# include "Port.hpp"

# define N_DIR 13
# define N_CONTEXTS 5

enum	contexts
{
	MAIN_CONTEXT = 0,
	SERVER_CONTEXT,
	LOCATION_CONTEXT//,
//	SWITCHING_TO_SERVER,
//	SWITCHING_TO_LOCATION
};

enum	directives
{
	DIR_ERROR = -1,
	SERVER,
	LISTEN,
	SERVER_NAME,
	CLIENT_MAX_BODY_SIZE,
	ROOT,
	ERROR_PAGE,
	AUTOINDEX,
	INDEX,
	RETURN,
	LIMIT_EXCEPT,
	LOCATION,
	OPENING_BRACKET,
	CLOSING_BRACKET,
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

class	Port ;

class	ConfigParser {

	private:
		//	PARSING INFOS
		int											_context;
		std::vector<std:: vector<std::string > >	_lines;
		std::vector<std::string>					_line;
		std::string									_curLine;
		int											_dir;
		int											_lineN;

		//	STATIC STRING CONSTANTS
		static const char *							_directives[N_DIR];
		static const char *							_contexts[N_CONTEXTS];

		//	TO STORE TEMP VALUES AS LONG AS PORT IS NOT DEFINED
		VirtualServer								_tmpVS;
		bool										_tmpVSIsStored;
		Location									_tmpLoc;
		bool										_tmpLocIsStored;

		//	PTRS TO KEEP TRACK OF CURRENT INSTANCES BEING TREATED
		Port										*_curPort;
		VirtualServer								*_curVS;
		Location									*_curLoc;

		//	UNDERLYING DATA STRUCTURE => List OF Ports
		std::list<Port *>							_portsList;

		ConfigParser	&operator=(ConfigParser const & rhs);
		ConfigParser(ConfigParser const &src);

	public:
		ConfigParser(void);
		~ConfigParser(void);

		//	ConfigParser.cpp
		int										getContext(void) const;
		std::vector<std::vector<std::string> >	getLines(void) const;	
		std::string								getCurLine(void) const;
		void		displayLine(std::vector<std::string> v) const;
		void		displayLines(void) const;
		void		displayContextSwitch(int newContext) const;

		//	ConfigParserFile.cpp	
		void		parse(char *arg);
		void		splitLineIntoTokens(void);
		int			validateDirective(void);
		int			validateArguments(void);
		bool		validateContext(void);
		bool		hasContent(void) const ;

		//	ConfigParserDirArgs.cpp	
		int			validateServerArgs(void);
		int			validateLocationArgs(void);
		int			validateListenArgs(void);
		int			validateServerNameArgs(void);
		int			validateClientMaxBodySizeArgs(void);
		int			validateRootArgs(void);
		int			validateErrorPageArgs(void);
		int			validateAutoindexArgs(void);
		int			validateIndexArgs(void);
		int			validateReturnArgs(void);
		int			validateLimitExceptArgs(void);
		int			validateOpeningBracketArgs(void);
		int			validateClosingBracketArgs(void);
		bool		noDuplicateErrorPage(void);
};

#endif
