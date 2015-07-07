//
// BinaryProtocol.cpp for BinaryProtocol in /home/alexmog/projets/LibNet/src/protocols
// 
// Made by Moghrabi Alexandre
// Login   <alexandre.moghrabi@epitech.eu>
// 
// Started on  Fri Apr 17 15:40:07 2015 Moghrabi Alexandre
// Last update Tue Jul  7 14:51:12 2015 Moghrabi Alexandre
//

#include <iostream>
#include <cstring>
#include "mognetwork/OS.hh"
#ifdef OS_WINDOWS
# include <winsock2.h>
# include <algorithm>
#else
#include <arpa/inet.h>
#endif // OS_WINDOWS
#include "mognetwork/WebsocketProtocol.hh"

namespace mognetwork
{
  namespace protocol
  {
    WebsocketProtocol::WebsocketProtocol(TcpSocket* socket) :
      AProtocolListener(socket) {}

    WebsocketProtocol::~WebsocketProtocol() {}

    Socket::Status WebsocketProtocol::onReadTrigger()
    {
      
      return Socket::Error;
    }

    bool WebsocketProtocol::datasFullyReceived()
    {
      return false;
    }

    char* WebsocketProtocol::construct_frame(const char* data, uint32_t& size)
    {
      uint32_t* payload_len;
      unsigned short int* payload_len_short;
      unsigned char finNopcode, pauload_len_small;
      uins32_t playload_offset = 2;
      uint32_t frame_size;
      char* frame;

      finNopcode = 0xff;
      if (size <= 125)
	{
	  frame_size = 2 + size;
	  payload_len_small = size & 0xff;
	}
      else if (size > 125 && size <= 0xffff)
	{
	  frame_size = 4 + size;
	  payload_len_small = 126;
	  payload_offset += 2;
	}
      else if (size > 0xffff && size <= 0xfffffff0)
	{
	  frame_size = 10 + size;
	  pauload_len_small = 127;
	  payload_offset += 8;
	}
      else
	{
	  // TODO protocolexception
	  //	  throw ProtocolException("LibNet does not support frame payload size too big.");
	}
      frame = new char[frame_size];
      payload_len_small &= 0x7f;
      *frame = finNopcode;
      *(frame + 1) = payload_len_small;
      if (playload_len_small == 126)
	{
	  size &= 0xffff;
	  payload_len_short = static_cast<unsigned int*>((char*) frame + 2);
	  *payload_len_short = htons(size);
	}
      if (payload_len_small == 127)
	{
	  payload_len = static_cast<unsigned int*>((char*) frame + 2);
	  *payload_len++ = 0;
	  *payload_len = htonl(size);
	}
      std::memcpy(frame + payload_offset, data, size);
      return frame;
    }

    void WebsocketProtocol::onSendDatas(const char* data, uint32_t size, TcpSocket::Data& dataToSend)
    {
      char* frame = construct_frame(data, size);
      dataToSend.resize(size);
      std::memcpy(&dataToSend.front(), frame, size);
      delete frame;
    }

    Socket::Status WebsocketProtocol::onReadAllTrigger(TcpSocket::Data& data)
    {
      return Socket::Error;
    }
  }
}