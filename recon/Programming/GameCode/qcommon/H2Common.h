#ifndef H2COMMON_H
#define H2COMMON_H

#ifdef H2COMMON_STATIC
#define H2COMMON_API
#else
#ifdef H2COMMON
#define H2COMMON_API __declspec(dllexport)

#ifdef __cplusplus
#define H2COMMON_API_CPLUSPLUS extern "C" __declspec(dllexport) 
#else
#define H2COMMON_API_CPLUSPLUS __declspec(dllexport) 
#endif
#else
#define H2COMMON_API __declspec(dllimport)
#ifdef __cplusplus
#define H2COMMON_API_CPLUSPLUS extern "C" __declspec(dllimport) 
#else
#define H2COMMON_API_CPLUSPLUS __declspec(dllimport) 
#endif
#endif
#endif

#endif
