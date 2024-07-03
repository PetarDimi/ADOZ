
#ifndef IIR_H
#define IIR_H

#include "tistdtypes.h"

Int16 second_order_IIR( Int16 input, Int16* coefficients, Int16* x_history, Int16* y_history);

Int16 fourth_order_IIR(Int16 input, Int16 coefficients[][6], Int16 x_history[][2], Int16 y_history[][2]);
		
Int16 sixth_order_IIR(Int16 input, Int16 coefficients[][6], Int16 x_history[][2], Int16 y_history[][2]);

#endif
