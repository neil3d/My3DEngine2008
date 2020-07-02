#ifndef _NEXUS_NET_PACKET_UTIL_H_
#define _NEXUS_NET_PACKET_UTIL_H_
#include "net_packet.h"
#include <queue>
#include <boost/pool/pool.hpp>
#include <boost/bind.hpp>
#include "../ncore/memory/nmemory.h"

namespace nexus
{
	///////////////////////////////////////////////////////////////////////////////
	struct nnet_pkt_reader
	{
		nnet_packet_ptr	pkt_ptr;
		char*			read_cursor;

		explicit nnet_pkt_reader(nnet_packet_ptr _pkt_ptr)
			: pkt_ptr(_pkt_ptr),
			read_cursor((char*)( _pkt_ptr.get())+sizeof(nnet_packet_header) )
		{}

		bool empty() const
		{
			return read_cursor == NULL
				|| read_cursor- (char*)( pkt_ptr.get()) >= pkt_ptr->pkt_size;
		}

		nnet_msg_header* front_msg() const
		{
			return (nnet_msg_header*)(read_cursor);
		}

		void pop_msg()
		{
			nnet_msg_header* cur_msg = front_msg();
			read_cursor += cur_msg->msg_size;
		}
	};

	///////////////////////////////////////////////////////////////////////////////
	struct nnet_pkt_writer
	{
		nnet_packet_ptr	pkt_ptr;
		size_t			max_size;
		char*			write_cursor;

		explicit nnet_pkt_writer(nnet_packet_ptr _pkt_ptr, size_t pkt_max_size)
		{
			reset(_pkt_ptr, max_size);
		}

		void reset(nnet_packet_ptr _pkt_ptr, size_t pkt_max_size)
		{
			pkt_ptr = _pkt_ptr;
			max_size = pkt_max_size;

			nnet_packet_header* pkt_header = pkt_ptr.get();
			pkt_header->pkt_size = sizeof(nnet_packet_header);

			write_cursor = (char*)(_pkt_ptr.get())+sizeof(nnet_packet_header);
		}

		bool full() const
		{
			nnet_packet_header* pkt_header = pkt_ptr.get();

			return pkt_header == NULL
				|| pkt_header->pkt_size >= max_size;
		}

		bool empty() const
		{
			nnet_packet_header* pkt_header = pkt_ptr.get();

			return pkt_header == NULL
				|| pkt_header->pkt_size <= sizeof(nnet_packet_header);
		}

		void push_msg(nnet_msg_header* msg)
		{
			if(write_cursor+msg->msg_size
				> (char*)pkt_ptr.get()+max_size+sizeof(nnet_packet_header) )
				throw std::exception("nnet_pkt_writer error: beyond capacity.");

			memcpy(write_cursor, msg, msg->msg_size);
			write_cursor += msg->msg_size;

			nnet_packet_header* pkt_header = pkt_ptr.get();
			pkt_header->pkt_size += msg->msg_size;
		}
	};

	///////////////////////////////////////////////////////////////////////////////
	struct _nexus_pool_alloc
	{
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;

		static char* malloc(const size_type bytes)
		{
			return (char*)( nMalloc(bytes) );
		}
		static void free(char* const block)
		{
			nFree(block);
		}
	};

	class nnet_pkt_pool
	{
		boost::pool<_nexus_pool_alloc>	m_pool;
	public:
		nnet_pkt_pool(size_t chunk_size)
			: m_pool(chunk_size)
		{}
		~nnet_pkt_pool(void)	{}

		nnet_packet_ptr alloc_pkt()
		{
			return nnet_packet_ptr((nnet_packet_header*)(m_pool.malloc()),
				boost::bind(&nnet_pkt_pool::_free_pkt, this, _1) );
		}

	private:
		void _free_pkt(void* ptr)
		{
			m_pool.free(ptr);
		}
	};

	///////////////////////////////////////////////////////////////////////////////
	class nnet_pkt_recv_queue
	{
		std::queue<nnet_pkt_reader>	m_recv_queue;
	public:
		nnet_pkt_recv_queue(void)
		{}
		~nnet_pkt_recv_queue(void)
		{}

		void append_packet(nnet_packet_ptr pkt)
		{
			m_recv_queue.push( nnet_pkt_reader(pkt) );
		}

		bool empty() const
		{
			return m_recv_queue.empty();
		}

		nnet_msg_header* front_msg() const
		{
			if( !m_recv_queue.empty() )
			{
				nnet_pkt_reader front_pkt = m_recv_queue.front();
				return front_pkt.front_msg();
			}

			return NULL;
		}

		void pop_msg()
		{
			if( !m_recv_queue.empty() )
			{
				nnet_pkt_reader front_pkt = m_recv_queue.front();
				front_pkt.pop_msg();

				if (front_pkt.empty())
				{
					m_recv_queue.pop();
				}
			}// end of if
		}
	};
}// namespace nexus

#endif // _NEXUS_NET_PACKET_UTIL_H_