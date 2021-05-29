/*
 * EThrowable.cpp
 *
 *  Created on: 2013-3-14
 *      Author: cxxjava@163.com
 */

#include "EThrowable.hh"
#include "EInteger.hh"

#ifdef WIN32
#include <windows.h>
#else
#include <cxxabi.h>
#include <execinfo.h>
#endif

namespace efc {

#ifdef DEBUG
volatile boolean EThrowable::stackTraceFlag = true;
#else
volatile boolean EThrowable::stackTraceFlag = false;
#endif

#define MAX_TRACE_LEVEL 32

#ifdef WIN32

//@see: https://github.com/chromium/chromium/blob/master/base/debug/stack_trace_win.cc

char* backtrace_get_from(es_string_t **trace, int top) {
	//TODO: https://github.com/froglogic/tracetool::tracetool-3.0.1/hooklib/backtrace_win.cpp
	return null;
}

#else //!

//@see: chromium-base/src/base/debug/stack_trace_posix.cc

// The prefix used for mangled symbols, per the Itanium C++ ABI:
// http://www.codesourcery.com/cxx-abi/abi.html#mangling
const char kMangledSymbolPrefix[] = "_Z";

// Characters that can be used for symbols, generated by Ruby:
// (('a'..'z').to_a+('A'..'Z').to_a+('0'..'9').to_a + ['_']).join
const char kSymbolCharacters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

// Demangles C++ symbols in the given text. Example:
//
// "out/Debug/base_unittests(_ZN10StackTraceC1Ev+0x20) [0x817778c]"
// =>
// "out/Debug/base_unittests(StackTrace::StackTrace()+0x20) [0x817778c]"
static void demangle_symbols(EString& text) {
	// Note: code in this function is NOT async-signal safe (std::string uses
	// malloc internally).

	int search_from = 0;
	while (search_from < text.length()) {
		// Look for the start of a mangled symbol, from search_from.
		int mangled_start = text.indexOf(kMangledSymbolPrefix, search_from);
		if (mangled_start == -1) {
			break; // Mangled symbol not found.
		}

		// Look for the end of the mangled symbol.
		int mangled_end = text.lastIndexOf(" +"); //?
		if (mangled_end == -1) {
			mangled_end = text.length();
		}
		EString mangled_symbol = text.substring(mangled_start, mangled_end);

		// Try to demangle the mangled symbol candidate.
		int status = 0;
		char * demangled_symbol = abi::__cxa_demangle(mangled_symbol.c_str(), NULL, 0,
				&status);
		if (status == 0 && demangled_symbol) { // Demangling is successful.
			// Remove the mangled symbol.
			text.erase(mangled_start, mangled_end - mangled_start);
			// Insert the demangled symbol.
			text.insert(mangled_start, demangled_symbol);
			// Next time, we'll start right after the demangled symbol we inserted.
			search_from = mangled_start + eso_strlen(demangled_symbol);
		} else {
			// Failed to demangle.  Retry after the "_Z" we just found.
			search_from = mangled_start + 2;
		}
		if (demangled_symbol) {
			eso_free(demangled_symbol);
		}
	}
}

char* backtrace_get_from(es_string_t **trace, int top)
{
	void *callers[MAX_TRACE_LEVEL];
	int frames;
	char **symbol;
	int i;

	if (!trace || top < 0 || top >= ES_ARRAY_LEN(callers)) return NULL;

	eso_mmeminit((es_data_t**)trace);

	frames = backtrace(callers, ES_ARRAY_LEN(callers));
	symbol = backtrace_symbols(callers, frames);
	for (i = top; i < frames; i++) {
		EString symname(symbol[i]);
		demangle_symbols(symname);
		eso_mstrcat(trace, symname.c_str());
		eso_mstrcat(trace, "\n");
	}
	free(symbol);

	return *trace;
}

#endif //!WIN32

# define SYNCBLOCK(lock) { \
	Sentry __synchronizer__(lock);

EThrowable::~EThrowable() {
	if (cause && cause != this) delete cause;
}

EThrowable::EThrowable(const char *_file_, int _line_, int errn): cause(null)
{
	_file = _file_;
	_line = _line_;

	_errn = (errn==0) ? errno : errn;
	if (_errn) {
		detailMessage = strerror(_errn);
	}
	else {
		detailMessage = "Null";
	}

	initStackTrace();
}

EThrowable::EThrowable(const char *_file_, int _line_, const char *message, int errn): cause(null)
{
	_file = _file_;
	_line = _line_;

	_errn = (errn==0) ? errno : errn;
	if (_errn && !message) {
		detailMessage = strerror(_errn);
	}
	else {
		detailMessage = message;
	}

	initStackTrace();
}

EThrowable::EThrowable(const char *_file_, int _line_, EThrowable* cause): cause(null)
{
	_file = _file_;
	_line = _line_;

	_errn = errno;
	if (_errn) {
		detailMessage = strerror(_errn);
	}
	else {
		detailMessage = "Null";
	}

	initStackTrace();

	if (cause && cause != this) {
		this->cause = new EThrowable(*cause);
	}
}

EThrowable::EThrowable(const char *_file_, int _line_, const char *message, EThrowable* cause): cause(null)
{
	_file = _file_;
	_line = _line_;

	_errn = errno;
	if (_errn && !message) {
		detailMessage = strerror(_errn);
	}
	else {
		detailMessage = message;
	}

	initStackTrace();

	if (cause && cause != this) {
		this->cause = new EThrowable(*cause);
	}
}

EThrowable::EThrowable(const EThrowable& that):
		_errn(that._errn),
		_line(that._line),
		_file(that._file),
		detailMessage(that.detailMessage),
		backtrace(that.backtrace) {
	cause = null;
	if (that.cause) {
		cause = new EThrowable(that.cause->_file.c_str(), that.cause->_line,
				that.cause->detailMessage.c_str(), that.cause->_errn);
	}
}

EThrowable* EThrowable::getCause() {
	SYNCBLOCK(&spin) {
		return cause;
	}}
}

