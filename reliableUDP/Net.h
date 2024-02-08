#pragma once
/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/


#ifndef NET_H
#define NET_H

#include <cstring> // for memcpy

// platform detection
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#else

#error unknown platform!

#endif

#include <assert.h>
#include <vector>
#include <map>
#include <stack>
#include <list>
#include <algorithm>
#include <functional>
const int PacketSizeHack = 256 + 128;
namespace net
{
	// platform independent wait for n seconds

#if PLATFORM == PLATFORM_WINDOWS

	void wait(float seconds)
	{
		Sleep((int)(seconds * 1000.0f));
	}

#else

#include <unistd.h>
	void wait(float seconds) { usleep((int)(seconds * 1000000.0f)); }

#endif

	// internet address

	class Address
	{
	public:

		Address()
		{
			address = 0;
			port = 0;
		}

		Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
		{
			this->address = (a << 24) | (b << 16) | (c << 8) | d;
			this->port = port;
		}

		Address(unsigned int address, unsigned short port)
		{
			this->address = address;
			this->port = port;
		}

		unsigned int GetAddress() const
		{
			return address;
		}

		unsigned char GetA() const
		{
			return (unsigned char)(address >> 24);
		}

		unsigned char GetB() const
		{
			return (unsigned char)(address >> 16);
		}

		unsigned char GetC() const
		{
			return (unsigned char)(address >> 8);
		}

		unsigned char GetD() const
		{
			return (unsigned char)(address);
		}

		unsigned short GetPort() const
		{
			return port;
		}

		bool operator == (const Address& other) const
		{
			return address == other.address && port == other.port;
		}

		bool operator != (const Address& other) const
		{
			return !(*this == other);
		}

		bool operator < (const Address& other) const
		{
			// note: this is so we can use address as a key in std::map
			if (address < other.address)
				return true;
			if (address > other.address)
				return false;
			else
				return port < other.port;
		}

	private:

		unsigned int address;
		unsigned short port;
	};

	// sockets

	inline bool InitializeSockets()
	{
#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData;
		return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
		return true;
#endif
	}

	inline void ShutdownSockets()
	{
#if PLATFORM == PLATFORM_WINDOWS
		WSACleanup();
#endif
	}

	class Socket
	{
	public:

		Socket()
		{
			socket = 0;
		}

		~Socket()
		{
			Close();
		}

