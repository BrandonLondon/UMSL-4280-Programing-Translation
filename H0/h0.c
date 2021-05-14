/* ====================================================================================================
 * # Author: Brandon London
 * # Course: Program Translation Techniques
 * # File Name: f12.c
 * # Date: 1/23/21
 * ==================================================================================================== */

#include "f12.h"
#include "helper.h"

static int g12 = 0;//Global Variable Not exposed to f3
void f1()
{
	fprintf(stderr, "f1 global: %d\n", g12);
	g12 += 1;
}

void f2()
{
	fprintf(stderr, "f2 global: %d\n", g12);
	g12 += 2;
}
/* ====================================================================================================
 * # Author: Brandon London
 * # Course: Program Translation Techniques
 * # File Name: f3.c
 * # Date: 1/23/21
 * ==================================================================================================== */

#include "f3.h"
#include "helper.h"
#define M1a
int g;

void f3()
{
	fprintf(stderr, "f3 global: %d \n");
	g += 1;
	f3a();
}

static void f3a()
{
	fprintf(stderr, "Hello\n");
}

/* ====================================================================================================
 * # Author: Brandon London
 * # Course: Program Translation Techniques
 * # File Name: helper.h
 * # Date: 1/23/21
 * =================================================================================================== */

#ifndef HELPER
	#define HELPER

	#define MACROS_M
	extern int g;//Available to all

	#include "f12.h"
	#include "f3.h"
#endif

/* ====================================================================================================
 * # Author: Brandon London
 * # Course: Program Translation Techniques
 * # File Name: f12.h
 * # Date: 1/23/21
 * =================================================================================================== */
#ifndef F12_HEADER
	#define F12_HEADER
	void f1();
	void f2();
#endif

/* ====================================================================================================
 * # Author: Brandon London
 * # Course: Program Translation Techniques
 * # File Name: f3.h
 * # Date: 1/23/21
 * ==================================================================================================== */

#ifndef F3_HEADER
	#define F3_Header
	void f3();
#endif













