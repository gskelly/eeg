#ifndef _CATH
#define _CATH

#define	_CAT3(a,b,c)	a## b ##c
#define CAT3(a,b,c)		_CAT3(a,b,c)
#define _CAT2(a,b)		a## b
#define CAT2(a,b)		_CAT2(a,b)
 
#endif // #ifndef _CATH