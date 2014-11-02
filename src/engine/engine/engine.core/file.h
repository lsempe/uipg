#pragma once

#include "engine.math/math.h"

#include <string>
#include <fstream>

namespace core
{

	class file
	{
	public:

		typedef unsigned int handle;
		enum class Mode
		{
			Write,
			Read,
			ReadWrite
		};

		enum class Seek
		{
			Start,
			CurrentPosition,
			End
		};

		virtual void open(const std::wstring , Mode ) {  }
		virtual void close() {}

		virtual void seek(size_t , Seek ) {}

		virtual void write(size_t , const void* ) {}
		virtual size_t read(size_t , void* ) { return 0; }

	protected:

		//handle m_handle;
	};

	class text_file : public file
	{
	public:
		//typedef std::wfstream handle;

		virtual void open(const std::wstring file, Mode mode)
		{			
			switch (mode)
			{
			case Mode::Read:
				m_stream = std::wfstream(file.c_str(), std::wfstream::in);
				break;
			case Mode::Write:
				m_stream = std::wfstream(file.c_str(), std::wfstream::out | std::wfstream::binary);
				break;
			case Mode::ReadWrite:
				m_stream = std::wfstream(file.c_str(), std::wfstream::in | std::wfstream::out);
				break;
			}			
		}

		virtual void close()
		{
			m_stream.close();
		}

		virtual void seek(size_t offset, Seek seek)
		{
			switch (seek)
			{
			case Seek::Start:
				m_stream.seekg(offset, std::ios_base::beg);
				break;
			case Seek::End:
				m_stream.seekg(offset, std::ios_base::end);
				break;
			case Seek::CurrentPosition:
				m_stream.seekg(offset, std::ios_base::cur);
				break;
			}			
		}

		virtual void write(const std::wstring& text)
		{
			m_stream.write(text.c_str(), text.size());
		}

		virtual void write_line(const std::wstring& text)
		{
			std::wstring line = text;
			line.append(L"\n");
			m_stream.write(line.c_str(), line.size());
		}

		virtual size_t read(size_t size, std::wstring& outText)
		{
			wchar_t* buffer = new wchar_t[size];

			std::streamoff position = m_stream.tellg();
			size_t bytesToRead = math::Min(size, static_cast<size_t>(m_stream.gcount() - position));

			m_stream.read(buffer, bytesToRead);
			
			outText.resize(size);
			outText = buffer;
			delete [] buffer;
			return outText.size();
		}

		virtual size_t read_line(std::wstring& outText)
		{
			wchar_t buffer[256];
			m_stream.getline(buffer, 256, '\n');
			outText = buffer;
			return outText.size();
		}


	protected:

		std::wfstream m_stream;
	};

}