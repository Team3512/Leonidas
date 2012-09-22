////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "../SFML/Network/SocketSelector.hpp"
#include "../SFML/Network/Socket.hpp"
#include "SocketImpl.hpp"
#include "../SFML/System/Err.hpp"
#include <utility>


namespace sf
{
////////////////////////////////////////////////////////////
struct SocketSelector::SocketSelectorImpl
{
    fd_set AllSockets;   ///< Set containing all the sockets handles
    fd_set SocketsReady; ///< Set containing handles of the sockets that are ready
    int    MaxSocket;    ///< Maximum socket handle
};


////////////////////////////////////////////////////////////
SocketSelector::SocketSelector() :
m_impl(new SocketSelectorImpl)
{
    clear();
}


////////////////////////////////////////////////////////////
SocketSelector::SocketSelector(const SocketSelector& copy) :
m_impl(new SocketSelectorImpl(*copy.m_impl))
{

}


////////////////////////////////////////////////////////////
SocketSelector::~SocketSelector()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
void SocketSelector::add(Socket& socket)
{
    SocketHandle handle = socket.getHandle();
    if (handle != priv::SocketImpl::invalidSocket())
    {
        FD_SET(handle, &m_impl->AllSockets);

        int size = static_cast<int>(handle);
        if (size > m_impl->MaxSocket)
            m_impl->MaxSocket = size;
    }
}


////////////////////////////////////////////////////////////
void SocketSelector::remove(Socket& socket)
{
    FD_CLR(socket.getHandle(), &m_impl->AllSockets);
    FD_CLR(socket.getHandle(), &m_impl->SocketsReady);
}


////////////////////////////////////////////////////////////
void SocketSelector::clear()
{
    FD_ZERO(&m_impl->AllSockets);
    FD_ZERO(&m_impl->SocketsReady);

    m_impl->MaxSocket = 0;
}


////////////////////////////////////////////////////////////
bool SocketSelector::wait(Time timeout)
{
    // Setup the timeout
    timeval time;
    time.tv_sec  = static_cast<long>(timeout.asMicroseconds() / 1000000);
    time.tv_usec = static_cast<long>(timeout.asMicroseconds() % 1000000);

    // Initialize the set that will contain the sockets that are ready
    m_impl->SocketsReady = m_impl->AllSockets;

    // Wait until one of the sockets is ready for reading, or timeout is reached
    int count = select(m_impl->MaxSocket + 1, &m_impl->SocketsReady, NULL, NULL, timeout != Time::Zero ? &time : NULL);

    return count > 0;
}


////////////////////////////////////////////////////////////
bool SocketSelector::isReady(Socket& socket) const
{
    return FD_ISSET(socket.getHandle(), &m_impl->SocketsReady) != 0;
}


////////////////////////////////////////////////////////////
SocketSelector& SocketSelector::operator =(const SocketSelector& right)
{
    SocketSelector temp(right);

    std::swap(m_impl, temp.m_impl);

    return *this;
}

} // namespace sf
