/* 
 * File:   common.h
 * Author: vins
 *
 * Created on 30 septembre 2012, 17:59
 */

#ifndef COMMON_H
#define	COMMON_H

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)


#endif	/* COMMON_H */

