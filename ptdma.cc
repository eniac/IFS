#include <iostream>
#include <random>

#include "pifo_pipeline_stage.h"
#include "pifo_pipeline.h"

int main() {
  try {
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<int> ele_dis(.4);

    // Traffic generation
    PIFOPacket test_packet;

    // Single PIFO pipeline stage consisting of
    // 1 priority and queue
    // (every PIFO pipeline stage has 1 calendar queue)
std::vector<PIFOArguments> targetFirst_Private={{1,QueueType::PRIORITY_QUEUE,0}};
std::vector<PIFOArguments> targetSecond_Private={{1,QueueType::PRIORITY_QUEUE,1}};
PIFOPipelineStage pifo1(1, "class", {{1, {Operation::DEQ,targetFirst_Private}},
				{2, {Operation::DEQ,targetSecond_Private}},
				},
[] (const auto & x) {if (x("ptr")==666){
	static std::map<uint32_t, uint32_t> last_fin_time = {{1, 0}, {2, 0}};
	auto ret = last_fin_time.at(static_cast<uint32_t>(x("class")));
	last_fin_time.at(static_cast<uint32_t>(x("class"))) += 1;
	return ret;}
uint32_t y =0; return y;});

PIFOPipelineStage pifo2(2, "class", {{1, {Operation::TRANSMIT, {}}},{2, {Operation::TRANSMIT, {}}},},
[] (const auto & x) {
	if (x("class")==1){return static_cast<uint32_t>(x("isDummy"));}
	if (x("class")==2){return static_cast<uint32_t>(x("isDummy"));}
uint32_t y =0; return y;});
PIFOPipeline PTDMA_pipeline({pifo1,pifo2,});
PIFOPipeline mesh=PTDMA_pipeline;

// Give every client a good amount of dummies
for (uint32_t i = 0; i < 1000; i++) {
	test_packet("class") = 1;
	test_packet("time") = 0;
	test_packet("ptr") = 666;
	test_packet("isDummy") = 1;
	mesh.enq(1, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);
	mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);

	test_packet("class") = 2;
	test_packet("time") = 0;
	test_packet("ptr") = 666;
	test_packet("isDummy") = 1;
	mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, 0);
	mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, 0);
}

for (uint32_t i = 0; i < 1000; i++){
	// enqueue victim packets
	for(uint32_t j = 0; j < ele_dis(gen); j++){
		test_packet("class") = 1;
		test_packet("time") = (int) i;
		test_packet("ptr") = 666;
		test_packet("isDummy") = 0;
		mesh.enq(1, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
		mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
	}
	

	// enqueue ping packets
	if(i % 5 == 0){
		test_packet("class") = 2;
		test_packet("time") = (int) i;
		test_packet("ptr") = 666;
		test_packet("isDummy") = 0;
		mesh.enq(1, QueueType::PRIORITY_QUEUE, 1, test_packet, i);
		mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
	}

	//dequeue
	auto result = mesh.deq(0, QueueType::PRIORITY_QUEUE, 0, i);
	std::cout << "tick: " << i << ", " << result << std::endl;
}


  } catch (const std::exception & e) {
    std::cerr << "Caught exception in main " << std::endl << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
