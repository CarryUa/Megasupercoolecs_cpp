#ifndef MSCE_ABI_MACROS_H_
#define MSCE_ABI_MACROS_H_

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef MSCE_BUILD_DLL
#ifdef __GNUC__
#define MSCE_ABI_EXPORT_ATTRIB __attribute__((dllexport))
#else
#define MSCE_ABI_EXPORT_ATTRIB __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define MSCE_ABI_EXPORT_ATTRIB __attribute__((dllimport))
#else
#define MSCE_ABI_EXPORT_ATTRIB __declspec(dllimport)
#endif
#endif
#else
#if __GNUC__ >= 4
#define MSCE_ABI_EXPORT_ATTRIB __attribute__((visibility("default")))
#else
#define MSCE_ABI_EXPORT_ATTRIB
#endif
#endif

#endif // MSCE_ABI_MACROS_H_