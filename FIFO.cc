#include <iostream>
#include <random>

#include "pifo_pipeline_stage.h"
#include "pifo_pipeline.h"

int main() {
  try {
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ele_dis(1, 20);

    // Traffic generation
    PIFOPacket test_packet;

    // Single PIFO pipeline stage consisting of
    // 1 priority and queue
    // (every PIFO pipeline stage has 1 calendar queue)
PIFOPipelineStage pifo1(1,
"ptr",
{{666, {Operation::TRANSMIT, {}}},},
[] (const auto & x) {if (x("ptr")==666){return x("time");}
int y = 0; return y;});
PIFOPipeline FIFO_pipeline({pifo1,});
PIFOPipeline mesh=FIFO_pipeline;

for (uint32_t i = 0; i < 500; i++) {
	// enqueue victim packets
	if(ele_dis(gen) == 1){
		for(uint32_t j = 0; j < 5; j++){
			test_packet("class") = 1;
			test_packet("time") = (int) i;
			test_packet("ptr") = 666;
			mesh.enq(0, QueueType::PRIORITY_QUEUE, 0, test_packet, i);
		}
	}

	// enqueue ping packets
	if(i % 4 == 0){
		test_packet("class") = 2;
		test_packet("time") = (int) i;
		test_packet("ptr") = 666;
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
