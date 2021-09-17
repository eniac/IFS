#include <iostream>
#include <fstream>
#include <random>

#include "pifo_pipeline_stage.h"
#include "pifo_pipeline.h"

int main() {
  //try {
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<int> ele_dis(.1);
    // Traffic generation
    PIFOPacket test_packet;

   
std::vector<PIFOArguments> targetPublic = {{1, QueueType::PRIORITY_QUEUE, 0}};
std::vector<PIFOArguments> targetPrivate = {{1, QueueType::PRIORITY_QUEUE, 1}};

PIFOPipelineStage privacyPriority(1, "privacy", {{0, {Operation::DEQ, targetPublic}}, {1, {Operation::DEQ, targetPrivate}}}, [] (const auto & x) { return x("privacy"); });



std::vector<PIFOArguments> targetFirst_Private={{2,QueueType::PRIORITY_QUEUE, 0}};
std::vector<PIFOArguments> targetSecond_Private={{2,QueueType::PRIORITY_QUEUE, 1}};
std::vector<PIFOArguments> targetThird_Private={{2,QueueType::PRIORITY_QUEUE, 2}};
std::vector<PIFOArguments> targetFourth_Private={{2,QueueType::PRIORITY_QUEUE, 3}};
PIFOPipelineStage roundRobin(2, "class", 
{{1, {Operation::DEQ,targetFirst_Private}},
{2, {Operation::DEQ,targetSecond_Private}},
{3, {Operation::DEQ,targetThird_Private}},
{4, {Operation::DEQ,targetThird_Private}},
{5, {Operation::TRANSMIT, {}}},
{6, {Operation::TRANSMIT, {}}},
{7, {Operation::TRANSMIT, {}}},
{8, {Operation::TRANSMIT, {}}}},
[] (const auto & x) {
if (x("privacy")==0){
	return x("time");
}
if (x("privacy")==1){
	static std::map<uint32_t, uint32_t> private_last_fin_time = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
	auto ret = private_last_fin_time.at(static_cast<uint32_t>(x("class")));
	private_last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1;
	return ret;
}
uint32_t y =0; return y;});

PIFOPipelineStage dummyPriority(4, "class", {{1, {Operation::TRANSMIT, {}}},{2, {Operation::TRANSMIT, {}}},{3, {Operation::TRANSMIT, {}}},{4, {Operation::TRANSMIT, {}}}},
[] (const auto & x) {return x("isDummy");});

PIFOPipeline PFS_pipeline({privacyPriority, roundRobin, dummyPriority});
PIFOPipeline mesh=PFS_pipeline;


 // Give every private client a good amount of dummies
for (uint32_t i = 0; i < 30000; i++){
	test_packet("class") = 1;
	test_packet("time") = 0;
	test_packet("ptr") = 666;
	test_packet("isDummy") = 1;
	test_packet("privacy") = 1;
	mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);
	mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, 0);
	mesh.enq(2, QueueType::PRIORITY_QUEUE, 0, test_packet,0);
	
	test_packet("class") = 2;
	test_packet("time") = 0;
	test_packet("ptr") = 666;
	test_packet("isDummy") = 1;
	test_packet("privacy") = 1;
	mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);
	mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, 0);
	mesh.enq(2, QueueType::PRIORITY_QUEUE, 1, test_packet,0);

	test_packet("class") = 3;
	test_packet("time") = 0;
	test_packet("ptr") = 666;
	test_packet("isDummy") = 1;
	test_packet("privacy") = 1;
	mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);
	mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, 0);
	mesh.enq(2, QueueType::PRIORITY_QUEUE, 2, test_packet,0);
}


std::ifstream input( "load.in" );

int slot, packet_source, packet_size;
int finished = 0;
int emptied = 0;
int overflow = 0;
int capacity = 10500;

int padded_size = 1518;


if(input >> slot >> packet_source >> packet_size){
	finished = 0;
}
else{
	finished = 1;
}

int c;
uint32_t i = 0;


while ((i < 500000) && (!finished || !emptied)){
	std::cout << i << std::endl;
	emptied = 0;
	c = capacity - overflow;
	
	while(i == slot && !finished){
		int j = packet_source;

		if(j > 1 && j < 5){
			j = j - 1;

			test_packet("class") = j;
			test_packet("time") = (int) i;
			test_packet("ptr") = 666;
			test_packet("isDummy") = 0;
			test_packet("privacy") = 1;
			test_packet("size") = packet_size;
			mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
			mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, i);
			mesh.enq(2, QueueType::PRIORITY_QUEUE, uint32_t(j - 1), test_packet, i);
		}
		else{
			j = (j % 3) + 4;
			test_packet("class") = j;
			test_packet("time") = (int) i;
			test_packet("ptr") = 666;
			test_packet("isDummy") = 0;
			test_packet("privacy") = 0;
			test_packet("size") = packet_size;
			mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
			mesh.enq(1, QueueType::PRIORITY_QUEUE, 0, test_packet, i);

		}

		if(input >> slot >> packet_source >> packet_size){
			finished = 0;
		}
		else{
			finished = 1;
		}
	}


	//dequeue
	while(!emptied && c > 0){
		auto result = mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, i);
		if(result.initialized()){
			if((int) (result.get()("privacy")) == 0){
				c -= (int) (result.get()("size"));
			}
			else{
				c -= padded_size;
			}
			
			if(c >= 0){
				std::cout << "tick: " << i << ", " << result << std::endl;
				overflow = 0;
			}
			else{
				std::cout << "tick: " << i+1 << ", " << result << std::endl;
				overflow = -c;
			}
			
		}
		else{
			emptied = 1;
		}
	}
	i ++;
  }



 // } catch (const std::exception & e) {
//    std::cerr << "Caught exception in main " << std::endl << e.what() << std::endl;
 //   return EXIT_FAILURE;
//  }
}
