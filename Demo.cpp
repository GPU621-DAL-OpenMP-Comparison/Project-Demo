//Signature Space
#include "Tester.h"

//TODO accept CL arguments
int main() {

	//Tester demo("../sample_images/field.jpg");
	//D:\School_Win23\GPU625\Project\Project-Demo\Project-Demo\sample_images
	//Tester demo("D:/School_Win23/GPU625/Project/Project-Demo/Project-Demo/sample_images/field.jpg");
	
	Tester demo("test.jpg");
	//show starter images
	//do omp run
	//show times and altered images
	//do DAAL run
	//show times and altered images
	demo.display_img(0);

	demo.omp_brighten(50);
	demo.omp_sharpen();
	demo.omp_saturate(2.0);
}
