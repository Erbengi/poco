//
// TCPServerParams.h
//
// $Id: //poco/Main/Net/include/Poco/Net/TCPServerParams.h#2 $
//
// Library: Net
// Package: TCPServer
// Module:  TCPServerParams
//
// Definition of the TCPServerParams class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Net_TCPServerParams_INCLUDED
#define Net_TCPServerParams_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Timespan.h"


namespace Poco {
namespace Net {


class Net_API TCPServerParams: public Poco::RefCountedObject
	/// This class is used to specify parameters to both the
	/// TCPServer, as well as to TCPServerDispatcher objects.
	///
	/// Subclasses may add new parameters to the class.
{
public:
	TCPServerParams();
		/// Creates the TCPServerParams.
		///
		/// Sets the following default values:
		///   - threadIdleTime:       10 seconds
		///   - maxThreads:           0
		///   - maxQueued:            64

	void setThreadIdleTime(const Poco::Timespan& idleTime);
		/// Sets the maximum idle time for a thread before
		/// it is terminated.
		///
		/// The default idle time is 10 seconds;
		
	const Poco::Timespan& getThreadIdleTime() const;
		/// Returns the maximum thread idle time.

	void setMaxQueued(int count);
		/// Sets the maximum number of queued connections.
		/// Must be greater than 0.
		///
		/// If there are already the maximum number of connections
		/// in the queue, new connections will be silently discarded.
		///
		/// The default number is 64.
		
	int getMaxQueued() const;
		/// Returns the maximum number of queued connections.

	void setMaxThreads(int count);
		/// Sets the maximum number of simultaneous threads
		/// available for this TCPServerDispatcher.
		///
		/// Must be greater than or equal to 0.
		/// If 0 is specified, the TCPServerDispatcher will
		/// set this parameter to the number of available threads
		/// in its thread pool.
		///
		/// The thread pool used by the TCPServerDispatcher
		/// must at least have the capacity for the given
		/// number of threads.
		
	int getMaxThreads() const;
		/// Returns the maximum number of simultaneous threads
		/// available for this TCPServerDispatcher.	

protected:
	virtual ~TCPServerParams();
		/// Destroys the TCPServerParams.

private:
	Poco::Timespan _threadIdleTime;
	int _maxThreads;
	int _maxQueued;
};


//
// inlines
//
inline const Poco::Timespan& TCPServerParams::getThreadIdleTime() const
{
	return _threadIdleTime;
}


inline int TCPServerParams::getMaxThreads() const
{
	return _maxThreads;
}


inline int TCPServerParams::getMaxQueued() const
{
	return _maxQueued;
}


} } // namespace Poco::Net


#endif // Net_TCPServerParams_INCLUDED
