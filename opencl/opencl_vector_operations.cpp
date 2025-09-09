/*
Dieses Programm demonstriert die Nutzung von OpenCL zur parallelen
Berechnung zweier Vektoroperationen auf großen Datenmengen.
Es initialisiert drei Eingabevektoren, lädt sie in den GPU-Speicher
und führt zwei verschiedene Rechenkerne (Kernels) aus, die jeweils
Elementweise Operationen auf den Vektoren ausführen.

Zusätzlich wird die gleiche Berechnung auf der CPU mit OpenMP
parallelisiert, um die Ergebnisse zu validieren und die Performance
zwischen CPU- und GPU-Berechnung zu vergleichen.

Die Ausgabe bestätigt, ob die Ergebnisse der GPU und CPU übereinstimmen.
*/


#include <omp.h>
#include <cmath>
#include <CL/opencl.hpp>
#include <iostream>

#define PROBLEM_SIZE 0x1000000UL


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

    std::string erste_kernel =
        "void kernel adder1(global const float* W1, global const float* W2, global const float* W3,global float"
        "*O1){"
               "O1[get_global_id(0)]=(W1[get_global_id(0)] + W2[get_global_id(0)]+ W3[get_global_id(0)]) * (W1[get_global_id(0)] - W2[get_global_id(0)] - W3[get_global_id(0)]);"
        "}";
    
    std::string zweite_kernel =
        "void kernel adder2(global const float* W1, global const float* W2, global const float* W3,global float"
        "*O2){"
               "O2[get_global_id(0)]= floor(W1[get_global_id(0)]) * (W1[get_global_id(0)] - floor(W1[get_global_id(0)])) + floor(W2[get_global_id(0)]) * (W2[get_global_id(0)] - floor(W2[get_global_id(0)])) + floor(W3[get_global_id(0)]) * (W3[get_global_id(0)] - floor(W3[get_global_id(0)]));"
        "}";
    

    sources.push_back({erste_kernel.c_str(), erste_kernel.length()});
    sources.push_back({zweite_kernel.c_str(), zweite_kernel.length()});
    cl::Program program(context, sources);

    // Compile the program
    if (program.build({mydevice}) != CL_SUCCESS) {
        std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(mydevice) << std::endl;
        exit(1);
    }
    std::cout << "Sucessfully build the kernel" << std::endl;

    // Create input and output buffers
    cl::Buffer buffer_W1(context, CL_MEM_READ_WRITE, sizeof(float) * PROBLEM_SIZE);
    cl::Buffer buffer_W2(context, CL_MEM_READ_WRITE, sizeof(float) * PROBLEM_SIZE);
    cl::Buffer buffer_W3(context, CL_MEM_READ_WRITE, sizeof(float) * PROBLEM_SIZE);
    cl::Buffer buffer_O1(context, CL_MEM_READ_WRITE, sizeof(float) * PROBLEM_SIZE);
    cl::Buffer buffer_O2(context, CL_MEM_READ_WRITE, sizeof(float) * PROBLEM_SIZE);
    
    // Create local arrays for the inputs
    float *W1data = new float[PROBLEM_SIZE];
    float *W2data = new float[PROBLEM_SIZE];
    float *W3data = new float[PROBLEM_SIZE];

    std::cout << "Create test data" << std::endl;
    // Fill the arrays with identic values
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        W1data[i] = i;
        W2data[i] = i;
        W3data[i] = i;
    }

    cl::CommandQueue queue(context, mydevice);

    std::cout << "Copy data into buffers" << std::endl;
    // Trigger a copy of the local buffers to the opencl buffers
    queue.enqueueWriteBuffer(buffer_W1, CL_TRUE, 0, sizeof(float) * PROBLEM_SIZE, W1data);
    queue.enqueueWriteBuffer(buffer_W2, CL_TRUE, 0, sizeof(float) * PROBLEM_SIZE, W2data);
    queue.enqueueWriteBuffer(buffer_W3, CL_TRUE, 0, sizeof(float) * PROBLEM_SIZE, W3data);

    // Create a kernel to call the "adder" function
    cl::Kernel kernel1(program, "adder1");
    kernel1.setArg(0, buffer_W1);
    kernel1.setArg(1, buffer_W2);
    kernel1.setArg(2, buffer_W3);
    kernel1.setArg(3, buffer_O1); // 4 !!!

    // Create a kernel to call the "adder" function
    cl::Kernel kernel2(program, "adder2");
    kernel2.setArg(0, buffer_W1);
    kernel2.setArg(1, buffer_W2);
    kernel2.setArg(2, buffer_W3);
    kernel2.setArg(3, buffer_O2);
    

    // get maximum work group size for kernel
    std::size_t wgsize1;
    kernel1.getWorkGroupInfo(mydevice, CL_KERNEL_WORK_GROUP_SIZE, &wgsize1);
    std::cout << "Max work group size for kernel: " << wgsize1 << std::endl;
    std::size_t wgsize2;
    kernel2.getWorkGroupInfo(mydevice, CL_KERNEL_WORK_GROUP_SIZE, &wgsize2);
    std::cout << "Max work group size for kernel: " << wgsize2 << std::endl;

    // Set the execution range for the kernel
    cl::NDRange global(PROBLEM_SIZE);
    // get maximum work item size
    int wisize1 = std::min(mydevice.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[2], wgsize1); // TODO 
    std::cout << "Max work item size for kernel1: " << wisize1 << std::endl;
    int wisize2 = std::min(mydevice.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[2], wgsize2); // TODO 
    std::cout << "Max work item size for kernel2: " << wisize2 << std::endl;

    // cl::NDRange local(1);
    cl::NDRange local1(wisize1);
    cl::NDRange local2(wisize2);

    std::cout << "Starting " << mydevice.getInfo<CL_DEVICE_NAME>() << " version" << std::endl;

    queue.finish();
    double before = omp_get_wtime();
    // Execute the kernel and measure the time
    queue.enqueueNDRangeKernel(kernel1, cl::NullRange, global, local1);
    queue.enqueueNDRangeKernel(kernel2, cl::NullRange, global, local2);
    queue.finish();
    double end = omp_get_wtime();

    std::cout << "OpenCL version took " << (end - before) * 1000000 << "µs" << std::endl;

    // Create a local buffer for the output and copy data from the opencl system
    float *O1data = new float[PROBLEM_SIZE];
    queue.enqueueReadBuffer(buffer_O1, CL_TRUE, 0, sizeof(float) * PROBLEM_SIZE, O1data);
    float *O2data = new float[PROBLEM_SIZE];
    queue.enqueueReadBuffer(buffer_O2, CL_TRUE, 0, sizeof(float) * PROBLEM_SIZE, O2data);

    // Perform CPU computation to validate the result
    float *correctcdata1 = new float[PROBLEM_SIZE]; // maybe back to int !!!
    float *correctcdata2 = new float[PROBLEM_SIZE]; // maybe back to int !!!

    before = omp_get_wtime();
#pragma omp parallel for
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        correctcdata1[i] = (W1data[i] + W2data[i]+ W3data[i]) * (W1data[i] - W2data[i] - W3data[i]);
    }
#pragma omp parallel for
for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
correctcdata2[i] = floor(W1data[i]) * (W1data[i] - floor(W1data[i])) + floor(W2data[i]) * (W2data[i] - floor(W2data[i])) + floor(W3data[i]) * (W3data[i] - floor(W3data[i]));
}
    end = omp_get_wtime();
    std::cout << "OpenMP version took " << (end - before) * 1000000 << "µs"
              << std::endl;

    bool ok = true;
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        if (correctcdata1[i] != O1data[i]) {
            std::cout << "The result is wrong at index " << i << ", expected "
                      << correctcdata1[i] << " but found " << O1data[i] << std::endl;
            ok = false;
            break;
        }
    }
    for (unsigned long i = 0; i < PROBLEM_SIZE; i++) {
        if (correctcdata2[i] != O2data[i]) {
            std::cout << "The result is wrong at index " << i << ", expected "
                      << correctcdata2[i] << " but found " << O2data[i] << std::endl;
            ok = false;
            break;
        }
    }
    if (ok) std::cout << "Results are consistent" << std::endl;
}

