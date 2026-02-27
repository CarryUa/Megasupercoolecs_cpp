#ifndef _MSCE_SYSTEM_H_
#define _MSCE_SYSTEM_H_
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <typeindex>
using namespace std;

class System
{
public:
    /// @brief
    /// @tparam TSys is the type of child object being created
    System();
    virtual void Init();
    virtual void Update(double delta_t);
};

#endif // _MSCE_SYSTEM_H_