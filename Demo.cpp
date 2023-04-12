//Signature Space
#include "Tester.h"

//argument is ../sample_images/test.jpg
int main(int argc, char* argv[]) {
	
	Tester demo(argv[1]);	
	demo.display_img(0);	

	//run omp
	//omp_set_num_threads(15); //Olivia- 15 was opt choice for my system
	demo.omp_brighten(50);
	demo.omp_sharpen();
	demo.omp_saturate(2.0);
	//disable OpenMP so it can't be incidently used in the backend
	omp_set_num_threads(1);
	omp_set_dynamic(0);
	
	//run ipp
	demo.ipp_brighten(50);
	demo.ipp_sharpen();
	demo.ipp_saturate();

	//run serial
	cv::setNumThreads(0);	//turn all parallelization of the backend off
	demo.serial_brighten(50);
	demo.serial_sharpen();
	demo.serial_saturate(2.0);

	return 0;
}
