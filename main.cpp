#include <windows.h>
#include <iostream>
#include <limits>

typedef double (*LinearFn)(double, double, double, double, double);
typedef double (*LagrangeFn)(const double*, const double*, int, double);
typedef double (*InterpolateFn)(const double*, const double*, int, double);
typedef double (*ErrorFn)(const double*, const double*, int, double, double);

int main(int argc, char* argv[]) {
    const char* dllName = (argc > 1) ? argv[1] : "InterpolationLib.dll";
    HMODULE h = LoadLibraryA(dllName);
    if (!h) {
        std::cerr << "ERROR: cannot load " << dllName 
                  << " (GetLastError = " << GetLastError() << ")\n";
        return 1;
    }
    std::cout << "DLL loaded: " << dllName << "\n";

    auto linear      = reinterpret_cast<LinearFn>(GetProcAddress(h, "linearInterpolation"));
    auto lagrange    = reinterpret_cast<LagrangeFn>(GetProcAddress(h, "lagrange"));
    auto interpolate = reinterpret_cast<InterpolateFn>(GetProcAddress(h, "interpolate"));
    auto interpErr   = reinterpret_cast<ErrorFn>(GetProcAddress(h, "interpolationError"));

    if (!linear || !lagrange || !interpolate || !interpErr) {
        std::cerr << "ERROR: functions not found in DLL\n";
        FreeLibrary(h);
        return 1;
    }

    const double xs[] = {0, 1, 2, 3};
    const double ys[] = {0, 1, 4, 9};
    const double nan = std::numeric_limits<double>::quiet_NaN();

    std::cout << "linear(0,0,2,4,1)       = " << linear(0, 0, 2, 4, 1) << " (expected 2)\n";
    std::cout << "lagrange(x=1.5)         = " << lagrange(xs, ys, 4, 1.5) << " (expected 2.25)\n";
    std::cout << "interpolate(x=1.5)      = " << interpolate(xs, ys, 4, 1.5) << " (expected 2.5)\n";
    std::cout << "interpolate(x=4)        = " << interpolate(xs, ys, 4, 4) << " (extrapolation, 14)\n";
    std::cout << "error(x=1.5,true=2.25)  = " << interpErr(xs, ys, 4, 1.5, 2.25) << " (~0)\n";

    FreeLibrary(h);
 


   std::cout << "DLL unloaded, no crash\n";
    return 0;
}
