#ifndef MAIN_HPP__
#define MAIN_HPP__

#define DEBUG 

#ifdef DEBUG
#undef DEBUG
#define DEBUG(x) x
#else 
#define DEBUG(x)
#endif

#endif