#ifndef PATHFINDING_CORE_EXPORT_HPP
#define PATHFINDING_CORE_EXPORT_HPP

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef PATHFINDING_BUILDING_DLL
#define PATHFINDING_API __declspec(dllexport)
#elif defined(PATHFINDING_DLL)
#define PATHFINDING_API __declspec(dllimport)
#else // !PATHFINDING_BUILDING_DLL && !PATHFINDING_DLL
#define PATHFINDING_API
#endif // PATHFINDING_BUILDING_DLL
#else  // !_WIN32 && !__CYGWIN__
#if __GNUC__ >= 4
#define PATHFINDING_API __attribute__((visibility("default")))
#else // __GNUC__ < 4
#define PATHFINDING_API
#endif // __GNUC__ >= 4
#endif // _WIN32 || __CYGWIN__

#endif // !PATHFINDING_CORE_EXPORT_HPP
