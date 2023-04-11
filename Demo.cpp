//Signature Space
#include "Tester.h"

//TODO accept CL arguments
int main() {
	
	Tester demo("../sample_images/test.jpg");	
	demo.display_img(0);	

	//run omp
	omp_set_num_threads(15);
	demo.omp_brighten(50);
	demo.omp_sharpen();
	demo.omp_saturate(2.0);
	//disables OpenMP for further runs
	omp_set_num_threads(1);
	omp_set_dynamic(0);
	

	//run TBB/DAAL

	//run serial
	cv::setNumThreads(0);	//turn all parallelization of the backend off
	demo.serial_brighten(50);
	demo.serial_sharpen();
	demo.serial_saturate(2.0);

	demo.display_img(1);
}