		bool Open(unsigned short port)
		{
			assert(!IsOpen());	// Ensure the socket is not already open

			// create socket

			socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			if (socket <= 0)
			{
				printf("failed to create socket\n");
				socket = 0;
				return false;
			}

			// bind to port

			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons((unsigned short)port);

			if (bind(socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
			{
				printf("failed to bind socket\n");
				Close();
				return false;
			}

			// set non-blocking io

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

			int nonBlocking = 1;
			if (fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
			{
				printf("failed to set non-blocking socket\n");
				Close();
				return false;
			}

#elif PLATFORM == PLATFORM_WINDOWS

			DWORD nonBlocking = 1;
			if (ioctlsocket(socket, FIONBIO, &nonBlocking) != 0)
			{
				printf("failed to set non-blocking socket\n");
				Close();
				return false;
			}

#endif

			return true;
		}

		void Close()
		{
			if (socket != 0)
			{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
				close(socket);
#elif PLATFORM == PLATFORM_WINDOWS
				closesocket(socket);
#endif
				socket = 0;
			}
		}

		bool IsOpen() const
		{
			return socket != 0;
		}

		bool Send(const Address& destination, const void* data, int size)
		{
			assert(data); // Ensure that the data pointer is not nul;
			assert(size > 0);	// Ensure that the size of the data is greater than 0

			if (socket == 0)
				return false;

			assert(destination.GetAddress() != 0);	// Ensure that the destination address is not 0
			assert(destination.GetPort() != 0);	// Ensure that the destination port is not 0

			sockaddr_in address;
			address.sin_family = AF_INET;	// Set the address family to IPv4
			address.sin_addr.s_addr = htonl(destination.GetAddress());	// Set the destination IP address
			address.sin_port = htons((unsigned short)destination.GetPort());	// Set the destination port

			// Use the sendto function to send data over the network
			int sent_bytes = sendto(socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

			// Return true if all bytes were sent successfully, false otherwise
			return sent_bytes == size;
		}

		int Receive(Address& sender, void* data, int size)
		{
			assert(data);
			assert(size > 0);

			// Check if the socket is valid
			if (socket == 0)
				return false;

#if PLATFORM == PLATFORM_WINDOWS
			typedef int socklen_t;
#endif

			sockaddr_in from;
			socklen_t fromLength = sizeof(from);

			// Receive data from the socket
			int received_bytes = recvfrom(socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength);

			// If no bytes were received, return 0
			if (received_bytes <= 0)
				return 0;

			// Extract sender's address information
			unsigned int address = ntohl(from.sin_addr.s_addr);
			unsigned short port = ntohs(from.sin_port);

			// Update the sender's address
			sender = Address(address, port);

			// Return the number of received bytes
			return received_bytes;
		}

	private:

		int socket;
	};

	// connection

	class Connection
	{
	public:

		enum Mode
		{
			None,
			Client,
			Server
		};

		Connection(unsigned int protocolId, float timeout)
		{
			this->protocolId = protocolId;
			this->timeout = timeout;
			mode = None;
			running = false;
			ClearData();
		}

		virtual ~Connection()
		{
			if (IsRunning())
				Stop();
		}

		bool Start(int port)
		{
			assert(!running);
			printf("start connection on port %d\n", port);
			if (!socket.Open(port))
				return false;
			running = true;
			OnStart();
			return true;
		}

		void Stop()
		{
			assert(running);

			printf("stop connection\n");

			bool connected = IsConnected();

			ClearData();
			socket.Close();
			running = false;

			if (connected)
				OnDisconnect();

			OnStop();
		}

		bool IsRunning() const
		{
			return running;
		}

		void Listen()
		{
			printf("server listening for connection\n");

			bool connected = IsConnected();

			ClearData();

			if (connected)
				OnDisconnect();

			mode = Server;
			state = Listening;
		}

		void Connect(const Address& address)
		{
			printf("client connecting to %d.%d.%d.%d:%d\n",
				address.GetA(), address.GetB(), address.GetC(), address.GetD(), address.GetPort());

			bool connected = IsConnected();

			ClearData();

			if (connected)
				OnDisconnect();

			// Set the mode to Client and update the state to Connecting
			mode = Client;
			state = Connecting;

			this->address = address;
		}

		bool IsConnecting() const
		{
			return state == Connecting;
		}

		bool ConnectFailed() const
		{
			return state == ConnectFail;
		}

		bool IsConnected() const
		{
			return state == Connected;
		}

		bool IsListening() const
		{
			return state == Listening;
		}

		Mode GetMode() const
		{
			return mode;
		}

		virtual void Update(float deltaTime)
		{
			assert(running);

			timeoutAccumulator += deltaTime;	// Accumulate elapsed time

			// Check if the accumulated timeout has exceeded the specified timeout duration
			if (timeoutAccumulator > timeout)
			{
				// Handle timeout based on the current connection state
				if (state == Connecting)
				{
					printf("connect timed out\n");
					ClearData();
					state = ConnectFail;
					OnDisconnect();
				}
				else if (state == Connected)
				{
					printf("connection timed out\n");
					ClearData();

					// Check if the state was still in Connecting (double-check to avoid unnecessary state changes)
					if (state == Connecting)
						state = ConnectFail;
					OnDisconnect();
				}
			}
		}

		virtual bool SendPacket(const unsigned char data[], int size)
		{
			assert(running);		// Assertion to ensure that the system is in a valid state

			if (address.GetAddress() == 0)
				return false;

			unsigned char packet[PacketSizeHack + 4];		// Create a buffer for the packet data

			// Add the 4-byte protocol identifier to the packet
			packet[0] = (unsigned char)(protocolId >> 24);
			packet[1] = (unsigned char)((protocolId >> 16) & 0xFF);
			packet[2] = (unsigned char)((protocolId >> 8) & 0xFF);
			packet[3] = (unsigned char)((protocolId) & 0xFF);
			
			std::memcpy(&packet[4], data, size);	    // Copy the actual data to be sent into the packet buffer
			return socket.Send(address, packet, size + 4);		    // Use the socket to send the packet to the specified address
		}

		virtual int ReceivePacket(unsigned char data[], int size)
		{
			assert(running);

			unsigned char packet[PacketSizeHack + 4];	// Buffer to hold received packet data
			Address sender;	// Address of the sender
			int bytes_read = socket.Receive(sender, packet, size + 4);	// Receive data from the socket

			// If no bytes were read, return 0 (indicating no data received)
			if (bytes_read == 0)
				return 0;

			// If less than or equal to 4 bytes were read, return 0 (invalid packet)
			if (bytes_read <= 4)
				return 0;

			// Check if the received packet has the correct protocol identifier
			if (packet[0] != (unsigned char)(protocolId >> 24) ||
				packet[1] != (unsigned char)((protocolId >> 16) & 0xFF) ||
				packet[2] != (unsigned char)((protocolId >> 8) & 0xFF) ||
				packet[3] != (unsigned char)(protocolId & 0xFF))
				return 0;

			// If the mode is Server and not yet connected, print and update state
			if (mode == Server && !IsConnected())
			{
				printf("server accepts connection from client %d.%d.%d.%d:%d\n",
					sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(), sender.GetPort());
				state = Connected;
				address = sender;
				OnConnect();
			}


			if (sender == address)
			{

				// If the mode is Client and in Connecting state, print and update state
				if (mode == Client && state == Connecting)
				{
					printf("client completes connection with server\n");
					state = Connected;
					OnConnect();
				}
				timeoutAccumulator = 0.0f;	// Reset timeout accumulator
				memcpy(data, &packet[4], bytes_read - 4);   // Copy the payload data from the received packet to the 'data' buffer
				return bytes_read - 4;	 // Return the size of the received payload data
			}
			return 0;
		}

		int GetHeaderSize() const
		{
			return 4;
		}

	protected:

		virtual void OnStart() {}
		virtual void OnStop() {}
		virtual void OnConnect() {}
		virtual void OnDisconnect() {}

	private:

		void ClearData()
		{
			state = Disconnected;
			timeoutAccumulator = 0.0f;
			address = Address();
		}

		enum State
		{
			Disconnected,
			Listening,
			Connecting,
			ConnectFail,
			Connected
		};

		unsigned int protocolId;
		float timeout;

		bool running;
		Mode mode;
		State state;
		Socket socket;
		float timeoutAccumulator;
		Address address;
	};

	// packet queue to store information about sent and received packets sorted in sequence order
	//  + we define ordering using the "sequence_more_recent" function, this works provided there is a large gap when sequence wrap occurs

	struct PacketData
	{
		unsigned int sequence;			// packet sequence number
		float time;					    // time offset since packet was sent or received (depending on context)
		int size;						// packet size in bytes
	};

	inline bool sequence_more_recent(unsigned int s1, unsigned int s2, unsigned int max_sequence)
	{
		auto half_max = max_sequence / 2;
		return (
			((s1 > s2) && (s1 - s2 <= half_max))
			||
			((s2 > s1) && (s2 - s1 > half_max))
			);
	}

	class PacketQueue : public std::list<PacketData>
	{
	public:

		bool exists(unsigned int sequence)
		{
			for (iterator itor = begin(); itor != end(); ++itor)
				if (itor->sequence == sequence)
					return true;	//Return true if a packet with the given sequence number is found
			return false;	//Otherwise return false
		}

		void insert_sorted(const PacketData& p, unsigned int max_sequence)
		{
			// If the queue is empty, directly push the packet to the back
			if (empty())
			{
				push_back(p);
			}
			else
			{
				if (!sequence_more_recent(p.sequence, front().sequence, max_sequence))
				{
					push_front(p);		// If the new packet is more recent than the front, push it to the front
				}
				else if (sequence_more_recent(p.sequence, back().sequence, max_sequence))
				{
					push_back(p);		// If the new packet is less recent than the back, push it to the back
				}
				else
				{
					// Iterate through the queue to find the appropriate position to insert the packet
					for (PacketQueue::iterator itor = begin(); itor != end(); itor++)
					{
						assert(itor->sequence != p.sequence);	// Ensure no duplicate sequences

						// Insert the packet at the appropriate position
						if (sequence_more_recent(itor->sequence, p.sequence, max_sequence))
						{
							insert(itor, p);
							break;
						}
					}
				}
			}
		}

		void verify_sorted(unsigned int max_sequence)
		{
			PacketQueue::iterator prev = end();		// Iterate through the queue to verify sorting and sequence range
			for (PacketQueue::iterator itor = begin(); itor != end(); itor++)
			{
				assert(itor->sequence <= max_sequence);		// Ensure sequence is within the allowed range
				if (prev != end())
				{
					assert(sequence_more_recent(itor->sequence, prev->sequence, max_sequence));		// Ensure packets are sorted in descending order of sequence numbers
					prev = itor;
				}
			}
		}
	};

	// reliability system to support reliable connection
	//  + manages sent, received, pending ack and acked packet queues
	//  + separated out from reliable connection because it is quite complex and i want to unit test it!

	class ReliabilitySystem
	{
	public:

		ReliabilitySystem(unsigned int max_sequence = 0xFFFFFFFF)
		{
			this->rtt_maximum = rtt_maximum;
			this->max_sequence = max_sequence;
			Reset();
		}

		void Reset()
		{
			local_sequence = 0;
			remote_sequence = 0;
			sentQueue.clear();
			receivedQueue.clear();
			pendingAckQueue.clear();
			ackedQueue.clear();
			sent_packets = 0;
			recv_packets = 0;
			lost_packets = 0;
			acked_packets = 0;
			sent_bandwidth = 0.0f;
			acked_bandwidth = 0.0f;
			rtt = 0.0f;
			rtt_maximum = 1.0f;
		}

		void PacketSent(int size)
		{
			// Check if the local sequence number already exists in the sent queue
			if (sentQueue.exists(local_sequence))
			{
				printf("local sequence %d exists\n", local_sequence);
				for (PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor)
					printf(" + %d\n", itor->sequence);
			}

			// Ensure that the local sequence number does not exist in the sent or pending acknowledgment queues
			assert(!sentQueue.exists(local_sequence));
			assert(!pendingAckQueue.exists(local_sequence));

			// Create a PacketData structure to represent the sent packet
			PacketData data;
			data.sequence = local_sequence;
			data.time = 0.0f;
			data.size = size;

			// Add the packet data to both the sent and pending acknowledgment queues
			sentQueue.push_back(data);
			pendingAckQueue.push_back(data);


			sent_packets++;		
			local_sequence++;

			// Wrap the local sequence number if it exceeds the maximum allowed sequence number
			if (local_sequence > max_sequence)
				local_sequence = 0;
		}

		void PacketReceived(unsigned int sequence, int size)
		{
			recv_packets++;

			// Check if the received packet with the given sequence number already exists in the received queue
			if (receivedQueue.exists(sequence))
				return;

			// Create a PacketData structure to represent the received packet
			PacketData data;
			data.sequence = sequence;
			data.time = 0.0f;
			data.size = size;

			receivedQueue.push_back(data);	    // Add the received packet data to the received queue

			// Update the remote sequence number if the received sequence is more recent
			if (sequence_more_recent(sequence, remote_sequence, max_sequence))
				remote_sequence = sequence;
		}

		unsigned int GenerateAckBits()
		{
			return generate_ack_bits(GetRemoteSequence(), receivedQueue, max_sequence);
		}

		void ProcessAck(unsigned int ack, unsigned int ack_bits)
		{
			process_ack(ack, ack_bits, pendingAckQueue, ackedQueue, acks, acked_packets, rtt, max_sequence);
		}

		void Update(float deltaTime)
		{
			acks.clear();
			AdvanceQueueTime(deltaTime);
			UpdateQueues();
			UpdateStats();
#ifdef NET_UNIT_TEST
			Validate();
#endif
		}

		void Validate()
		{
			sentQueue.verify_sorted(max_sequence);
			receivedQueue.verify_sorted(max_sequence);
			pendingAckQueue.verify_sorted(max_sequence);
			ackedQueue.verify_sorted(max_sequence);
		}

		// utility functions

	/*
		static bool sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence )
		{
			return ( s1 > s2 ) && ( s1 - s2 <= max_sequence/2 ) || ( s2 > s1 ) && ( s2 - s1 > max_sequence/2 );
		}
	*/

		static int bit_index_for_sequence(unsigned int sequence, unsigned int ack, unsigned int max_sequence)
		{
			// Ensure that the given sequence is not equal to the acknowledgment sequence
			assert(sequence != ack);

			// Ensure that the given sequence is not more recent than the acknowledgment sequence
			assert(!sequence_more_recent(sequence, ack, max_sequence));

			if (sequence > ack)
			{
				assert(ack < 33);
				assert(max_sequence >= sequence);
				return ack + (max_sequence - sequence);
			}
			else
			{
				assert(ack >= 1);
				assert(sequence <= ack - 1);
				return ack - 1 - sequence;
			}
		}

		static unsigned int generate_ack_bits(unsigned int ack, const PacketQueue& received_queue, unsigned int max_sequence)
		{
			unsigned int ack_bits = 0;

			// Iterate through the received_queue to determine acknowledgment status
			for (PacketQueue::const_iterator itor = received_queue.begin(); itor != received_queue.end(); itor++)
			{
				// If the sequence number matches the acknowledgment or is more recent, stop processing
				if (itor->sequence == ack || sequence_more_recent(itor->sequence, ack, max_sequence))
					break;

				// Calculate the bit index for the sequence number
				int bit_index = bit_index_for_sequence(itor->sequence, ack, max_sequence);

				// Set the corresponding bit in ack_bits if the bit index is within the valid range
				if (bit_index <= 31)
					ack_bits |= 1 << bit_index;
			}
			return ack_bits;
		}

		static void process_ack(unsigned int ack, unsigned int ack_bits,
			PacketQueue& pending_ack_queue, PacketQueue& acked_queue,
			std::vector<unsigned int>& acks, unsigned int& acked_packets,
			float& rtt, unsigned int max_sequence)
		{
			// Check if the pending_ack_queue is empty
			if (pending_ack_queue.empty())
				return;

			// Iterate through the pending_ack_queue
			PacketQueue::iterator itor = pending_ack_queue.begin();
			while (itor != pending_ack_queue.end())
			{
				bool acked = false;

				// Check if the current packet is acknowledged by comparing sequence numbers
				if (itor->sequence == ack)
				{
					acked = true;
				}

				// If the current packet's sequence number is not the same as the acknowledgment use the ack_bits field to check acknowledgment status for the specific sequence
				else if (!sequence_more_recent(itor->sequence, ack, max_sequence))
				{
					int bit_index = bit_index_for_sequence(itor->sequence, ack, max_sequence);
					if (bit_index <= 31)
						acked = (ack_bits >> bit_index) & 1;
				}

				// If the packet is acknowledged, update statistics and move it to the acked_queue
				if (acked)
				{
					// Update round-trip time using exponential smoothing
					rtt += (itor->time - rtt) * 0.1f;

					// Insert the acknowledged packet into the acked_queue in a sorted manner
					acked_queue.insert_sorted(*itor, max_sequence);

					// Add the acknowledged sequence number to the acks vector
					acks.push_back(itor->sequence);

					acked_packets++;

					// Erase the acknowledged packet from the pending_ack_queue
					itor = pending_ack_queue.erase(itor);
				}
				else
					// Move to the next packet in the pending_ack_queue
					++itor;
			}
		}

		// data accessors

		unsigned int GetLocalSequence() const
		{
			return local_sequence;
		}

		unsigned int GetRemoteSequence() const
		{
			return remote_sequence;
		}

		unsigned int GetMaxSequence() const
		{
			return max_sequence;
		}

		void GetAcks(unsigned int** acks, int& count)
		{
			*acks = &this->acks[0];
			count = (int)this->acks.size();
		}

		unsigned int GetSentPackets() const
		{
			return sent_packets;
		}

		unsigned int GetReceivedPackets() const
		{
			return recv_packets;
		}

		unsigned int GetLostPackets() const
		{
			return lost_packets;
		}

		unsigned int GetAckedPackets() const
		{
			return acked_packets;
		}

		float GetSentBandwidth() const
		{
			return sent_bandwidth;
		}

		float GetAckedBandwidth() const
		{
			return acked_bandwidth;
		}

		float GetRoundTripTime() const
		{
			return rtt;
		}

		int GetHeaderSize() const
		{
			return 12;
		}

	protected:

		void AdvanceQueueTime(float deltaTime)
		{
			// Advance the time for entries in the sentQueue
			for (PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); itor++)
				itor->time += deltaTime;

			// Advance the time for entries in the receivedQueue
			for (PacketQueue::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++)
				itor->time += deltaTime;

			// Advance the time for entries in the pendingAckQueue
			for (PacketQueue::iterator itor = pendingAckQueue.begin(); itor != pendingAckQueue.end(); itor++)
				itor->time += deltaTime;

			// Advance the time for entries in the ackedQueue
			for (PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); itor++)
				itor->time += deltaTime;
		}

		void UpdateQueues()
		{
			const float epsilon = 0.001f;

			// Remove entries from the sentQueue that have exceeded the round-trip time limit
			while (sentQueue.size() && sentQueue.front().time > rtt_maximum + epsilon)
				sentQueue.pop_front();

			// Update the receivedQueue by removing entries that have sequence numbers outside the valid range
			if (receivedQueue.size())
			{
				const unsigned int latest_sequence = receivedQueue.back().sequence;
				const unsigned int minimum_sequence = latest_sequence >= 34 ? (latest_sequence - 34) : max_sequence - (34 - latest_sequence);

				// Remove entries from the receivedQueue that have sequence numbers outside the valid range
				while (receivedQueue.size() && !sequence_more_recent(receivedQueue.front().sequence, minimum_sequence, max_sequence))
					receivedQueue.pop_front();
			}

			// Remove entries from the ackedQueue that have exceeded twice the round-trip time limit
			while (ackedQueue.size() && ackedQueue.front().time > rtt_maximum * 2 - epsilon)
				ackedQueue.pop_front();


			// Remove entries from the pendingAckQueue that have exceeded the round-trip time limit, and update the lost_packets count accordingly
			while (pendingAckQueue.size() && pendingAckQueue.front().time > rtt_maximum + epsilon)
			{
				pendingAckQueue.pop_front();
				lost_packets++;
			}
		}

		void UpdateStats()
		{
			int sent_bytes_per_second = 0;

			// Calculate the total sent bytes per second by iterating through the sentQueue
			for (PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor)
				sent_bytes_per_second += itor->size;

			int acked_packets_per_second = 0;
			int acked_bytes_per_second = 0;

			// Calculate the total acknowledged packets and bytes per second
			for (PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); ++itor)
			{
				// Consider only packets that have been acknowledged within the maximum round-trip time (rtt_maximum)
				if (itor->time >= rtt_maximum)
				{
					acked_packets_per_second++;
					acked_bytes_per_second += itor->size;
				}
			}

			// Calculate the average sent bytes and acknowledged bytes per second over the maximum round-trip time
			sent_bytes_per_second /= rtt_maximum;
			acked_bytes_per_second /= rtt_maximum;

			// Calculate the sent and acknowledged bandwidth in kilobits per second
			sent_bandwidth = sent_bytes_per_second * (8 / 1000.0f);
			acked_bandwidth = acked_bytes_per_second * (8 / 1000.0f);
		}

	private:

		unsigned int max_sequence;			// maximum sequence value before wrap around (used to test sequence wrap at low # values)
		unsigned int local_sequence;		// local sequence number for most recently sent packet
		unsigned int remote_sequence;		// remote sequence number for most recently received packet

		unsigned int sent_packets;			// total number of packets sent
		unsigned int recv_packets;			// total number of packets received
		unsigned int lost_packets;			// total number of packets lost
		unsigned int acked_packets;			// total number of packets acked

		float sent_bandwidth;				// approximate sent bandwidth over the last second
		float acked_bandwidth;				// approximate acked bandwidth over the last second
		float rtt;							// estimated round trip time
		float rtt_maximum;					// maximum expected round trip time (hard coded to one second for the moment)

		std::vector<unsigned int> acks;		// acked packets from last set of packet receives. cleared each update!

		PacketQueue sentQueue;				// sent packets used to calculate sent bandwidth (kept until rtt_maximum)
		PacketQueue pendingAckQueue;		// sent packets which have not been acked yet (kept until rtt_maximum * 2 )
		PacketQueue receivedQueue;			// received packets for determining acks to send (kept up to most recent recv sequence - 32)
		PacketQueue ackedQueue;				// acked packets (kept until rtt_maximum * 2)
	};

