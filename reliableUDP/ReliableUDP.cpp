/*
	Reliability and Flow Control Example
	From "Networking for Game Programmers" - http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/
#include "TransferProtocol.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#include "Net.h"
#include "HashGen.h"
#include "TransferProtocol.h"

//#define SHOW_ACKS

#pragma warning(suppress : 4996)



using namespace std;
using namespace net;

const int ServerPort = 30000;
const int ClientPort = 30001;
const int ProtocolId = 0x11223344;
const float DeltaTime = 1.0f / 30.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;

class FlowControl
{
public:

	FlowControl()
	{
		printf("flow control initialized\n");
		Reset();
	}

	void Reset()
	{
		mode = Bad;
		penalty_time = 4.0f;
		good_conditions_time = 0.0f;
		penalty_reduction_accumulator = 0.0f;
	}

	void Update(float deltaTime, float rtt)
	{
		const float RTT_Threshold = 250.0f;

		if (mode == Good)
		{
			if (rtt > RTT_Threshold)
			{
				printf("*** dropping to bad mode ***\n");
				mode = Bad;
				if (good_conditions_time < 10.0f && penalty_time < 60.0f)
				{
					penalty_time *= 2.0f;
					if (penalty_time > 60.0f)
						penalty_time = 60.0f;
					printf("penalty time increased to %.1f\n", penalty_time);
				}
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				return;
			}

			good_conditions_time += deltaTime;
			penalty_reduction_accumulator += deltaTime;

			if (penalty_reduction_accumulator > 10.0f && penalty_time > 1.0f)
			{
				penalty_time /= 2.0f;
				if (penalty_time < 1.0f)
					penalty_time = 1.0f;
				printf("penalty time reduced to %.1f\n", penalty_time);
				penalty_reduction_accumulator = 0.0f;
			}
		}

		if (mode == Bad)
		{
			if (rtt <= RTT_Threshold)
				good_conditions_time += deltaTime;
			else
				good_conditions_time = 0.0f;

			if (good_conditions_time > penalty_time)
			{
				printf("*** upgrading to good mode ***\n");
				good_conditions_time = 0.0f;
				penalty_reduction_accumulator = 0.0f;
				mode = Good;
				return;
			}
		}
	}

	float GetSendRate()
	{
		return mode == Good ? 30.0f : 10.0f;
	}

private:

	enum Mode
	{
		Good,
		Bad
	};

	Mode mode;
	float penalty_time;
	float good_conditions_time;
	float penalty_reduction_accumulator;
};

// ----------------------------------------------

int main(int argc, char* argv[])
{
	// parse command line

	//ADDITIONAL COMMAND LINE ARGS REQUIRED

	//filename

	//bitfield struct

	//File mode enum


	enum Mode
	{
		Client,
		Server
	};


	HashGen hashGen;
	FileInfo fileInfo;
	Mode mode = Server;
	Address address;

	if (argc >= 2) //the ip - filename
	{
		int a, b, c, d;

#pragma warning(suppress : 4996)
		if (sscanf(argv[1], "%d.%d.%d.%d", &a, &b, &c, &d))
		{
			mode = Client;
			address = Address(a, b, c, d, ServerPort);
		}

		sscanf_s(argv[2], "%s", fileInfo.fileName, MAX_FILE_NAME_CHARS);
		//Scan in file size and set file type here...
	}

	// initialize

	if (!InitializeSockets())
	{
		printf("failed to initialize sockets\n");
		return 1;
	}

	ReliableConnection connection(ProtocolId, TimeOut);

	const int port = mode == Server ? ServerPort : ClientPort;

	if (!connection.Start(port))
	{
		printf("could not start connection on port %d\n", port);
		return 1;
	}

	if (mode == Client)
		connection.Connect(address);



	else
		connection.Listen();


	//Get the filesize
	//Determine how many packets will be required based on file size, and data payload / per packet

	//Generate the whole-file hash

	//Populate FileInfo struct with required metadata for transfer protocol (filename, filesize, hash)

	//open input stream
	ifstream in((char*)fileInfo.fileName, ios::binary |ios::ate);
	size_t bytesToRead = in.tellg();
	in.close();

	ifstream inputFile((char*)fileInfo.fileName, ios::binary);


	FILE* file = NULL;
	ofstream outFile;


	if(mode == Client)
	{
		hashGen.hashValue((char*)fileInfo.fileName);
		const char* hash = hashGen.getHash();

		cout << "Sent Hash: " << hash << endl;
	}



/*	size_t bytesToRead ;
	bytesToRead = inputFile.gcount(); */// Actual bytes read, important for handling the last chunk


	if(mode == Server)
	{
		outFile = ofstream((char*)fileInfo.fileName, ios::binary);
	}

	bool connected = false;
	float sendAccumulator = 0.0f;
	float statsAccumulator = 0.0f;

	FlowControl flowControl;

	while (true)
	{
		// update flow control

		if (connection.IsConnected())
			flowControl.Update(DeltaTime, connection.GetReliabilitySystem().GetRoundTripTime() * 1000.0f);

		const float sendRate = flowControl.GetSendRate();

		// detect changes in connection state

		if (mode == Server && connected && !connection.IsConnected())
		{
			flowControl.Reset();
			printf("reset flow control\n");
			connected = false;
		}

		if (!connected && connection.IsConnected())
		{
			printf("client connected to server\n");
			connected = true;
		}

		if (!connected && connection.ConnectFailed())
		{
			printf("connection failed\n");
			break;
		}

		// send and receive packets

		sendAccumulator += DeltaTime;




		while (sendAccumulator > 1.0f / sendRate)
		{
			unsigned char packet[PacketSize];
			memset(packet, 0, sizeof(packet));

			//If mode == Server
				//If transfer complete and hash is checked
					//Inform user of success or failure


			if (mode == Client)
			{
				
				inputFile.read(reinterpret_cast<char*>(packet), PacketSize);



				//while (bytesToRead > PacketSize)
				//{
				//	inputFile.read(reinterpret_cast<char*>(packet), PacketSize);
				//	bytesToRead -= PacketSize;
				//}


				//inputFile.read(reinterpret_cast<char*>(packet), PacketSize);

				//printf("%llu", bytesToRead);

				//printf("%llu", bytesToRead);
				//printf("%s", packet);


				//if(inputFile.eof())
				//{
				//	break;
				//}



				//while(bytesRead >= 0)
				//{
				//	inputFile.read(reinterpret_cast<char*>(packet), PacketSize);
				//}


				//while(bytesToRead >= 0)
				//{
				//	inputFile.read(reinterpret_cast<char*>(packet), PacketSize);
				//	bytesToRead -= PacketSize;
				//}






				//if (clientState.errorState == true)	//if an error state was set when receiving/parsing the last message
				//{
				//	//send error message
				//	return 0;
				//}
				////create transfer request
				//else if (clientState.requestSent == false)
				//{
				//	if (!createRequestPacket(packetToSend, fileInfo.fileName))
				//	{
				//		clientState.errorState = true; //error packet will be created next send loop
				//	}
				//	clientState.requestSent = true;
				//}
				////if request was acknowledged and not sending data, send first data packet
				//else if (clientState.ackReceived == true && clientState.sendingData == false)
				//{
				//	//initialize data sending process
				//	//send first packet
				//	clientState.sendingData = true;

				//	//Load packet
				//		//Header 
				//			//Filename
				//			//Filesize
				//		//Payload
				//		//Hash
				//}
				////if last packet hasn't been sent, continue sending data
				//else if (clientState.sendingData == true && clientState.lastPacketSent == false)
				//{
				//	if (packetStatus == DATAPACKET_FAILURE)
				//	{
				//		clientState.errorState = true; //error packet will be created next send loop
				//		return 0;
				//	}
				//	else if (packetStatus == LAST_PACKET) //flip lastPacketSent based on size of packet (i.e. less than 210 bytes)
				//	{
				//		clientState.lastPacketSent = true;
				//	}
				//}
				////if last packet has been sent, send hash
				//else if (clientState.lastPacketSent == true && clientState.hashSent == false)
				//{
				//	if (!createHashPacket(packetToSend, fileInfo.fileName))
				//	{
				//		clientState.errorState = true; //error packet will be created next send loop
				//	}
				//	else
				//	{
				//		clientState.hashSent = true;
				//	}
				//	return 0;
				//}
			}
			if (mode == Server)
			{


				//if (serverState.errorState == true)	//if an error state was set when receiving/parsing the last message
				//{
				//	//send error message
				//	return 0;
				//}
				//else if (serverState.requestReceived == false || serverState.confirmationSent == true) //if request not received, or transfer is done, keep waiting
				//{
				//	//wait
				//	continue;
				//}
				//else if (serverState.requestReceived == true && serverState.requestAckSent == false)
				//{
				//	//send request ack
				//	createAckResponsePacket(packetToSend, fileInfo.transferID);
				//	serverState.requestAckSent = true;
				//}
				//else if (serverState.hashReceived == true && serverState.confirmationSent == false)
				//{
				//	//send confirmation
				//	createTransferConfirmationPacket(packetToSend, fileInfo.transferSpeed);
				//	serverState.confirmationSent = true;
				//}
			}


			connection.SendPacket(packet, sizeof(packet));
			sendAccumulator -= 1.0f / sendRate;
		}

		while (true)
		{
			unsigned char packet[256];
			int bytes_read = connection.ReceivePacket(packet, sizeof(packet));
			if (bytes_read == 0)
				break;



			outFile.write(reinterpret_cast<const char*>(packet), PacketSize);


			//while(bytesToRead > 0)
			//{
			//	outFile.write(reinterpret_cast<const char*>(packet), PacketSize);

			//	bytesToRead -= PacketSize;
			//}

			//printf("%s", packet);

			//for(int i = 0; i < 256; i++)
			//{
			//	fprintf((FILE*)outFile, (char*)packet[i]);
			//}

			//std::vector<unsigned char> received_packets;

			//// Open the output file
			//std::ofstream outfile("new.txt", std::ios::binary);

			//for (const auto& packet : received_packets) {
			//	outfile.write(reinterpret_cast<const char*>(&packet), sizeof(packet));
			//}

			//call to message router method to parse header




			//If mode == Server
				//If first packet
					// Create file to write to

				//Parse out information
					//Write payload to file

				//If transfer complete
					//Close file 
					//Conduct integrity check

			//If mode == Client
				//Send message to reliableUI informing client of success/failure
		}

		// show packets that were acked this frame

#ifdef SHOW_ACKS
		unsigned int* acks = NULL;
		int ack_count = 0;
		connection.GetReliabilitySystem().GetAcks(&acks, ack_count);
		if (ack_count > 0)
		{
			printf("acks: %d", acks[0]);
			for (int i = 1; i < ack_count; ++i)
				printf(",%d", acks[i]);
			printf("\n");
		}
#endif

		// update connection

		connection.Update(DeltaTime);

		// show connection stats

		statsAccumulator += DeltaTime;

		while (statsAccumulator >= 0.25f && connection.IsConnected())
		{
			float rtt = connection.GetReliabilitySystem().GetRoundTripTime();

			unsigned int sent_packets = connection.GetReliabilitySystem().GetSentPackets();
			unsigned int acked_packets = connection.GetReliabilitySystem().GetAckedPackets();
			unsigned int lost_packets = connection.GetReliabilitySystem().GetLostPackets();

			float sent_bandwidth = connection.GetReliabilitySystem().GetSentBandwidth();
			float acked_bandwidth = connection.GetReliabilitySystem().GetAckedBandwidth();

			printf("rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n",
				rtt * 1000.0f, sent_packets, acked_packets, lost_packets,
				sent_packets > 0.0f ? (float)lost_packets / (float)sent_packets * 100.0f : 0.0f,
				sent_bandwidth, acked_bandwidth);

			statsAccumulator -= 0.25f;
		}

		net::wait(DeltaTime);
	}

	ShutdownSockets();

	return 0;
}

