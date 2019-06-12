/*
 * Team Id: HC#145
 * Author List: Pratap

 * Filename: return_node.h
 * Theme:HOMECOMMING(HC)
 * Functions: decode,
 * Global Variables: NONE
 */

#ifndef RETRURN_NODE_H_
#define RETRURN_NODE_H_

/*
 * Function Name:decode
 * Input: int x
 * Output: character pointer
 * Logic: it returns a specific node by takaing a habitat location
 * Example Call: arr=decode(4)
 */

char * decode(int x){

	switch(x){
		case 1:
		return "B1" ;
		case 2:
		return "C1";
		case 3:
		return "D1";
		case 4:
		return "E1";
		case 5:
		return "F1";
		case 6:
		return "B2";
		case 7:
		return "C2";

		case 8:
		return "D2";
		case 9:
		return "E2";


		case 10:
		return "F2";

		case 11:
		return "B3";
		case 12:
		return "C3";
		case 13:
		return "D3";
		case 14:
		return "E3";
		case 15:
		return "F3";
		case 16:
		return "B4";

		case 17:
		return "C4";
		case 18:
		return "D4";
		case 19:
		return "E4";
		case 20:
		return "F4";
		case 21:
		return "B5";

		case 22:
		return "C5";
		case 23:
		return "D5";
		case 24:
		return "E5";
		case 25:
		return "F5";
	}
}




#endif /* RETRURN_NODE_H_ */