	// connection with reliability (seq/ack)

	class ReliableConnection : public Connection
	{
	public:

		ReliableConnection(unsigned int protocolId, float timeout, unsigned int max_sequence = 0xFFFFFFFF)
			: Connection(protocolId, timeout), reliabilitySystem(max_sequence)
		{
			ClearData();
#ifdef NET_UNIT_TEST
			packet_loss_mask = 0;
#endif
		}

		~ReliableConnection()
		{
			if (IsRunning())
				Stop();
		}

		// overriden functions from "Connection"

		bool SendPacket(const unsigned char data[], int size)
		{
#ifdef NET_UNIT_TEST
			if (reliabilitySystem.GetLocalSequence() & packet_loss_mask)
			{
				reliabilitySystem.PacketSent(size);
				return true;
			}
#endif
			const int header = 12;		// Size of the header, including sequence, acknowledgment, and acknowledgment bits
			unsigned char packet[header + PacketSizeHack];	// Buffer for the complete packet

			// Retrieve sequence, acknowledgment, and acknowledgment bits from the reliability system
			unsigned int seq = reliabilitySystem.GetLocalSequence();
			unsigned int ack = reliabilitySystem.GetRemoteSequence();
			unsigned int ack_bits = reliabilitySystem.GenerateAckBits();

			// Write the header into the packet buffer
			WriteHeader(packet, seq, ack, ack_bits);

			// Copy the actual data into the packet buffer after the header
			std::memcpy(packet + header, data, size);

			// Use the base class's SendPacket to send the constructed packet over the network
			if (!Connection::SendPacket(packet, size + header))
				return false;

			// Update the reliability system to track the sent packet
			reliabilitySystem.PacketSent(size);
			return true;
		}

