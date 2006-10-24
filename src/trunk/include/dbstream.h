/***************************************************************************
                         dbstream.h  -  description
                            -------------------
   begin                : Sat Jan 25 2003
   copyright            : (C) 2003 by Chris Thomas
   email                : chris_alex_thomas@yahoo.co.uk
   
   description:
   
   This class provides a stream like interface into a debug system where
   you can decide the output method and apply filters to the debug stream
   to restrict it's output in some way, since it extends from the STL C++
   classes, it works just like std::cout.  Which is a huge advantage.

Changes:
	22/03/2006

	Changed licence to BSD, because there are situations which I'm more
	comfortable using this in my company because of it's high performance
	and ease of use.
***************************************************************************/

/******************************************************************************************
Christopher Thomas (chris.alex.thomas@gmail.com)
Antimatter Studios
22nd March 2006

In the original BSD license, both occurrences of the phrase "COPYRIGHT HOLDERS AND CONTRIBUTORS" in the disclaimer read "REGENTS AND CONTRIBUTORS".

Here is the license template:

Copyright (c) 2006, Christopher Thomas
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the Antimatter Studios nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************************/

#ifndef _DEBUGSTREAM_H_
	#define _DEBUGSTREAM_H_
	
#include <iostream>
#include <fstream>
#include <ostream>
#include <streambuf>
#include <vector>
#include <string>

#ifdef _WIN32
	//	Disable Warnings about......
	#pragma warning(disable: 4355) // using 'this' in the member initialisation list
	#pragma warning(disable: 4786) // truncating a symbol to 255 characters in debug info
	#pragma warning(disable: 4244) // conversion from __w64 int, to int, possible loss of data
	//	NOTE: is disabling these safe?
#endif

namespace dbg{

	typedef void (*callback)(std::string text);

	static void Tokenise(std::string str, std::string search, std::vector<std::string> &token)
	{
		//	FIXME: perhaps I should have here
		//			while(str.length() != temp.length()) ??
		//			instead of the possibly dangerous while(1)
		while(1){
			std::string temp = str.substr(0,str.find(search.c_str()));
			token.push_back(temp);
			if(str.length() == temp.length()) return;
			str = str.substr(temp.length()+1);
		}
	}

	class debugbuf: public std::streambuf{
	protected:
		std::string				m_buffer;

		std::vector<std::string>m_allow_filters;
		std::vector<std::string>m_deny_filters;

		bool					m_console;
		bool					m_file;
		bool					m_callback;

		dbg::callback			m_cbFunc;
		std::fstream			m_outpfile;
	public:
		debugbuf(){}
		
		virtual ~debugbuf(){
			sync();
		}
		
		int overflow(int ch){
			sync();
			
			if(ch != EOF){
				if(pbase() == epptr()){
					cache(ch);
				}else{
					sputc(ch);
				}
			}
			
			return 0;
		}
		
		int sync(){
			if(pbase() != pptr()){
				int len = (pptr() - pbase());
				
				cache(pbase());
				
				setp(pbase(),epptr());
			}
			
			return 0;
		}
		
		void cache(const char *text){
			m_buffer += text;
			output();
		}
		
		void cache(char ch){
			m_buffer += ch;
			output();
		}
		
		void output(void)
		{
			/*	FIXME:	There is perhaps a bug here, it says, if you find \n in the buffer
						output the entire buffer to one of the output destrinations
						except, what if \n is in the middle of the buffer? you end up
						outputting the entire buffer, whereas AFAIK the idea was to output
						line by line.
						
						probably not serious			
			*/
			if(m_buffer.find("\n") != std::string::npos){
				unsigned int matches = 0,a;
			
				for(a=0;a<m_allow_filters.size();a++){
					if(m_buffer.find(m_allow_filters[a]) != std::string::npos) matches++;
				}

				for(a=0;a<m_deny_filters.size();a++){
					if(m_buffer.find(m_deny_filters[a]) != std::string::npos) return;
				}
				
				if(matches > 0 || m_allow_filters.size() == 0){
					if(m_console	== true)	std::cout << m_buffer;
					if(m_file		== true)	m_outpfile << m_buffer;
					if(m_callback	== true)	m_cbFunc(m_buffer);
				}

				//	MSVC++ 6.0 doesnt support clear(), so perhaps this instead
				m_buffer.erase(m_buffer.begin(), m_buffer.end());
			}
		}
	};

	class debugstream: private debugbuf, public std::basic_ostream<char>{
	public:
		debugstream(): std::basic_ostream<char>(this){}
		debugstream(std::string file): std::basic_ostream<char>(this){
			enableFile(file);
		}

		//	Methods to append allow/deny filters to the debug strings
		virtual void AddAllowFilter(std::string text){
			std::vector<std::string> token;
			Tokenise(text,",",token);
			for(unsigned int a=0;a<token.size();a++) m_allow_filters.push_back(token[a]);
		}

		virtual void AddDenyFilter(std::string text){
			std::vector<std::string> token;
			Tokenise(text,",",token);
			for(unsigned int a=0;a<token.size();a++) m_deny_filters.push_back(token[a]);
		}

		//	Methods to setup the output methods of the debug class
		virtual void enableConsole(void){
			m_console = true;
		}
		
		virtual void disableConsole(void){
			m_console = false;
		}

		virtual bool enableFile(std::string fn){
			//	Close/clear the current file
			m_outpfile.clear();
			
			m_outpfile.open(fn.c_str(),std::ios::out);
			
			if(m_outpfile.is_open() == false){
				checkFileState();
				disableFile();
			}else{
				m_file = true;
			}
			
			return m_file;
		}
		
		virtual void disableFile(void){
			m_file = false;
			m_outpfile.close();
			m_outpfile.clear();
		}

		virtual void enableCustomFunc(dbg::callback func){
			if(func != NULL){
				m_cbFunc = func;
				m_callback = true;
			}
		}
		
		virtual void disableCustomFunc(void){
			m_cbFunc = NULL;
			m_callback = false;
		}

		//	Method to check the current state of the stream
		//	(can sometimes mess up, this gives an easy way to check)
		//	Cannot use the debug stream for this, obviously
		virtual void checkState(const std::basic_ostream<char> &stream)
		{
			int rds = stream.rdstate();
			if(rds & std::ios::goodbit)	std::cout << "rdstate(): No error condition" << std::endl;
			if(rds & std::ios::eofbit)	std::cout << "rdstate(): End of file reached" << std::endl;
			if(rds & std::ios::failbit)	std::cout << "rdstate(): A possibly recoverable formatting or conversion error" << std::endl;
			if(rds & std::ios::badbit)	std::cout << "rdstate(): A severe I/O error or unknown state" << std::endl;
		}
		
		virtual void checkState(void)
		{
			checkState(*this);
			clear();
		}
		
		virtual void checkFileState(void)
		{
			checkState(m_outpfile);
			m_outpfile.clear();
		}
	};

	static debugstream	output;

}	// namespace dbg

#endif // #ifndef _DEBUGSTREAM_H_
