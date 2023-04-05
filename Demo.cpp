//Signature Space
#include "Tester.h"

//TODO accept CL arguments
int main() {

	Tester demo("../sample_images/field.jpg");
	//show starter images
	//do omp run
	//show times and altered images
	//do DAAL run
	//show times and altered images

	demo.omp_brighten(50);
	demo.omp_sharpen();
}
