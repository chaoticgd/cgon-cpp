/*
	Copyright (c) 2018 chaoticgd

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef _CGON_DEMANGLE_H
#define _CGON_DEMANGLE_H

#include <string>

#ifdef __GNUC__
	#include <cxxabi.h>
#endif

namespace cgon {
	static std::string demangle_name(std::string mangled_name) {
		#ifdef __GNUC__
			const size_t max_length = 256;
			std::string result;
			result.resize(max_length);
			int status;

			abi::__cxa_demangle(
				mangled_name.c_str(),
				result.data(),
				const_cast<size_t*>(&max_length),
				&status
			);

			if(status != 0) {
				return mangled_name;
			}

			result.resize(result.find('\0'));
			return result;
		#else
			return mangled_name;
		#endif
	}
}

#endif