		int ReceivePacket(unsigned char data[], int size)
		{
			const int header = 12;

			if (size <= header)
				return false;

			unsigned char packet[header + PacketSizeHack];


			int received_bytes = Connection::ReceivePacket(packet, size + header);

			// Check if no bytes were received
			if (received_bytes == 0)
				return false;

			// Check if the received bytes are less than or equal to the header size
			if (received_bytes <= header)
				return false;

			unsigned int packet_sequence = 0;
			unsigned int packet_ack = 0;
			unsigned int packet_ack_bits = 0;

			// Read the header to extract sequence, acknowledgment, and acknowledgment bits
			ReadHeader(packet, packet_sequence, packet_ack, packet_ack_bits);

			// Notify the reliability system of the received packet and process acknowledgment information
			reliabilitySystem.PacketReceived(packet_sequence, received_bytes - header);
			reliabilitySystem.ProcessAck(packet_ack, packet_ack_bits);
			// Copy the payload data from the received packet into the provided data buffer
			std::memcpy(data, packet + header, received_bytes - header);

			return received_bytes - header;
		}

		void Update(float deltaTime)
		{
			Connection::Update(deltaTime);
			reliabilitySystem.Update(deltaTime);
		}

		int GetHeaderSize() const
		{
			return Connection::GetHeaderSize() + reliabilitySystem.GetHeaderSize();
		}

