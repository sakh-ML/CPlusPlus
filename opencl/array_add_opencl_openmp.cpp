/*
* Dieses Programm demonstriert die parallele Addition zweier großer
* Ganzzahl-Arrays mithilfe von OpenCL und OpenMP. 
* 
* Es sucht zuerst nach verfügbaren OpenCL-Plattformen und -Geräten,
* kompiliert einen einfachen Kernel, der zwei Eingabearrays elementweise
* addiert, und führt diesen Kernel auf dem OpenCL-Gerät aus.
* 
* Anschließend wird die gleiche Addition mit OpenMP parallel auf der CPU
* durchgeführt, um die Korrektheit der OpenCL-Ergebnisse zu überprüfen.
* Die Laufzeiten beider Methoden werden gemessen und ausgegeben.
* 
* Ziel ist der Vergleich von GPU-beschleunigter Berechnung (OpenCL) mit
* CPU-Parallelisierung (OpenMP) bezüglich Leistung und Ergebnisgenauigkeit.
*/


#include <omp.h>
#include <CL/cl.hpp>
#include <iostream>

#define PROBLEM_SIZE 0x40000000UL

int main() {
    // Search the compute platform first
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (cl::Platform &p : platforms) {
        std::cout << "Platform: " << p.getInfo<CL_PLATFORM_NAME>() << std::endl;
    }
    // Choose the default platform
    cl::Platform myplat = platforms[0];

    // Search all compute devices in the platform
    std::vector<cl::Device> devices;
    myplat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    for (cl::Device &d : devices) {
        std::cout << "------" << std::endl;
        std::cout << d.getInfo<CL_DEVICE_NAME>() << std::endl;
        std::cout << "Max supported work item sizes per dimension: (";
        for (int i = 0; i < d.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>(); i++)
            std::cout << d.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[i] << ",";
        std::cout << ")" << std::endl;
    }

    // Choose the default compute device
    cl::Device mydevice = devices[0];

    // Create a program context and a simple OpenCL source
    cl::Context context({mydevice});
    cl::Program::Sources sources;

    std::string simple_kernel =
        "void kernel adder(global const int*A, global const int *B, global int "
        "*C){"
        "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];"
        "}";
    sources.push_back({simple_kernel.c_str(), simple_kernel.length()});
    cl::Program program(context, sources);

    // Compile the program
    if (program.build({mydevice}) != CL_SUCCESS) {
        std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(mydevice) << std::endl;
        exit(1);
    }
    std::cout << "Sucessfully build the kernel" << std::endl;

    // Create input and output buffers
    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * PROBLEM_SIZE);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * PROBLEM_SIZE);
    cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * PROBLEM_SIZE);

    // Create local arrays for the inputs
    int *adata = new int[PROBLEM_SIZE];
    int *bdata = new int[PROBLEM_SIZE];

    std::cout << "Create test data" << std::endl;
    // Fill the arrays with identic values
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        adata[i] = i;
        bdata[i] = i;
    }

    cl::CommandQueue queue(context, mydevice);

    std::cout << "Copy data into buffers" << std::endl;
    // Trigger a copy of the local buffers to the opencl buffers
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * PROBLEM_SIZE, adata);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * PROBLEM_SIZE, bdata);

    // Create a kernel to call the "adder" function
    cl::Kernel kernel(program, "adder");
    kernel.setArg(0, buffer_A);
    kernel.setArg(1, buffer_B);
    kernel.setArg(2, buffer_C);

    // get maximum work group size for kernel
    std::size_t wgsize;
    kernel.getWorkGroupInfo(mydevice, CL_KERNEL_WORK_GROUP_SIZE, &wgsize);
    std::cout << "Max work group size for kernel: " << wgsize << std::endl;

    // Set the execution range for the kernel
    cl::NDRange global(PROBLEM_SIZE);
    // get maximum work item size
    int wisize = std::min(mydevice.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[2], wgsize);
    std::cout << "Max work item size for kernel: " << wisize << std::endl;

    // cl::NDRange local(1);
    cl::NDRange local(wisize);

    std::cout << "Starting " << mydevice.getInfo<CL_DEVICE_NAME>() << " version" << std::endl;

    queue.finish();
    double before = omp_get_wtime();
    // Execute the kernel and measure the time
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);
    queue.finish();
    double end = omp_get_wtime();

    std::cout << "OpenCL version took " << (end - before) * 1000000 << "µs" << std::endl;

    // Create a local buffer for the output and copy data from the opencl system
    int *cdata = new int[PROBLEM_SIZE];
    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * PROBLEM_SIZE, cdata);

    // Perform CPU computation to validate the result
    int *correctcdata = new int[PROBLEM_SIZE];

    before = omp_get_wtime();
#pragma omp parallel for
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        correctcdata[i] = adata[i] + bdata[i];
    }
    end = omp_get_wtime();
    std::cout << "OpenMP version took " << (end - before) * 1000000 << "µs"
              << std::endl;

    bool ok = true;
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        if (correctcdata[i] != cdata[i]) {
            std::cout << "The result is wrong at index " << i << ", expected "
                      << correctcdata[i] << " but found " << cdata[i] << std::endl;
            ok = false;
            break;
        }
    }
    if (ok) std::cout << "Results are consistent" << std::endl;
}

