#include "display_decode.h"

//Display decode table
code char decode_table[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, //[0] -[9]  : Digit 0~9
    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x3d, 0x76, 0x0f, 0x0e, //[10]-[35] : Alphabet A~Z
    0x75, 0x38, 0x37, 0x54, 0x5c, 0x73, 0x67, 0x31, 0x49, 0x78,
    0x3e, 0x1c, 0x7e, 0x64, 0x6e, 0x59,
    0x00, 0x80                                                        //[36]      : Whitespace
};

// char ch_to_index(char ch)
// {
//     if('0' <= ch && ch <= '9')
//         return ch - '0';
    
//     if('a' <= ch && ch <= 'z')
//         return ch - 'a' + 10;

//     if('A' <= ch && ch <= 'A')
//         return ch - 'A';

//     if(ch == ' ')
//         return 36;
		
// 		return 36;
// }