EThrowable* EThrowable::initCause(EThrowable* cause) {
	SYNCBLOCK(&spin) {
		if (this->cause && this->cause != this) {
			/* @see:
			throw EIllegalStateException("Can't overwrite cause with " +
											Objects.toString(cause, "a null"), this);
			*/
			return null;
		}
		if (cause == this) {
			/* @see:
			throw new IllegalArgumentException("Self-causation not permitted", this);
			*/
			return null;
		}
		if (cause) this->cause = new EThrowable(*cause);
		return this;
	}}
}

int EThrowable::getErrno()
{
	return _errn;
}

const char* EThrowable::getMessage()
{
	return detailMessage.c_str();
}

const char* EThrowable::getStackTrace() {
	return backtrace.c_str();
}

const char* EThrowable::getSourceFile() {
	return _file.c_str();
}

int EThrowable::getSourceLine() {
	return _line;
}

EString EThrowable::toString() {
	EString str;
	str.format("Throwable:\nfile: %s\nline: %d\nerrno: %d\nmessage: %s\n",
			getSourceFile(), _line, _errn, getMessage());
	if (cause) {
		str << "Caused by:\n";
		str << cause->toString();
	}
	return str;
}

void EThrowable::printStackTrace()
{
	EString msg = toString();
	fprintf(stderr, "%s\n", msg.c_str());
	fprintf(stderr, "Stack trace: %s\n", getStackTrace());
	if (cause) {
		fprintf(stderr, "Caused by: \n");
		cause->printStackTrace();
	}
}

void EThrowable::initStackTrace() {
	if (stackTraceFlag) {
		es_string_t *trace = NULL;
		backtrace_get_from(&trace, 2);
		if (trace) {
			backtrace = trace;
			eso_mfree(trace);
		}
	}
}

void EThrowable::setStackTrace(boolean on) {
	stackTraceFlag = on;
}

} /* namespace efc */
