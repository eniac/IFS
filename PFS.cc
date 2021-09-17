#include <iostream>
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
	static std::map<uint32_t, uint32_t> private_last_fin_time = {{5, 0}, {6, 0}, {7, 0}, {8, 0}};
	auto ret = private_last_fin_time.at(static_cast<uint32_t>(x("class")));
	private_last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1;
	return ret;
}
if (x("privacy")==1){
	static std::map<uint32_t, uint32_t> public_last_fin_time = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
	auto ret = public_last_fin_time.at(static_cast<uint32_t>(x("class")));
	public_last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1;
	return ret;
}
uint32_t y =0; return y;});

PIFOPipelineStage dummyPriority(4, "class", {{1, {Operation::TRANSMIT, {}}},{2, {Operation::TRANSMIT, {}}},{3, {Operation::TRANSMIT, {}}},{4, {Operation::TRANSMIT, {}}}},
[] (const auto & x) {return x("isDummy");});

PIFOPipeline PFS_pipeline({privacyPriority, roundRobin, dummyPriority});
PIFOPipeline mesh=PFS_pipeline;


 // Give every private client a good amount of dummies
for (uint32_t i = 0; i < 500; i++){
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

	test_packet("class") = 4;
	test_packet("time") = 0;
	test_packet("ptr") = 666;
	test_packet("isDummy") = 1;
	test_packet("privacy") = 1;
	mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);
	mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, 0);
	mesh.enq(2, QueueType::PRIORITY_QUEUE, 3, test_packet,0);
}



for (uint32_t i = 0; i < 500; i++){
	// enqueue victim packets
	for(int j = 1; j < 1; j++){
		for(int k = 0; k < ele_dis(gen); k++){
			test_packet("class") = j;
			test_packet("time") = (int) i;
			test_packet("ptr") = 666;
			test_packet("isDummy") = 0;
			test_packet("privacy") = 1;
			mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
			mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, i);
			mesh.enq(2, QueueType::PRIORITY_QUEUE, uint32_t(j - 1), test_packet, i);

		}
	}
	
	for(int j = 5; j < 7; j++){
		for(int k = 0; k < ele_dis(gen); k++){
			test_packet("class") = j;
			test_packet("time") = (int) i;
			test_packet("ptr") = 666;
			test_packet("isDummy") = 0;
			test_packet("privacy") = 0;
			mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
			mesh.enq(1, QueueType::PRIORITY_QUEUE, 0, test_packet, i);

		}
	}


	//dequeue
	auto result = mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, i);
	std::cout << "tick: " << i << ", " << result << std::endl;
}

	





 // } catch (const std::exception & e) {
//    std::cerr << "Caught exception in main " << std::endl << e.what() << std::endl;
 //   return EXIT_FAILURE;
//  }
}
