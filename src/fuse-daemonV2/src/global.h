/*
 * global.h
 *
 *  Created on: Sep 27, 2013
 *      Author: root
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define _try
#define _finally _quit:;
#define _try_return(s) do {s; goto _quit;} while(0)

#endif /* GLOBAL_H_ */
