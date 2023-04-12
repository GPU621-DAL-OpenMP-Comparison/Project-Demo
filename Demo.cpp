//Signature Space
#include "Tester.h"

//argument is ../sample_images/test.jpg
int main(int argc, char* argv[]) {
	
	Tester demo(argv[1]);	
	demo.display_img(0);	

	//run omp
	omp_set_num_threads(15);
	demo.omp_brighten(50);
	demo.omp_sharpen();
	demo.omp_saturate(2.0);
	//disable OpenMP so it can't be incidently utilized in the backend
	omp_set_num_threads(1);
	omp_set_dynamic(0);
	
	std::cout << "\n";

	//run ipp
	demo.ipp_brighten(50);
	demo.ipp_sharpen();
	demo.ipp_saturate();

	std::cout << "\n";

	//run tbb
	demo.tbb_brighten(50);
	demo.tbb_sharpen();
	demo.tbb_saturate(2.0);
	
	std::cout << "\n";

	//run serial
	cv::setNumThreads(0);	//turn all parallelization of the backend off
	demo.serial_brighten(50);
	demo.serial_sharpen();
	demo.serial_saturate(2.0);

	return 0;
}