		ReliabilitySystem& GetReliabilitySystem()
		{
			return reliabilitySystem;
		}

		// unit test controls

#ifdef NET_UNIT_TEST
		void SetPacketLossMask(unsigned int mask)
		{
			packet_loss_mask = mask;
		}
#endif

	protected:

		void WriteInteger(unsigned char* data, unsigned int value)
		{
			data[0] = (unsigned char)(value >> 24);
			data[1] = (unsigned char)((value >> 16) & 0xFF);
			data[2] = (unsigned char)((value >> 8) & 0xFF);
			data[3] = (unsigned char)(value & 0xFF);
		}

		void WriteHeader(unsigned char* header, unsigned int sequence, unsigned int ack, unsigned int ack_bits)
		{
			WriteInteger(header, sequence);
			WriteInteger(header + 4, ack);
			WriteInteger(header + 8, ack_bits);
		}

		void ReadInteger(const unsigned char* data, unsigned int& value)
		{
			value = (((unsigned int)data[0] << 24) | ((unsigned int)data[1] << 16) |
				((unsigned int)data[2] << 8) | ((unsigned int)data[3]));
		}

		void ReadHeader(const unsigned char* header, unsigned int& sequence, unsigned int& ack, unsigned int& ack_bits)
		{
			ReadInteger(header, sequence);
			ReadInteger(header + 4, ack);
			ReadInteger(header + 8, ack_bits);
		}

		virtual void OnStop()
		{
			ClearData();
		}

		virtual void OnDisconnect()
		{
			ClearData();
		}

	private:

		void ClearData()
		{
			reliabilitySystem.Reset();
		}

#ifdef NET_UNIT_TEST
		unsigned int packet_loss_mask;			// mask sequence number, if non-zero, drop packet - for unit test only
#endif

		ReliabilitySystem reliabilitySystem;	// reliability system: manages sequence numbers and acks, tracks network stats etc.
	};
}

#endif
